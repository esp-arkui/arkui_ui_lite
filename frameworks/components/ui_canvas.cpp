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
#include <utility>
#include "components/ui_canvas.h"
#include "common/image.h"
#include "draw/draw_arc.h"
#include "draw/draw_image.h"
#include "engines/gfx/gfx_engine_manager.h"
#include "gfx_utils/graphic_log.h"
#include <agg_gradient_lut.h>
#include <components/ui_view_group.h>
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

void UICanvas::AddRect(const Point& point, int16_t height, int16_t width,const Paint& paint)
{
    AddRect(point,height,width);

    paint.GetDrawGraphicsContext()->rectangle(point.x,point.y,point.x+width,point.y+height);

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

void UICanvas::DrawCurve(const Point& startPoint,
                         const Point& control1,
                         const Point& control2,
                         const Point& endPoint,
                         const Paint& paint)
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
}

void UICanvas::ClearRect(const Point &clearRect, int clearHeight,int clearWidth,
                         const Paint &paint)
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
    //OpacityType opa = DrawUtils::GetMixOpacity(this->opaScale_, this->style_->bgOpa_);
    cmd.paint.SetOpacity(this->opaScale_);
    cmd.param = clearRectParam;
    cmd.DeleteParam = DeleteRectParam;
    cmd.DrawGraphics = DoClearRect;
    drawCmdList_.PushBack(cmd);
    Invalidate();
}

void UICanvas::DrawRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
{
    if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE)||(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_GRADIENT)) {
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

    if ((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE)||(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_GRADIENT)) {
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

void UICanvas::DrawSector(const Point& center,
                          uint16_t radius,
                          int16_t startAngle,
                          int16_t endAngle,
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

void UICanvas::DrawArc(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle, const Paint& paint)
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

void UICanvas::DrawLabel(const Point& startPoint,
                         const char* text,
                         uint16_t maxWidth,
                         const FontStyle& fontStyle,
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

    void* param = nullptr;
    ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
    Rect coords = GetOrigRect();
    Rect trunc = invalidatedArea;
    if (trunc.Intersect(trunc, coords)) {

        int16_t posViewLeft=rect.GetX()-trunc.GetX();
        int16_t posViewTop=rect.GetY()-trunc.GetY();
        int16_t realLeft=rect.GetLeft() + style_->paddingLeft_ + style_->borderWidth_;
        int16_t realTop=rect.GetTop() + style_->paddingTop_ + style_->borderWidth_;
        std::unique_ptr<BufferInfo> gfxMapBuffer(new BufferInfo);
        if (memcpy_s(gfxMapBuffer.get(), sizeof(BufferInfo), &gfxDstBuffer, sizeof(BufferInfo)) != 0) {
            return;
        }
//        //BufferInfo* modeBuff = BaseGfxEngine::GetInstance()->GetFBBufferInfo();
        uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(gfxDstBuffer.mode);
//        //uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(gfxDstBuffer.mode);


        uint32_t destStride= gfxMapBuffer->width* destByteSize;
        uint32_t buffSize = gfxMapBuffer->height * destStride;
        gfxMapBuffer->virAddr = BaseGfxEngine::GetInstance()->AllocBuffer(
                    buffSize, BUFFER_MAP_SURFACE);
        memset_s(gfxMapBuffer->virAddr, buffSize, 0, buffSize);
        //BaseGfxEngine::GetInstance()->Fill(*gfxMapBuffer, invalidatedArea, Color::GetColorFromRGBA(0,0,0,0), opaScale_);

        gfxMapBuffer->phyAddr = gfxMapBuffer->virAddr;
        //添加的处理机制的。。。
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {

            //应该是实现画布的处理机制..
            param = curDraw->data_.param;

            InitDrawEnvironment(*gfxMapBuffer,trunc,
                                Rect(realLeft,
                                     realTop,
                                     realLeft+trunc.GetWidth() - 1,
                                     realTop+trunc.GetHeight() - 1),
                                Rect(posViewLeft,
                                     posViewTop,
                                     posViewLeft+trunc.GetWidth() - 1,
                                     posViewTop+trunc.GetHeight() - 1),curDraw->data_.paint);

            curDraw->data_.DrawGraphics(*gfxMapBuffer, param, curDraw->data_.paint, rect, trunc, *style_);

        }
        BaseGfxExtendEngine::Image imageBuffer((unsigned char*)gfxMapBuffer->virAddr,
                                                       gfxMapBuffer->width,gfxMapBuffer->height,
                                                       gfxMapBuffer->stride);
        BaseGfxExtendEngine m_graphics_Image;

        uint8_t* destBuf = static_cast<uint8_t*>(gfxDstBuffer.virAddr);
        if (gfxDstBuffer.virAddr == nullptr) {
            return;
        }

        ColorMode mode = gfxDstBuffer.mode;
        int32_t offset = static_cast<int32_t>(trunc.GetTop()) * gfxDstBuffer.width +
                trunc.GetLeft();
        destBuf += offset * destByteSize;

        m_graphics_Image.attach(destBuf,trunc.GetWidth(),
                           trunc.GetHeight(),gfxDstBuffer.stride);
        m_graphics_Image.viewport(realLeft,
                                  realTop,
                                  realLeft+trunc.GetWidth() - 1,
                                  realTop+trunc.GetHeight() - 1,
                                  posViewLeft,posViewTop,posViewLeft+trunc.GetWidth() - 1,
                                  posViewTop+trunc.GetHeight() - 1,
                             BaseGfxExtendEngine::XMinYMin);
                             //BaseGfxExtendEngine::XMidYMid);
        OpacityType opa = DrawUtils::GetMixOpacity(opaScale_, style_->imageOpa_);

        m_graphics_Image.BlendFromImage(imageBuffer,gfxMapBuffer->rect.GetLeft(),
                                            gfxMapBuffer->rect.GetTop(),opa);

//       ImageInfo imageInfo;
//       imageInfo.header.colorMode = gfxMapBuffer->mode;
//       imageInfo.dataSize = gfxMapBuffer->width * gfxMapBuffer->height *
//               DrawUtils::GetByteSizeByColorMode(imageInfo.header.colorMode);
//       imageInfo.header.width = gfxMapBuffer->width;
//       imageInfo.header.height = gfxMapBuffer->height;
//       imageInfo.header.reserved = 0;

//       imageInfo.data = reinterpret_cast<uint8_t*>(gfxMapBuffer->virAddr);

//       DrawImage::DrawCommon(gfxDstBuffer, Rect(gfxDstBuffer.rect.GetLeft(),
//                                                gfxDstBuffer.rect.GetTop(),
//                                                gfxDstBuffer.rect.GetRight(),
//                                                gfxDstBuffer.rect.GetBottom()),
//                             Rect(gfxMapBuffer->rect.GetLeft(),
//                                  gfxMapBuffer->rect.GetTop(),
//                                  gfxMapBuffer->rect.GetRight(),
//                                  gfxMapBuffer->rect.GetBottom()),
//                             &imageInfo,*style_, opaScale_);

        BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)gfxMapBuffer->virAddr);

    }
}

bool UICanvas::InitDrawEnvironment(const BufferInfo& gfxDstBuffer,const Rect& fillArea,const Rect &worldRect,
                                   const Rect &screenRect,const Paint& paint)
{

    BaseGfxExtendEngine* m_graphics= paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return false;
    }
//    BaseGfxExtendEngine* m_graphics_Image = paint.GetImageBufferContext();
//    if(m_graphics_Image==nullptr) {
//        return false;
//    }
    int16_t posLeft= fillArea.GetLeft();// + style_->paddingLeft_ + style_->borderWidth_;
    int16_t posTop= fillArea.GetTop();// + style_->paddingTop_ + style_->borderWidth_;

    uint8_t* destBuf = static_cast<uint8_t*>(gfxDstBuffer.virAddr);
    if (gfxDstBuffer.virAddr == nullptr) {
        return false;
    }

    ColorMode mode = gfxDstBuffer.mode;
    uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(mode);
    int32_t offset = static_cast<int32_t>(posTop) * gfxDstBuffer.width +
            posLeft;
    destBuf += offset * destByteSize;

    m_graphics->attach(destBuf,fillArea.GetWidth(),
                       fillArea.GetHeight(),gfxDstBuffer.stride);
    m_graphics->viewport(worldRect.GetLeft(),worldRect.GetTop(),worldRect.GetRight(),worldRect.GetBottom(),
                         screenRect.GetLeft(),screenRect.GetTop(),screenRect.GetRight(),screenRect.GetBottom(),
                         BaseGfxExtendEngine::XMinYMin);
                         //BaseGfxExtendEngine::XMidYMid);
    return true;

}

