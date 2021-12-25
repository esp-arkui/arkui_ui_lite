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
#include "common/image.h"
#include "draw/draw_arc.h"
#include "draw/draw_image.h"
#include "engines/gfx/gfx_engine_manager.h"
#include "gfx_utils/graphic_log.h"

#include <draw/clip_utils.h>

#include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_base.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_gradient.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_gradient_lut.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_interpolator.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_pattern_rgba.h>

#include <render/graphic_render_pixfmt_rgba_comp.h>

#include <gfx_utils/graphics/graphic_transform/graphic_transform_image_accessors.h>

#include <gfx_utils/graphics/graphic_filter/graphic_filter_blur.h>
#include <gfx_utils/graphics/graphic_geometry/graphic_geometry_bounding_rect.h>


namespace OHOS {

void UICanvas::BeginPath()
{
    vertices_ = new UICanvasVertices();
    if (vertices_ == nullptr) {
        GRAPHIC_LOGE("new UICanvasPath fail");
        return;
    }
    if (vertices_ == nullptr) {
        vertices_ = new UICanvasVertices();
        if (vertices_ == nullptr) {
            GRAPHIC_LOGE("new UICanvasPath fail");
            return;
        }
    }else{
        vertices_->RemoveAll();
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
    float sinma = radius * Sin(startAngle);
    float cosma = radius * Sin(QUARTER_IN_DEGREE - startAngle);
    if (vertices_->TotalVertices() != 0) {
        vertices_->LineTo(float(center.x + sinma), float(center.y - cosma));
    } else {
        vertices_->MoveTo(float(center.x + sinma), float(center.y - cosma));
    }
    if (MATH_ABS(startAngle - endAngle) < CIRCLE_IN_DEGREE) {
        sinma = radius * Sin(endAngle);
        cosma = radius * Sin(QUARTER_IN_DEGREE - endAngle);
    }
    int16_t start;
    int16_t end;
    if (startAngle > endAngle) {
        start = endAngle;
        end = startAngle;
    } else {
        start = startAngle;
        end = endAngle;
    }
    vertices_->ArcTo(radius, radius,( end -  start), 0, 1,float(center.x + sinma), float(center.y - cosma));
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

void UICanvas::StrokeRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint){
    if(!paint.GetChangeFlag()){
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
    }else{
        BeginPath();
        MoveTo(startPoint);
        LineTo({static_cast<int16_t>(startPoint.x + width), startPoint.y});
        LineTo({static_cast<int16_t>(startPoint.x + width), static_cast<int16_t>(startPoint.y + height)});
        LineTo({startPoint.x, static_cast<int16_t>(startPoint.y + height)});
        ClosePath();
        DrawPath(paint);
    }
}

void UICanvas::ClearRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint){
    Paint paint_;
    paint_.SetFillColor(this->style_->bgColor_);
    paint_.SetStyle(Paint::FILL_STYLE);
    BeginPath();
    MoveTo(startPoint);
    LineTo({static_cast<int16_t>(startPoint.x + width), startPoint.y});
    LineTo({static_cast<int16_t>(startPoint.x + width), static_cast<int16_t>(startPoint.y + height)});
    LineTo({startPoint.x, static_cast<int16_t>(startPoint.y + height)});
    ClosePath();
    FillPath(paint_);
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

void UICanvas::DrawPath(const Paint& paint)
{
    PathParam* pathParam = new PathParam;
    if (pathParam == nullptr) {
        GRAPHIC_LOGE("new LineParam fail");
        return;
    }
    pathParam->path = vertices_;

    if(paint.GetStyle()==Paint::PATTERN){
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

        imageParam->image->SetSrc(paint.GetPatternImage());
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->start = {0, 0};
        imageParam->height = header.height;
        imageParam->width = header.width;
        pathParam->imageParam = imageParam;
    }

    DrawCmd cmd;
    cmd.paint = paint;
    cmd.param = pathParam;
    cmd.DeleteParam = DeletePathParam;
    cmd.DrawGraphics = DoDrawPath;
    drawCmdList_.PushBack(cmd);
    Invalidate();
}

void UICanvas::FillPath(const Paint& paint)
{
    PathParam* pathParam = new PathParam;
    if (pathParam == nullptr) {
        GRAPHIC_LOGE("new LineParam fail");
        return;
    }
    pathParam->path = vertices_;

    if(paint.GetStyle()==Paint::PATTERN){
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

        imageParam->image->SetSrc(paint.GetPatternImage());
        ImageHeader header = {0};
        imageParam->image->GetHeader(header);
        imageParam->start = {0, 0};
        imageParam->height = header.height;
        imageParam->width = header.width;
        pathParam->imageParam = imageParam;
    }

    DrawCmd cmd;
    cmd.paint = paint;
    cmd.param = pathParam;
    cmd.DeleteParam = DeletePathParam;
    cmd.DrawGraphics = DoFillPath;
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
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DrawGraphics(gfxDstBuffer, param, curDraw->data_.paint, rect, trunc, *style_);
        }
    }
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

