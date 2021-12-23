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

    void UICanvas::BeginPath()
    {
        //初始化UICanvasPath
        if (vertices_ != nullptr) {
            delete vertices_;
            vertices_ = nullptr;
        }

        vertices_ = new UICanvasVertices();
        if (vertices_ == nullptr) {
            GRAPHIC_LOGE("new UICanvasPath fail");
            return;
        }

    }

    void UICanvas::MoveTo(const Point& point)
    {
        if(vertices_==nullptr){
            return ;
        }
        vertices_->MoveTo(point.x,point.y);
    }

    void UICanvas::LineTo(const Point& point)
    {
        if(vertices_==nullptr){
            return ;
        }
        vertices_->LineTo(point.x,point.y);
    }

    void UICanvas::ArcTo(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle)
    {
        if (vertices_ == nullptr) {
            return;
        }
        vertices_->ArcTo(radius, radius,(startAngle - endAngle)* OHOS::PI / OHOS::BOXER, 0, 1,center.x, center.y);
    }
    void UICanvas::AddRect(const Point& point, int16_t height, int16_t width)
    {
        if (vertices_ == nullptr) {
            return;
        }

        MoveTo(point);
        LineTo({static_cast<int16_t>(point.x + width), point.y});
        LineTo({static_cast<int16_t>(point.x + width), static_cast<int16_t>(point.y + height)});
        LineTo({point.x, static_cast<int16_t>(point.y + height)});
        ClosePath();
    }

    void UICanvas::ClosePath()
    {
        if (vertices_ == nullptr) {
            return;
        }
        vertices_->ClosePolygon();
    }

    UICanvas::~UICanvas()
    {
        if (vertices_ != nullptr) {
            delete vertices_;
            vertices_ = nullptr;
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
    }

    void UICanvas::Clear()
    {
        if (vertices_ != nullptr) {
            delete vertices_;
            vertices_ = nullptr;
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
        if (!paint.GetChangeFlag()&&(paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CAPNONE)) {
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
        }else {
            vertices_->RemoveAll();
            vertices_->MoveTo(startPoint_.x,startPoint_.y);
            vertices_->LineTo(startPoint_.x,startPoint_.y);
            Stroke(paint);
        }
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
        if (!paint.GetChangeFlag()&&(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE)) {
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
            Invalidate();

        }else{
            vertices_->RemoveAll();
            vertices_->MoveTo(startPoint.x,startPoint.y);
            vertices_->LineTo(startPoint.x+width,startPoint.y);
            vertices_->LineTo(startPoint.x+width,startPoint.y+height);
            vertices_->LineTo(startPoint.x,startPoint.y+height);
            vertices_->ClosePolygon();
            Stroke(paint);
        }
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
    }

    void UICanvas::DrawRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
    {
        if(paint.GetChangeFlag()){
            vertices_->RemoveAll();
            vertices_->MoveTo(startPoint.x,startPoint.y);
            vertices_->LineTo(startPoint.x+width,startPoint.y);
            vertices_->LineTo(startPoint.x+width,startPoint.y+height);
            vertices_->LineTo(startPoint.x,startPoint.y+height);
            vertices_->ClosePolygon();
            Fill(paint);
            Stroke(paint);
        }else{
            if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE)) {
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
            if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
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
    }

    void UICanvas::DrawCircle(const Point& center, uint16_t radius, const Paint& paint)
    {

        if(paint.GetChangeFlag()){
            vertices_->RemoveAll();
            OHOS::BezierArc arc(center.x, center.y, radius, radius, 0, OHOS::TWO_TIMES * PI);
            vertices_->ConcatPath(arc, 0);
            vertices_->ClosePolygon();
            Stroke(paint);
            Fill(paint);
        }else{
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
        }
        Invalidate();
    }

    void UICanvas::DrawSector(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle,
                              const Paint& paint)
    {
        //TODO::需要添加操作
        if ((!paint.GetChangeFlag())&&(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE)) {
            Paint innerPaint = paint;
            innerPaint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
            innerPaint.SetStrokeWidth(radius);
            innerPaint.SetStrokeColor(paint.GetFillColor());
            radius >>= 1;
            DrawArc(center, radius, startAngle, endAngle, innerPaint);
        }else{
            vertices_->RemoveAll();
            vertices_->MoveTo(center.x,center.y);
            vertices_->ArcTo(radius, radius,(startAngle - endAngle)* OHOS::PI / OHOS::BOXER, 0, 1,center.x, center.y);
            vertices_->ClosePolygon();
            Fill(paint);
        }
    }

    void UICanvas::DrawArc(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle,
                           const Paint& paint)
    {

        if (!paint.GetChangeFlag()&&((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE))) {
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
        }else{
            vertices_->RemoveAll();
            vertices_->ArcTo(radius, radius,(startAngle - endAngle)* OHOS::PI / OHOS::BOXER, 0, 1,center.x, center.y);
            Stroke(paint);
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

    void UICanvas::DrawImage(const Point& startPoint, const char* image, const Paint& paint)
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
        imageParam->path = image;
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

        if (IsGif(image)) {
            imageParam->gifImageAnimator = new GifCanvasImageAnimator(imageParam, this, image);
            imageParam->gifImageAnimator->Start();
        }
        drawCmdList_.PushBack(cmd);

        Invalidate();
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
        Fill(paint);
    }
    void UICanvas::DrawPath(const Paint& paint)
    {
        Stroke(paint);
    }

    void UICanvas::OnDraw(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea)
    {
        Rect rect = GetOrigRect();
        BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, rect, invalidatedArea, *style_, opaScale_);

        void* param = nullptr;
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        Rect coords = GetOrigRect();
        Rect trunc = invalidatedArea;
        if (trunc.Intersect(trunc, coords)) {
            for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
                param = curDraw->data_.param;
                curDraw->data_.DrawGraphics(gfxDstBuffer, param, curDraw->data_.paint, rect, trunc, *style_);
            }
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
        BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea, paint.GetStrokeWidth(),
                                               paint.GetStrokeColor(), paint.GetOpacity());
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
    }

    void UICanvas::DoDrawRect(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style)
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

    void UICanvas::DoFillRect(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style)
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
        Point start;
        GetAbsolutePosition(rectParam->start, rect, style, start);

        Rect coords;
        coords.SetPosition(start.x + (lineWidth + 1) / 2, start.y + (lineWidth + 1) / 2); // 2: half
        coords.SetHeight(rectParam->height - lineWidth);
        coords.SetWidth(rectParam->width - lineWidth);

        Style drawStyle = StyleDefault::GetDefaultStyle();
        drawStyle.bgColor_ = paint.GetFillColor();
        drawStyle.bgOpa_ = paint.GetOpacity();
        drawStyle.borderRadius_ = 0;
        BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, coords, invalidatedArea, drawStyle, OPA_OPAQUE);
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

    }

    void UICanvas::StrokePattern(const Paint& paint)
    {

    }
    void UICanvas::Gradient(const Paint& paint)
    {

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
        if (static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_STYLE)) {



            PathParam* pathParam = new PathParam;
            if (pathParam == nullptr) {
                GRAPHIC_LOGE("new LineParam fail");
                return;
            }
            pathParam->path = vertices_;

            DrawCmd cmd;
            cmd.paint = paint;
            cmd.param = pathParam;
            cmd.DeleteParam = DeleteLineParam;
            cmd.DrawGraphics = DoStroke;
            drawCmdList_.PushBack(cmd);
            Invalidate();




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

    void UICanvas::DoDrawCircle(BufferInfo& gfxDstBuffer,
                                void* param,
                                const Paint& paint,
                                const Rect& rect,
                                const Rect& invalidatedArea,
                                const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        CircleParam* circleParam = static_cast<CircleParam*>(param);

        Style drawStyle = StyleDefault::GetDefaultStyle();
        drawStyle.lineOpa_ = paint.GetOpacity();

        ArcInfo arcInfo = {{0}};
        arcInfo.imgPos = Point{0, 0};
        arcInfo.startAngle = 0;
        arcInfo.endAngle = CIRCLE_IN_DEGREE;
        GetAbsolutePosition(circleParam->center, rect, style, arcInfo.center);
        uint8_t enableStroke = static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE;
        uint16_t halfLineWidth = enableStroke ? (paint.GetStrokeWidth() >> 1) : 0;
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
            arcInfo.radius = circleParam->radius - halfLineWidth;
            drawStyle.lineWidth_ = arcInfo.radius;
            drawStyle.lineColor_ = paint.GetFillColor();
            BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                  CapType::CAP_NONE);
        }

        if (enableStroke) {
            arcInfo.radius = circleParam->radius + halfLineWidth - 1;
            drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
            drawStyle.lineColor_ = paint.GetStrokeColor();
            BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                  CapType::CAP_NONE);
        }
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

    void UICanvas::DoStroke(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                   const Rect& invalidatedArea, const Style& style)
    {
        if (param == nullptr) {
            return;
        }
        TransAffine transform;
        RenderingBuffer renderBuffer;
         //初始化buffer和 m_transform
        InitRendAndTransform(gfxDstBuffer,renderBuffer,rect,transform,style);

        //路径
        typedef OHOS::DepictCurve<OHOS::UICanvasVertices> ConvCurve;
        typedef OHOS::DepictStroke<ConvCurve> ConvStroke;
        typedef OHOS::DepictDash<ConvCurve> ConvDashCurve;
        typedef OHOS::DepictStroke<ConvDashCurve> ConvDashStroke;

        typedef OHOS::DepictTransform<ConvCurve> PathTransform;
        typedef OHOS::DepictTransform<ConvStroke> StrokeTransform;
        typedef OHOS::DepictTransform<ConvDashStroke> DashStrokeTransform;

        PathParam* pathParam = static_cast<PathParam*>(param);
        ConvCurve convCurve(*pathParam->path);
//        PathTransform pathtransform(convCurve,m_transform);
        ConvStroke convStroke(convCurve);
        convStroke.Width(paint.GetStrokeWidth());//线条样式相关
        convStroke.LineCap(OHOS::LineCapEnum::ROUND_CAP);
        convStroke.LineJoin(OHOS::LineJoinEnum::ROUND_JOIN);

        OHOS::RasterizerScanlineAntiAlias<> rasterizer;
        OHOS::ScanlineUnPackedContainer m_scanline;
        rasterizer.Reset();
        StrokeTransform strokeTransform(convStroke,transform);

        ConvDashCurve dashCurve(convCurve);
        ConvDashStroke dashStroke(dashCurve);

        rasterizer.AddPath(strokeTransform);

        //组装renderbase

        typedef OHOS::Rgba8 ColorType;
        // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
        typedef OHOS::OrderBgra ComponentOrder;
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
        typedef OHOS::BlenderRgba<ColorType, ComponentOrder> Blender;
        typedef OHOS::PixfmtAlphaBlendRgba<Blender, OHOS::RenderingBuffer> PixFormat;
        typedef OHOS::RendererBase<PixFormat> RendererBase;



        PixFormat m_pixFormat(renderBuffer);
        RendererBase m_renBase(m_pixFormat);

        m_renBase.ResetClipping(true);
        m_renBase.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());
        ColorType fillColor(paint.GetStrokeColor().red,paint.GetStrokeColor().green,paint.GetStrokeColor().blue,paint.GetStrokeColor().alpha);
        OHOS::RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, m_renBase, fillColor);

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
        Rect trunc(invalidatedArea);
        if (!paint.IsTransform()) {
            graphics->BlendFromImage(imageBuffer, start.x, start.y, opa);
        } else {
            double x = start.x;
            double y = start.y;
            double parallelogram[6] = {x, y, x + imageParam->width, y, x + imageParam->width, y + imageParam->height};
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
        //TODO::需要补齐代码
    }

    void UICanvas::DoFillPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style)
    {
        //TODO::需要补齐代码
    }

    void UICanvas::InitRendAndTransform(BufferInfo& gfxDstBuffer,RenderingBuffer& renderBuffer, const Rect& rect,
                                        TransAffine& transform, const Style& style)
    {
        int16_t realLeft = rect.GetLeft() + style.paddingLeft_ + style.borderWidth_;
        int16_t realTop = rect.GetTop() + style.paddingTop_ + style.borderWidth_;
        transform.Reset();
        transform.Translate(realLeft,realTop);//偏移到画布上
        renderBuffer.Attach(static_cast<uint8_t*>(gfxDstBuffer.virAddr),gfxDstBuffer.width,gfxDstBuffer.height,gfxDstBuffer.stride);
    }

} // namespace OHOS
