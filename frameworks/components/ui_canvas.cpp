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

//void UICanvas::createPattern(UIImageView & image,Paint& paint)
//{



//}

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
        //这个地方定义了一个
        BufferInfo* gfxMapBuffer = new BufferInfo;
        if (memcpy_s(gfxMapBuffer, sizeof(BufferInfo), &gfxDstBuffer, sizeof(BufferInfo)) != 0) {
            delete gfxMapBuffer;
            gfxMapBuffer = nullptr;
            return;
        }
        BaseGfxExtendEngine* m_graphics_Image = curDraw->data_.paint.GetImageBufferContext();
        if(m_graphics_Image==nullptr) {
            return;
        }
        //BufferInfo* modeBuff = BaseGfxEngine::GetInstance()->GetFBBufferInfo();
        uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(gfxMapBuffer->mode);
        //uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(gfxDstBuffer.mode);
        uint32_t destStride= gfxMapBuffer->width* destByteSize;
        uint32_t buffSize = gfxMapBuffer->height * destStride;
        //uint32_t buffSize = textRect.GetWidth() * textRect.GetHeight() * (pxSize >> 3);
        gfxMapBuffer->virAddr = BaseGfxEngine::GetInstance()->AllocBuffer(
                    buffSize, BUFFER_MAP_SURFACE);

        gfxMapBuffer->phyAddr = gfxMapBuffer->virAddr;

        int16_t posViewLeft=rect.GetX()-trunc.GetX();
        int16_t posViewTop=rect.GetY()-trunc.GetY();
        int16_t realLeft=rect.GetLeft() + style_->paddingLeft_ + style_->borderWidth_;
        int16_t realTop=rect.GetTop() + style_->paddingTop_ + style_->borderWidth_;

        InitDrawEnvironment(gfxDstBuffer,gfxMapBuffer,trunc,
                            Rect(realLeft,
                                 realTop,
                                 realLeft+trunc.GetWidth() - 1,
                                 realTop+trunc.GetHeight() - 1),
                            Rect(posViewLeft,
                                 posViewTop,
                                 posViewLeft+trunc.GetWidth() - 1,
                                 posViewTop+trunc.GetHeight() - 1),curDraw->data_.paint);

        //添加的处理机制的。。。
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DrawGraphics(*gfxMapBuffer, param, curDraw->data_.paint, rect, trunc, *style_);
        }
        //const Image& img, int dstX1, int dstY1,
        //int dstX2, int dstY2
        BaseGfxExtendEngine::Image imageBuffer((unsigned char*)gfxMapBuffer->virAddr,
                                               gfxMapBuffer->width,gfxMapBuffer->height,
                                               gfxMapBuffer->stride);
        m_graphics_Image->blend_from(imageBuffer,BaseGfxExtendEngine::Rect(gfxMapBuffer->rect.GetLeft(),
                                                                           gfxMapBuffer->rect.GetTop(),
                                                                           gfxMapBuffer->rect.GetRight(),
                                                                           gfxMapBuffer->rect.GetBottom()),
                                     BaseGfxExtendEngine::Rect(gfxDstBuffer.rect.GetLeft(),
                                                               gfxDstBuffer.rect.GetTop(),
                                                               gfxDstBuffer.rect.GetRight(),
                                                               gfxDstBuffer.rect.GetBottom()));
        BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)gfxMapBuffer->phyAddr);
        delete gfxMapBuffer;
    }
}


bool UICanvas::InitDrawEnvironment(const BufferInfo& gfxDstBuffer,const BufferInfo* gfxImageBuffer,const Rect& fillArea,const Rect &worldRect,
                                   const Rect &screenRect,const Paint& paint)
{

    BaseGfxExtendEngine* m_graphics= paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return false;
    }
    BaseGfxExtendEngine* m_graphics_Image = paint.GetImageBufferContext();
    if(m_graphics_Image==nullptr) {
        return false;
    }
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

    m_graphics_Image->attach(destBuf,fillArea.GetWidth(),
                       fillArea.GetHeight(),gfxDstBuffer.stride);
    m_graphics_Image->viewport(worldRect.GetLeft(),worldRect.GetTop(),worldRect.GetRight(),worldRect.GetBottom(),
                         screenRect.GetLeft(),screenRect.GetTop(),screenRect.GetRight(),screenRect.GetBottom(),
                         BaseGfxExtendEngine::Anisotropic);
                         //BaseGfxExtendEngine::XMidYMid);

    offset = static_cast<int32_t>(posTop) * gfxImageBuffer->width +
                posLeft;
    destBuf= static_cast<uint8_t*>(gfxImageBuffer->virAddr);
    destBuf += offset * destByteSize;

    m_graphics->attach(destBuf,fillArea.GetWidth(),
        fillArea.GetHeight(),gfxImageBuffer->stride);


    m_graphics->viewport(worldRect.GetLeft(),worldRect.GetTop(),worldRect.GetRight(),worldRect.GetBottom(),
                               screenRect.GetLeft(),screenRect.GetTop(),screenRect.GetRight(),screenRect.GetBottom(),
                               BaseGfxExtendEngine::Anisotropic);

    m_graphics->clearAll(agg::srgba8(0,0,0,0));
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

    BaseGfxExtendEngine* m_graphics = paint.GetDrawGraphicsContext();
    if(m_graphics==nullptr) {
        return;
    }

    Point start;
    GetAbsolutePosition(rectParam->start, rect, style, start);

    m_graphics->masterAlpha((double)paint.GetGlobalAlpha());
    m_graphics->noLine();
    fill(*m_graphics,paint,rect,style);//填充颜色
    m_graphics->rectangle(start.x,start.y,start.x+rectParam->width,start.y+rectParam->height);

}