void UICanvas::SetLineDash(float *dashArray, unsigned int ndash,Paint& paint)
{
    paint.SetLineDash(dashArray,ndash);
}

void UICanvas::GetAbsolutePosition(const Point& prePoint, const Rect& rect, const Style& style, Point& point)
{
    point.x = prePoint.x + rect.GetLeft() + style.paddingLeft_ + style.borderWidth_;
    point.y = prePoint.y + rect.GetTop() + style.paddingTop_ + style.borderWidth_;
}

void UICanvas::DoDrawLine(BufferInfo& gfxDstBuffer,
                          void* param,
                          const Paint& paint,
                          const Rect& rect,
                          const Rect& invalidatedArea,
                          const Style& style)
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
    if(m_graphics==nullptr) {
        return;
    }
    if(paint.GetLineCap()==BaseGfxExtendEngine::LineCap::CapNone) {

        BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea, paint.GetStrokeWidth(),
                                               paint.GetStrokeColor(), paint.GetOpacity());
    } else {
        ColorType colorType=paint.GetStrokeColor();
        m_graphics->lineColor(colorType.red, colorType.green, colorType.blue,colorType.alpha);
        m_graphics->lineWidth(paint.GetStrokeWidth());
        m_graphics->lineCap(paint.GetLineCap());
        m_graphics->noFill();
        m_graphics->line(start.x,start.y,end.x,end.y);
    }
}

void UICanvas::DoDrawCurve(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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
                                                  paint.GetStrokeWidth(), paint.GetStrokeColor(), paint.GetOpacity());
}

void UICanvas::DoClearRect(BufferInfo &gfxDstBuffer, void *param, const Paint &paint, const Rect &rect, const Rect &invalidatedArea, const Style &style)
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
    BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer,
                                           Rect(start.x,start.y,start.x + rectParam->width -1,start.y+
                                                rectParam->height -1),invalidatedArea,
                                           fillStyle, fillStyle.bgOpa_);
}

