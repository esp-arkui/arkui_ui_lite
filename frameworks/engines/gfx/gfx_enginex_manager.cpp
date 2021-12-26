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

#include "engines/gfx/gfx_enginex_manager.h"

static const double g_approxScale = 2.0;
namespace OHOS {
    BaseGfxExtendEngine::~BaseGfxExtendEngine()
    {
        if (dashes) {
            delete[] dashes;
            dashes = NULL;
        }
    }

    BaseGfxExtendEngine::BaseGfxExtendEngine() :
        m_rbuf(),
        m_scanline(),
        m_rasterizer(),
        m_pixFormat(m_rbuf),
        m_pixFormatComp(m_rbuf),
        m_pixFormatPre(m_rbuf),
        m_pixFormatCompPre(m_rbuf),
        m_renBase(m_pixFormat),
        m_renBaseComp(m_pixFormatComp),
        m_renBasePre(m_pixFormatPre),
        m_renBaseCompPre(m_pixFormatCompPre),
        m_renSolid(m_renBase),
        m_renSolidComp(m_renBaseComp),
        m_allocator(),
        m_clipBox(0, 0, 0, 0),
        m_blendMode(BLENDALPHA),
        m_imageBlendMode(BLENDDST),
        m_imageBlendColor(0, 0, 0),
        m_fillColor(OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM),
        m_lineColor(0, 0, 0),
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        m_lineCap(CAPROUND),
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        m_lineJoin(JOINROUND),
#endif
        m_fillGradientMatrix(),
        m_lineGradientMatrix(),
        m_fillRadialMatrix(),
        m_antiAliasGamma(1.0),
        m_miterLimit(OHOS::DEFAULTMITERLIMIT),
        m_fillGradientD1(0.0),
        m_lineGradientD1(0.0),
        m_fillGradientInterpolator(m_fillGradientMatrix),
        m_lineGradientInterpolator(m_lineGradientMatrix),
        m_linearGradientFunction(),
        m_fillGradientFlag(SOLID),
        m_lineGradientFlag(SOLID),
        m_interpolator_type(m_fillRadialMatrix),
        m_radialGradientFunction(),
        m_fillGradientD2(0.0),
        m_lineGradientD2(0.0),
        m_lineWidth(1),
        m_path(),
        m_transform(),
        m_convCurve(m_path),
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
        m_convDashCurve(m_convCurve),
        m_convDashStroke(m_convDashCurve),
        m_dashStrokeTransform(m_convDashStroke, m_transform),
#endif
        m_convStroke(m_convCurve),

        m_pathTransform(m_convCurve, m_transform),
        m_strokeTransform(m_convStroke, m_transform),
        m_evenOddFlag(false),
        is_dash(false),
        ndashes(0),
        dashes(nullptr),
        shadowColor_(Color(OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM)),
        m_masterAlpha(1.0),
        shadowOffsetY_(0),
        shadowOffsetX_(0),
        shadowBlurRadius_(0),
        dDashOffset(0)
    {
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        SetLineCap(m_lineCap);
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        SetLineJoin(m_lineJoin);
#endif
    }

    BaseGfxExtendEngine::BaseGfxExtendEngine(const BaseGfxExtendEngine& baseGfxExtendEngine) :
        m_scanline(),
        m_rasterizer(),
        m_fillGradientMatrix(baseGfxExtendEngine.m_fillGradientMatrix),
        m_lineGradientMatrix(baseGfxExtendEngine.m_lineGradientMatrix),
        m_fillGradientInterpolator(m_fillGradientMatrix),
        m_lineGradientInterpolator(m_lineGradientMatrix),
        m_path(baseGfxExtendEngine.m_path),
        m_convCurve(m_path),
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
        m_convDashCurve(m_convCurve),
        m_convDashStroke(m_convDashCurve),
        m_dashStrokeTransform(m_convDashStroke, m_transform),
#endif
        m_convStroke(m_convCurve),
        m_pathTransform(m_convCurve, m_transform),
        m_strokeTransform(m_convStroke, m_transform)

