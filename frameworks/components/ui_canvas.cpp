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



namespace OHOS {

void UICanvas::BeginPath()
{
    //初始化UICanvasPath
//    if (vertices_ != nullptr) {
//        delete vertices_;
//        vertices_ = nullptr;
//    }

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

    DrawCmd cmd;
    cmd.paint = paint;
    cmd.param = pathParam;
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
     TransAffine transform;
     RenderingBuffer renderBuffer;
      //初始化buffer和 m_transform
     InitRendAndTransform(gfxDstBuffer,renderBuffer,rect,transform,style);

     //路径
     typedef OHOS::DepictCurve<OHOS::UICanvasVertices> UICanvasPath;
     typedef OHOS::DepictStroke<UICanvasPath> StrokeLineStyle;
//     typedef OHOS::DepictDash<UICanvasPath> DashStyle;
//     typedef OHOS::DepictStroke<DashStyle> StrokeDashStyle;

//     typedef OHOS::DepictTransform<UICanvasPath> PathTransform;
     typedef OHOS::DepictTransform<StrokeLineStyle> StrokeTransform;
//     typedef OHOS::DepictTransform<StrokeDashStyle> DashStrokeTransform;

     PathParam* pathParam = static_cast<PathParam*>(param);
     UICanvasPath canvasPath(*pathParam->path);
//        PathTransform pathtransform(convCurve,m_transform);
     StrokeLineStyle strokeLineStyle(canvasPath);
     strokeLineStyle.Width(paint.GetStrokeWidth());//线条样式相关
     strokeLineStyle.LineCap(paint.GetLineCap());
     strokeLineStyle.LineJoin(paint.GetLineJoin());

     OHOS::RasterizerScanlineAntiAlias<> rasterizer;
     OHOS::ScanlineUnPackedContainer m_scanline;
     rasterizer.Reset();
     StrokeTransform strokeTransform(strokeLineStyle,transform);

//     DashStyle dashCurve(canvasPath);
//     StrokeDashStyle dashStroke(dashCurve);

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
     ColorType StrokeColor(paint.GetStrokeColor().red,paint.GetStrokeColor().green,paint.GetStrokeColor().blue,paint.GetStrokeColor().alpha);
     OHOS::RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, m_renBase, StrokeColor);
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