void UICanvas::DoStrokeRect(BufferInfo &gfxDstBuffer, void *param, const Paint &paint, const Rect &rect, const Rect &invalidatedArea, const Style &style)
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
    BaseGfxExtendEngine* m_graphics=
        paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }
    if(paint.IsLineDash()) {
        m_graphics->lineDashOffset(paint.GetLineDashOffset());
        m_graphics->SetLineDash(paint.GetLineDash(),paint.GetLineDashCount());
    } else {
        m_graphics->SetLineDash(nullptr,0);
    }
    m_graphics->lineColor(drawStyle.bgColor_.red, drawStyle.bgColor_.green, drawStyle.bgColor_.blue,drawStyle.bgOpa_);
    m_graphics->lineWidth(lineWidth);
    if(paint.GetStyle() == Paint::PaintStyle::STROKE_STYLE) {
        m_graphics->noFill();
    }

    m_graphics->rectangle(start.x,start.y,start.x + rectParam->width -1,start.y+
                          rectParam->height -1);

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

    if(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_GRADIENT){

        BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
        if(m_graphics==nullptr) {
            return;
        }
        Point start;
        GetAbsolutePosition(rectParam->start, rect, style, start);
        setGradient(*m_graphics,paint,rect,style);//填充颜色
        m_graphics->lineWidth(lineWidth);
        m_graphics->rectstroke(start.x,start.y,start.x+rectParam->width,start.y+rectParam->height);
    }else{
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

    BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }


    if((static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE)||(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_GRADIENT)){
        Point start;
        GetAbsolutePosition(rectParam->start, rect, style, start);

        if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                        paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
        }

        m_graphics->masterAlpha((double)paint.GetGlobalAlpha());
        m_graphics->noLine();
        setGradient(*m_graphics,paint,rect,style);//填充颜色
        m_graphics->rectangle(start.x,start.y,start.x+rectParam->width,start.y+rectParam->height);

    }
}

void UICanvas::addColorGradient(BaseGfxExtendEngine & m_graphics,List<GradientControl::StopAndColor> & stopAndColors){
    m_graphics.remove_all_color();
    ListNode<GradientControl::StopAndColor>* iter = stopAndColors.Begin();
    uint16_t count=0;
    for (; count <stopAndColors.Size(); count++) {
       ColorType stopColor = iter->data_.color;
        m_graphics.add_color(iter->data_.stop,BaseGfxExtendEngine::Color(stopColor.red,  stopColor.green,stopColor.blue,stopColor.alpha));
        iter = iter->next_;
    }
    m_graphics.build_lut();
}


void UICanvas::fill(const Paint& paint)
{


    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN)){

        if (strcmp(paint.image,"")==0) {
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
        imageParam->start = {0,0};
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
    }

    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)){

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

    }

    Invalidate();
}

void UICanvas::stroke(const Paint& paint)
{
    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN)){

        if (strcmp(paint.image,"")==0) {
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
        imageParam->start = {0,0};
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

    }

    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)){

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

    }


    Invalidate();

}


void UICanvas::fill(const Paint& paint,const PolygonPath * polygonPath)
{

    if(!(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::PATTERN)){
        return ;
    }

    if (paint.image == nullptr) {
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
    imageParam->start = {0,0};
    imageParam->height = header.height;
    imageParam->width = header.width;

    Point start{0,0};

    if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::PATTERN)
    {
        if(paint.patternRepeat==paint.REPEAT){
            for(;;){

                if(start.y>GetHeight()){
                    break;
                }
                Point temp = start;
                for(;;){

                    if(temp.x>GetWidth()){
                        break;
                    }
                    UIImageView* imageVIew = new UIImageView();
                    imageVIew->SetSrc(paint.image);
                    Images_ images_;
                    images_.startp={temp.x, temp.y};
                    images_.img=imageVIew;
                    imageList_.PushBack(images_);
                    temp.x+=imageParam->width;
                }
                start.y+=imageParam->height;
            }
        }else if(paint.patternRepeat==paint.REPEAT_X) {
            for(;;){
                if(start.x>GetWidth()){
                    break;
                }
                UIImageView* imageVIew = new UIImageView();
                imageVIew->SetSrc(paint.image);
                Images_ images_;
                images_.startp={start.x, start.y};
                images_.img=imageVIew;
                imageList_.PushBack(images_);
                start.x+=imageParam->width;
            }
        }else if(paint.patternRepeat==paint.REPEAT_Y) {
            for(;;){
                if(start.y>GetHeight()){
                    break;
                }
                UIImageView* imageVIew = new UIImageView();
                imageVIew->SetSrc(paint.image);
                Images_ images_;
                images_.startp={start.x, start.y};
                images_.img=imageVIew;
                imageList_.PushBack(images_);
                start.y+=imageParam->height;
            }

        }else if(paint.patternRepeat==paint.NO_REPEAT) {
            UIImageView* imageVIew = new UIImageView();
            imageVIew->SetSrc(paint.image);
            Images_ images_;
                images_.startp={start.x, start.y};
                images_.img=imageVIew;
                imageList_.PushBack(images_);
        }
    }

    auto  images =imageList_.Begin();
    if(polygonPath!=nullptr){
        const ImageInfo *  imginfo= images->data_.img->GetImageInfo();
        PolygonUtils polygonUtils;
        int16_t h = images->data_.img->GetImageInfo()->header.height;
        int16_t w = images->data_.img->GetImageInfo()->header.width;
        int16_t img_h = this->GetHeight()/imginfo->header.height+1;
        int16_t img_w = this->GetWidth()/imginfo->header.width+1;
        PolygonImageBlitter blitter(imageList_,w,h,img_w,img_h);
        polygonUtils.PerformScan(*polygonPath, blitter);
    }
    for(int i=0;i<imageList_.Size();i++){
        DrawImage(images->data_.startp,images->data_.img->GetImageInfo(), paint);
        images = images->next_;
    }
}

