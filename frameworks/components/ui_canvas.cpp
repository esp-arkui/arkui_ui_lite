/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "components/ui_canvas.h"

#include <components/ui_view_group.h>

#include "common/image.h"
#include "draw/draw_arc.h"
#include "draw/draw_image.h"
#include "engines/gfx/gfx_engine_manager.h"
#include "gfx_utils/graphic_log.h"
#include "gif_lib.h"
#include "securec.h"

namespace OHOS {
    UICanvas::UICanvasPath::~UICanvasPath()
    {
        points_.Clear();
        cmd_.Clear();
        arcParam_.Clear();
    }

    void UICanvas::BeginPath()
    {
        /* If the previous path is not added to the drawing linked list, it should be destroyed directly. */
        if (path_ != nullptr && path_->strokeCount_ == 0) {
            delete path_;
            path_ = nullptr;
        }

        path_ = new UICanvasPath();
        if (path_ == nullptr) {
            GRAPHIC_LOGE("new UICanvasPath fail");
            return;
        }
    }

    void UICanvas::MoveTo(const Point& point)
    {
        if (path_ == nullptr) {
            return;
        }

        path_->startPos_ = point;
        /* If the previous command is also CMD_MOVE_TO, the previous command is overwritten. */
        if ((path_->cmd_.Size() != 0) && (path_->cmd_.Tail()->data_ == CMD_MOVE_TO)) {
            path_->points_.Tail()->data_ = point;
            return;
        }
        path_->points_.PushBack(point);
        path_->cmd_.PushBack(CMD_MOVE_TO);
    }

    void UICanvas::LineTo(const Point& point)
    {
        if (path_ == nullptr) {
            return;
        }

        path_->points_.PushBack(point);
        if (path_->cmd_.Size() == 0) {
            path_->startPos_ = point;
            path_->cmd_.PushBack(CMD_MOVE_TO);
        } else {
            path_->cmd_.PushBack(CMD_LINE_TO);
        }
    }

    void UICanvas::ArcTo(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle)
    {
        if (path_ == nullptr) {
            return;
        }

        /*
         * If there is no command before CMD_ARC, only the arc is drawn. If there is a command in front of
         * CMD_ARC, the start point of arc must be connected to the end point of the path.
         */
        float sinma = radius * Sin(startAngle);
        float cosma = radius * Sin(QUARTER_IN_DEGREE - startAngle);
        if (path_->cmd_.Size() != 0) {
            path_->points_.PushBack({MATH_ROUND(center.x + sinma), MATH_ROUND(center.y - cosma)});
            path_->cmd_.PushBack(CMD_LINE_TO);
        } else {
            path_->startPos_ = {MATH_ROUND(center.x + sinma), MATH_ROUND(center.y - cosma)};
        }

        /* If the ARC scan range exceeds 360 degrees, the end point of the path is the position of the start angle. */
        if (MATH_ABS(startAngle - endAngle) < CIRCLE_IN_DEGREE) {
            sinma = radius * Sin(endAngle);
            cosma = radius * Sin(QUARTER_IN_DEGREE - endAngle);
        }
        path_->points_.PushBack({MATH_ROUND(center.x + sinma), MATH_ROUND(center.y - cosma)});
        path_->cmd_.PushBack(CMD_ARC);

        int16_t start;
        int16_t end;
        if (startAngle > endAngle) {
            start = endAngle;
            end = startAngle;
        } else {
            start = startAngle;
            end = endAngle;
        }

        DrawArc::GetInstance()->GetDrawRange(start, end);
        ArcParam param;
        param.center = center;
        param.radius = radius;
        param.startAngle = start;
        param.endAngle = end;
        path_->arcParam_.PushBack(param);
    }
    void UICanvas::AddRect(const Point& point, int16_t height, int16_t width)
    {
        if (path_ == nullptr) {
            return;
        }

        MoveTo(point);
        LineTo({static_cast<int16_t>(point.x + width), point.y});
        LineTo({static_cast<int16_t>(point.x + width), static_cast<int16_t>(point.y + height)});
        LineTo({point.x, static_cast<int16_t>(point.y + height)});
        ClosePath();
    }

    void UICanvas::AddRect(const Point& point, int16_t height, int16_t width, const Paint& paint)
    {
        AddRect(point, height, width);

        paint.GetDrawGraphicsContext()->Rectangle(point.x, point.y, point.x + width, point.y + height);
    }

    void UICanvas::ClosePath()
    {
        if ((path_ == nullptr) || (path_->cmd_.Size() == 0)) {
            return;
        }

        path_->points_.PushBack(path_->startPos_);
        path_->cmd_.PushBack(CMD_CLOSE);
    }