    BaseGfxEngine::GetInstance()->DrawLine(gfxDstBuffer, start, end, invalidatedArea, paint.GetStrokeWidth(),
                                           paint.GetStrokeColor(), paint.GetOpacity());
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
                          imageParam->image->GetPath(), style, paint.GetOpacity());
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

void UICanvas::DoDrawPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint,
                          const Rect& rect, const Rect& invalidatedArea, const Style& style)
{
    if(paint.GetGlobalCompositeOperation()==Paint::SOURCE_OVER){
        DoRender(gfxDstBuffer,param,paint,rect,invalidatedArea,style,true);
    }else{
        DoRenderBlend(gfxDstBuffer,param,paint,rect,invalidatedArea,style,true);
    }
}


void UICanvas::DoFillPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint,
                          const Rect& rect, const Rect& invalidatedArea, const Style& style)
{
    if(paint.GetGlobalCompositeOperation()==Paint::SOURCE_OVER){
        DoRender(gfxDstBuffer,param,paint,rect,invalidatedArea,style,false);
    }else{
        DoRenderBlend(gfxDstBuffer,param,paint,rect,invalidatedArea,style,false);
    }
}

void UICanvas::DoRender(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                          const Rect& invalidatedArea, const Style& style, const bool& isStroke)
{

    if (param == nullptr) {
         return;
    }
    if(paint.HaveShadow()){
        DoDrawShadow(gfxDstBuffer,param,paint,rect,invalidatedArea,style,isStroke);
    }
    TransAffine transform;
    RenderingBuffer renderBuffer;
     //初始化buffer和 m_transform
    InitRendAndTransform(gfxDstBuffer,renderBuffer,rect,transform,style);

    typedef DepictCurve<UICanvasVertices> UICanvasPath;
    RasterizerScanlineAntiAlias<> rasterizer;
    ScanlineUnPackedContainer m_scanline;

    PathParam* pathParam = static_cast<PathParam*>(param);
    UICanvasPath canvasPath(*pathParam->path);
    if(isStroke){
        if(paint.IsLineDash()){
            typedef DepictDash<UICanvasPath> DashStyle;
            typedef DepictStroke<DashStyle> StrokeDashStyle;
            typedef DepictTransform<StrokeDashStyle> StrokeDashTransform;
            DashStyle dashStyle(canvasPath);
            LineDashStyleCalc(dashStyle,paint);
            StrokeDashStyle strokeDashStyle(dashStyle);
            LineStyleCalc(strokeDashStyle,paint);
            StrokeDashTransform strokeDashTransform(strokeDashStyle,transform);
            rasterizer.Reset();
            rasterizer.AddPath(strokeDashTransform);

        }else{
            typedef DepictStroke<UICanvasPath> StrokeLineStyle;
            typedef DepictTransform<StrokeLineStyle> StrokeTransform;
            StrokeLineStyle strokeLineStyle(canvasPath);
            LineStyleCalc(strokeLineStyle,paint);

            StrokeTransform strokeTransform(strokeLineStyle,transform);
            rasterizer.Reset();
            rasterizer.AddPath(strokeTransform);
        }
    }else{
        typedef OHOS::DepictTransform<UICanvasPath> PathTransform;
        PathTransform  pathTransform(canvasPath,transform);
        rasterizer.Reset();
        rasterizer.AddPath(pathTransform);
    }


    typedef Rgba8 Rgba8Color;
    //组装renderbase
    // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
    typedef OrderBgra ComponentOrder;
    // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
    typedef BlenderRgba<Rgba8Color, ComponentOrder> Blender;
    typedef PixfmtAlphaBlendRgba<Blender, RenderingBuffer> PixFormat;
    typedef RendererBase<PixFormat> RendererBase;
    typedef OHOS::SpanFillColorAllocator<Rgba8Color> SpanAllocator;

    PixFormat m_pixFormat(renderBuffer);
    RendererBase m_renBase(m_pixFormat);
    SpanAllocator allocator;

    m_renBase.ResetClipping(true);
    m_renBase.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());



    if(paint.GetStyle()==Paint::STROKE_STYLE||
       paint.GetStyle()==Paint::FILL_STYLE||
       paint.GetStyle()==Paint::STROKE_FILL_STYLE){

        Rgba8Color rgba8Color;
        if(isStroke){
            if(paint.GetStyle()==Paint::STROKE_STYLE||
               paint.GetStyle()==Paint::STROKE_FILL_STYLE){
                rgba8Color.redValue = paint.GetStrokeColor().red;
                rgba8Color.greenValue = paint.GetStrokeColor().green;
                rgba8Color.blueValue = paint.GetStrokeColor().blue;
                rgba8Color.alphaValue = paint.GetStrokeColor().alpha * paint.GetGlobalAlpha();
            }
        }else{
            if(paint.GetStyle()==Paint::FILL_STYLE||
               paint.GetStyle()==Paint::STROKE_FILL_STYLE){
                rgba8Color.redValue = paint.GetFillColor().red;
                rgba8Color.greenValue = paint.GetFillColor().green;
                rgba8Color.blueValue = paint.GetFillColor().blue;
                rgba8Color.alphaValue = paint.GetFillColor().alpha * paint.GetGlobalAlpha();
            }
        }
        RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, m_renBase, rgba8Color);
    }

    if(paint.GetStyle()==Paint::GRADIENT){
        // 设定线段分配器
        // 设定渐变数组的构造器设定颜色插值器和颜色模板等
        typedef GradientColorCalibration<OHOS::ColorInterpolator<OHOS::Srgba8>, 1024> GradientColorMode;
        // 设定放射渐变的算法
        typedef GradientRadialCalculate RadialGradientCalculate;
        // 设定线段插值器
        typedef SpanInterpolatorLinear<> InterpolatorType;
        // 设定线性渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, GradientLinearCalculate, GradientColorMode> LinearGradientSpan;
        // 设定放射渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, RadialGradientCalculate, GradientColorMode> RadialGradientSpan;

        typedef OHOS::CompOpAdaptorRgba<Rgba8Color, ComponentOrder> BlenderComp;
        typedef OHOS::PixfmtCustomBlendRgba<BlenderComp, RenderingBuffer> PixFormatComp;
        typedef OHOS::RendererBase<PixFormatComp> RendererBaseComp;

        PixFormatComp pixFormatComp(renderBuffer);
        RendererBaseComp m_renBaseComp(pixFormatComp);

        m_renBaseComp.ResetClipping(true);
        m_renBaseComp.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());
        TransAffine gradientMatrix;
        InterpolatorType interpolatorType(gradientMatrix);
        GradientLinearCalculate gradientLinearCalculate;

        GradientColorMode gradientColorMode;

        gradientColorMode.RemoveAll();
        ListNode<Paint::StopAndColor>* iter = paint.getStopAndColor().Begin();
        uint16_t count = 0;
        for (; count < paint.getStopAndColor().Size(); count++) {
            ColorType stopColor = iter->data_.color;
            gradientColorMode.AddColor(iter->data_.stop, Rgba8Color(stopColor.red, stopColor.green,
                                                                             stopColor.blue, stopColor.alpha * paint.GetGlobalAlpha()));
            iter = iter->next_;
        }
        gradientColorMode.BuildLut();


        double d1;
        double d2;

        if(paint.GetGradient()==Paint::Linear){
            Paint::LinearGradientPoint linearPoint =paint.GetLinearGradientPoint();

            double angle = atan2(linearPoint.y1 - linearPoint.y0, linearPoint.x1 - linearPoint.x0);
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineRotation(angle);
            gradientMatrix *= OHOS::TransAffineTranslation(linearPoint.x0, linearPoint.y0);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            d1 = 0.0;
            d2 = sqrt((linearPoint.x1 - linearPoint.x0) * (linearPoint.x1 - linearPoint.x0) + (linearPoint.y1 - linearPoint.y0) * (linearPoint.y1 - linearPoint.y0));

            LinearGradientSpan span(interpolatorType,gradientLinearCalculate,gradientColorMode,d1,d2);
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBaseComp, allocator, span);
        }

        if(paint.GetGradient()==Paint::Radial){
            Paint::RadialGradientPoint radialPoint =paint.GetRadialGradientPoint();
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineTranslation(radialPoint.x1, radialPoint.y1);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            d1 = radialPoint.r0;
            d2 = radialPoint.r1;
            GradientRadialCalculate gradientRadialCalculate(radialPoint.r1, radialPoint.x0 - radialPoint.x1, radialPoint.y0 - radialPoint.y1);
            RadialGradientSpan span(interpolatorType,gradientRadialCalculate,gradientColorMode,d1,d2);
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBaseComp, allocator, span);
        }

    }

    if(paint.GetStyle()==Paint::PATTERN){

        ImageParam* imageParam =pathParam->imageParam;
        if (imageParam->image == nullptr) {
            return;
        }
        // 渲染器缓冲区
        typedef OHOS::RenderingBuffer PatternBuffer;
        // 设定图像观察器的模式为Wrap设定X,Y轴上WrapModeRepeat模式，即X,Y上都重复图片
        typedef OHOS::ImageAccessorWrap<PixFormat, OHOS::WrapModeRepeat, OHOS::WrapModeRepeat> ImgSourceTypeRepeat;
        // 设定图像观察器的模式为RepeatX设定X轴上WrapModeRepeat模式，即X上都重复图片
        typedef OHOS::ImageAccessorRepeatX<PixFormat, OHOS::WrapModeRepeat> imgSourceTypeRepeatX;
        // 设定图像观察器的模式为RepeatY设定Y轴上WrapModeRepeat模式，即Y上都重复图片
        typedef OHOS::ImageAccessorRepeatY<PixFormat, OHOS::WrapModeRepeat> imgSourceTypeRepeatY;
        // 设定图像观察器的模式为NoRepeat即X,Y轴上都不重复，只有一张原本的图片
        typedef OHOS::ImageAccessorNoRepeat<PixFormat> imgSourceTypeNoRepeat;
        // 通过线段生成器SpanPatternRgba设定相应的图像观察器对应的模式生成相应线段
        //  x,y轴都重复
        typedef OHOS::SpanPatternFillRgba<ImgSourceTypeRepeat> spanPatternTypeRepeat;
        //  x轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatX> spanPatternTypeRepeatX;
        //  y轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatY> spanPatternTypeRepeatY;
        // 不重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeNoRepeat> spanPatternTypeNoRepeat;

        PatternBuffer patternBuffer;
        uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imageParam->image->GetImageInfo()->header.colorMode);
        patternBuffer.Attach((unsigned char*)imageParam->image->GetImageInfo()->data,imageParam->width,imageParam->height,imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
        PixFormat img_pixf(patternBuffer); // 获取图片

        if(paint.GetPatternRepeatMode()==Paint::REPEAT){
            ImgSourceTypeRepeat img_src(img_pixf);
            spanPatternTypeRepeat m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBase, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::REPEAT_X){
            imgSourceTypeRepeatX img_src(img_pixf);
            spanPatternTypeRepeatX m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBase, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::REPEAT_Y){
            imgSourceTypeRepeatY img_src(img_pixf);
            spanPatternTypeRepeatY m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBase, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::NO_REPEAT){
            imgSourceTypeNoRepeat img_src(img_pixf);
            spanPatternTypeNoRepeat m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBase, allocator, m_spanPatternType);
        }
    }