void UICanvas::addColorGradient(BaseGfxExtendEngine & m_graphics,List<Paint::StopAndColor> & stopAndColors){
    m_graphics.remove_all_color();
    ListNode<Paint::StopAndColor>* iter = stopAndColors.Begin();
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

    DrawCmd cmd;
    cmd.paint = paint;
    cmd.param = imageParam;
    cmd.DeleteParam = DeleteImageParam;
    cmd.DrawGraphics = FillImage;
    drawCmdList_.PushBack(cmd);

    Invalidate();
}

void UICanvas::fill(const Paint& paint,const PolygonPath * polygonPath)
{

    fill(paint);

    if(polygonPath&&(static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::PATTERN)){
        ImageInfo imageInfo;
        this->GetBitmap(imageInfo);
        this->Clear();
        PolygonUtils polygonUtils;
        ClipImageBlitter blitter(&imageInfo);
        polygonUtils.PerformScan(*polygonPath, blitter);
        this->DrawImage(startPoint_,&imageInfo, paint);
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



void UICanvas::fill(BaseGfxExtendEngine &m_graphics,const Paint& paint,const Rect& rect,const Style& style){

    List<Paint::StopAndColor>  stopAndColors= paint.getStopAndColor();
    if(stopAndColors.Size()>0){
          addColorGradient(m_graphics,stopAndColors);
    }

    if(paint.gradientfalg==paint.Linear){//线性渐变
        double x0 = paint.getLinearGradientPoit().x0;
        double y0 = paint.getLinearGradientPoit().y0;
        double x1 = paint.getLinearGradientPoit().x1;
        double y1 = paint.getLinearGradientPoit().y1;

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
    if(paint.gradientfalg==paint.Radial){//放射渐变
        Paint::RadialGradientPoint rp=paint.getRadialGradientPoint();
//        m_graphics.fillRadialGradient(rp.x0,rp.y0,rp.r0,rp.x1,rp.y1,rp.r1);

        Point start;
        Point orgstart;
        orgstart.x=rp.x0;
        orgstart.y=rp.y0;
        GetAbsolutePosition(orgstart, rect, style, start);
//        m_graphics.fillRadialGradient(start.x,start.y,rp.r0,rp.x1,rp.y1,rp.r1);

        Point end;
        Point orgend;
        orgend.x=rp.x1;
        orgend.y=rp.y1;
        GetAbsolutePosition(orgend, rect, style, end);

        m_graphics.fillRadialGradient(start.x,start.y,rp.r0,end.x,end.y,rp.r1);
    }

    if(paint.gradientfalg==paint.Solid){//纯色渐变
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
    if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::FILL_STYLE) {
        arcInfo.radius = circleParam->radius - halfLineWidth;
        drawStyle.lineWidth_ = arcInfo.radius;
        drawStyle.lineColor_ = paint.GetFillColor();
        drawStyle.bgOpa_ = paint.GetOpacity();
        if(paint.GetGlobalAlpha() == 1.0f && !paint.IsLineDash() &&
                paint.globalCompositeOperation()== BaseGfxExtendEngine::BlendMode::BlendSrcOver) {
            BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                                  CapType::CAP_NONE);
        } else {
            m_graphics->noLine();
            m_graphics->fillColor(drawStyle.lineColor_.red, drawStyle.lineColor_.green,
                              drawStyle.lineColor_.blue,drawStyle.bgOpa_);
            m_graphics->blendMode(paint.globalCompositeOperation());
            m_graphics->ellipse(arcInfo.center.x,arcInfo.center.y,
                                arcInfo.radius,arcInfo.radius);
        }
        // Add/Sub/Contrast Blending Modes
    }

    if (enableStroke) {
        arcInfo.radius = circleParam->radius + halfLineWidth - 1;
        drawStyle.lineWidth_ = static_cast<int16_t>(paint.GetStrokeWidth());
        drawStyle.lineColor_ = paint.GetStrokeColor();
        drawStyle.lineOpa_= paint.GetOpacity();
        if(paint.GetGlobalAlpha() == 1.0f && !paint.IsLineDash() &&
                paint.globalCompositeOperation()== BaseGfxExtendEngine::BlendMode::BlendSrcOver) {
            BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, drawStyle, OPA_OPAQUE,
                                              CapType::CAP_NONE);
        } else {
            if(paint.IsLineDash()) {
                m_graphics->lineDashOffset(paint.GetLineDashOffset());
                m_graphics->SetLineDash(paint.GetLineDash(),paint.GetLineDashCount());
            } else {
                m_graphics->SetLineDash(nullptr,0);
            }
            m_graphics->blendMode(paint.globalCompositeOperation());
            m_graphics->lineColor(drawStyle.lineColor_.red, drawStyle.lineColor_.green,
                                      drawStyle.lineColor_.blue,drawStyle.lineOpa_);
            m_graphics->noFill();
            m_graphics->ellipse(arcInfo.center.x,arcInfo.center.y,
                                arcInfo.radius-1,arcInfo.radius-1);

        }
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
    DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea,
        imageParam->image->GetImageInfo(), style, paint.GetOpacity());
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
    if(!isLineJoin || !isLineCap) {
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
    if(!isLineJoin || !isLineCap) {
        m_graphics->drawPath(BaseGfxExtendEngine::DrawPathFlag::StrokeOnly);
    }
}
} // namespace OHOS