    {
        m_rbuf = baseGfxExtendEngine.m_rbuf;
        m_pixFormat = PixFormat(m_rbuf);
        m_pixFormatComp = PixFormatComp(m_rbuf);
        m_pixFormatPre = PixFormatPre(m_rbuf);
        m_pixFormatCompPre = PixFormatCompPre(m_rbuf);
        m_renBase = RendererBase(m_pixFormat);
        m_renBaseComp = RendererBaseComp(m_pixFormatComp);
        m_renBasePre = RendererBasePre(m_pixFormatPre);
        m_renBaseCompPre = RendererBaseCompPre(m_pixFormatCompPre);
        m_renSolid = RendererSolid(m_renBase);
        m_renSolidComp = RendererSolidComp(m_renBaseComp);
        m_allocator = baseGfxExtendEngine.m_allocator;
        m_clipBox = baseGfxExtendEngine.m_clipBox;
        m_blendMode = baseGfxExtendEngine.m_blendMode;
        m_imageBlendMode = baseGfxExtendEngine.BLENDDST;
        m_imageBlendColor = baseGfxExtendEngine.m_imageBlendColor;
        m_masterAlpha = baseGfxExtendEngine.m_masterAlpha;
        m_antiAliasGamma = baseGfxExtendEngine.m_antiAliasGamma;
        m_fillColor = baseGfxExtendEngine.m_fillColor;
        m_lineColor = baseGfxExtendEngine.m_lineColor;
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        m_lineCap = baseGfxExtendEngine.m_lineCap;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        m_lineJoin = baseGfxExtendEngine.m_lineJoin;
#endif
        m_miterLimit = baseGfxExtendEngine.m_miterLimit;
        m_fillGradientFlag = baseGfxExtendEngine.SOLID;
        m_lineGradientFlag = baseGfxExtendEngine.SOLID;
        m_fillGradientD1 = baseGfxExtendEngine.m_fillGradientD1;
        m_lineGradientD1 = baseGfxExtendEngine.m_lineGradientD1;
        m_fillGradientD2 = baseGfxExtendEngine.m_fillGradientD2;
        m_lineGradientD2 = baseGfxExtendEngine.m_lineGradientD2;
        m_linearGradientFunction = baseGfxExtendEngine.m_linearGradientFunction;
        m_radialGradientFunction = baseGfxExtendEngine.m_radialGradientFunction;
        m_lineWidth = baseGfxExtendEngine.m_lineWidth;
        m_evenOddFlag = baseGfxExtendEngine.m_evenOddFlag;
        m_transform = baseGfxExtendEngine.m_transform;
        shadowBlurRadius_ = baseGfxExtendEngine.shadowBlurRadius_;
        shadowColor_ = baseGfxExtendEngine.shadowColor_;
        shadowOffsetX_ = baseGfxExtendEngine.shadowOffsetX_;
        shadowOffsetY_ = baseGfxExtendEngine.shadowOffsetY_;
        is_dash = baseGfxExtendEngine.is_dash;
        if (is_dash) {
            ndashes = baseGfxExtendEngine.ndashes;
            dDashOffset = baseGfxExtendEngine.dDashOffset;
            dashes = new float[ndashes];
            if (dashes) {
                for (unsigned int i = 0; i < ndashes; i++) {
                    dashes[i] = baseGfxExtendEngine.dashes[i];
                }
            } else {
                ndashes = 0;
                dDashOffset = 0;
                is_dash = false;
            }
        }
    }

    void BaseGfxExtendEngine::Attach(unsigned char* buf, unsigned width, unsigned height, int stride)
    {
        m_rbuf.Attach(buf, width, height, stride);
        m_renBase.ResetClipping(true);
        m_renBaseComp.ResetClipping(true);
        m_renBasePre.ResetClipping(true);
        m_renBaseCompPre.ResetClipping(true);
        ResetTransformations();
        SetLineWidth(1.0);
        SetLineColor(0, 0, 0);
        SetFillColor(OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM, OHOS::MAX_COLOR_NUM);
        ClipBox(0, 0, width, height);
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        SetLineCap(CAPROUND);
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        SetLineJoin(JOINROUND);
#endif
        m_masterAlpha = 1.0;
        m_antiAliasGamma = 1.0;
        m_rasterizer.GammaFunction(OHOS::GammaNone());
        m_blendMode = BLENDALPHA;
    }

    void BaseGfxExtendEngine::Attach(Image& img)
    {
        Attach(img.renBuf.GetBuf(), img.renBuf.GetWidth(), img.renBuf.GetHeight(), img.renBuf.GetStride());
    }

    void BaseGfxExtendEngine::ClipBox(double x1, double y1, double x2, double y2)
    {
        m_clipBox = RectD(x1, y1, x2, y2);
        int rx1 = int(x1);
        int ry1 = int(y1);
        int rx2 = int(x2);
        int ry2 = int(y2);
        m_renBase.ClipBox(rx1, ry1, rx2, ry2);
        m_renBaseComp.ClipBox(rx1, ry1, rx2, ry2);
        m_renBasePre.ClipBox(rx1, ry1, rx2, ry2);
        m_renBaseCompPre.ClipBox(rx1, ry1, rx2, ry2);
        m_rasterizer.ClipBox(x1, y1, x2, y2);
    }

    BaseGfxExtendEngine::RectD BaseGfxExtendEngine::GetClipBox() const
    {
        return m_clipBox;
    }

    void BaseGfxExtendEngine::SetBlendMode(BlendMode bMode)
    {
        m_blendMode = bMode;
        m_pixFormatComp.CompOp(bMode);
        m_pixFormatCompPre.CompOp(bMode);
    }