//    if(paint.GetGlobalCompositeOperation()!=Paint::SOURCE_OVER){
//        DoRenderBlend(gfxDstBuffer,param,paint,rect,invalidatedArea,style,false);
//    }

}

void UICanvas::DoRenderBlend(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                          const Rect& invalidatedArea, const Style& style, const bool& isStroke)
{

    if (param == nullptr) {
         return;
    }
    if(paint.HaveShadow()){
        DoDrawShadow(gfxDstBuffer,param,paint,rect,invalidatedArea,style,isStroke);
    }
    TransAffine transform;
    RenderingBuffer renderBuffer;




    typedef Rgba8 Rgba8Color;
    //组装renderbase
    // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
    typedef OrderBgra ComponentOrder;
    // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
    typedef BlenderRgba<Rgba8Color, ComponentOrder> Blender;
    typedef PixfmtAlphaBlendRgba<Blender, RenderingBuffer> PixFormat;
    typedef RendererBase<PixFormat> RendererBasePre;
    typedef OHOS::SpanFillColorAllocator<Rgba8Color> SpanAllocator;

    PixFormat m_pixFormat(renderBuffer);
    RendererBasePre m_renBasePre(m_pixFormat);







     //初始化buffer和 m_transform
    InitRendAndTransform(gfxDstBuffer,renderBuffer,rect,transform,style);

    typedef DepictCurve<UICanvasVertices> UICanvasPath;
    RasterizerScanlineAntiAlias<> rasterizer;
    ScanlineUnPackedContainer m_scanline;

    PathParam* pathParam = static_cast<PathParam*>(param);
    UICanvasPath canvasPath(*pathParam->path);
    if(isStroke){
        if(paint.IsLineDash()){
            typedef DepictDash<UICanvasPath> DashStyle;
            typedef DepictStroke<DashStyle> StrokeDashStyle;
            typedef DepictTransform<StrokeDashStyle> StrokeDashTransform;
            DashStyle dashStyle(canvasPath);
            LineDashStyleCalc(dashStyle,paint);
            StrokeDashStyle strokeDashStyle(dashStyle);
            LineStyleCalc(strokeDashStyle,paint);
            StrokeDashTransform strokeDashTransform(strokeDashStyle,transform);
            rasterizer.Reset();
            rasterizer.AddPath(strokeDashTransform);

        }else{
            typedef DepictStroke<UICanvasPath> StrokeLineStyle;
            typedef DepictTransform<StrokeLineStyle> StrokeTransform;
            StrokeLineStyle strokeLineStyle(canvasPath);
            LineStyleCalc(strokeLineStyle,paint);

            StrokeTransform strokeTransform(strokeLineStyle,transform);
            rasterizer.Reset();
            rasterizer.AddPath(strokeTransform);
        }
    }else{
        typedef OHOS::DepictTransform<UICanvasPath> PathTransform;
        PathTransform  pathTransform(canvasPath,transform);
        rasterizer.Reset();
        rasterizer.AddPath(pathTransform);
    }


    typedef Rgba8 Rgba8Color;
    //组装renderbase
    // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
    typedef OrderBgra ComponentOrder;
    // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
    typedef CompOpAdaptorRgba<Rgba8Color, ComponentOrder> BlenderCom;
    typedef PixfmtCustomBlendRgba<BlenderCom, RenderingBuffer> PixFormatCom;
    typedef RendererBase<PixFormatCom> RendererBaseCom;
    typedef OHOS::SpanFillColorAllocator<Rgba8Color> SpanAllocator;

    PixFormatCom pixFormatCom(renderBuffer);
    RendererBaseCom renBaseCom(pixFormatCom);
    SpanAllocator allocator;

    m_renBasePre.BlendFrom(renBaseCom);

    renBaseCom.ResetClipping(true);
    renBaseCom.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());

    pixFormatCom.CompOp(paint.GetGlobalCompositeOperation());


    if(paint.GetStyle()==Paint::STROKE_STYLE||
       paint.GetStyle()==Paint::FILL_STYLE||
       paint.GetStyle()==Paint::STROKE_FILL_STYLE){

        Rgba8Color rgba8Color;
        if(isStroke){
            if(paint.GetStyle()==Paint::STROKE_STYLE||
               paint.GetStyle()==Paint::STROKE_FILL_STYLE){
                rgba8Color.redValue = paint.GetStrokeColor().red;
                rgba8Color.greenValue = paint.GetStrokeColor().green;
                rgba8Color.blueValue = paint.GetStrokeColor().blue;
                rgba8Color.alphaValue = paint.GetStrokeColor().alpha * paint.GetGlobalAlpha();
            }
        }else{
            if(paint.GetStyle()==Paint::FILL_STYLE||
               paint.GetStyle()==Paint::STROKE_FILL_STYLE){
                rgba8Color.redValue = paint.GetFillColor().red;
                rgba8Color.greenValue = paint.GetFillColor().green;
                rgba8Color.blueValue = paint.GetFillColor().blue;
                rgba8Color.alphaValue = paint.GetFillColor().alpha * paint.GetGlobalAlpha();
            }
        }
        RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, renBaseCom, rgba8Color);
    }

    if(paint.GetStyle()==Paint::GRADIENT){
        // 设定线段分配器
        // 设定渐变数组的构造器设定颜色插值器和颜色模板等
        typedef GradientColorCalibration<OHOS::ColorInterpolator<OHOS::Srgba8>, 1024> GradientColorMode;
        // 设定放射渐变的算法
        typedef GradientRadialCalculate RadialGradientCalculate;
        // 设定线段插值器
        typedef SpanInterpolatorLinear<> InterpolatorType;
        // 设定线性渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, GradientLinearCalculate, GradientColorMode> LinearGradientSpan;
        // 设定放射渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, RadialGradientCalculate, GradientColorMode> RadialGradientSpan;

        typedef OHOS::CompOpAdaptorRgba<Rgba8Color, ComponentOrder> BlenderComp;
        typedef OHOS::PixfmtCustomBlendRgba<BlenderComp, RenderingBuffer> PixFormatComp;
        typedef OHOS::RendererBase<PixFormatComp> RendererBaseComp;

        PixFormatComp pixFormatComp(renderBuffer);
        RendererBaseComp m_renBaseComp(pixFormatComp);

        m_renBaseComp.ResetClipping(true);
        m_renBaseComp.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());
        TransAffine gradientMatrix;
        InterpolatorType interpolatorType(gradientMatrix);
        GradientLinearCalculate gradientLinearCalculate;

        GradientColorMode gradientColorMode;

        gradientColorMode.RemoveAll();
        ListNode<Paint::StopAndColor>* iter = paint.getStopAndColor().Begin();
        uint16_t count = 0;
        for (; count < paint.getStopAndColor().Size(); count++) {
            ColorType stopColor = iter->data_.color;
            gradientColorMode.AddColor(iter->data_.stop, Rgba8Color(stopColor.red, stopColor.green,
                                                                             stopColor.blue, stopColor.alpha * paint.GetGlobalAlpha()));
            iter = iter->next_;
        }
        gradientColorMode.BuildLut();


        double d1;
        double d2;

        if(paint.GetGradient()==Paint::Linear){
            Paint::LinearGradientPoint linearPoint =paint.GetLinearGradientPoint();

            double angle = atan2(linearPoint.y1 - linearPoint.y0, linearPoint.x1 - linearPoint.x0);
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineRotation(angle);
            gradientMatrix *= OHOS::TransAffineTranslation(linearPoint.x0, linearPoint.y0);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            d1 = 0.0;
            d2 = sqrt((linearPoint.x1 - linearPoint.x0) * (linearPoint.x1 - linearPoint.x0) + (linearPoint.y1 - linearPoint.y0) * (linearPoint.y1 - linearPoint.y0));

            LinearGradientSpan span(interpolatorType,gradientLinearCalculate,gradientColorMode,d1,d2);
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBaseComp, allocator, span);
        }

        if(paint.GetGradient()==Paint::Radial){
            Paint::RadialGradientPoint radialPoint =paint.GetRadialGradientPoint();
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineTranslation(radialPoint.x1, radialPoint.y1);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            d1 = radialPoint.r0;
            d2 = radialPoint.r1;
            GradientRadialCalculate gradientRadialCalculate(radialPoint.r1, radialPoint.x0 - radialPoint.x1, radialPoint.y0 - radialPoint.y1);
            RadialGradientSpan span(interpolatorType,gradientRadialCalculate,gradientColorMode,d1,d2);
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBaseComp, allocator, span);
        }

    }

    if(paint.GetStyle()==Paint::PATTERN){

        ImageParam* imageParam =pathParam->imageParam;
        if (imageParam->image == nullptr) {
            return;
        }
        // 渲染器缓冲区
        typedef OHOS::RenderingBuffer PatternBuffer;
        // 设定图像观察器的模式为Wrap设定X,Y轴上WrapModeRepeat模式，即X,Y上都重复图片
        typedef OHOS::ImageAccessorWrap<PixFormat, OHOS::WrapModeRepeat, OHOS::WrapModeRepeat> ImgSourceTypeRepeat;
        // 设定图像观察器的模式为RepeatX设定X轴上WrapModeRepeat模式，即X上都重复图片
        typedef OHOS::ImageAccessorRepeatX<PixFormat, OHOS::WrapModeRepeat> imgSourceTypeRepeatX;
        // 设定图像观察器的模式为RepeatY设定Y轴上WrapModeRepeat模式，即Y上都重复图片
        typedef OHOS::ImageAccessorRepeatY<PixFormat, OHOS::WrapModeRepeat> imgSourceTypeRepeatY;
        // 设定图像观察器的模式为NoRepeat即X,Y轴上都不重复，只有一张原本的图片
        typedef OHOS::ImageAccessorNoRepeat<PixFormat> imgSourceTypeNoRepeat;
        // 通过线段生成器SpanPatternRgba设定相应的图像观察器对应的模式生成相应线段
        //  x,y轴都重复
        typedef OHOS::SpanPatternFillRgba<ImgSourceTypeRepeat> spanPatternTypeRepeat;
        //  x轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatX> spanPatternTypeRepeatX;
        //  y轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatY> spanPatternTypeRepeatY;
        // 不重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeNoRepeat> spanPatternTypeNoRepeat;

        PatternBuffer patternBuffer;
        uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imageParam->image->GetImageInfo()->header.colorMode);
        patternBuffer.Attach((unsigned char*)imageParam->image->GetImageInfo()->data,imageParam->width,imageParam->height,imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
        PixFormat img_pixf(patternBuffer); // 获取图片

        if(paint.GetPatternRepeatMode()==Paint::REPEAT){
            ImgSourceTypeRepeat img_src(img_pixf);
            spanPatternTypeRepeat m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBasePre, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::REPEAT_X){
            imgSourceTypeRepeatX img_src(img_pixf);
            spanPatternTypeRepeatX m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBasePre, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::REPEAT_Y){
            imgSourceTypeRepeatY img_src(img_pixf);
            spanPatternTypeRepeatY m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBasePre, allocator, m_spanPatternType);
        }
        if(paint.GetPatternRepeatMode()==Paint::NO_REPEAT){
            imgSourceTypeNoRepeat img_src(img_pixf);
            spanPatternTypeNoRepeat m_spanPatternType(img_src, 0-rect.GetLeft(), 0-rect.GetTop());
            RenderScanlinesAntiAlias(rasterizer, m_scanline, m_renBasePre, allocator, m_spanPatternType);
        }
    }

}


