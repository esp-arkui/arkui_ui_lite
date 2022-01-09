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
 * limitations under the License..
 */

#include "components/ui_canvas.h"

#include <draw/clip_utils.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_image_rgba.h>

#include "common/image.h"
#include "draw/draw_arc.h"
#include "draw/draw_image.h"
#include "gfx_utils/graphic_log.h"

namespace OHOS {
    void UICanvas::BeginPath()
    {
        vertices_ = new UICanvasVertices();
    }

    void UICanvas::MoveTo(const Point& point)
    {
        if (vertices_ == nullptr) {
            return;
        }
        vertices_->MoveTo(point.x, point.y);
    }

    void UICanvas::LineTo(const Point& point)
    {
        if (vertices_ == nullptr) {
            return;
        }
        vertices_->LineTo(point.x, point.y);
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
        vertices_->ArcTo(radius, radius, (end - start), 0, 1, float(center.x + sinma), float(center.y - cosma));
    }

    void UICanvas::AddRect(const Point& point, int16_t height, int16_t width)
    {
        if (vertices_ == nullptr) {
            return;
        }

        int16_t right = static_cast<int16_t>(point.x + width);
        int16_t bottom = static_cast<int16_t>(point.y + height);

        float fright = (float)width + (float)point.x;
        float fbottom = (float)height + (float)point.y;
        const int16_t setup = 3;
        if (fright > INT16_MAX) {
            right += setup;
        }
        if (fbottom > INT16_MAX) {
            bottom += setup;
        }
        MoveTo(point);
        LineTo({right, point.y});
        LineTo({right, bottom});
        LineTo({point.x, bottom});
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
        void* param = nullptr;
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DeleteParam(param);
            curDraw->data_.param = nullptr;
        }
        drawCmdList_.Clear();
        if (vertices_ != nullptr) {
            delete vertices_;
            vertices_ = nullptr;
        }
        DestroyMapBufferInfo();
    }

    void UICanvas::Clear()
    {
        void* param = nullptr;
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            param = curDraw->data_.param;
            curDraw->data_.DeleteParam(param);
            curDraw->data_.param = nullptr;
        }
        drawCmdList_.Clear();
        if (vertices_ != nullptr) {
            delete vertices_;
            vertices_ = nullptr;
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
        if (!paint.GetChangeFlag()) {
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
        } else {
            BeginPath();
            MoveTo(startPoint);
            LineTo({static_cast<int16_t>(startPoint.x + width), startPoint.y});
            LineTo({static_cast<int16_t>(startPoint.x + width), static_cast<int16_t>(startPoint.y + height)});
            LineTo({startPoint.x, static_cast<int16_t>(startPoint.y + height)});
            ClosePath();
            FillPath(paint);
            DrawPath(paint);
        }
        Invalidate();
    }

    void UICanvas::StrokeRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
    {
        if (!paint.GetChangeFlag()) {
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
        } else {
            BeginPath();
            MoveTo(startPoint);
            LineTo({static_cast<int16_t>(startPoint.x + width), startPoint.y});
            LineTo({static_cast<int16_t>(startPoint.x + width),
                    static_cast<int16_t>(startPoint.y + height)});
            LineTo({startPoint.x, static_cast<int16_t>(startPoint.y + height)});
            ClosePath();
            DrawPath(paint);
        }
        SetStartPosition(startPoint);
    }

    void UICanvas::ClearRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint)
    {
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
        if (paint.GetChangeFlag()) {
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
            if (vertices_ == nullptr) {
                vertices_ = new UICanvasVertices();
            }
            vertices_->RemoveAll();
            OHOS::BezierArc arc(center.x, center.y, radius, radius, 0, TWO_TIMES * PI);
            vertices_->ConcatPath(arc, 0);
            vertices_->ClosePolygon();
            FillPath(paint);
#endif
        } else {
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

    void UICanvas::DrawArc(const Point& center, uint16_t radius, int16_t startAngle,
                           int16_t endAngle, const Paint& paint)
    {
        if (static_cast<uint8_t>(paint.GetStyle()) & Paint::PaintStyle::STROKE_STYLE) {
            if (paint.GetChangeFlag()) {
                ArcTo(center, radius, startAngle, endAngle);
                DrawPath(paint);
            } else {
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
            }
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
#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWIMAGE
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

        if (IsGif(image)) {
            imageParam->gifImageAnimator = new GifCanvasImageAnimator(imageParam, this, image);
            imageParam->gifImageAnimator->Start();
        }
        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoDrawImage;
        drawCmdList_.PushBack(cmd);

        Invalidate();
        SetStartPosition(startPoint);
    }

    void UICanvas::DrawImage(const Point& startPoint, const char* image, Paint& paint, int16_t width, int16_t height)
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

        if (IsGif(image)) {
            imageParam->gifImageAnimator = new GifCanvasImageAnimator(imageParam, this, image);
            imageParam->gifImageAnimator->Start();
            imageParam->height = imageParam->gifImageAnimator->GetSize().y;
            imageParam->width = imageParam->gifImageAnimator->GetSize().x;
        }

        float scaleX = 1.0;
        float scaleY = 1.0;

        if (width > 0 && imageParam->width > 0) {
            scaleX = (float)width / (float)imageParam->width;
        }

        if (height > 0 && imageParam->height > 0) {
            scaleY = (float)height / (float)imageParam->height;
        }

        paint.Scale(scaleX, scaleY);

        DrawCmd cmd;
        cmd.paint = paint;
        cmd.param = imageParam;
        cmd.DeleteParam = DeleteImageParam;
        cmd.DrawGraphics = DoDrawImage;
        drawCmdList_.PushBack(cmd);

        Invalidate();
        SetStartPosition(startPoint);
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
#endif
    void UICanvas::DrawPath(const Paint& paint)
    {
        PathParam* pathParam = new PathParam;
        if (pathParam == nullptr) {
            GRAPHIC_LOGE("new LineParam fail");
            return;
        }
        if (vertices_ == nullptr) {
            return;
        }
        pathParam->vertices = vertices_;
        pathParam->isStroke = true;
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        if (paint.GetStyle() == Paint::PATTERN) {
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
#endif
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
        if (vertices_ == nullptr) {
            return;
        }

        pathParam->vertices = vertices_;
        pathParam->isStroke = false;
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        if (paint.GetStyle() == Paint::PATTERN) {
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
#endif
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
            // 添加的处理机制的。。。
            bool haveComposite = false;
            for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
                // 应该是实现画布的处理机制..
                if (curDraw->data_.paint.HaveComposite()) {
                    haveComposite = true;
                    break;
                }
            }

            if (haveComposite) {
                OnBlendDraw(gfxDstBuffer, trunc);
            } else {
                curDraw = drawCmdList_.Begin();
                for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
                    param = curDraw->data_.param;
                    curDraw->data_.DrawGraphics(gfxDstBuffer, param, curDraw->data_.paint, rect, trunc, *style_);
                }
            }
        }
    }

    void UICanvas::OnBlendDraw(BufferInfo& gfxDstBuffer, const Rect& trunc)
    {
        Rect rect = GetOrigRect();
        RenderingBuffer renderBuffer;
        TransAffine transform;
        ListNode<DrawCmd>* curDrawEnd = drawCmdList_.Begin();
        RasterizerScanlineAntiAlias<> blendRasterizer;
        typedef SpanSoildColor<Rgba8Color> SpanSoildColor;
        DrawCmd drawCmd;
        int count = 0;
        for (; curDrawEnd != drawCmdList_.End(); curDrawEnd = curDrawEnd->next_) {
            if (curDrawEnd->data_.paint.HaveComposite()) {
                drawCmd = curDrawEnd->data_;
                count++;
            }
        }
        if (drawCmd.param == nullptr) {
            return;
        }
        PathParam* pathParamBlend = static_cast<PathParam*>(drawCmd.param);
        ListNode<DrawCmd>* curDraw = drawCmdList_.Begin();
        InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, *style_, curDraw->data_.paint);
        blendRasterizer.ClipBox(0, 0, gfxDstBuffer.width, gfxDstBuffer.height);
        SetRasterizer(*pathParamBlend->vertices, drawCmd.paint, blendRasterizer, transform, pathParamBlend->isStroke);
        ScanlineUnPackedContainer scanline;
        PixFormat pixFormat(renderBuffer);
        RendererBase renBase(pixFormat);
        renBase.ResetClipping(true);
        renBase.ClipBox(trunc.GetLeft(), trunc.GetTop(), trunc.GetRight(), trunc.GetBottom());
        for (; curDraw != drawCmdList_.End(); curDraw = curDraw->next_) {
            if (curDraw->data_.paint.HaveComposite()) {
                drawCmd = curDraw->data_;
                count--;
            }
            if (count <= 0) {
                continue;
            }
            RasterizerScanlineAntiAlias<> rasterizer;
            if (curDraw->data_.param == nullptr) {
                continue;
            }
            PathParam* pathParam = static_cast<PathParam*>(curDraw->data_.param);
            if (curDraw->data_.paint.HaveShadow()) {
                DoDrawShadow(gfxDstBuffer, curDraw->data_.param, curDraw->data_.paint, rect,
                             trunc, *style_, pathParam->isStroke);
            }
            InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, *style_, curDraw->data_.paint);
            rasterizer.ClipBox(0, 0, gfxDstBuffer.width, gfxDstBuffer.height);
            SetRasterizer(*pathParam->vertices, curDraw->data_.paint, rasterizer, transform, pathParam->isStroke);
            if (isSoild(curDraw->data_.paint)) {
                Rgba8Color color;
                RenderBlendSolid(curDraw->data_.paint,color,pathParam->isStroke);
                SpanSoildColor spanSoildColor(color);
                BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,spanSoildColor,rect,pathParamBlend->isStroke);
            }

            if (curDraw->data_.paint.GetStyle() == Paint::GRADIENT) {
                TransAffine gradientMatrix;
                InterpolatorType interpolatorType(gradientMatrix);
                GradientColorMode gradientColorMode;
                BuildGradientColor(curDraw->data_.paint,gradientColorMode);
                if (curDraw->data_.paint.GetGradient() == Paint::Linear) {
                    double distance = 0;
                    BuildLineGradientMatrix(drawCmd.paint,gradientMatrix,transform,distance);
                    GradientLinearCalculate gradientLinearCalculate;
                    LinearGradientSpan span(interpolatorType, gradientLinearCalculate, gradientColorMode, 0, distance);
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,span,trunc,pathParamBlend->isStroke);
                }
                if (curDraw->data_.paint.GetGradient() == Paint::Radial) {
                    Paint::RadialGradientPoint radialPoint = drawCmd.paint.GetRadialGradientPoint();
                    double startRadius = 0;
                    double endRadius = 0;
                    BuildRadialGradientMatrix(drawCmd.paint,gradientMatrix,transform,startRadius,endRadius);
                    GradientRadialCalculate gradientRadialCalculate(endRadius, radialPoint.x0 - radialPoint.x1,
                                                                    radialPoint.y0 - radialPoint.y1);
                    RadialGradientSpan span(interpolatorType, gradientRadialCalculate, gradientColorMode,
                                            startRadius, endRadius);
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,span,trunc,pathParamBlend->isStroke);
                }
            }

            if (curDraw->data_.paint.GetStyle() == Paint::PATTERN) {
                if (curDraw->data_.param == nullptr) {
                    return;
                }
                PathParam* pathParam = static_cast<PathParam*>(curDraw->data_.param);
                ImageParam* imageParam = static_cast<ImageParam*>(pathParam->imageParam);
                if (imageParam->image == nullptr) {
                    return;
                }
                PatternBuffer patternBuffer;
                uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imageParam->image->GetImageInfo()->header.colorMode);
                patternBuffer.Attach((unsigned char*)imageParam->image->GetImageInfo()->data,
                                     imageParam->width,
                                     imageParam->height,
                                     imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
                PixFormat img_pixf(patternBuffer); // 获取图片

                if (curDraw->data_.paint.GetPatternRepeatMode() == Paint::REPEAT) {
                    ImgSourceTypeRepeat img_src(img_pixf);
                    spanPatternTypeRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,m_spanPatternType,trunc,pathParamBlend->isStroke);
                }
                if (curDraw->data_.paint.GetPatternRepeatMode() == Paint::REPEAT_X) {
                    imgSourceTypeRepeatX img_src(img_pixf);
                    spanPatternTypeRepeatX m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,m_spanPatternType,trunc,pathParamBlend->isStroke);
                }
                if (curDraw->data_.paint.GetPatternRepeatMode() == Paint::REPEAT_Y) {
                    imgSourceTypeRepeatY img_src(img_pixf);
                    spanPatternTypeRepeatY m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,m_spanPatternType,trunc,pathParamBlend->isStroke);
                }
                if (curDraw->data_.paint.GetPatternRepeatMode() == Paint::NO_REPEAT) {
                    imgSourceTypeNoRepeat img_src(img_pixf);
                    spanPatternTypeNoRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendRaster(drawCmd.paint,drawCmd.param,blendRasterizer,rasterizer,renBase,transform,m_spanPatternType,trunc,pathParamBlend->isStroke);
                }
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

        BaseGfxEngine::GetInstance()->DrawCubicBezier(gfxDstBuffer,
                                                      start,
                                                      control1,
                                                      control2,
                                                      end,
                                                      invalidatedArea,
                                                      paint.GetStrokeWidth(),
                                                      paint.GetStrokeColor(),
                                                      paint.GetOpacity());
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
#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWIMAGE
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

        if (paint.GetChangeFlag()) {
            TransAffine transform;
            RenderingBuffer renderBuffer;
            // 初始化buffer和 m_transform.
            InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, style, paint);
            transform.Translate(imageParam->start.x, imageParam->start.y);
            RenderingBuffer imageRendBuffer;
            uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imageParam->image->GetImageInfo()->header.colorMode);
            imageRendBuffer.Attach((unsigned char*)imageParam->image->GetImageInfo()->data, imageParam->width,
                                   imageParam->height, imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
            DoRenderImage(renderBuffer, paint, invalidatedArea, transform, imageRendBuffer);
        } else {
            DrawImage::DrawCommon(gfxDstBuffer, cordsTmp, invalidatedArea, imageParam->image->GetImageInfo(), style,
                                  paint.GetOpacity());
        }
    }