void UICanvas::FillImage(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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

    if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::PATTERN)
    {
        BaseGfxExtendEngine* m_graphics= paint.GetDrawGraphicsContext();
        if(m_graphics==nullptr) {
            return;
        }
        if(paint.patternRepeat==paint.REPEAT){
            for(;;){

                if(start.x>invalidatedArea.GetRight()){
                    break;
                }
                Point temp = start;
                for(;;){
                    if(temp.y>invalidatedArea.GetBottom()){
                        break;
                    }
                    Rect cordsTmp;
                    cordsTmp.SetPosition(temp.x, temp.y);
                    cordsTmp.SetHeight(imageParam->height);
                    cordsTmp.SetWidth(imageParam->width);
                    DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,imageParam->image->GetImageInfo(), style, paint.GetOpacity());
                    temp.y+=imageParam->height;
                }
                start.x+=imageParam->width;
            }
        }else if(paint.patternRepeat==paint.REPEAT_X) {
            for(;;){
                if(start.x>invalidatedArea.GetRight()){
                    break;
                }
                Rect cordsTmp;
                cordsTmp.SetPosition(start.x, start.y);
                cordsTmp.SetHeight(imageParam->height);
                cordsTmp.SetWidth(imageParam->width);
                DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,imageParam->image->GetImageInfo(), style, paint.GetOpacity());
                start.x+=imageParam->width;
            }
        }else if(paint.patternRepeat==paint.REPEAT_Y) {
            for(;;){
                if(start.y>invalidatedArea.GetBottom()){
                    break;
                }
                Rect cordsTmp;
                cordsTmp.SetPosition(start.x, start.y);
                cordsTmp.SetHeight(imageParam->height);
                cordsTmp.SetWidth(imageParam->width);
                DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,imageParam->image->GetImageInfo(), style, paint.GetOpacity());
                start.y+=imageParam->height;
            }

        }else if(paint.patternRepeat==paint.NO_REPEAT) {
                Rect cordsTmp;
                cordsTmp.SetPosition(start.x, start.y);
                cordsTmp.SetHeight(imageParam->height);
                cordsTmp.SetWidth(imageParam->width);
                DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,imageParam->image->GetImageInfo(), style, paint.GetOpacity());
        }
    }
}