void UICanvas::DoDrawShadow(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                          const Rect& invalidatedArea, const Style& style, const bool& isStroke)
{

    if (param == nullptr) {
         return;
    }
    TransAffine transform;
    RenderingBuffer renderBuffer;
     //初始化buffer和 m_transform
    InitRendAndTransform(gfxDstBuffer,renderBuffer,rect,transform,style);

    transform.Translate(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());

    typedef DepictCurve<UICanvasVertices> UICanvasPath;
    RasterizerScanlineAntiAlias<> rasterizer;
    ScanlineUnPackedContainer m_scanline;
    RectD bbox;

    PathParam* pathParam = static_cast<PathParam*>(param);
    UICanvasPath canvasPath(*pathParam->path);
    if(isStroke){
        if(paint.IsLineDash()){
            typedef DepictDash<UICanvasPath> DashStyle;
            typedef DepictStroke<DashStyle> StrokeDashStyle;
            typedef DepictTransform<StrokeDashStyle> StrokeDashTransform;
            DashStyle dashStyle(canvasPath);
            LineDashStyleCalc(dashStyle,paint);
            StrokeDashStyle strokeDashStyle(dashStyle);
            LineStyleCalc(strokeDashStyle,paint);
            StrokeDashTransform strokeDashTransform(strokeDashStyle,transform);
            BoundingRectSingle(strokeDashTransform, 0, &bbox.x1, &bbox.y1, &bbox.x2, &bbox.y2);
            rasterizer.Reset();
            rasterizer.AddPath(strokeDashTransform);

        }else{
            typedef DepictStroke<UICanvasPath> StrokeLineStyle;
            typedef DepictTransform<StrokeLineStyle> StrokeTransform;
            StrokeLineStyle strokeLineStyle(canvasPath);
            LineStyleCalc(strokeLineStyle,paint);

            StrokeTransform strokeTransform(strokeLineStyle,transform);
            BoundingRectSingle(strokeTransform, 0, &bbox.x1, &bbox.y1, &bbox.x2, &bbox.y2);

            rasterizer.Reset();
            rasterizer.AddPath(strokeTransform);
        }
    }else{
        typedef OHOS::DepictTransform<UICanvasPath> PathTransform;
        PathTransform  pathTransform(canvasPath,transform);
        BoundingRectSingle(pathTransform, 0, &bbox.x1, &bbox.y1, &bbox.x2, &bbox.y2);
        rasterizer.Reset();
        rasterizer.AddPath(pathTransform);
    }


    typedef Rgba8 Rgba8Color;
    //组装renderbase
    // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
    typedef OrderBgra ComponentOrder;
    // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
    typedef BlenderRgba<Rgba8Color, ComponentOrder> Blender;
    typedef PixfmtAlphaBlendRgba<Blender, RenderingBuffer> PixFormat;
    typedef RendererBase<PixFormat> RendererBase;
    typedef OHOS::SpanFillColorAllocator<Rgba8Color> SpanAllocator;

    PixFormat m_pixFormat(renderBuffer);
    RendererBase m_renBase(m_pixFormat);
    SpanAllocator allocator;

    m_renBase.ResetClipping(true);
    m_renBase.ClipBox(invalidatedArea.GetLeft(),invalidatedArea.GetTop(),invalidatedArea.GetRight(),invalidatedArea.GetBottom());

    Rgba8Color rgba8Color;
    rgba8Color.redValue = paint.GetShadowColor().red;
    rgba8Color.greenValue = paint.GetShadowColor().green;
    rgba8Color.blueValue = paint.GetShadowColor().blue;
    rgba8Color.alphaValue = paint.GetShadowColor().alpha * paint.GetGlobalAlpha();

    RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, m_renBase, rgba8Color);

    typedef OHOS::StackBlur<Rgba8Color, OHOS::StackBlurCalcRGBA<>> DrawBlur;
    typedef OHOS::PixfmtAlphaBlendRgba<Blender, OHOS::RenderingBuffer> PixfmtAlphaBlendRgba;
    DrawBlur drawBlur;
    bbox.x1 -= paint.GetShadowBlur();
    bbox.y1 -= paint.GetShadowBlur();
    bbox.x2 += paint.GetShadowBlur();
    bbox.y2 += paint.GetShadowBlur();
    RenderingBuffer shadowBuffer;
    PixfmtAlphaBlendRgba pixf2(shadowBuffer);

    Rect shadowRect ={int16_t(bbox.x1),int16_t(bbox.y1),int16_t(bbox.x2),int16_t(bbox.y2)};
    shadowRect.Intersect(shadowRect,invalidatedArea);
    pixf2.Attach(m_pixFormat, shadowRect.GetLeft(),shadowRect.GetTop(),shadowRect.GetRight(),shadowRect.GetBottom());
    drawBlur.Blur(pixf2, OHOS::Uround(paint.GetShadowBlur()));
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
//        textParam->isDrawTrans = paint.IsTransform();
        DrawCmd cmd;
        cmd.param = textParam;
        cmd.DeleteParam = DeleteTextParam;
        cmd.DrawGraphics = DoDrawText;
        cmd.paint = paint;
        drawCmdList_.PushBack(cmd);
        Invalidate();
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

    text->OnDraw(gfxDstBuffer, invalidatedArea, textRect, textRect, 0, drawStyle,
                 Text::TEXT_ELLIPSIS_END_INV, opa);

}

} // namespace OHOS