#endif
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
        BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcinfo, invalidatedArea,
                                              style, OPA_OPAQUE, CapType::CAP_NONE);
    }

    void UICanvas::DoDrawPath(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style)
    {
        DoRender(gfxDstBuffer, param, paint, rect, invalidatedArea, style, true);
    }

    void UICanvas::DoFillPath(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style)
    {
        DoRender(gfxDstBuffer, param, paint, rect, invalidatedArea, style, false);
    }

    void UICanvas::SetRasterizer(UICanvasVertices& vertices,
                                 const Paint& paint,
                                 RasterizerScanlineAntiAlias<>& rasterizer,
                                 TransAffine& transform,
                                 const bool& isStroke)
    {
        typedef DepictCurve<UICanvasVertices> UICanvasPath;
        UICanvasPath canvasPath(vertices);
        if (isStroke) {
            if (paint.IsLineDash()) {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                typedef DepictDash<UICanvasPath> DashStyle;
                typedef DepictStroke<DashStyle> StrokeDashStyle;
                typedef DepictTransform<StrokeDashStyle> StrokeDashTransform;
                DashStyle dashStyle(canvasPath);
                LineDashStyleCalc(dashStyle, paint);
                StrokeDashStyle strokeDashStyle(dashStyle);
                LineStyleCalc(strokeDashStyle, paint);
                StrokeDashTransform strokeDashTransform(strokeDashStyle, transform);
                rasterizer.Reset();
                rasterizer.AddPath(strokeDashTransform);
#endif
            } else {
                typedef DepictStroke<UICanvasPath> StrokeLineStyle;
                typedef DepictTransform<StrokeLineStyle> StrokeTransform;
                StrokeLineStyle strokeLineStyle(canvasPath);
                LineStyleCalc(strokeLineStyle, paint);

                StrokeTransform strokeTransform(strokeLineStyle, transform);
                rasterizer.Reset();
                rasterizer.AddPath(strokeTransform);
            }
        } else {
            typedef OHOS::DepictTransform<UICanvasPath> PathTransform;
            PathTransform pathTransform(canvasPath, transform);
            rasterizer.Reset();
            rasterizer.AddPath(pathTransform);
        }
    }

    void UICanvas::DoRender(BufferInfo& gfxDstBuffer,
                            void* param,
                            const Paint& paint,
                            const Rect& rect,
                            const Rect& invalidatedArea,
                            const Style& style,
                            const bool& isStroke)
    {
        if (param == nullptr) {
            return;
        }
#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
        if (paint.HaveShadow()) {
            DoDrawShadow(gfxDstBuffer, param, paint, rect, invalidatedArea, style, isStroke);
        }
#endif
        TransAffine transform;
        RenderingBuffer renderBuffer;
        // 初始化buffer和 m_transform
        InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, style, paint);

        RasterizerScanlineAntiAlias<> rasterizer;
        ScanlineUnPackedContainer m_scanline;

        PathParam* pathParam = static_cast<PathParam*>(param);
        rasterizer.ClipBox(0, 0, gfxDstBuffer.width, gfxDstBuffer.height);
        SetRasterizer(*pathParam->vertices, paint, rasterizer, transform, isStroke);

        PixFormat m_pixFormat(renderBuffer);
        RendererBase renBase(m_pixFormat);
        SpanAllocator allocator;

        renBase.ResetClipping(true);
        renBase.ClipBox(invalidatedArea.GetLeft(), invalidatedArea.GetTop(), invalidatedArea.GetRight(),
                        invalidatedArea.GetBottom());

        if (paint.GetStyle() == Paint::STROKE_STYLE || paint.GetStyle() == Paint::FILL_STYLE ||
            paint.GetStyle() == Paint::STROKE_FILL_STYLE) {
            RenderSolid(paint, rasterizer, renBase, isStroke);
        }
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        if (paint.GetStyle() == Paint::GRADIENT) {
            RenderGradient(paint, rasterizer, transform, renBase, renderBuffer, allocator, invalidatedArea);
        }