    BaseGfxExtendEngine::BlendMode BaseGfxExtendEngine::GetBlendMode() const
    {
        return m_blendMode;
    }

    void BaseGfxExtendEngine::SetMasterAlpha(double alpha)
    {
        if (m_masterAlpha != alpha) {
            m_masterAlpha = alpha;
            UpdateRasterizerGamma();
        }
    }

    double BaseGfxExtendEngine::GetMasterAlpha() const
    {
        return m_masterAlpha;
    }

    void BaseGfxExtendEngine::ClearAll(Color color)
    {
        m_renBase.Clear(color);
    }

    void BaseGfxExtendEngine::ClearAll(unsigned red, unsigned green, unsigned blue, unsigned alpha)
    {
        ClearAll(Color(red, green, blue, alpha));
    }

    void BaseGfxExtendEngine::WorldToScreen(double& x, double& y) const
    {
        m_transform.Transform(&x, &y);
    }

    void BaseGfxExtendEngine::ScreenToWorld(double& x, double& y) const
    {
        m_transform.InverseTransform(&x, &y);
    }

    double BaseGfxExtendEngine::WorldToScreen(double scalar) const
    {
        double x1 = 0;
        double y1 = 0;
        double x2 = scalar;
        double y2 = scalar;
        WorldToScreen(x1, y1);
        WorldToScreen(x2, y2);
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * OHOS::COS45;
    }

    double BaseGfxExtendEngine::ScreenToWorld(double scalar) const
    {
        double x1 = 0;
        double y1 = 0;
        double x2 = scalar;
        double y2 = scalar;
        ScreenToWorld(x1, y1);
        ScreenToWorld(x2, y2);
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * OHOS::COS45;
    }

    void BaseGfxExtendEngine::ResetTransformations()
    {
        m_transform.Reset();
    }

    void BaseGfxExtendEngine::Rotate(double angle)
    {
        m_transform *= OHOS::TransAffineRotation(angle);
    }
    void BaseGfxExtendEngine::Rotate(double x, double y, double angle)
    {
        m_transform *= OHOS::TransAffineTranslation(-x, -y);
        m_transform *= OHOS::TransAffineRotation(angle * Pi() / OHOS::BOXER);
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }
    void BaseGfxExtendEngine::Translate(double x, double y)
    {
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }

    void BaseGfxExtendEngine::SetAffine(const Affine& tr)
    {
        m_transform *= tr;
        m_convCurve.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
#endif
        }
    }

    void BaseGfxExtendEngine::SetAffine(const Transformations& tr)
    {
        Affine(OHOS::TransAffine(tr.affineMatrix[0], tr.affineMatrix[1], tr.affineMatrix[2],
                                 tr.affineMatrix[3], tr.affineMatrix[4], tr.affineMatrix[5]));
    }

    void BaseGfxExtendEngine::Scale(double sx, double sy)
    {
        m_transform *= OHOS::TransAffineScaling(sx, sy);
        m_convCurve.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
#endif
        }
    }

    void BaseGfxExtendEngine::Viewport(double worldX1, double worldY1, double worldX2, double worldY2,
                                       double screenX1, double screenY1, double screenX2, double screenY2,
                                       ViewportOption opt)
    {
        OHOS::TransViewPort vp;
        switch (opt) {
            case ANISOTROPIC: vp.PreserveAspectRatio(0.0, 0.0, OHOS::ASPECT_RATIO_STRETCH); break;
            case XMINYMIN: vp.PreserveAspectRatio(0.0, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMIDYMIN: vp.PreserveAspectRatio(HALF, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMAXYMIN: vp.PreserveAspectRatio(1.0, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMINYMID: vp.PreserveAspectRatio(0.0, HALF, OHOS::ASPECT_RATIO_MEET); break;
            case XMIDYMID: vp.PreserveAspectRatio(HALF, HALF, OHOS::ASPECT_RATIO_MEET); break;
            case XMAXYMID: vp.PreserveAspectRatio(1.0, HALF, OHOS::ASPECT_RATIO_MEET); break;
            case XMINYMAX: vp.PreserveAspectRatio(0.0, 1.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMIDYMAX: vp.PreserveAspectRatio(HALF, 1.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMAXYMAX: vp.PreserveAspectRatio(1.0, 1.0, OHOS::ASPECT_RATIO_MEET); break;
        }
        vp.WorldViewPort(worldX1, worldY1, worldX2, worldY2);
        vp.DeviceViewPort(screenX1, screenY1, screenX2, screenY2);
        m_transform *= vp.ToAffine();
        m_convCurve.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
        } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.ApproximationScale(WorldToScreen(1.0) * g_approxScale);
#endif
        }
    }

    void BaseGfxExtendEngine::SetFillColor(Color color)
    {
        m_fillColor = color;
        m_fillGradientFlag = SOLID;
    }

    void BaseGfxExtendEngine::SetFillColor(unsigned read, unsigned green, unsigned blue, unsigned alpha)
    {
        SetFillColor(Color(read, green, blue, alpha));
    }

    void BaseGfxExtendEngine::NoFill()
    {
        SetFillColor(Color(0, 0, 0, 0));
    }

    void BaseGfxExtendEngine::SetLineColor(Color color)
    {
        m_lineColor = color;
        m_lineGradientFlag = SOLID;
    }

    void BaseGfxExtendEngine::SetLineColor(unsigned read, unsigned green, unsigned blue, unsigned alpha)
    {
        SetLineColor(Color(read, green, blue, alpha));
    }

    void BaseGfxExtendEngine::NoLine()
    {
        SetLineColor(Color(0, 0, 0, 0));
    }

    BaseGfxExtendEngine::Color BaseGfxExtendEngine::GetFillColor() const
    {
        return m_fillColor;
    }

    BaseGfxExtendEngine::Color BaseGfxExtendEngine::GetLineColor() const
    {
        return m_lineColor;
    }
    void BaseGfxExtendEngine::SetFillColor(const OHOS::ColorType& color)
    {
        SetFillColor(Color(color.red, color.green, color.blue, color.alpha));
    }

    void BaseGfxExtendEngine::SetLineColor(const OHOS::ColorType& color)
    {
        SetLineColor(Color(color.red, color.green, color.blue, color.alpha));
    }

    void BaseGfxExtendEngine::RemoveAllColor()
    {
        m_fillRadialGradient.RemoveAll();
    }

    void BaseGfxExtendEngine::AddColor(double offset, Color c1)
    {
        m_fillRadialGradient.AddColor(offset, c1);
    }

    void BaseGfxExtendEngine::BuildLut()
    {
        m_fillRadialGradient.BuildLut();
    }

    void BaseGfxExtendEngine::SetRadialGradient(
        double start_x, double start_y, double start_r, double end_x, double end_y, double end_r)
    {
        m_fillRadialMatrix.Reset();
        m_fillRadialMatrix *= OHOS::TransAffineTranslation(end_x, end_y);
        m_fillRadialMatrix *= m_transform;
        m_fillRadialMatrix.Invert();
        m_interpolator_type.SetTransformer(m_fillRadialMatrix);
        m_fillGradientD1 = start_r;
        m_fillGradientD2 = end_r;
        m_radialGradientFunction = OHOS::GradientRadialCalculate(end_r, start_x - end_x, start_y - end_y);
        m_fillGradientFlag = RADIAL;
        m_fillColor = Color(0, 0, 0, OHOS::MAX_COLOR_NUM);
    }

    void BaseGfxExtendEngine::SetLinearGradient(double start_x, double start_y, double end_x, double end_y)
    {
        double angle = atan2(end_y - start_y, end_x - start_x);
        m_fillGradientMatrix.Reset();
        m_fillGradientMatrix *= OHOS::TransAffineRotation(angle);
        m_fillGradientMatrix *= OHOS::TransAffineTranslation(start_x, start_y);
        m_fillGradientMatrix *= m_transform;
        m_fillGradientMatrix.Invert();
        m_fillGradientD1 = 0.0;
        m_fillGradientD2 = sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
        m_fillGradientFlag = LINEAR;
        m_fillColor = Color(0, 0, 0);
    }

    void BaseGfxExtendEngine::SetLineWidth(double w)
    {
        if (w < 0.0) {
            w = abs(w);
        }
        m_lineWidth = w;
        if (!this->is_dash) {
            m_convStroke.Width(w);
        } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.Width(w);
#endif
        }
    }

    double BaseGfxExtendEngine::GetLineWidth() const
    {
        return m_lineWidth;
    }
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
    void BaseGfxExtendEngine::SetLineCap(LineCap cap)
    {
        m_lineCap = cap;
        if (!this->is_dash) {
            m_convStroke.LineCap((OHOS::LineCapEnum)cap);
        } else {
#    if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.LineCap((OHOS::LineCapEnum)cap);
#    endif
        }
    }

    BaseGfxExtendEngine::LineCap BaseGfxExtendEngine::GetLineCap() const
    {
        return m_lineCap;
    }
#endif

#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
    void BaseGfxExtendEngine::SetLineJoin(LineJoin join)
    {
        m_lineJoin = join;
        if (!this->is_dash) {
            m_convStroke.LineJoin((OHOS::LineJoinEnum)join);
        } else {
#    if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.LineJoin((OHOS::LineJoinEnum)join);
#    endif
        }
    }

    BaseGfxExtendEngine::LineJoin BaseGfxExtendEngine::GetLineJoin() const
    {
        return m_lineJoin;
    }

    void BaseGfxExtendEngine::SetMiterLimit(double limitValue)
    {
        m_miterLimit = limitValue;
        if (!this->is_dash) {
            m_convStroke.MiterLimit(limitValue);
        } else {
#    if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
            m_convDashStroke.MiterLimit(limitValue);
#    endif
        }
    }

    double BaseGfxExtendEngine::GetMiterLimit() const
    {
        return m_miterLimit;
    }
#endif
    void BaseGfxExtendEngine::AddLine(double x1, double y1, double x2, double y2)
    {
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y2);
    }

    void BaseGfxExtendEngine::Line(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        AddLine(x1, y1, x2, y2);
        DrawPath(STROKEONLY);
    }

    void BaseGfxExtendEngine::Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x3, y3);
        m_path.ClosePolygon();
        DrawPath(FILLANDSTROKE);
    }

    void BaseGfxExtendEngine::Rectangle(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x1, y2);
        m_path.ClosePolygon();
        DrawPath(FILLANDSTROKE);
    }
    void BaseGfxExtendEngine::Rectstroke(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x1, y2);
        m_path.ClosePolygon();
        Stroke();
    }
    void BaseGfxExtendEngine::Round(double cx, double cy, double radius)
    {
        m_path.RemoveAll();
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
        OHOS::BezierArc arc(cx, cy, radius, radius, 0, OHOS::TWO_TIMES * Pi());
        m_path.ConcatPath(arc, 0);
#endif
        m_path.ClosePolygon();
        DrawPath(FILLANDSTROKE);
    }
    void BaseGfxExtendEngine::ResetPath()
    {
        m_path.RemoveAll();
    }

    void BaseGfxExtendEngine::MoveTo(double x, double y)
    {
        m_path.MoveTo(x, y);
    }

    void BaseGfxExtendEngine::LineTo(double x, double y)
    {
        m_path.LineTo(x, y);
    }

    void BaseGfxExtendEngine::ArcTo(double rx, double ry,
                                    double angle,
                                    bool largeArcFlag,
                                    bool sweepFlag,
                                    double x, double y)
    {
        m_path.ArcTo(rx, ry, angle, largeArcFlag, sweepFlag, x, y);
    }

    void BaseGfxExtendEngine::ClosePolygon()
    {
        m_path.ClosePolygon();
    }

    void BaseGfxExtendEngine::TransformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             double dstX1, double dstY1, double dstX2, double dstY2)
    {
        ResetPath();
        MoveTo(dstX1, dstY1);
        LineTo(dstX2, dstY1);
        LineTo(dstX2, dstY2);
        LineTo(dstX1, dstY2);
        ClosePolygon();
        double parallelogram[OHOS::INDEX_SIX] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};
        RenderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
    }

    void BaseGfxExtendEngine::TransformImage(
        const Image& img, double dstX1, double dstY1, double dstX2, double dstY2)
    {
        ResetPath();
        MoveTo(dstX1, dstY1);
        LineTo(dstX2, dstY1);
        LineTo(dstX2, dstY2);
        LineTo(dstX1, dstY2);
        ClosePolygon();
        double parallelogram[OHOS::INDEX_SIX] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};

        RenderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram);
    }

    void BaseGfxExtendEngine::TransformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             const double* parallelogram)
    {
        ResetPath();
        MoveTo(parallelogram[OHOS::INDEX_ZERO], parallelogram[OHOS::INDEX_ONE]);
        LineTo(parallelogram[OHOS::INDEX_TWO], parallelogram[OHOS::INDEX_THREE]);
        LineTo(parallelogram[OHOS::INDEX_FOUR], parallelogram[OHOS::INDEX_FIVE]);
        LineTo(parallelogram[OHOS::INDEX_ZERO] + parallelogram[OHOS::INDEX_FOUR] - parallelogram[OHOS::INDEX_TWO],
               parallelogram[OHOS::INDEX_ONE] + parallelogram[OHOS::INDEX_FIVE] - parallelogram[OHOS::INDEX_THREE]);
        ClosePolygon();
        RenderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
    }

    void BaseGfxExtendEngine::TransformImage(const Image& img, const double* parallelogram)
    {
        ResetPath();
        MoveTo(parallelogram[OHOS::INDEX_ZERO], parallelogram[OHOS::INDEX_ONE]);
        LineTo(parallelogram[OHOS::INDEX_TWO], parallelogram[OHOS::INDEX_THREE]);
        LineTo(parallelogram[OHOS::INDEX_FOUR], parallelogram[OHOS::INDEX_FIVE]);
        LineTo(parallelogram[OHOS::INDEX_ZERO] + parallelogram[OHOS::INDEX_FOUR] - parallelogram[OHOS::INDEX_TWO],
               parallelogram[OHOS::INDEX_ONE] + parallelogram[OHOS::INDEX_FIVE] - parallelogram[OHOS::INDEX_THREE]);
        ClosePolygon();

        RenderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram);
    }

    void BaseGfxExtendEngine::DrawShadow(
        double x, double y, double angle, double scaleX, double scaleY,
        double transLateX, double transLateY)
    {
        m_rasterizer.Reset();
        OHOS::TransAffine transform(m_transform.scaleX, m_transform.shearY,
                                    m_transform.shearX, m_transform.scaleY, m_transform.translateX, m_transform.translateY);
        PathTransform shadow_trans(m_convCurve, transform);
        transform.Translate(shadowOffsetX_, shadowOffsetY_);
        transform *= OHOS::TransAffineTranslation(-x, -y);
        if (angle != 0) {
            transform *= OHOS::TransAffineRotation(angle * Pi() / OHOS::BOXER);
        }
        if (scaleX != 0 || scaleY != 0) {
            transform *= OHOS::TransAffineScaling(scaleX, scaleY);
        }
        if (transLateX != 0 || transLateY != 0) {
            transform *= OHOS::TransAffineTranslation(transLateX, transLateY);
        }
        transform *= OHOS::TransAffineTranslation(x, y);
        m_rasterizer.AddPath(shadow_trans);
        OHOS::RenderScanlinesAntiAliasSolid(m_rasterizer, m_scanline, m_renBase, shadowColor_);
        if (shadowBlurRadius_ != 0) {
            RectD bbox;
            BoundingRectSingle(0, &bbox, shadow_trans);
            bbox.x1 -= shadowBlurRadius_;
            bbox.y1 -= shadowBlurRadius_;
            bbox.x2 += shadowBlurRadius_;
            bbox.y2 += shadowBlurRadius_;
            RenderingBuffer m_rbuf_window;
            PixFormat pixf2(m_rbuf_window);
            pixf2.Attach(m_pixFormat, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2));
#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
            m_stack_blur.Blur(pixf2, OHOS::Uround(shadowBlurRadius_));
#endif
        }
        m_rasterizer.Reset();
    }
    void BaseGfxExtendEngine::DrawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry,
                                         double x, double y, double angle, double scaleX, double scaleY,
                                         double transLateX, double transLateY)
    {
        m_path.RemoveAll();
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
        OHOS::BezierArc arc(cx, cy, rx, ry, 0, OHOS::TWO_TIMES * Pi());
        m_path.ConcatPath(arc, 0);
#endif
        m_path.ClosePolygon();
        DrawShadow(x, y, angle, scaleX, scaleY, transLateX, transLateY);
    }
    void BaseGfxExtendEngine::Scale(double x, double y, double scaleX, double scaleY)
    {
        m_transform *= OHOS::TransAffineTranslation(-x, -y);
        m_transform *= OHOS::TransAffineScaling(scaleX, scaleY);
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }

    void BaseGfxExtendEngine::DrawPath(DrawPathFlag flag)
    {
        m_rasterizer.Reset();
        switch (flag) {
            case FILLONLY:
                if (m_fillColor.alphaValue) {
                    m_rasterizer.AddPath(m_pathTransform);
                    Render(true);
                }
                break;

            case STROKEONLY:
                if (m_lineColor.alphaValue && m_lineWidth > 0.0) {
                    if (!this->is_dash) {
                        m_rasterizer.AddPath(m_strokeTransform);
                    } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                        for (unsigned int i = 0; i < this->ndashes; i += OHOS::TWO_STEP) {
                            m_convDashCurve.AddDash(dashes[i], dashes[i + 1]);
                        }
                        m_convDashCurve.DashStart(dDashOffset);
                        m_rasterizer.AddPath(m_dashStrokeTransform);
#endif
                    }
                    Render(false);
                }
                break;

            case FILLANDSTROKE:
                if (m_fillColor.alphaValue) {
                    m_rasterizer.AddPath(m_pathTransform);
                    Render(true);
                }

                if (m_lineColor.alphaValue && m_lineWidth > 0.0) {
                    if (!this->is_dash) {
                        m_rasterizer.AddPath(m_strokeTransform);
                    } else {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                        for (unsigned int i = 0; i < this->ndashes; i += OHOS::TWO_STEP) {
                            m_convDashCurve.AddDash(dashes[i], dashes[i + 1]);
                        }
                        m_convDashCurve.DashStart(dDashOffset);
                        m_rasterizer.AddPath(m_dashStrokeTransform);
#endif
                    }
                    Render(false);
                }
                break;

            case FILLWITHLINECOLOR:
                if (m_lineColor.alphaValue) {
                    m_rasterizer.AddPath(m_pathTransform);
                    Render(false);
                }
                break;
        }
    }

    void BaseGfxExtendEngine::Stroke()
    {
        m_rasterizer.Reset();
        m_rasterizer.AddPath(m_strokeTransform);
        Render(false);
    }

    class BaseGfxExtendEngineRenderer {
    public:
        template <class BaseRenderer, class SolidRenderer>
        void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase, SolidRenderer& renSolid, bool fillColor)
        {
            using SpanAllocatorType = OHOS::SpanFillColorAllocator<BaseGfxExtendEngine::ColorType>;
            using RendererLinearGradient = OHOS::RendererScanlineAntiAlias<BaseRenderer, SpanAllocatorType,
                                                                           BaseGfxExtendEngine::LinearGradientSpan>;
            using RendererRadialGradient = OHOS::RendererScanlineAntiAlias<BaseRenderer, SpanAllocatorType,
                                                                           BaseGfxExtendEngine::RadialGradientSpan>;

            if (gr.m_fillGradientFlag == BaseGfxExtendEngine::LINEAR) {
                if (fillColor) {
                    BaseGfxExtendEngine::LinearGradientSpan span(gr.m_fillGradientInterpolator,
                                                                 gr.m_linearGradientFunction,
                                                                 gr.m_fillRadialGradient,
                                                                 gr.m_fillGradientD1,
                                                                 gr.m_fillGradientD2);
                    RendererLinearGradient ren(renBase, gr.m_allocator, span);
                    OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, ren);
                } else {
                    BaseGfxExtendEngine::LinearGradientSpan span(gr.m_fillGradientInterpolator,
                                                                 gr.m_linearGradientFunction,
                                                                 gr.m_fillRadialGradient,
                                                                 gr.m_fillGradientD1,
                                                                 gr.m_fillGradientD2);

                    OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, span);
                }
            } else if (gr.m_fillGradientFlag == BaseGfxExtendEngine::RADIAL) {
                if (fillColor) {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                        gr.m_interpolator_type,
                        gr.m_radialGradientFunction,
                        gr.m_fillRadialGradient,
                        gr.m_fillGradientD1,
                        gr.m_fillGradientD2);
                    RendererRadialGradient ren(renBase, gr.m_allocator, span);
                    OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, ren);
                } else {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                        gr.m_interpolator_type,
                        gr.m_radialGradientFunction,
                        gr.m_fillRadialGradient,
                        gr.m_fillGradientD1,
                        gr.m_fillGradientD2);
                    OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, span);
                }
            } else {
                renSolid.SetColor(fillColor ? gr.m_fillColor : gr.m_lineColor);
                OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, renSolid);
            }
        }

        template <class BaseRenderer, class SolidRenderer, class Rasterizer, class Scanline>
        void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase,
                           SolidRenderer& renSolid, Rasterizer& ras, Scanline& sl)
        {
            using SpanAllocatorType = OHOS::SpanFillColorAllocator<BaseGfxExtendEngine::ColorType>;
            using RendererLinearGradient = OHOS::RendererScanlineAntiAlias<BaseRenderer, SpanAllocatorType,
                                                                           BaseGfxExtendEngine::LinearGradientSpan>;
            using RendererRadialGradient = OHOS::RendererScanlineAntiAlias<BaseRenderer, SpanAllocatorType,
                                                                           BaseGfxExtendEngine::RadialGradientSpan>;

            if (gr.m_fillGradientFlag == BaseGfxExtendEngine::LINEAR) {
                BaseGfxExtendEngine::LinearGradientSpan span(
                    gr.m_fillGradientInterpolator,
                    gr.m_linearGradientFunction,
                    gr.m_fillRadialGradient,
                    gr.m_fillGradientD1,
                    gr.m_fillGradientD2);
                RendererLinearGradient ren(renBase, gr.m_allocator, span);
                OHOS::RenderScanlines(ras, sl, ren);
            } else {
                if (gr.m_fillGradientFlag == BaseGfxExtendEngine::RADIAL) {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                        gr.m_interpolator_type,
                        gr.m_radialGradientFunction,
                        gr.m_fillRadialGradient,
                        gr.m_fillGradientD1,
                        gr.m_fillGradientD2);
                    RendererRadialGradient ren(renBase, gr.m_allocator, span);
                    OHOS::RenderScanlines(ras, sl, ren);
                } else {
                    renSolid.color(gr.m_fillColor);
                    OHOS::RenderScanlines(ras, sl, renSolid);
                }
            }
        }

        template <class BaseRenderer, class Interpolator>
        static void renderImage(BaseGfxExtendEngine& gr, const BaseGfxExtendEngine::Image& img,
                                BaseRenderer& renBase, Interpolator& interpolator)
        {
            BaseGfxExtendEngine::Image& imgc = const_cast<BaseGfxExtendEngine::Image&>(img);
            BaseGfxExtendEngine::PixFormat img_pixf(imgc.renBuf);
            using ImgSourceType = OHOS::ImageAccessorClone<BaseGfxExtendEngine::PixFormat>;
            ImgSourceType source(img_pixf);
            using SpanGenType = OHOS::SpanImageRgba<ImgSourceType, Interpolator>;
            SpanGenType sg(source, interpolator);
            OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, sg);
        }
    };

    void BaseGfxExtendEngine::Render(bool fillColor)
    {
        if (m_blendMode == BLENDALPHA) {
            BaseGfxExtendEngineRenderer::render(*this, m_renBase, m_renSolid, fillColor);
        } else {
            BaseGfxExtendEngineRenderer::render(*this, m_renBaseComp, m_renSolidComp, fillColor);
        }
    }

    void BaseGfxExtendEngine::RenderImage(const Image& img, int x1, int y1, int x2, int y2,
                                          const double* parl)
    {
        OHOS::TransAffine mtx((double)x1,
                              (double)y1,
                              (double)x2,
                              (double)y2,
                              parl);
        mtx *= m_transform;
        mtx.Invert();

        m_rasterizer.Reset();
        m_rasterizer.AddPath(m_pathTransform);

        typedef OHOS::SpanInterpolatorLinear<OHOS::TransAffine> Interpolator;
        Interpolator interpolator(mtx);

        if (m_blendMode == BLENDALPHA) {
            BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBase, interpolator);
        } else {
            BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBaseComp, interpolator);
        }
    }

    struct BaseGfxExtendEngineRasterizerGamma {
        BaseGfxExtendEngineRasterizerGamma(double alpha, double gamma) :
            m_alpha(alpha), m_gamma(gamma)
        {}

        double operator()(double x) const
        {
            return m_alpha(m_gamma(x));
        }
        OHOS::GammaMultiply m_alpha;
        OHOS::GammaPower m_gamma;
    };

    void BaseGfxExtendEngine::UpdateRasterizerGamma()
    {
        m_rasterizer.GammaFunction(BaseGfxExtendEngineRasterizerGamma(m_masterAlpha, m_antiAliasGamma));
    }

    void BaseGfxExtendEngine::BlendImage(Image& img,
                                         int imgX1, int imgY1, int imgX2, int imgY2,
                                         double dstX, double dstY, unsigned alpha)
    {
        WorldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        Rect r(imgX1, imgY1, imgX2, imgY2);
        if (m_blendMode == BLENDALPHA) {
            m_renBasePre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
        } else {
            m_renBaseCompPre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
        }
    }

    void BaseGfxExtendEngine::PatternImageFill(Image& img, double offsetX, double offsetY, const char* pattternMode)
    {
        WorldToScreen(offsetX, offsetY);
        m_rasterizer.AddPath(m_pathTransform);
        pixfmt img_pixf(img.renBuf); // 获取图片
        if (strcmp(pattternMode, "repeat") == 0) {
            imgSourceTypeRepeat img_src(img_pixf);
            spanPatternTypeRepeat m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-x") == 0) {
            imgSourceTypeRepeatX img_src(img_pixf);
            spanPatternTypeRepeatX m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-y") == 0) {
            imgSourceTypeRepeatY img_src(img_pixf);
            spanPatternTypeRepeatY m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "no-repeat") == 0) {
            imgSourceTypeNoRepeat img_src(img_pixf);
            spanPatternTypeNoRepeat m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        }
    }

    void BaseGfxExtendEngine::PatternImageStroke(Image& img, double offsetX, double offsetY, const char* pattternMode)
    {
        WorldToScreen(offsetX, offsetY);
        pixfmt img_pixf(img.renBuf); // 获取图片
        m_rasterizer.AddPath(m_strokeTransform);
        if (strcmp(pattternMode, "repeat") == 0) {
            imgSourceTypeRepeat img_src(img_pixf);
            spanPatternTypeRepeat m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-x") == 0) {
            imgSourceTypeRepeatX img_src(img_pixf);
            spanPatternTypeRepeatX m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-y") == 0) {
            imgSourceTypeRepeatY img_src(img_pixf);
            spanPatternTypeRepeatY m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "no-repeat") == 0) {
            imgSourceTypeNoRepeat img_src(img_pixf);
            spanPatternTypeNoRepeat m_spanPatternType(img_src, 0 - offsetX, 0 - offsetY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        }
    }

    void BaseGfxExtendEngine::BlendFromImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             double dstX, double dstY, unsigned alpha)
    {
        WorldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        Rect r(imgX1, imgY1, imgX2, imgY2);
        m_renBase.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
    }

    void BaseGfxExtendEngine::BlendFromImage(Image& img, double dstX, double dstY, unsigned alpha)
    {
        WorldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        m_renBase.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
    }

    bool BaseGfxExtendEngine::BoundingRectSingle(unsigned int path_id, RectD* rect, PathTransform& path)
    {
        return OHOS::BoundingRectSingle(path, path_id, &rect->x1, &rect->y1, &rect->x2, &rect->y2);
    }
} // namespace OHOS