void UICanvas::setGradient(BaseGfxExtendEngine &m_graphics,const Paint& paint,const Rect& rect,const Style& style)
{
    GradientControl gradientControl=paint.getGradientControl();
    List<GradientControl::StopAndColor>  stopAndColors= gradientControl.getStopAndColor();
    if(stopAndColors.Size()>0){
          addColorGradient(m_graphics,stopAndColors);
    }
    if(gradientControl.gradientflag==gradientControl.Linear){//线性渐变
        double x0 = gradientControl.getLinearGradientPoint().x0;
        double y0 = gradientControl.getLinearGradientPoint().y0;
        double x1 = gradientControl.getLinearGradientPoint().x1;
        double y1 = gradientControl.getLinearGradientPoint().y1;

        Point start;
        Point orgstart;
        orgstart.x=x0;
        orgstart.y=y0;
        GetAbsolutePosition(orgstart, rect, style, start);

        Point end;
        Point orgend;
        orgend.x=x1;
        orgend.y=y1;
        GetAbsolutePosition(orgend, rect, style, end);

        m_graphics.fillLinearGradient(start.x,start.y,end.x,end.y);

    }

    if(gradientControl.gradientflag==gradientControl.Radial){//放射渐变
        GradientControl::RadialGradientPoint rp=gradientControl.getRadialGradientPoint();

        Point start;
        Point orgstart;
        orgstart.x=rp.x0;
        orgstart.y=rp.y0;
        GetAbsolutePosition(orgstart, rect, style, start);

        Point end;
        Point orgend;
        orgend.x=rp.x1;
        orgend.y=rp.y1;
        GetAbsolutePosition(orgend, rect, style, end);

        m_graphics.fillRadialGradient(start.x,start.y,rp.r0,end.x,end.y,rp.r1);
    }

    if(gradientControl.gradientflag==gradientControl.Solid){//纯色渐变
        ColorType color=paint.GetFillColor();
        m_graphics.fillColor(BaseGfxExtendEngine::Color(color.red,  color.green,color.blue,color.alpha));
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

    ArcInfo arcInfo = {{0}};
    arcInfo.imgPos = Point{0, 0};
    arcInfo.startAngle = 0;
    arcInfo.endAngle = CIRCLE_IN_DEGREE;
    GetAbsolutePosition(circleParam->center, rect, style, arcInfo.center);
    uint8_t enableStroke = static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE;
    uint16_t halfLineWidth = enableStroke ? (paint.GetStrokeWidth() >> 1) : 0;
    BaseGfxExtendEngine* m_graphics= paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }


    double rotateCenterX=0,rotateCenterY=0,rotateAngle=0;
    arcInfo.radius = circleParam->radius + halfLineWidth - 1;

    //int16_t posViewLeft=rect.GetX()-invalidatedArea.GetX();
    //int16_t posViewTop=rect.GetY()-invalidatedArea.GetY();

//    Rect imgRect(posViewLeft, posViewLeft, posViewLeft+1, posViewLeft+1);
//    TransformMap backwardMap;
//    Vector2<float> pivot;
//    pivot.x_ = arcInfo.center.x;
//    pivot.y_ = arcInfo.center.y;

////    /* Rotate the specified angle,  BaseGfxExtendEngine::deg2Rad()*/
//    backwardMap.Rotate(30, pivot);
////    Rect redraw = hand.target_;
////    hand.target_ = backwardMap.GetBoxRect();
////    hand.trans_ = backwardMap;
////    hand.preAngle_ = hand.nextAngle_;
//    Matrix4<float> mRotate=backwardMap.GetTransformMatrix();
//    BaseGfxExtendEngine::Transformations mTransformations;
//    const float* mfloat=mRotate.GetData();
//    mTransformations.affineMatrix[0]=mfloat[0];
//    mTransformations.affineMatrix[1]=mfloat[1];
//    mTransformations.affineMatrix[2]=mfloat[2];
//    mTransformations.affineMatrix[3]=mfloat[5];
//    mTransformations.affineMatrix[4]=mfloat[8];
//    mTransformations.affineMatrix[5]=mfloat[9];
//    m_graphics->transformations(mTransformations);
    int16_t posViewLeft=rect.GetX()-invalidatedArea.GetX();
    int16_t posViewTop=rect.GetY()-invalidatedArea.GetY();
    m_graphics->translate(-posViewLeft,-posViewTop);

    m_graphics->rotate(BaseGfxExtendEngine::deg2Rad(0));

    m_graphics->translate(posViewLeft,posViewTop);

    if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
        drawStyle.bgColor_ = paint.GetFillColor();
        drawStyle.bgOpa_ = paint.GetOpacity();
        m_graphics->fillColor(drawStyle.bgColor_.red, drawStyle.bgColor_.green,
                                  drawStyle.bgColor_.blue,drawStyle.bgOpa_);

        if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                        paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
        }
        if(paint.GetRotateAngle()!=0){
            rotateCenterX=paint.GetRotateCenterX()+rect.GetX()-invalidatedArea.GetX();
            rotateCenterY=paint.GetRotateCenterY()+rect.GetY()-invalidatedArea.GetY();
            rotateAngle=paint.GetRotateAngle();
        }
        if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                        paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
            m_graphics->drawShadow(arcInfo.center.x,arcInfo.center.y,arcInfo.radius,arcInfo.radius,
                                rotateCenterX,rotateCenterY,rotateAngle,paint.GetScaleX(),paint.GetScaleY());
        }
        if(paint.GetRotateAngle()!=0){
            m_graphics->rotate(rotateCenterX,rotateCenterY,rotateAngle);
        }
        if(paint.GetScaleX()!=0||paint.GetScaleY()!=0){
            m_graphics->scale(rotateCenterX,rotateCenterY,paint.GetScaleX(),paint.GetScaleY());
        }
    }
    if (enableStroke) {

        drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
        drawStyle.lineColor_ = paint.GetStrokeColor();
        drawStyle.lineOpa_= paint.GetOpacity();
        if(paint.IsLineDash()) {
            m_graphics->lineDashOffset(paint.GetLineDashOffset());
            m_graphics->SetLineDash(paint.GetLineDash(),paint.GetLineDashCount());
        } else {
            m_graphics->SetLineDash(nullptr,0);
        }
        m_graphics->lineWidth(drawStyle.lineWidth_);
        m_graphics->lineColor(drawStyle.lineColor_.red, drawStyle.lineColor_.green,
                                  drawStyle.lineColor_.blue,drawStyle.lineOpa_);

        if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                        paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
        }
        if(paint.GetRotateAngle()!=0){
            rotateCenterX=paint.GetRotateCenterX()+rect.GetX()-invalidatedArea.GetX();
            rotateCenterY=paint.GetRotateCenterY()+rect.GetY()-invalidatedArea.GetY();
            rotateAngle=paint.GetRotateAngle();
        }
        if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
            m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
            m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
            m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                        paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
            m_graphics->drawShadow(arcInfo.center.x,arcInfo.center.y,arcInfo.radius,arcInfo.radius,
                                rotateCenterX,rotateCenterY,rotateAngle,paint.GetScaleX(),paint.GetScaleY());
        }
        if(paint.GetRotateAngle()!=0){
            m_graphics->rotate(rotateCenterX,rotateCenterY,rotateAngle);
        }
        if(paint.GetScaleX()!=0||paint.GetScaleY()!=0){
            m_graphics->scale(rotateCenterX,rotateCenterY,paint.GetScaleX(),paint.GetScaleY());
        }
    }

    if(paint.GetGlobalAlpha() == 1.0f && !paint.IsLineDash()
            && paint.globalCompositeOperation() == BaseGfxExtendEngine::BlendMode::BlendNone) {
        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                          CapType::CAP_NONE);
    } else {
        if (!(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE)) {
            m_graphics->noFill();
        }
        if (!enableStroke) {
            m_graphics->noLine();
        }

        m_graphics->masterAlpha((double)paint.GetGlobalAlpha());
        m_graphics->blendMode(paint.globalCompositeOperation());
        //double xx=circleParam->center.x,yy=circleParam->center.y;
        //m_graphics->screenToWorld(xx,yy);
        m_graphics->ellipse(arcInfo.center.x,arcInfo.center.y,
                            arcInfo.radius,arcInfo.radius);

    }
}


void UICanvas::DoDrawArc(BufferInfo& gfxDstBuffer,
                         void* param,
                         const Paint& paint,
                         const Rect& rect,
                         const Rect& invalidatedArea,
                         const Style& style)
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


void UICanvas::DoDrawPattern(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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
                                                   imageParam->image->GetImageInfo()->header.width,imageParam->image->GetImageInfo()->header.height,
                                                   imageParam->image->GetImageInfo()->header.width*4);

    if(paint.patternRepeat==paint.REPEAT){
        paint.GetDrawGraphicsContext()->patternImageFill(imageBuffer,start.x, start.y,"repeat");
    }else if(paint.patternRepeat==paint.REPEAT_X){
        paint.GetDrawGraphicsContext()->patternImageFill(imageBuffer,start.x, start.y,"repeat-x");
    }else if(paint.patternRepeat==paint.REPEAT_Y){
        paint.GetDrawGraphicsContext()->patternImageFill(imageBuffer,start.x, start.y,"repeat-y");
    }else{
        paint.GetDrawGraphicsContext()->patternImageFill(imageBuffer,start.x, start.y,"no-repeat");
    }
}