#endif
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        if (paint.GetStyle() == Paint::PATTERN) {
            RenderPattern(paint, pathParam->imageParam, rasterizer, renBase, allocator, rect);
        }
#endif
    }

#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
    void UICanvas::DoDrawShadow(BufferInfo& gfxDstBuffer,
                                void* param,
                                const Paint& paint,
                                const Rect& rect,
                                const Rect& invalidatedArea,
                                const Style& style,
                                const bool& isStroke)
    {
        if (param == nullptr) {
            return;
        }
#    if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
        TransAffine transform;
        RenderingBuffer renderBuffer;
        // 初始化buffer和 m_transform
        InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, style, paint);

        transform.Translate(paint.GetShadowOffsetX(), paint.GetShadowOffsetY());

        RasterizerScanlineAntiAlias<> rasterizer;
        ScanlineUnPackedContainer m_scanline;
        PathParam* pathParam = static_cast<PathParam*>(param);
        rasterizer.ClipBox(0, 0, gfxDstBuffer.width, gfxDstBuffer.height);
        SetRasterizer(*pathParam->vertices, paint, rasterizer, transform, isStroke);
        RectD bbox(rasterizer.MinX(), rasterizer.MinY(), rasterizer.MaxX(), rasterizer.MaxY());

        PixFormat m_pixFormat(renderBuffer);
        RendererBase m_renBase(m_pixFormat);
        SpanAllocator allocator;

        m_renBase.ResetClipping(true);
        m_renBase.ClipBox(invalidatedArea.GetLeft(), invalidatedArea.GetTop(), invalidatedArea.GetRight(),
                          invalidatedArea.GetBottom());

        Rgba8Color shadowColor;
        ChangeColor(shadowColor, paint.GetShadowColor(), paint.GetShadowColor().alpha * paint.GetGlobalAlpha());

        RenderScanlinesAntiAliasSolid(rasterizer, m_scanline, m_renBase, shadowColor);