    UICanvas::~UICanvas()
    {
        if ((path_ != nullptr) && (path_->strokeCount_ == 0)) {
            delete path_;
            path_ = nullptr;
        }

        void* param = nullptr;
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DeleteParam(param);
            curDraw->data_.param = nullptr;
        }
        drawCmdList_.Clear();
        while (!PaintStack.empty()) {
            PaintStack.pop();
        }
        //        if (m_graphics != nullptr) {
        //            delete m_graphics;
        //            m_graphics = nullptr;
        //        }
    }

    void UICanvas::Clear()
    {
        if ((path_ != nullptr) && (path_->strokeCount_ == 0)) {
            delete path_;
            path_ = nullptr;
        }

        void* param = nullptr;
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DeleteParam(param);
            curDraw->data_.param = nullptr;
        }
        drawCmdList_.Clear();
        while (!PaintStack.empty()) {
            PaintStack.pop();
        }
        Invalidate();
    }

    void UICanvas::DrawLine(const Point& endPoint, const Paint& paint)
    {
        DrawLine(startPoint_, endPoint, paint);
    }

    void UICanvas::DrawLine(const Point& startPoint, const Point& endPoint, const Paint& paint)
    {
        LineParam* lineParam = new LineParam;
        if (lineParam == nullptr) {
            GRAPHIC_LOGE("new LineParam fail");
            return;
        }
        lineParam->start = startPoint;
        lineParam->end = endPoint;

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = lineParam;
        cmd.DeleteParam = DeleteLineParam;
        cmd.DrawGraphics = DoDrawLine;
        drawCmdList_.PushBack(cmd);

        Invalidate();
        SetStartPosition(endPoint);
    }

    void UICanvas::DrawCurve(const Point& control1, const Point& control2, const Point& endPoint, const Paint& paint)
    {
        DrawCurve(startPoint_, control1, control2, endPoint, paint);
    }

    void UICanvas::DrawCurve(const Point& startPoint, const Point& control1, const Point& control2,
                             const Point& endPoint, const Paint& paint)
    {
        CurveParam* curveParam = new CurveParam;
        if (curveParam == nullptr) {
            GRAPHIC_LOGE("new CurveParam fail");
            return;
        }
        curveParam->start = startPoint;
        curveParam->control1 = control1;
        curveParam->control2 = control2;
        curveParam->end = endPoint;

        DrawCmd cmd;
        cmd.paint = paint;
        if (paint.GetStrokeWidth() > MAX_CURVE_WIDTH) {
            cmd.paint.SetStrokeWidth(MAX_CURVE_WIDTH);
        }
        cmd.param = curveParam;
        cmd.DeleteParam = DeleteCurveParam;
        cmd.DrawGraphics = DoDrawCurve;
        drawCmdList_.PushBack(cmd);

        Invalidate();
        SetStartPosition(endPoint);
    }

    void UICanvas::StrokeRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE) {
            RectParam* rectParam = new RectParam;
            if (rectParam == nullptr) {
                GRAPHIC_LOGE("new RectParam fail");
                return;
            }

            rectParam->start = startPoint;
            rectParam->height = height;
            rectParam->width = width;

            DrawCmd cmd;
            cmd.paint = paint;
            cmd.param = rectParam;
            cmd.DeleteParam = DeleteRectParam;
            cmd.DrawGraphics = DoStrokeRect;
            drawCmdList_.PushBack(cmd);
        }

        Invalidate();
        SetStartPosition(startPoint);
    }

    void UICanvas::ClearRect(const Point& clearRect, int clearHeight, int clearWidth, const Paint& paint)
    {
        RectParam* clearRectParam = new RectParam;
        if (clearRectParam == nullptr) {
            GRAPHIC_LOGE("new ClearRectParam fail");
            return;
        }

        clearRectParam->start = clearRect;
        clearRectParam->height = clearHeight;
        clearRectParam->width = clearWidth;

        DrawCmd cmd;
        cmd.paint.SetFillColor(this->style_->bgColor_);
        cmd.paint.SetOpacity(this->opaScale_);
        cmd.param = clearRectParam;
        cmd.DeleteParam = DeleteRectParam;
        cmd.DrawGraphics = DoClearRect;
        drawCmdList_.PushBack(cmd);
        Invalidate();
        SetStartPosition(clearRect);
    }

    void UICanvas::DrawRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
    {
        if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE) ||
            (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_GRADIENT)) {
            RectParam* rectParam = new RectParam;
            if (rectParam == nullptr) {
                GRAPHIC_LOGE("new RectParam fail");
                return;
            }
            rectParam->start = startPoint;
            rectParam->height = height;
            rectParam->width = width;

            DrawCmd cmd;
            cmd.paint = paint;
            cmd.param = rectParam;
            cmd.DeleteParam = DeleteRectParam;
            cmd.DrawGraphics = DoDrawRect;
            drawCmdList_.PushBack(cmd);
        }

        if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) ||
            (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_GRADIENT)) {
            RectParam* rectParam = new RectParam;
            if (rectParam == nullptr) {
                GRAPHIC_LOGE("new RectParam fail");
                return;
            }
            rectParam->start = startPoint;
            rectParam->height = height;
            rectParam->width = width;

            DrawCmd cmd;
            cmd.paint = paint;
            cmd.param = rectParam;
            cmd.DeleteParam = DeleteRectParam;
            cmd.DrawGraphics = DoFillRect;
            drawCmdList_.PushBack(cmd);
        }

        Invalidate();
    }

    void UICanvas::DrawCircle(const Point& center, uint16_t radius, const Paint& paint)
    {
        CircleParam* circleParam = new CircleParam;
        if (circleParam == nullptr) {
            GRAPHIC_LOGE("new CircleParam fail");
            return;
        }
        circleParam->center = center;
        circleParam->radius = radius;

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = circleParam;
        cmd.DeleteParam = DeleteCircleParam;
        cmd.DrawGraphics = DoDrawCircle;
        drawCmdList_.PushBack(cmd);

        Invalidate();
    }

    void UICanvas::DrawSector(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle,
                              const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
            Paint innerPaint = paint;
            innerPaint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
            innerPaint.SetStrokeWidth(radius);
            innerPaint.SetStrokeColor(paint.GetFillColor());
            radius >>= 1;
            DrawArc(center, radius, startAngle, endAngle, innerPaint);
        }
    }

    void UICanvas::DrawArc(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle,
                           const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE) {
            ArcParam* arcParam = new ArcParam;
            if (arcParam == nullptr) {
                GRAPHIC_LOGE("new ArcParam fail");
                return;
            }
            arcParam->center = center;
            arcParam->radius = radius;

            int16_t start;
            int16_t end;
            if (startAngle > endAngle) {
                start = endAngle;
                end = startAngle;
            } else {
                start = startAngle;
                end = endAngle;
            }

            DrawArc::GetInstance()->GetDrawRange(start, end);
            arcParam->startAngle = start;
            arcParam->endAngle = end;

            DrawCmd cmd;
            cmd.paint = paint;
            cmd.param = arcParam;
            cmd.DeleteParam = DeleteArcParam;
            cmd.DrawGraphics = DoDrawArc;
            drawCmdList_.PushBack(cmd);

            Invalidate();
        }
    }

    void UICanvas::StrokeText(const char* text, const Point& point, const FontStyle& fontStyle, const Paint& paint)
    {
        if (text == nullptr) {
            return;
        }
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
            TextParam* textParam = new TextParam;
            if (textParam == nullptr) {
                GRAPHIC_LOGE("new TextParam fail");
                return;
            }
            textParam->text = text;
            textParam->fontStyle = fontStyle;
            textParam->fontOpa = paint.GetOpacity();
            textParam->fontColor = paint.GetFillColor();
            textParam->position = point;
            textParam->isDrawTrans = paint.IsTransform();
            DrawCmd cmd;
            cmd.param = textParam;
            cmd.DeleteParam = DeleteTextParam;
            cmd.DrawGraphics = DoDrawText;
            cmd.paint = paint;
            drawCmdList_.PushBack(cmd);

            Invalidate();
        }
    }

    Point UICanvas::MeasureText(const char* text, const FontStyle& fontStyle, const Paint& paint)
    {
        Text* textCompent = new Text;
        textCompent->SetText(text);
        textCompent->SetFont(fontStyle.fontName, fontStyle.fontSize);
        textCompent->SetDirect(static_cast<UITextLanguageDirect>(fontStyle.direct));
        textCompent->SetAlign(static_cast<UITextLanguageAlignment>(fontStyle.align));
        Style drawStyle;
        drawStyle.SetStyle(STYLE_LETTER_SPACE, fontStyle.letterSpace);
        textCompent->ReMeasureTextSize(this->GetRect(), drawStyle);
        Point textSize = textCompent->GetTextSize();
        delete textCompent;
        return textSize;
    }

    void UICanvas::DrawLabel(const Point& startPoint, const char* text, uint16_t maxWidth, const FontStyle& fontStyle,
                             const Paint& paint)
    {
        if (text == nullptr) {
            return;
        }
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
            UILabel* label = new UILabel();
            if (label == nullptr) {
                GRAPHIC_LOGE("new UILabel fail");
                return;
            }
            label->SetLineBreakMode(UILabel::LINE_BREAK_CLIP);
            label->SetPosition(startPoint.x, startPoint.y);
            label->SetWidth(maxWidth);
            label->SetHeight(GetHeight());
            label->SetText(text);
            label->SetFont(fontStyle.fontName, fontStyle.fontSize);
            label->SetAlign(fontStyle.align);
            label->SetDirect(fontStyle.direct);
            label->SetStyle(STYLE_LETTER_SPACE, fontStyle.letterSpace);
            label->SetStyle(STYLE_TEXT_COLOR, paint.GetFillColor().full);
            label->SetStyle(STYLE_TEXT_OPA, paint.GetOpacity());

            DrawCmd cmd;
            cmd.param = label;
            cmd.DeleteParam = DeleteLabel;
            cmd.DrawGraphics = DoDrawLabel;
            drawCmdList_.PushBack(cmd);

            Invalidate();
        }
    }

    void UICanvas::DrawImage(const Point& startPoint, const char* image, const Paint& paint,int16_t width, int16_t height)
    {
        if (image == nullptr) {
            return;
        }

        ImageParam* imageParam = new ImageParam;
        if (imageParam == nullptr) {
            GRAPHIC_LOGE("new ImageParam fail");
            return;
        }
        imageParam->image = new Image();
        if (imageParam->image == nullptr) {
            delete imageParam;
            imageParam = nullptr;
            return;
        }
        imageParam->path = new char[strlen(image) + 1];
        strcpy_s(imageParam->path, strlen(image) + 1, image);
        imageParam->image->SetSrc(imageParam->path);
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->gifImageAnimator = nullptr;
        imageParam->start = startPoint;
        imageParam->height = header.height;
        imageParam->width = header.width;

        if (IsGif(image)) {
            imageParam->gifImageAnimator = new GifCanvasImageAnimator(imageParam, this, imageParam->path);
            Point gifSize = imageParam->gifImageAnimator->GetSize();
            imageParam->width = gifSize.x;
            imageParam->height = gifSize.y;
            imageParam->gifImageAnimator->Start();
        }

        imageParam->newWidth = width;
        imageParam->newHeight = height;
		DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoDrawImage;
        drawCmdList_.PushBack(cmd);

        Invalidate();
        SetStartPosition(startPoint);
    }

    void UICanvas::DrawImage(const Point& startPoint, const ImageInfo* image, const Paint& paint)
    {
        if (image == nullptr) {
            return;
        }

        ImageParam* imageParam = new ImageParam;
        if (imageParam == nullptr) {
            GRAPHIC_LOGE("new ImageParam fail");
            return;
        }
        imageParam->image = new Image();
        if (imageParam->image == nullptr) {
            delete imageParam;
            imageParam = nullptr;
            return;
        }
        imageParam->path = nullptr;
        imageParam->image->SetSrc(image);
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->start = startPoint;
        imageParam->height = header.height;
        imageParam->width = header.width;

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoDrawImage;
        drawCmdList_.PushBack(cmd);

        Invalidate();
    }

    void UICanvas::FillPath(const Paint& paint)
    {
        if ((path_ == nullptr) || (path_->cmd_.Size() == 0)) {
            return;
        }
        path_->strokeCount_++;
        PathParam* param = new PathParam;
        if (param == nullptr) {
            GRAPHIC_LOGE("new PathParam fail");
            return;
        }
        param->path = path_;
        param->count = path_->cmd_.Size();
        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = param;
        cmd.DeleteParam = DeletePathParam;
        cmd.DrawGraphics = DoFillPath;
        drawCmdList_.PushBack(cmd);
        Invalidate();
    }
    void UICanvas::DrawPath(const Paint& paint)
    {
        if ((path_ == nullptr) || (path_->cmd_.Size() == 0)) {
            return;
        }
        path_->strokeCount_++;
        PathParam* param = new PathParam;
        if (param == nullptr) {
            GRAPHIC_LOGE("new PathParam fail");
            return;
        }
        param->path = path_;
        param->count = path_->cmd_.Size();
        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = param;
        cmd.DeleteParam = DeletePathParam;
        cmd.DrawGraphics = DoDrawPath;
        drawCmdList_.PushBack(cmd);
        Invalidate();
    }

    void UICanvas::OnDraw(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea)
    {
        Rect rect = GetOrigRect();
        BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, rect, invalidatedArea, *style_, opaScale_);
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        Rect trunc = invalidatedArea;
        if (trunc.Intersect(trunc, GetOrigRect())) {
            int16_t posViewLeft = rect.GetX() - trunc.GetX();
            int16_t posViewTop = rect.GetY() - trunc.GetY();
            int16_t realLeft = rect.GetLeft() + style_->paddingLeft_ + style_->borderWidth_;
            int16_t realTop = rect.GetTop() + style_->paddingTop_ + style_->borderWidth_;
            BufferInfo* gfxMapBuffer = nullptr;
            uint8_t destByteSize;
            // 添加的处理机制的。。。
            bool isChangeBlend = false;
            for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
                // 应该是实现画布的处理机制..
                if (curDraw->data_.paint.GetGlobalCompositeOperation() !=
                    BaseGfxExtendEngine::BlendMode::BLENDSRCOVER) {
                    isChangeBlend = true;
                    break;
                }
            }
            if (isChangeBlend) {
                if (gfxMapBuffer == nullptr) {
                    gfxMapBuffer = new BufferInfo();
                    if (memcpy_s(gfxMapBuffer, sizeof(BufferInfo), &gfxDstBuffer, sizeof(BufferInfo)) != 0) {
                        return;
                    }
                    destByteSize = DrawUtils::GetByteSizeByColorMode(gfxDstBuffer.mode);
                    uint32_t destStride = gfxMapBuffer->width * destByteSize;
                    uint32_t buffSize = gfxMapBuffer->height * destStride;
                    gfxMapBuffer->virAddr = BaseGfxEngine::GetInstance()->AllocBuffer(buffSize, BUFFER_MAP_SURFACE);
                    memset_s(gfxMapBuffer->virAddr, buffSize, 0, buffSize);
                    gfxMapBuffer->phyAddr = gfxMapBuffer->virAddr;
                }
                InitDrawEnvironment(
                    *gfxMapBuffer, trunc,
                    Rect(realLeft, realTop, realLeft + trunc.GetWidth() - 1, realTop + trunc.GetHeight() - 1),
                    Rect(posViewLeft, posViewTop, posViewLeft + trunc.GetWidth() - 1,
                         posViewTop + trunc.GetHeight() - 1));
            } else {
                InitDrawEnvironment(
                    gfxDstBuffer, trunc,
                    Rect(realLeft, realTop, realLeft + trunc.GetWidth() - 1, realTop + trunc.GetHeight() - 1),
                    Rect(posViewLeft, posViewTop, posViewLeft + trunc.GetWidth() - 1,
                         posViewTop + trunc.GetHeight() - 1));
            }
            for (curDraw = drawCmdList_.Begin(); curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
                // 应该是实现画布的处理机制..
                if (isChangeBlend) {
                    if (BaseGfxExtendEngine::BlendMode::BLENDCOPY ==
                        curDraw->data_.paint.GetGlobalCompositeOperation()) {
                        BaseGfxEngine::GetInstance()->DrawRect(*gfxMapBuffer, rect, invalidatedArea, *style_, opaScale_);

                        curDraw->data_.paint.SetGlobalCompositeOperation(BaseGfxExtendEngine::BlendMode::BLENDSRCOVER);
                        curDraw->data_.DrawGraphics(*gfxMapBuffer, curDraw->data_.param,
                                                    curDraw->data_.paint, rect, trunc, *style_);
                        curDraw->data_.paint.SetGlobalCompositeOperation(BaseGfxExtendEngine::BlendMode::BLENDCOPY);
                        continue;
                    }
                    curDraw->data_.DrawGraphics(*gfxMapBuffer, curDraw->data_.param,
                                                curDraw->data_.paint, rect, trunc, *style_);
                } else {
                    curDraw->data_.DrawGraphics(gfxDstBuffer, curDraw->data_.param,
                                                curDraw->data_.paint, rect, trunc, *style_);
                }
            }
            if (gfxMapBuffer == nullptr) {
                return;
            }
            BaseGfxExtendEngine::Image imageBuffer((unsigned char*)gfxMapBuffer->virAddr, gfxMapBuffer->width,
                                                   gfxMapBuffer->height, gfxMapBuffer->stride);
            BaseGfxExtendEngine m_graphics_Image;
            uint8_t* destBuf = static_cast<uint8_t*>(gfxDstBuffer.virAddr);
            if (gfxDstBuffer.virAddr == nullptr) {
                return;
            }
            int32_t offset = static_cast<int32_t>(trunc.GetTop()) * gfxDstBuffer.width + trunc.GetLeft();
            destBuf += offset * destByteSize;
            m_graphics_Image.Attach(destBuf, trunc.GetWidth(), trunc.GetHeight(), gfxDstBuffer.stride);
            m_graphics_Image.Viewport(realLeft, realTop, realLeft + trunc.GetWidth() - 1,
                                      realTop + trunc.GetHeight() - 1, posViewLeft, posViewTop,
                                      posViewLeft + trunc.GetWidth() - 1, posViewTop + trunc.GetHeight() - 1,
                                      BaseGfxExtendEngine::XMINYMIN);
            m_graphics_Image.BlendImage(imageBuffer, gfxMapBuffer->rect.GetLeft(), gfxMapBuffer->rect.GetTop(),
                                        gfxMapBuffer->rect.GetRight(), gfxMapBuffer->rect.GetBottom(),
                                        gfxDstBuffer.rect.GetLeft(), gfxDstBuffer.rect.GetTop(),
                                        DrawUtils::GetMixOpacity(opaScale_, style_->imageOpa_));
            BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)gfxMapBuffer->virAddr);
            delete gfxMapBuffer;
            gfxMapBuffer = nullptr;
        }
    }

    bool UICanvas::InitDrawEnvironment(const BufferInfo& gfxDstBuffer, const Rect& fillArea, const Rect& worldRect,
                                       const Rect& screenRect)
    {
        //BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();

        int16_t posLeft = fillArea.GetLeft();
        int16_t posTop = fillArea.GetTop();

        uint8_t* destBuf = static_cast<uint8_t*>(gfxDstBuffer.virAddr);
        if (gfxDstBuffer.virAddr == nullptr) {
            return false;
        }

        ColorMode mode = gfxDstBuffer.mode;
        uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(mode);
        int32_t offset = static_cast<int32_t>(posTop) * gfxDstBuffer.width + posLeft;
        destBuf += offset * destByteSize;

        m_graphics.Attach(destBuf, fillArea.GetWidth(), fillArea.GetHeight(), gfxDstBuffer.stride);
        m_graphics.Viewport(worldRect.GetLeft(), worldRect.GetTop(), worldRect.GetRight(), worldRect.GetBottom(),
                            screenRect.GetLeft(), screenRect.GetTop(), screenRect.GetRight(), screenRect.GetBottom(),
                            BaseGfxExtendEngine::XMINYMIN);
        return true;
    }

    void UICanvas::SetLineDash(float* dashArray, unsigned int ndash, Paint& paint)
    {
        paint.SetLineDash(dashArray, ndash);
    }

    void UICanvas::GetAbsolutePosition(const Point& prePoint, const Rect& rect, const Style& style, Point& point)
    {
        point.x = prePoint.x + rect.GetLeft() + style.paddingLeft_ + style.borderWidth_;
        point.y = prePoint.y + rect.GetTop() + style.paddingTop_ + style.borderWidth_;
    }

    void UICanvas::DoDrawLine(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }

        LineParam* lineParam = static_cast<LineParam*>(param);
        Point start;
        Point end;
        GetAbsolutePosition(lineParam->start, rect, style, start);
        GetAbsolutePosition(lineParam->end, rect, style, end);

        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }
        if (paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CAPNONE) {
            BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea, paint.GetStrokeWidth(),
                                                   paint.GetStrokeColor(), paint.GetOpacity());
        } else {
            ColorType colorType = paint.GetStrokeColor();
            m_graphics->SetLineColor(colorType.red, colorType.green, colorType.blue, colorType.alpha);
            m_graphics->SetLineWidth(paint.GetStrokeWidth());
            m_graphics->SetLineCap(paint.GetLineCap());
            m_graphics->NoFill();
            m_graphics->Line(start.x, start.y, end.x, end.y);
        }
    }

    void UICanvas::DoDrawCurve(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        CurveParam* curveParam = static_cast<CurveParam*>(param);
        Point start;
        Point end;
        Point control1;
        Point control2;
        GetAbsolutePosition(curveParam->start, rect, style, start);
        GetAbsolutePosition(curveParam->end, rect, style, end);
        GetAbsolutePosition(curveParam->control1, rect, style, control1);
        GetAbsolutePosition(curveParam->control2, rect, style, control2);

        BaseGfxEngine::GetInstance()->DrawCubicBezier(gfxDstBuffer, start, control1, control2, end, invalidatedArea,
                                                      paint.GetStrokeWidth(), paint.GetStrokeColor(),
                                                      paint.GetOpacity());
    }

    void UICanvas::DoClearRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        RectParam* rectParam = static_cast<RectParam*>(param);

        Style fillStyle = StyleDefault::GetDefaultStyle();
        fillStyle.bgColor_ = paint.GetFillColor();
        fillStyle.bgOpa_ = paint.GetOpacity();
        Point start;
        GetAbsolutePosition(rectParam->start, rect, style, start);
        BaseGfxEngine::GetInstance()->DrawRect(
            gfxDstBuffer, Rect(start.x, start.y, start.x + rectParam->width - 1, start.y + rectParam->height - 1),
            invalidatedArea, fillStyle, fillStyle.bgOpa_);
    }

    void UICanvas::DoStrokeRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        RectParam* rectParam = static_cast<RectParam*>(param);
        Style drawStyle = StyleDefault::GetDefaultStyle();
        drawStyle.bgColor_ = paint.GetStrokeColor();
        drawStyle.bgOpa_ = paint.GetOpacity();
        drawStyle.borderRadius_ = 0;

        int16_t lineWidth = static_cast<int16_t>(paint.GetStrokeWidth());
        Point start;
        GetAbsolutePosition(rectParam->start, rect, style, start);
        int16_t x = start.x - lineWidth / 2;
        int16_t y = start.y - lineWidth / 2;
        Rect coords;
        if ((rectParam->height <= lineWidth) || (rectParam->width <= lineWidth)) {
            coords.SetPosition(x, y);
            coords.SetHeight(rectParam->height + lineWidth);
            coords.SetWidth(rectParam->width + lineWidth);
            BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);
            return;
        }
        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }
        if (paint.IsLineDash()) {
            m_graphics->SetLineDashOffset(paint.GetLineDashOffset());
            m_graphics->SetLineDash(paint.GetLineDash(), paint.GetLineDashCount());
        } else {
            m_graphics->SetLineDash(nullptr, 0);
        }
        m_graphics->SetLineColor(drawStyle.bgColor_.red, drawStyle.bgColor_.green, drawStyle.bgColor_.blue,
                                 drawStyle.bgOpa_);
        m_graphics->SetLineWidth(lineWidth);
        if (paint.GetStyle() == Paint::PaintStyle::STROKE_STYLE) {
            m_graphics->NoFill();
        }

        m_graphics->Rectangle(start.x, start.y, start.x + rectParam->width - 1, start.y + rectParam->height - 1);
    }

    void UICanvas::DoDrawRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        RectParam* rectParam = static_cast<RectParam*>(param);
        Style drawStyle = StyleDefault::GetDefaultStyle();
        drawStyle.bgColor_ = paint.GetStrokeColor();
        drawStyle.bgOpa_ = paint.GetOpacity();
        drawStyle.borderRadius_ = 0;
        int16_t lineWidth = static_cast<int16_t>(paint.GetStrokeWidth());

        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_GRADIENT) {
            BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
            if (m_graphics == nullptr) {
                return;
            }
            Point start;
            GetAbsolutePosition(rectParam->start, rect, style, start);
            setGradient(*m_graphics, paint, rect, style); // 填充颜色
            m_graphics->SetLineWidth(lineWidth);
            m_graphics->Rectstroke(start.x, start.y, start.x + rectParam->width, start.y + rectParam->height);
        } else {
            Point start;
            GetAbsolutePosition(rectParam->start, rect, style, start);

            int16_t x = start.x - lineWidth / 2; // 2: half
            int16_t y = start.y - lineWidth / 2; // 2: half
            Rect coords;
            if ((rectParam->height <= lineWidth) || (rectParam->width <= lineWidth)) {
                coords.SetPosition(x, y);
                coords.SetHeight(rectParam->height + lineWidth);
                coords.SetWidth(rectParam->width + lineWidth);
                BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);
                return;
            }

            coords.SetPosition(x, y);
            coords.SetHeight(lineWidth);
            coords.SetWidth(rectParam->width);
            BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);

            coords.SetPosition(x + rectParam->width, y);
            coords.SetHeight(rectParam->height);
            coords.SetWidth(lineWidth);
            BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);

            coords.SetPosition(x, y + lineWidth);
            coords.SetHeight(rectParam->height);
            coords.SetWidth(lineWidth);
            BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);

            coords.SetPosition(x + lineWidth, y + rectParam->height);
            coords.SetHeight(lineWidth);
            coords.SetWidth(rectParam->width);
            BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);
        }
    }

    void UICanvas::DoFillRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        RectParam* rectParam = static_cast<RectParam*>(param);
        uint8_t enableStroke = static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE;
        int16_t lineWidth = enableStroke ? paint.GetStrokeWidth() : 0;
        if ((rectParam->height <= lineWidth) || (rectParam->width <= lineWidth)) {
            return;
        }

        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }

        if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) ||
            (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_GRADIENT)) {
            Point start;
            GetAbsolutePosition(rectParam->start, rect, style, start);

            if (paint.GetShadowOffsetX() != 0 || paint.GetShadowOffsetY() != 0) {
                m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
                m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
                m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                           paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
            }

            m_graphics->SetMasterAlpha((double)paint.GetGlobalAlpha());
            m_graphics->NoLine();
            setGradient(*m_graphics, paint, rect, style); // 填充颜色
            m_graphics->Rectangle(start.x, start.y, start.x + rectParam->width, start.y + rectParam->height);
        }
    }

    void UICanvas::addColorGradient(BaseGfxExtendEngine& m_graphics, List<GradientControl::StopAndColor>& stopAndColors)
    {
        m_graphics.RemoveAllColor();
        ListNode<GradientControl::StopAndColor>* iter = stopAndColors.Begin();
        uint16_t count = 0;
        for (; count < stopAndColors.Size(); count++) {
            ColorType stopColor = iter->data_.color;
            m_graphics.AddColor(iter->data_.stop, BaseGfxExtendEngine::Color(stopColor.red, stopColor.green,
                                                                             stopColor.blue, stopColor.alpha));
            iter = iter->next_;
        }
        m_graphics.BuildLut();
    }

    void UICanvas::FillPattern(const Paint& paint)
    {
        if (strcmp(paint.image, "") == 0) {
            return;
        }

        ImageParam* imageParam = new ImageParam;
        if (imageParam == nullptr) {
            GRAPHIC_LOGE("new ImageParam fail");
            return;
        }
        imageParam->image = new Image();
        if (imageParam->image == nullptr) {
            delete imageParam;
            imageParam = nullptr;
            return;
        }

        imageParam->image->SetSrc(paint.image);
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->start = {0, 0};
        imageParam->height = header.height;
        imageParam->width = header.width;

        PathParam* param = new PathParam;
        if (param == nullptr) {
            GRAPHIC_LOGE("new PathParam fail");
            return;
        }
        param->path = path_;
        param->count = path_->cmd_.Size();
        DrawCmd cmd2;
        cmd2.paint = paint;
        cmd2.param = param;
        cmd2.DeleteParam = DeletePathParam;
        cmd2.DrawGraphics = DoDrawPath;
        drawCmdList_.PushBack(cmd2);

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoDrawPattern;
        drawCmdList_.PushBack(cmd);
        Invalidate();
    }

    void UICanvas::StrokePattern(const Paint& paint)
    {
        if (strcmp(paint.image, "") == 0) {
            return;
        }

        ImageParam* imageParam = new ImageParam;
        if (imageParam == nullptr) {
            GRAPHIC_LOGE("new ImageParam fail");
            return;
        }
        imageParam->image = new Image();
        if (imageParam->image == nullptr) {
            delete imageParam;
            imageParam = nullptr;
            return;
        }

        imageParam->image->SetSrc(paint.image);
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->start = {0, 0};
        imageParam->height = header.height;
        imageParam->width = header.width;

        PathParam* param = new PathParam;
        if (param == nullptr) {
            GRAPHIC_LOGE("new PathParam fail");
            return;
        }
        param->path = path_;
        param->count = path_->cmd_.Size();
        DrawCmd cmd2;
        cmd2.paint = paint;
        cmd2.param = param;
        cmd2.DeleteParam = DeletePathParam;
        cmd2.DrawGraphics = DoDrawPath;
        drawCmdList_.PushBack(cmd2);

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoStrokePattern;
        drawCmdList_.PushBack(cmd);
        Invalidate();
    }
    void UICanvas::Gradient(const Paint& paint)
    {
        PathParam* param = new PathParam;
        if (param == nullptr) {
            GRAPHIC_LOGE("new PathParam fail");
            return;
        }
        param->path = path_;
        param->count = path_->cmd_.Size();
        DrawCmd cmd2;
        cmd2.paint = paint;
        cmd2.param = param;
        cmd2.DeleteParam = DeletePathParam;
        cmd2.DrawGraphics = DoGradient;
        drawCmdList_.PushBack(cmd2);
        Invalidate();
    }

    void UICanvas::Fill(const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN)) {
            FillPattern(paint);
        }
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)) {
            Gradient(paint);
        }
    }

    void UICanvas::Stroke(const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN)) {
            StrokePattern(paint);
        }
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)) {
            Gradient(paint);
        }
    }

    void UICanvas::setGradient(BaseGfxExtendEngine& m_graphics, const Paint& paint, const Rect& rect,
                               const Style& style)
    {
        GradientControl gradientControl = paint.getGradientControl();
        List<GradientControl::StopAndColor> stopAndColors = gradientControl.getStopAndColor();
        if (stopAndColors.Size() > 0) {
            addColorGradient(m_graphics, stopAndColors);
        }
        if (gradientControl.gradientflag == gradientControl.Linear) { // 线性渐变
            double x0 = gradientControl.getLinearGradientPoint().x0;
            double y0 = gradientControl.getLinearGradientPoint().y0;
            double x1 = gradientControl.getLinearGradientPoint().x1;
            double y1 = gradientControl.getLinearGradientPoint().y1;

            Point start;
            Point orgstart;
            orgstart.x = x0;
            orgstart.y = y0;
            GetAbsolutePosition(orgstart, rect, style, start);

            Point end;
            Point orgend;
            orgend.x = x1;
            orgend.y = y1;
            GetAbsolutePosition(orgend, rect, style, end);

            m_graphics.SetLinearGradient(start.x, start.y, end.x, end.y);
        }

        if (gradientControl.gradientflag == gradientControl.Radial) { // 放射渐变
            GradientControl::RadialGradientPoint rp = gradientControl.getRadialGradientPoint();

            Point start;
            Point orgstart;
            orgstart.x = rp.x0;
            orgstart.y = rp.y0;
            GetAbsolutePosition(orgstart, rect, style, start);

            Point end;
            Point orgend;
            orgend.x = rp.x1;
            orgend.y = rp.y1;
            GetAbsolutePosition(orgend, rect, style, end);

            m_graphics.SetRadialGradient(start.x, start.y, rp.r0, end.x, end.y, rp.r1);
        }

        if (gradientControl.gradientflag == gradientControl.Solid) { // 纯色渐变
            ColorType color = paint.GetFillColor();
            m_graphics.SetFillColor(BaseGfxExtendEngine::Color(color.red, color.green, color.blue, color.alpha));
        }
    }

    void UICanvas::DoDrawCircle(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        CircleParam* circleParam = static_cast<CircleParam*>(param);

        Style drawStyle = StyleDefault::GetDefaultStyle();

        ArcInfo arcInfo = {{0}};
        arcInfo.imgPos = Point{0, 0};
        arcInfo.startAngle = 0;
        arcInfo.endAngle = CIRCLE_IN_DEGREE;
        GetAbsolutePosition(circleParam->center, rect, style, arcInfo.center);
        uint8_t enableStroke = static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE;
        uint16_t halfLineWidth = enableStroke ? (paint.GetStrokeWidth() >> 1) : 0;
        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }

        double transFormCenterX = 0, transFormCenterY = 0, rotateAngle = 0;
        arcInfo.radius = circleParam->radius + halfLineWidth - 1;
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
            drawStyle.bgColor_ = paint.GetFillColor();
            drawStyle.bgOpa_ = paint.GetOpacity();
            m_graphics->SetFillColor(drawStyle.bgColor_.red, drawStyle.bgColor_.green, drawStyle.bgColor_.blue,
                                     drawStyle.bgOpa_);
        }
        if (enableStroke) {
            drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
            drawStyle.lineColor_ = paint.GetStrokeColor();
            drawStyle.lineOpa_ = paint.GetOpacity();
            if (paint.IsLineDash()) {
                m_graphics->SetLineDashOffset(paint.GetLineDashOffset());
                m_graphics->SetLineDash(paint.GetLineDash(), paint.GetLineDashCount());
            } else {
                m_graphics->SetLineDash(nullptr, 0);
            }
            m_graphics->SetLineWidth(drawStyle.lineWidth_);
            m_graphics->SetLineColor(drawStyle.lineColor_.red, drawStyle.lineColor_.green, drawStyle.lineColor_.blue,
                                     drawStyle.lineOpa_);
        }

        if (paint.GetGlobalAlpha() == 1.0f && !paint.IsLineDash() &&
            paint.GetGlobalCompositeOperation() == BaseGfxExtendEngine::BlendMode::BLENDNONE) {
            BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                  CapType::CAP_NONE);
        } else {
            transFormCenterX = paint.GetTransformCenterX() + rect.GetX() - invalidatedArea.GetX();
            transFormCenterY = paint.GetTransformCenterY() + rect.GetY() - invalidatedArea.GetY();

            StartTransform(rect, invalidatedArea, paint);
            if (!(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE)) {
                m_graphics->NoFill();
            }
            if (!enableStroke) {
                m_graphics->NoLine();
            }

            m_graphics->SetMasterAlpha((double)paint.GetGlobalAlpha());
            m_graphics->SetBlendMode(paint.GetGlobalCompositeOperation());
            if (paint.GetShadowOffsetX() != 0 || paint.GetShadowOffsetY() != 0) {
                m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
                m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
                m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                           paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
                m_graphics->DrawShadow(arcInfo.center.x, arcInfo.center.y, arcInfo.radius, arcInfo.radius,
                                       transFormCenterX, transFormCenterY, rotateAngle,
                                       paint.GetScaleX(), paint.GetScaleY(), paint.GetTransLateX(),
                                       paint.GetTransLateY());
            }
            m_graphics->Round(arcInfo.center.x, arcInfo.center.y, arcInfo.radius);
        }

        //m_graphics->ResetTransformations();
    }

    void UICanvas::DoDrawArc(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                             const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        ArcParam* arcParam = static_cast<ArcParam*>(param);

        ArcInfo arcInfo = {{0}};
        arcInfo.imgPos = Point{0, 0};
        arcInfo.startAngle = arcParam->startAngle;
        arcInfo.endAngle = arcParam->endAngle;
        Style drawStyle = StyleDefault::GetDefaultStyle();
        drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
        drawStyle.lineColor_ = paint.GetStrokeColor();
        drawStyle.lineOpa_ = paint.GetOpacity();
        arcInfo.radius = arcParam->radius + ((paint.GetStrokeWidth() + 1) >> 1);

        GetAbsolutePosition(arcParam->center, rect, style, arcInfo.center);
        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                              CapType::CAP_NONE);
    }

    void UICanvas::DoDrawPattern(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                 const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        ImageParam* imageParam = static_cast<ImageParam*>(param);

        if (imageParam->image == nullptr) {
            return;
        }

        Point start;
        GetAbsolutePosition(imageParam->start, rect, style, start);

        BaseGfxExtendEngine::Image imageBuffer((unsigned char*)imageParam->image->GetImageInfo()->data,
                                               imageParam->image->GetImageInfo()->header.width,
                                               imageParam->image->GetImageInfo()->header.height,
                                               imageParam->image->GetImageInfo()->header.width * OHOS::STRIDE_FACTOR);

        if (paint.patternRepeat == paint.REPEAT) {
            paint.GetDrawGraphicsContext()->PatternImageFill(imageBuffer, start.x, start.y, "repeat");
        } else if (paint.patternRepeat == paint.REPEAT_X) {
            paint.GetDrawGraphicsContext()->PatternImageFill(imageBuffer, start.x, start.y, "repeat-x");
        } else if (paint.patternRepeat == paint.REPEAT_Y) {
            paint.GetDrawGraphicsContext()->PatternImageFill(imageBuffer, start.x, start.y, "repeat-y");
        } else {
            paint.GetDrawGraphicsContext()->PatternImageFill(imageBuffer, start.x, start.y, "no-repeat");
        }
    }

    void UICanvas::DoStrokePattern(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                   const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        ImageParam* imageParam = static_cast<ImageParam*>(param);

        if (imageParam->image == nullptr) {
            return;
        }

        Point start;
        GetAbsolutePosition(imageParam->start, rect, style, start);

        Rect cordsTmp;
        cordsTmp.SetPosition(start.x, start.y);
        cordsTmp.SetHeight(imageParam->height);
        cordsTmp.SetWidth(imageParam->width);

        BaseGfxExtendEngine::Image imageBuffer((unsigned char*)imageParam->image->GetImageInfo()->data,
                                               imageParam->image->GetImageInfo()->header.width,
                                               imageParam->image->GetImageInfo()->header.height,
                                               imageParam->image->GetImageInfo()->header.width * OHOS::STRIDE_FACTOR);

        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        m_graphics->SetLineWidth(paint.GetStrokeWidth());
        if (paint.patternRepeat == paint.REPEAT) {
            m_graphics->PatternImageStroke(imageBuffer, start.x, start.y, "repeat");
        } else if (paint.patternRepeat == paint.REPEAT_X) {
            m_graphics->PatternImageStroke(imageBuffer, start.x, start.y, "repeat-x");
        } else if (paint.patternRepeat == paint.REPEAT_Y) {
            m_graphics->PatternImageStroke(imageBuffer, start.x, start.y, "repeat-y");
        } else {
            m_graphics->PatternImageStroke(imageBuffer, start.x, start.y, "no-repeat");
        }
    }

    void UICanvas::DoGradient(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        m_graphics->SetMasterAlpha((double)paint.GetGlobalAlpha());
        GradientControl gradientControl = paint.getGradientControl();
        List<GradientControl::StopAndColor> stopAndColors = gradientControl.getStopAndColor();
        if (stopAndColors.Size() > 0) {
            addColorGradient(*m_graphics, stopAndColors);
        }
        if (gradientControl.gradientflag == gradientControl.Linear) { // 线性渐变
            double x0 = gradientControl.getLinearGradientPoint().x0;
            double y0 = gradientControl.getLinearGradientPoint().y0;
            double x1 = gradientControl.getLinearGradientPoint().x1;
            double y1 = gradientControl.getLinearGradientPoint().y1;

            Point start;
            Point orgstart;
            orgstart.x = x0;
            orgstart.y = y0;
            GetAbsolutePosition(orgstart, rect, style, start);

            Point end;
            Point orgend;
            orgend.x = x1;
            orgend.y = y1;
            GetAbsolutePosition(orgend, rect, style, end);

            m_graphics->SetLinearGradient(start.x, start.y, end.x, end.y);
        }

        if (gradientControl.gradientflag == gradientControl.Radial) { // 放射渐变
            GradientControl::RadialGradientPoint rp = gradientControl.getRadialGradientPoint();

            Point start;
            Point orgstart;
            orgstart.x = rp.x0;
            orgstart.y = rp.y0;
            GetAbsolutePosition(orgstart, rect, style, start);

            Point end;
            Point orgend;
            orgend.x = rp.x1;
            orgend.y = rp.y1;
            GetAbsolutePosition(orgend, rect, style, end);

            m_graphics->SetRadialGradient(start.x, start.y, rp.r0, end.x, end.y, rp.r1);
        }

        if (gradientControl.gradientflag == gradientControl.Solid) { // 纯色渐变
            ColorType color = paint.GetFillColor();
            m_graphics->SetFillColor(BaseGfxExtendEngine::Color(color.red, color.green, color.blue, color.alpha));
        }

        DoDrawPath(gfxDstBuffer, param, paint, rect, invalidatedArea, style);
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)) {
            m_graphics->NoLine();
            m_graphics->DrawPath(BaseGfxExtendEngine::FILLANDSTROKE);
        }
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)) {
            m_graphics->SetLineWidth(paint.GetStrokeWidth());
            m_graphics->Stroke();
        }
    }

    void UICanvas::DoDrawImage(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        ImageParam* imageParam = static_cast<ImageParam*>(param);

        if (imageParam->image == nullptr) {
            return;
        }

        Point start;
        GetAbsolutePosition(imageParam->start, rect, style, start);

        OpacityType opa = paint.GetOpacity();
        if ((imageParam->height == 0) || (imageParam->width == 0)) {
            return;
        }
        const ImageInfo* imgInfo = imageParam->image->GetImageInfo();
        if (imgInfo == nullptr) {
            return;
        }
        uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imgInfo->header.colorMode);
        BaseGfxExtendEngine::Image imageBuffer((unsigned char*)imageParam->image->GetImageInfo()->data,
                                               (int)imageParam->width, (int)imageParam->height,
                                               (int)imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
        BaseGfxExtendEngine* graphics = paint.GetDrawGraphicsContext();
        if (graphics == nullptr) {
            return;
        }
        int16_t width =imageParam->width;
        int16_t height =imageParam->height;
        if(imageParam->newWidth >= 0){
            width = imageParam->newWidth;
        }
        if(imageParam->newHeight >= 0){
            height = imageParam->newHeight;
        }
        Rect trunc(invalidatedArea);
        if (!paint.IsTransform()) {
            double x = start.x;
            double y = start.y;
            double parallelogram[6] = {x, y, x + width, y, x + width, y + height};
            uint8_t formatType = imageParam->image->GetImgType();
            graphics->TransformImage(imageBuffer, parallelogram, formatType != 0);

        } else {
            double x = start.x;
            double y = start.y;

            double parallelogram[6] = {x, y, x + width, y, x + width, y + height};
            uint8_t formatType = imageParam->image->GetImgType();
            StartTransform(rect, invalidatedArea, paint);
            graphics->TransformImage(imageBuffer, parallelogram, formatType != 0);
        }
    }

    void UICanvas::StartTransform(const Rect& rect, const Rect& invalidatedArea, const Paint& paint)
    {
        BaseGfxExtendEngine* graphics = paint.GetDrawGraphicsContext();
        if (graphics == nullptr) {
            return;
        }
        int16_t posViewLeft = paint.GetTransformCenterX() + rect.GetX() - invalidatedArea.GetX();
        int16_t posViewTop = paint.GetTransformCenterY() + rect.GetY() - invalidatedArea.GetY();
        graphics->Translate(-posViewLeft, -posViewTop);
        auto tr = paint.GetTransform();
        graphics->SetAffine(tr);
        graphics->Translate(posViewLeft, posViewTop);
    }

    void UICanvas::DoDrawLabel(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        UILabel* label = static_cast<UILabel*>(param);
        Point startPos = {label->GetX(), label->GetY()};
        Point start;
        GetAbsolutePosition({startPos.x, startPos.y}, rect, style, start);
        label->SetPosition(start.x, start.y);
        label->OnDraw(gfxDstBuffer, invalidatedArea);
        label->SetPosition(startPos.x, startPos.y);
    }

    void UICanvas::BlitMapBuffer(BufferInfo& gfxDstBuffer, BufferInfo& gfxMapBuffer, Rect& textRect,
                                 TransformMap& transMap, const Rect& invalidatedArea)
    {
        Rect invalidRect = textRect;
        if (invalidRect.Intersect(invalidRect, invalidatedArea)) {
            uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(gfxDstBuffer.mode);
            ImageInfo imageInfo;
            imageInfo.header.colorMode = gfxDstBuffer.mode;
            imageInfo.dataSize = gfxMapBuffer.width * gfxMapBuffer.height * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR);
            imageInfo.header.width = gfxMapBuffer.width;
            imageInfo.header.height = gfxMapBuffer.height;
            uint8_t* addr = reinterpret_cast<uint8_t*>(gfxMapBuffer.virAddr);
            imageInfo.data = addr;
            TransformDataInfo imageTranDataInfo = {imageInfo.header, imageInfo.data, pxSize, LEVEL0, BILINEAR};
            BaseGfxEngine::GetInstance()->DrawTransform(gfxDstBuffer, invalidRect, {0, 0}, Color::Black(), OPA_OPAQUE,
                                                        transMap, imageTranDataInfo);
        }
    }

    void UICanvas::DoDrawText(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        TextParam* textParam = static_cast<TextParam*>(param);
        if (textParam == nullptr) {
            return;
        }
        if (textParam->fontStyle.fontSize <= 0) {
            return;
        }
        BaseGfxExtendEngine* graphicsContext = paint.GetDrawGraphicsContext();
        if (graphicsContext == nullptr) {
            return;
        }
        Text* text = textParam->textComment;
        text->SetText(textParam->text);
        text->SetFont(textParam->fontStyle.fontName, textParam->fontStyle.fontSize);
        text->SetDirect(static_cast<UITextLanguageDirect>(textParam->fontStyle.direct));
        text->SetAlign(static_cast<UITextLanguageAlignment>(textParam->fontStyle.align));

        Point start;
        Rect textRect = invalidatedArea;
        GetAbsolutePosition(textParam->position, rect, style, start);
        textRect.SetPosition(start.x, start.y);
        Style drawStyle = style;
        drawStyle.textColor_ = textParam->fontColor;
        drawStyle.lineColor_ = textParam->fontColor;
        drawStyle.bgColor_ = textParam->fontColor;
        drawStyle.SetStyle(STYLE_LETTER_SPACE, textParam->fontStyle.letterSpace);
        text->ReMeasureTextSize(textRect, drawStyle);
        if (text->GetTextSize().x == 0 || text->GetTextSize().y == 0) {
            return;
        }
        textRect.SetWidth(text->GetTextSize().x);
        textRect.SetHeight(text->GetTextSize().y);
        OpacityType opa = DrawUtils::GetMixOpacity(textParam->fontOpa, style.bgOpa_);

        if (!paint.IsTransform()) {
            text->OnDraw(gfxDstBuffer, invalidatedArea, textRect, textRect, 0, drawStyle,
                         Text::TEXT_ELLIPSIS_END_INV, opa);
        } else {
            std::shared_ptr<BufferInfo> pGfxMapBuffer = std::make_shared<BufferInfo>();

            Rect textImageRect(0, 0, textRect.GetWidth(), textRect.GetHeight());

            pGfxMapBuffer->rect = textRect;
            pGfxMapBuffer->width = textRect.GetWidth();
            pGfxMapBuffer->height = textRect.GetHeight();

            uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(gfxDstBuffer.mode);
            uint32_t destStride = pGfxMapBuffer->width * destByteSize;
            pGfxMapBuffer->stride = destStride;

            pGfxMapBuffer->mode = gfxDstBuffer.mode;
            pGfxMapBuffer->color = gfxDstBuffer.color;
            uint32_t buffSize = pGfxMapBuffer->height * pGfxMapBuffer->width * destByteSize;
            pGfxMapBuffer->virAddr = BaseGfxEngine::GetInstance()->AllocBuffer(buffSize, BUFFER_MAP_SURFACE);
            errno_t err = memset_s(pGfxMapBuffer->virAddr, buffSize, 0, buffSize);
            if (err != EOK) {
                BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)pGfxMapBuffer->virAddr);
                GRAPHIC_LOGE("memset_s pGfxMapBuffer fail");
                return;
            }
            pGfxMapBuffer->phyAddr = pGfxMapBuffer->virAddr;

            text->OnDraw(*pGfxMapBuffer, textImageRect, textImageRect, textImageRect, 0, drawStyle,
                         Text::TEXT_ELLIPSIS_END_INV, opa);

            BaseGfxExtendEngine::Image imageBuffer(static_cast<unsigned char*>(pGfxMapBuffer->virAddr),
                                                   pGfxMapBuffer->width, pGfxMapBuffer->height, pGfxMapBuffer->stride);
            double x = start.x;
            double y = start.y;
            double parallelogram[6] = {x, y, x + textRect.GetWidth(), y, x + textRect.GetWidth(), y + textRect.GetHeight()};

            StartTransform(rect, invalidatedArea, paint);
            graphicsContext->TransformImage(imageBuffer, parallelogram, false);
            //graphicsContext->ResetTransformations();
            BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)pGfxMapBuffer->virAddr);
        }
    }

    void UICanvas::DoDrawLineJoin(BufferInfo& gfxDstBuffer, const Point& center, const Rect& invalidatedArea,
                                  const Paint& paint)
    {
        ArcInfo arcinfo = {{0}};
        arcinfo.center = center;
        arcinfo.imgPos = Point{0, 0};
        arcinfo.radius = (paint.GetStrokeWidth() + 1) >> 1;
        arcinfo.startAngle = 0;
        arcinfo.endAngle = CIRCLE_IN_DEGREE;

        Style style;
        style.lineColor_ = paint.GetStrokeColor();
        style.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
        style.lineOpa_ = OPA_OPAQUE;
        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcinfo, invalidatedArea, style, OPA_OPAQUE,
                                              CapType::CAP_NONE);
    }

    bool UICanvas::IsGif(const char* src)
    {
        if (src == nullptr) {
            return false;
        }
        const static uint8_t IMG_BYTES_TO_CHECK = 4; // 4: check 4 bytes of image file
        char buf[IMG_BYTES_TO_CHECK] = {0};
        int32_t fd = open(src, O_RDONLY);
        if (fd < 0) {
            return false;
        }
        if (read(fd, buf, IMG_BYTES_TO_CHECK) != IMG_BYTES_TO_CHECK) {
            close(fd);
            return false;
        }
        close(fd);
        if ((static_cast<uint8_t>(buf[0]) == 0x47) && (static_cast<uint8_t>(buf[1]) == 0x49) &&
            (static_cast<uint8_t>(buf[2]) == 0x46)) { // 2: array index of GIF file's header
            return true;
        }
        return false;
    }

    void UICanvas::DoDrawPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        PathParam* pathParam = static_cast<PathParam*>(param);
        const UICanvasPath* path = pathParam->path;
        if (path == nullptr) {
            return;
        }
        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }
        Point pathEnd = {COORD_MIN, COORD_MIN};

        ListNode<Point>* pointIter = path->points_.Begin();
        ListNode<ArcParam>* arcIter = path->arcParam_.Begin();
        ListNode<PathCmd>* iter = path->cmd_.Begin();
        bool isLineJoin = (paint.GetLineJoin() == BaseGfxExtendEngine::LineJoin::JOINNONE);
        bool isLineCap = (paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CAPNONE);
        if ((!isLineJoin || !isLineCap) &&
            !(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)) &&
            (!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)) &&
            (!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN))) {
            m_graphics->SetLineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green,
                                     paint.GetStrokeColor().blue, paint.GetStrokeColor().alpha);
            m_graphics->SetLineWidth(paint.GetStrokeWidth());
            m_graphics->SetLineCap(paint.GetLineCap());
            m_graphics->SetLineJoin(paint.GetLineJoin());
            if (paint.GetLineJoin() == BaseGfxExtendEngine::JOINMITER) {
                m_graphics->SetMiterLimit(paint.GetMiterLimit());
            }
            m_graphics->NoFill();
        }

        m_graphics->ResetPath();
        for (uint16_t i = 0; (i < pathParam->count) && (iter != path->cmd_.End()); i++, iter = iter->next_) {
            switch (iter->data_) {
                case CMD_MOVE_TO: {
                    Point start;
                    GetAbsolutePosition(pointIter->data_, rect, style, start);
                    if (!isLineJoin || !isLineCap) {
                        m_graphics->MoveTo(start.x, start.y);
                    }
                    pointIter = pointIter->next_;
                    break;
                }
                case CMD_LINE_TO: {
                    Point start = pointIter->prev_->data_;
                    Point end = pointIter->data_;
                    pointIter = pointIter->next_;
                    if ((start.x == end.x) && (start.y == end.y)) {
                        break;
                    }

                    GetAbsolutePosition(start, rect, style, start);
                    GetAbsolutePosition(end, rect, style, end);
                    if (isLineJoin && isLineCap) {
                        BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                               paint.GetStrokeWidth(), paint.GetStrokeColor(),
                                                               OPA_OPAQUE);
                        if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                            DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                        }
                    } else {
                        m_graphics->LineTo(end.x, end.y);
                    }
                    pathEnd = end;
                    break;
                }
                case CMD_ARC: {
                    ArcInfo arcInfo = {{0}};
                    arcInfo.imgPos = Point{0, 0};
                    arcInfo.startAngle = arcIter->data_.startAngle;
                    arcInfo.endAngle = arcIter->data_.endAngle;
                    Style drawStyle = StyleDefault::GetDefaultStyle();
                    drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
                    drawStyle.lineColor_ = paint.GetStrokeColor();
                    drawStyle.lineOpa_ = OPA_OPAQUE;
                    arcInfo.radius = arcIter->data_.radius + ((paint.GetStrokeWidth() + 1) >> 1);

                    GetAbsolutePosition(arcIter->data_.center, rect, style, arcInfo.center);
                    if (isLineJoin && isLineCap) {
                        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle,
                                                              OPA_OPAQUE, CapType::CAP_NONE);
                        if (pointIter != path->points_.Begin()) {
                            DoDrawLineJoin(gfxDstBuffer, pathEnd, invalidatedArea, paint);
                        }
                    } else {
                        m_graphics->ArcTo(arcInfo.radius, arcInfo.radius,
                                          BaseGfxExtendEngine::Deg2Rad(arcInfo.endAngle - arcInfo.startAngle), 0, 1,
                                          arcInfo.center.x, arcInfo.center.y);
                    }
                    GetAbsolutePosition(pointIter->data_, rect, style, pathEnd);

                    pointIter = pointIter->next_;
                    arcIter = arcIter->next_;
                    break;
                }
                case CMD_CLOSE: {
                    Point start = pointIter->prev_->data_;
                    Point end = pointIter->data_;
                    GetAbsolutePosition(start, rect, style, start);
                    GetAbsolutePosition(end, rect, style, end);
                    if (!isLineJoin || !isLineCap) {
                        m_graphics->LineTo(end.x, end.y);
                    } else {
                        if ((start.x != end.x) || (start.y != end.y)) {
                            BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                                   paint.GetStrokeWidth(), paint.GetStrokeColor(),
                                                                   OPA_OPAQUE);
                            if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                                DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                            }
                            pathEnd = end;
                        }

                        if ((pathEnd.x == end.x) && (pathEnd.y == end.y)) {
                            DoDrawLineJoin(gfxDstBuffer, end, invalidatedArea, paint);
                        }
                    }
                    pointIter = pointIter->next_;
                    break;
                }
                default: break;
            }
        }
        if ((!isLineJoin || !isLineCap) &&
            !(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)) &&
            (!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)) &&
            (!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN))) {
            m_graphics->DrawPath(BaseGfxExtendEngine::DrawPathFlag::STROKEONLY);
        }
    }

    void UICanvas::DoFillPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        PathParam* pathParam = static_cast<PathParam*>(param);
        const UICanvasPath* path = pathParam->path;
        if (path == nullptr) {
            return;
        }
        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if (m_graphics == nullptr) {
            return;
        }
        if (paint.IsLineDash()) {
            m_graphics->SetLineDashOffset(paint.GetLineDashOffset());
            m_graphics->SetLineDash(paint.GetLineDash(), paint.GetLineDashCount());
        } else {
            m_graphics->SetLineDash(nullptr, 0);
        }


        Point pathEnd = {COORD_MIN, COORD_MIN};

        ListNode<Point>* pointIter = path->points_.Begin();
        ListNode<ArcParam>* arcIter = path->arcParam_.Begin();
        ListNode<PathCmd>* iter = path->cmd_.Begin();
        bool isLineJoin = (paint.GetLineJoin() == BaseGfxExtendEngine::LineJoin::JOINNONE);
        bool isLineCap = (paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CAPNONE);
        if (!isLineJoin || !isLineCap) {
            m_graphics->SetLineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green,
                                     paint.GetStrokeColor().blue, paint.GetStrokeColor().alpha);
            m_graphics->SetLineWidth(paint.GetStrokeWidth());
            m_graphics->SetLineCap(paint.GetLineCap());
            m_graphics->SetLineJoin(paint.GetLineJoin());
            if (paint.GetLineJoin() == BaseGfxExtendEngine::JOINMITER) {
                m_graphics->SetMiterLimit(paint.GetMiterLimit());
            }
        }
        if (paint.GetFillColor().alpha) {
            m_graphics->SetFillColor(paint.GetFillColor().red, paint.GetFillColor().green, paint.GetFillColor().blue,
                                     paint.GetFillColor().alpha);
        }
        if (paint.GetStrokeColor().alpha) {
            m_graphics->SetLineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green,
                                     paint.GetStrokeColor().blue, paint.GetStrokeColor().alpha);
            m_graphics->SetLineWidth(paint.GetStrokeWidth());
        }
        m_graphics->ResetPath();
        for (uint16_t i = 0; (i < pathParam->count) && (iter != path->cmd_.End()); i++, iter = iter->next_) {
            switch (iter->data_) {
                case CMD_MOVE_TO: {
                    Point start;
                    GetAbsolutePosition(pointIter->data_, rect, style, start);
                    if (!isLineJoin || !isLineCap) {
                        m_graphics->MoveTo(start.x, start.y);
                    }
                    pointIter = pointIter->next_;
                    break;
                }
                case CMD_LINE_TO: {
                    Point start = pointIter->prev_->data_;
                    Point end = pointIter->data_;
                    pointIter = pointIter->next_;
                    if ((start.x == end.x) && (start.y == end.y)) {
                        break;
                    }

                    GetAbsolutePosition(start, rect, style, start);
                    GetAbsolutePosition(end, rect, style, end);
                    if (isLineJoin && isLineCap) {
                        BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                               paint.GetStrokeWidth(), paint.GetStrokeColor(),
                                                               OPA_OPAQUE);
                        if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                            DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                        }
                    } else {
                        m_graphics->LineTo(end.x, end.y);
                    }
                    pathEnd = end;
                    break;
                }
                case CMD_ARC: {
                    ArcInfo arcInfo = {{0}};
                    arcInfo.imgPos = Point{0, 0};
                    arcInfo.startAngle = arcIter->data_.startAngle;
                    arcInfo.endAngle = arcIter->data_.endAngle;
                    Style drawStyle = StyleDefault::GetDefaultStyle();
                    drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
                    drawStyle.lineColor_ = paint.GetStrokeColor();
                    drawStyle.lineOpa_ = OPA_OPAQUE;
                    arcInfo.radius = arcIter->data_.radius + ((paint.GetStrokeWidth() + 1) >> 1);

                    GetAbsolutePosition(arcIter->data_.center, rect, style, arcInfo.center);
                    if (isLineJoin && isLineCap) {
                        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle,
                                                              OPA_OPAQUE, CapType::CAP_NONE);
                        if (pointIter != path->points_.Begin()) {
                            DoDrawLineJoin(gfxDstBuffer, pathEnd, invalidatedArea, paint);
                        }
                    } else {
                        m_graphics->ArcTo(arcInfo.radius, arcInfo.radius,
                                          BaseGfxExtendEngine::Deg2Rad(arcInfo.endAngle - arcInfo.startAngle), 0, 1,
                                          arcInfo.center.x, arcInfo.center.y);
                    }
                    GetAbsolutePosition(pointIter->data_, rect, style, pathEnd);

                    pointIter = pointIter->next_;
                    arcIter = arcIter->next_;
                    break;
                }
                case CMD_CLOSE: {
                    Point start = pointIter->prev_->data_;
                    Point end = pointIter->data_;
                    GetAbsolutePosition(start, rect, style, start);
                    GetAbsolutePosition(end, rect, style, end);
                    if (!isLineJoin || !isLineCap) {
                        m_graphics->LineTo(end.x, end.y);
                    } else {
                        if ((start.x != end.x) || (start.y != end.y)) {
                            BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                                   paint.GetStrokeWidth(), paint.GetStrokeColor(),
                                                                   OPA_OPAQUE);
                            if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                                DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                            }
                            pathEnd = end;
                        }

                        if ((pathEnd.x == end.x) && (pathEnd.y == end.y)) {
                            DoDrawLineJoin(gfxDstBuffer, end, invalidatedArea, paint);
                        }
                    }
                    m_graphics->ClosePolygon();
                    pointIter = pointIter->next_;
                    break;
                }
                default: break;
            }
        }

        if (paint.GetShadowOffsetX() != 0 || paint.GetShadowOffsetY() != 0) {
            double transFormCenterX = 0, transFormCenterY = 0, rotateAngle = 0;
            transFormCenterX = paint.GetTransformCenterX() + rect.GetX() - invalidatedArea.GetX();
            transFormCenterY = paint.GetTransformCenterY() + rect.GetY() - invalidatedArea.GetY();
            if (paint.GetRotateAngle() != 0) {
                rotateAngle = paint.GetRotateAngle();
            }
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                       paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
            m_graphics->DrawShadow(transFormCenterX, transFormCenterY, rotateAngle, paint.GetScaleX(),
                                   paint.GetScaleY(), paint.GetTransLateX(),
                                   paint.GetTransLateY());
        }

        StartTransform(rect, invalidatedArea, paint);
        setGradient(*m_graphics, paint, rect, style); // 填充颜色
        m_graphics->DrawPath(BaseGfxExtendEngine::FILLANDSTROKE);
    }
} // namespace OHOS