void UICanvas::DoStrokePattern(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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
                                                   imageParam->image->GetImageInfo()->header.width,imageParam->image->GetImageInfo()->header.height,
                                                   imageParam->image->GetImageInfo()->header.width*4);


    BaseGfxExtendEngine* m_graphics= paint.GetDrawGraphicsContext();
    m_graphics->lineWidth(paint.GetStrokeWidth());
    if(paint.patternRepeat==paint.REPEAT){
        m_graphics->patternImageStroke(imageBuffer,start.x, start.y,"repeat");
    }else if(paint.patternRepeat==paint.REPEAT_X){
        m_graphics->patternImageStroke(imageBuffer,start.x, start.y,"repeat-x");
    }else if(paint.patternRepeat==paint.REPEAT_Y){
        m_graphics->patternImageStroke(imageBuffer,start.x, start.y,"repeat-y");
    }else{
        m_graphics->patternImageStroke(imageBuffer,start.x, start.y,"no-repeat");
    }
}

void UICanvas::DoGradient(BufferInfo& gfxDstBuffer,
                          void* param,
                          const Paint& paint,
                          const Rect& rect,
                          const Rect& invalidatedArea,
                          const Style& style)
{


    BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
    m_graphics->masterAlpha((double)paint.GetGlobalAlpha());
    GradientControl gradientControl=paint.getGradientControl();
    List<GradientControl::StopAndColor>  stopAndColors= gradientControl.getStopAndColor();
    if(stopAndColors.Size()>0){
          addColorGradient(*m_graphics,stopAndColors);
    }
    if(gradientControl.gradientflag==gradientControl.Linear){//线性渐变
        double x0 = gradientControl.getLinearGradientPoint().x0;
        double y0 = gradientControl.getLinearGradientPoint().y0;
        double x1 = gradientControl.getLinearGradientPoint().x1;
        double y1 = gradientControl.getLinearGradientPoint().y1;

        Point start;
        Point orgstart;
        orgstart.x=x0;
        orgstart.y=y0;
        GetAbsolutePosition(orgstart, rect, style, start);

        Point end;
        Point orgend;
        orgend.x=x1;
        orgend.y=y1;
        GetAbsolutePosition(orgend, rect, style, end);

        m_graphics->fillLinearGradient(start.x,start.y,end.x,end.y);

    }

    if(gradientControl.gradientflag==gradientControl.Radial){//放射渐变
        GradientControl::RadialGradientPoint rp=gradientControl.getRadialGradientPoint();

        Point start;
        Point orgstart;
        orgstart.x=rp.x0;
        orgstart.y=rp.y0;
        GetAbsolutePosition(orgstart, rect, style, start);

        Point end;
        Point orgend;
        orgend.x=rp.x1;
        orgend.y=rp.y1;
        GetAbsolutePosition(orgend, rect, style, end);

        m_graphics->fillRadialGradient(start.x,start.y,rp.r0,end.x,end.y,rp.r1);
    }

    if(gradientControl.gradientflag==gradientControl.Solid){//纯色渐变
        ColorType color=paint.GetFillColor();
        m_graphics->fillColor(BaseGfxExtendEngine::Color(color.red,  color.green,color.blue,color.alpha));
    }

    DoDrawPath(gfxDstBuffer,param,paint,rect,invalidatedArea,style);
//    m_graphics->closePolygon();
    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT)){
        m_graphics->noLine();
        m_graphics->drawPath(BaseGfxExtendEngine::FillAndStroke);
    }
    if(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT)){
        m_graphics->lineWidth(paint.GetStrokeWidth());
       m_graphics->stroke();
    }
}



void UICanvas::DoDrawImage(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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
//    DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,
//        imageParam->image->GetImageInfo(), style, paint.GetOpacity());


    BaseGfxExtendEngine::Image imageBuffer((unsigned char*)imageParam->image->GetImageInfo()->data,
                                                   imageParam->image->GetImageInfo()->header.width,imageParam->image->GetImageInfo()->header.height,
                                                   imageParam->image->GetImageInfo()->header.width*4);
    paint.GetDrawGraphicsContext()->blendImage(imageBuffer,start.x, start.y,255);




}







void UICanvas::DoDrawLabel(BufferInfo& gfxDstBuffer,
                           void* param,
                           const Paint& paint,
                           const Rect& rect,
                           const Rect& invalidatedArea,
                           const Style& style)
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