#        if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
        typedef OHOS::StackBlur<Rgba8Color, OHOS::StackBlurCalcRGBA<>> DrawBlur;
        typedef OHOS::PixfmtAlphaBlendRgba<Blender, OHOS::RenderingBuffer> PixfmtAlphaBlendRgba;
        DrawBlur drawBlur;

        bbox.x1 -= paint.GetShadowBlur();
        bbox.y1 -= paint.GetShadowBlur();
        bbox.x2 += paint.GetShadowBlur();
        bbox.y2 += paint.GetShadowBlur();
        RenderingBuffer shadowBuffer;
        PixfmtAlphaBlendRgba pixf2(shadowBuffer);

        Rect shadowRect = {int16_t(bbox.x1), int16_t(bbox.y1), int16_t(bbox.x2), int16_t(bbox.y2)};
        shadowRect.Intersect(shadowRect, invalidatedArea);
        pixf2.Attach(m_pixFormat, shadowRect.GetLeft(), shadowRect.GetTop(),
                     shadowRect.GetRight(), shadowRect.GetBottom());
        drawBlur.Blur(pixf2, MATH_UROUND(paint.GetShadowBlur()));
#        endif
#    endif
    }
#endif

    void UICanvas::InitRendAndTransform(BufferInfo& gfxDstBuffer,
                                        RenderingBuffer& renderBuffer,
                                        const Rect& rect,
                                        TransAffine& transform,
                                        const Style& style,
                                        const Paint& paint)
    {
        int16_t realLeft = rect.GetLeft() + style.paddingLeft_ + style.borderWidth_;
        int16_t realTop = rect.GetTop() + style.paddingTop_ + style.borderWidth_;
        transform.Reset();
        transform *= paint.GetTransAffine();
        transform.Translate(realLeft, realTop); // 偏移到画布上
        renderBuffer.Attach(static_cast<uint8_t*>(gfxDstBuffer.virAddr), gfxDstBuffer.width, gfxDstBuffer.height,
                            gfxDstBuffer.stride);
    }

#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWTEXT
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
            DrawCmd cmd;
            cmd.param = textParam;
            cmd.DeleteParam = DeleteTextParam;
            cmd.DrawGraphics = DoDrawText;
            cmd.paint = paint;
            cmd.paint.SetUICanvas(this);
            drawCmdList_.PushBack(cmd);
            Invalidate();
            SetStartPosition(point);
        }
    }
#endif

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

#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWTEXT
    void UICanvas::DoDrawText(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style)
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
        text->SetFont(textParam->fontStyle.fontName, textParam->fontStyle.fontSize * paint.GetScaleX());
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

        if (paint.GetChangeFlag()) {
            Rect textImageRect(0, 0, textRect.GetWidth(), textRect.GetHeight());
            if (paint.GetUICanvas() == nullptr) {
                return;
            }
            BufferInfo* mapBufferInfo = paint.GetUICanvas()->UpdateMapBufferInfo(gfxDstBuffer, textRect);
            text->OnDraw(*mapBufferInfo, textImageRect, textImageRect, textImageRect, 0, drawStyle,
                         Text::TEXT_ELLIPSIS_END_INV, opa);

            TransAffine transform;
            RenderingBuffer renderBuffer;
            // 初始化buffer和 m_transform
            InitRendAndTransform(gfxDstBuffer, renderBuffer, rect, transform, style, paint);

            transform.Translate(textParam->position.x, textParam->position.y);
            if (paint.GetScaleX() != 0) {
                transform.SetData(0, transform.GetData()[0] * (1.0f / paint.GetScaleX()));
            }
            transform.Translate(textParam->position.x, textParam->position.y);
            RenderingBuffer imageRendBuffer;
            imageRendBuffer.Attach(static_cast<uint8_t*>(mapBufferInfo->phyAddr), mapBufferInfo->width,
                                   mapBufferInfo->height, mapBufferInfo->stride);
            DoRenderImage(renderBuffer, paint, invalidatedArea, transform, imageRendBuffer);
        } else {
            text->OnDraw(gfxDstBuffer, invalidatedArea, textRect, textRect, 0,
                         drawStyle, Text::TEXT_ELLIPSIS_END_INV, opa);
        }
    }