void UICanvas::DoDrawLineJoin(BufferInfo& gfxDstBuffer,
                              const Point& center,
                              const Rect& invalidatedArea,
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

void UICanvas::DoDrawPath(BufferInfo& gfxDstBuffer,
                          void* param,
                          const Paint& paint,
                          const Rect& rect,
                          const Rect& invalidatedArea,
                          const Style& style)
{
    if (param == nullptr) {
        return;
    }
    PathParam* pathParam = static_cast<PathParam*>(param);
    const UICanvasPath* path = pathParam->path;
    if (path == nullptr) {
        return;
    }
    BaseGfxExtendEngine* m_graphics=
        paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }
    Point pathEnd = {COORD_MIN, COORD_MIN};

    ListNode<Point>* pointIter = path->points_.Begin();
    ListNode<ArcParam>* arcIter = path->arcParam_.Begin();
    ListNode<PathCmd>* iter = path->cmd_.Begin();
    bool isLineJoin = (paint.GetLineJoin() == BaseGfxExtendEngine::LineJoin::JoinNone);
    bool isLineCap = (paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CapNone);
    if((!isLineJoin || !isLineCap)
            &&!(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT))
            &&(!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT))
            &&(!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN))) {
        m_graphics->lineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green,
                              paint.GetStrokeColor().blue,paint.GetStrokeColor().alpha);
        m_graphics->lineWidth(paint.GetStrokeWidth());
        m_graphics->lineCap(paint.GetLineCap());
        m_graphics->lineJoin(paint.GetLineJoin());
        if(paint.GetLineJoin()==BaseGfxExtendEngine::JoinMiter) {
            m_graphics->MiterLimit(paint.GetMiterLimit());
        }
        m_graphics->noFill();
    }

    m_graphics->resetPath();
    for (uint16_t i = 0; (i < pathParam->count) && (iter != path->cmd_.End()); i++, iter = iter->next_) {
        switch (iter->data_) {
        case CMD_MOVE_TO: {
            Point start;
            GetAbsolutePosition(pointIter->data_, rect, style, start);
            if(!isLineJoin || !isLineCap) {
                m_graphics->moveTo(start.x,start.y);
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
            if(isLineJoin&&isLineCap) {
                BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                       paint.GetStrokeWidth(), paint.GetStrokeColor(), OPA_OPAQUE);
                if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                    DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                }
            } else {
                m_graphics->lineTo(end.x,end.y);
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
            if(isLineJoin&&isLineCap) {
                BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                      CapType::CAP_NONE);
                if (pointIter != path->points_.Begin()) {
                    DoDrawLineJoin(gfxDstBuffer, pathEnd, invalidatedArea, paint);
                }
            } else {
                m_graphics->arcTo(arcInfo.radius,arcInfo.radius,
                                   BaseGfxExtendEngine::deg2Rad(arcInfo.endAngle-arcInfo.startAngle),
                                   0, 1,arcInfo.center.x,arcInfo.center.y);
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
            if(!isLineJoin || !isLineCap) {
                m_graphics->lineTo(end.x,end.y);
            } else {
                if ((start.x != end.x) || (start.y != end.y)) {
                    BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                           paint.GetStrokeWidth(), paint.GetStrokeColor(), OPA_OPAQUE);
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
        default:
            break;
        }
    }
    if((!isLineJoin || !isLineCap)
            &&!(static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::FILL_GRADIENT))
            &&(!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::STROKE_GRADIENT))
            &&(!static_cast<uint8_t>(paint.GetStyle() & Paint::PaintStyle::PATTERN))) {
        m_graphics->drawPath(BaseGfxExtendEngine::DrawPathFlag::StrokeOnly);
    }

//    if(!(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_GRADIENT)){
//        m_graphics->stroke();
//    }

}

void PolygonImageBlitter::DrawHorSpan(const List<Span>& span, int16_t yCur)
{
    if (imageList_.Size() <=0) {
        return;
    }

    for (int16_t y = iy_; y < yCur; y++) {
        DrawHorLine(0, y, img_w_*x_size_, OPA_TRANSPARENT);
    }
    int16_t index = 0;
    auto iter = span.Begin();
    while (iter != span.End()) {
        DrawHorLine(index, yCur, iter->data_.left - index, OPA_TRANSPARENT);
        DrawHorLine(iter->data_.left, yCur, iter->data_.right - iter->data_.left + 1, iter->data_.opa);
        index = iter->data_.right + 1;
        iter = iter->next_;
    }
    DrawHorLine(index, yCur, img_w_*x_size_ - index, OPA_TRANSPARENT);
    iy_ = yCur + 1;
}

void PolygonImageBlitter::Finish()
{
    if (imageList_.Size() <=0) {
        return;
    }

    for (int16_t y = iy_; y < img_h_*y_size_; y++) {
        DrawHorLine(0, y, img_w_*x_size_, OPA_TRANSPARENT);
    }
}

void PolygonImageBlitter::DrawPixel(int16_t x, int16_t y, uint8_t opa)
{
    if(imageList_.Size()<=0){
       return ;
    }
   auto  images =imageList_.Begin();

   for(int i=0;i<imageList_.Size();i++){

       int16_t h_r=i/x_size_;//第h_r行
       int16_t w_r=i%x_size_;//当前行第w_r个

       //处理repeat-y
       if(images->data_.startp.x==0){
        h_r=images->data_.startp.y/img_h_;
        w_r=0;
       }

       if (!((x>=img_w_*w_r&&x<img_w_*(w_r+1))&&(y>=img_h_*h_r&&y<img_h_*(h_r+1)))) {
           images = images->next_;
           continue;
       }


       int32_t offset =(x-img_w_*w_r)+(y-img_h_*h_r)*img_w_;
       switch (images->data_.img->GetImageInfo()->header.colorMode) {
           case ARGB8888: {
               Color32* buffer = reinterpret_cast<Color32*>(const_cast<uint8_t*>(images->data_.img->GetImageInfo()->data));
               buffer[offset].alpha = buffer[offset].alpha * opa / OPA_OPAQUE;
               break;
           }
           default: {
               GRAPHIC_LOGE("Only images in ARGB8888 format are supported!");
               break;
           }
       }
       images = images->next_;
   }
}

void PolygonImageBlitter::DrawHorLine(int16_t x, int16_t y, int16_t width, uint8_t opa)
{
    if (width < 0 || opa == OPA_OPAQUE) {
        return;
    }

    for (int16_t i = 0; i < width; i++) {
        DrawPixel(x + i, y, opa);
    }
}