#endif

    void UICanvas::DoRenderImage(RenderingBuffer& renderBuffer,
                                 const Paint& paint,
                                 const Rect& invalidatedArea,
                                 TransAffine& transform,
                                 RenderingBuffer& imageBuffer)
    {
        Rect cordsTmp;
        cordsTmp.SetPosition(0, 0);
        cordsTmp.SetHeight(imageBuffer.GetHeight());
        cordsTmp.SetWidth(imageBuffer.GetWidth());

        RasterizerScanlineAntiAlias<> rasterizer;
        ScanlineUnPackedContainer m_scanline;

        UICanvasVertices vertices;
        vertices.RemoveAll();
        vertices.MoveTo(cordsTmp.GetLeft(), cordsTmp.GetTop());
        vertices.LineTo(cordsTmp.GetRight(), cordsTmp.GetTop());
        vertices.LineTo(cordsTmp.GetRight(), cordsTmp.GetBottom());
        vertices.LineTo(cordsTmp.GetLeft(), cordsTmp.GetBottom());
        vertices.ClosePolygon();
        SetRasterizer(vertices, paint, rasterizer, transform, false);

        PixFormat m_pixFormat(renderBuffer);
        RendererBase renBase(m_pixFormat);
        SpanAllocator allocator;

        renBase.ResetClipping(true);
        renBase.ClipBox(invalidatedArea.GetLeft(), invalidatedArea.GetTop(), invalidatedArea.GetRight(),
                        invalidatedArea.GetBottom());

        float parallelogram[OHOS::INDEX_SIX] = {float(cordsTmp.GetLeft()), float(cordsTmp.GetTop()),
                                                float(cordsTmp.GetRight()), float(cordsTmp.GetTop()),
                                                float(cordsTmp.GetRight()), float(cordsTmp.GetBottom())};

        OHOS::TransAffine mtx((float)cordsTmp.GetLeft(), (float)cordsTmp.GetTop(), (float)cordsTmp.GetRight(),
                              (float)cordsTmp.GetBottom(), parallelogram);
        mtx *= transform;
        mtx.Invert();
        typedef OHOS::SpanInterpolatorLinear<OHOS::TransAffine> Interpolator;
        Interpolator interpolator(mtx);
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色

        typedef OHOS::ImageAccessorClone<PixFormat> ImgSourceType;
        typedef SpanImageRgba<ImgSourceType, Interpolator> SpanGenType;

        PixFormat imagPixfmtCom(imageBuffer);
        ImgSourceType source(imagPixfmtCom);
        SpanGenType sg(source, interpolator);
        OHOS::RenderScanlinesAntiAlias(rasterizer, m_scanline, renBase, allocator, sg);
        vertices.FreeAll();
    }
    void UICanvas::CopyBuffer(BufferInfo& gfxMapBuffer, BufferInfo& gfxDstBuffer)
    {
        uint8_t destByteSize;
        if (memcpy_s(&gfxMapBuffer, sizeof(BufferInfo), &gfxDstBuffer, sizeof(BufferInfo)) != 0) {
            return;
        }
        destByteSize = DrawUtils::GetByteSizeByColorMode(gfxDstBuffer.mode);
        uint32_t destStride = gfxMapBuffer.width * destByteSize;
        uint32_t buffSize = gfxMapBuffer.height * destStride;
        gfxMapBuffer.mode = gfxDstBuffer.mode;
        gfxMapBuffer.virAddr = BaseGfxEngine::GetInstance()->AllocBuffer(buffSize, BUFFER_MAP_SURFACE);
        gfxMapBuffer.stride = destStride;
        memset_s(gfxMapBuffer.virAddr, buffSize, 0, buffSize);
        gfxMapBuffer.phyAddr = gfxMapBuffer.virAddr;
    }

    void UICanvas::InitGfxMapBuffer(const BufferInfo& srcBuff, const Rect& rect)
    {
        gfxMapBuffer_ = new BufferInfo();
        gfxMapBuffer_->rect = rect;
        gfxMapBuffer_->mode = srcBuff.mode;
        gfxMapBuffer_->color = srcBuff.color;
        gfxMapBuffer_->width = rect.GetWidth();
        gfxMapBuffer_->height = rect.GetHeight();
        uint8_t destByteSize = DrawUtils::GetByteSizeByColorMode(srcBuff.mode);
        gfxMapBuffer_->stride = gfxMapBuffer_->width * destByteSize;
        gfxMapBuffer_->virAddr = UIMalloc(gfxMapBuffer_->height * gfxMapBuffer_->stride);
        gfxMapBuffer_->phyAddr = gfxMapBuffer_->virAddr;
        uint32_t buffSize = gfxMapBuffer_->height * gfxMapBuffer_->stride;
        errno_t err = memset_s(gfxMapBuffer_->virAddr, buffSize, 0, buffSize);
        if (err != EOK) {
            BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)gfxMapBuffer_->virAddr);
            GRAPHIC_LOGE("memset_s gfxMapBuffer_ fail");
            return;
        }
    }

    BufferInfo* UICanvas::UpdateMapBufferInfo(const BufferInfo& srcBuff, const Rect& rect)
    {
        if (gfxMapBuffer_ == nullptr) {
            InitGfxMapBuffer(srcBuff, rect);
            return gfxMapBuffer_;
        }

        if (rect.GetWidth() != gfxMapBuffer_->width ||
            rect.GetHeight() != gfxMapBuffer_->height ||
            srcBuff.mode != gfxMapBuffer_->mode) {
            DestroyMapBufferInfo();
            InitGfxMapBuffer(srcBuff, rect);
        } else {
            uint32_t buffSize = gfxMapBuffer_->height * gfxMapBuffer_->stride;
            errno_t err = memset_s(gfxMapBuffer_->virAddr, buffSize, 0, buffSize);
            if (err != EOK) {
                BaseGfxEngine::GetInstance()->FreeBuffer((uint8_t*)gfxMapBuffer_->virAddr);
                GRAPHIC_LOGE("memset_s gfxMapBuffer_ fail");
            }
        }
        return gfxMapBuffer_;
    }

    void UICanvas::DestroyMapBufferInfo()
    {
        if (gfxMapBuffer_ != nullptr) {
            BaseGfxEngine::GetInstance()->FreeBuffer(static_cast<uint8_t*>(gfxMapBuffer_->virAddr));
            gfxMapBuffer_->virAddr = nullptr;
            gfxMapBuffer_->phyAddr = nullptr;
            delete gfxMapBuffer_;
            gfxMapBuffer_ = nullptr;
        }
    }
} // namespace OHOS