void UICanvas::DoFillPath(BufferInfo& gfxDstBuffer,
                          void* param,
                          const Paint& paint,
                          const Rect& rect,
                          const Rect& invalidatedArea,
                          const Style& style)
{
    if (param == nullptr) {
        return;
    }
    PathParam* pathParam = static_cast<PathParam*>(param);
    const UICanvasPath* path = pathParam->path;
    if (path == nullptr) {
        return;
    }
    BaseGfxExtendEngine* m_graphics=
        paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }
    Point pathEnd = {COORD_MIN, COORD_MIN};

    ListNode<Point>* pointIter = path->points_.Begin();
    ListNode<ArcParam>* arcIter = path->arcParam_.Begin();
    ListNode<PathCmd>* iter = path->cmd_.Begin();
    bool isLineJoin = (paint.GetLineJoin() == BaseGfxExtendEngine::LineJoin::JoinNone);
    bool isLineCap = (paint.GetLineCap() == BaseGfxExtendEngine::LineCap::CapNone);

    if(!isLineJoin || !isLineCap) {
        m_graphics->lineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green,
                              paint.GetStrokeColor().blue,paint.GetStrokeColor().alpha);
        m_graphics->lineWidth(paint.GetStrokeWidth());
        m_graphics->lineCap(paint.GetLineCap());
        m_graphics->lineJoin(paint.GetLineJoin());
        if(paint.GetLineJoin()==BaseGfxExtendEngine::JoinMiter) {
            m_graphics->MiterLimit(paint.GetMiterLimit());
        }
//        m_graphics->noFill();
    }
    if (paint.GetFillColor().alpha) {
        m_graphics->fillColor(paint.GetFillColor().red, paint.GetFillColor().green, paint.GetFillColor().blue,
                              paint.GetFillColor().alpha);
    }
    if(paint.GetStrokeColor().alpha){
        m_graphics->lineColor(paint.GetStrokeColor().red, paint.GetStrokeColor().green, paint.GetStrokeColor().blue,
                              paint.GetStrokeColor().alpha);
        m_graphics->lineWidth(paint.GetStrokeWidth());
    }
    m_graphics->resetPath();
    for (uint16_t i = 0; (i < pathParam->count) && (iter != path->cmd_.End()); i++, iter = iter->next_) {
        switch (iter->data_) {
        case CMD_MOVE_TO: {
            Point start;
            GetAbsolutePosition(pointIter->data_, rect, style, start);
            if(!isLineJoin || !isLineCap) {
                m_graphics->moveTo(start.x,start.y);
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
            if(isLineJoin&&isLineCap) {
                BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                       paint.GetStrokeWidth(), paint.GetStrokeColor(), OPA_OPAQUE);
                if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                    DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                }
            } else {
                m_graphics->lineTo(end.x,end.y);
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
            if(isLineJoin&&isLineCap) {
                BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                      CapType::CAP_NONE);
                if (pointIter != path->points_.Begin()) {
                    DoDrawLineJoin(gfxDstBuffer, pathEnd, invalidatedArea, paint);
                }
            } else {
                m_graphics->arcTo(arcInfo.radius,arcInfo.radius,
                                   BaseGfxExtendEngine::deg2Rad(arcInfo.endAngle-arcInfo.startAngle),
                                   0, 1,arcInfo.center.x,arcInfo.center.y);
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
            if(!isLineJoin || !isLineCap) {
                m_graphics->lineTo(end.x,end.y);
            } else {
                if ((start.x != end.x) || (start.y != end.y)) {
                    BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea,
                                                           paint.GetStrokeWidth(), paint.GetStrokeColor(), OPA_OPAQUE);
                    if ((pathEnd.x == start.x) && (pathEnd.y == start.y)) {
                        DoDrawLineJoin(gfxDstBuffer, start, invalidatedArea, paint);
                    }
                    pathEnd = end;
                }

                if ((pathEnd.x == end.x) && (pathEnd.y == end.y)) {
                    DoDrawLineJoin(gfxDstBuffer, end, invalidatedArea, paint);
                }
            }
            m_graphics->closePolygon();
            pointIter = pointIter->next_;
            break;
        }
        default:
            break;
        }
    }
    double rotateCenterX=0,rotateCenterY=0,rotateAngle=0;
    if(paint.GetRotateAngle()!=0){
        rotateCenterX=paint.GetRotateCenterX()+rect.GetX()-invalidatedArea.GetX();
        rotateCenterY=paint.GetRotateCenterY()+rect.GetY()-invalidatedArea.GetY();
        rotateAngle=paint.GetRotateAngle();
    }
    if (paint.GetShadowOffsetX()!=0||paint.GetShadowOffsetY()!=0) {
        m_graphics->SetShadowBlurRadius(paint.GetShadowBlurRadius());
        m_graphics->SetShadowOffset(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());
        m_graphics->SetShadowColor(paint.GetShadowColor().red, paint.GetShadowColor().green,
                                    paint.GetShadowColor().blue, paint.GetShadowColor().alpha);
        m_graphics->drawShadow(rotateCenterX,rotateCenterY,rotateAngle,paint.GetScaleX(),paint.GetScaleY());
    }
    if(paint.GetRotateAngle()!=0){
        m_graphics->rotate(rotateCenterX,rotateCenterY,rotateAngle);
    }
    if(paint.GetScaleX()!=0||paint.GetScaleY()!=0){
        m_graphics->scale(rotateCenterX,rotateCenterY,paint.GetScaleX(),paint.GetScaleY());
    }
     setGradient(*m_graphics,paint,rect,style);//填充颜色
    m_graphics->drawPath(BaseGfxExtendEngine::FillAndStroke);

}

} // namespace OHOS
