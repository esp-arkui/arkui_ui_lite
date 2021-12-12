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

        m_blendMode(BlendAlpha),
        m_imageBlendMode(BlendDst),
        m_imageBlendColor(0, 0, 0),

        m_scanline(),
        m_rasterizer(),

        m_masterAlpha(1.0),
        m_antiAliasGamma(1.0),

        m_fillColor(255, 255, 255),
        m_lineColor(0, 0, 0),
        m_fillGradient(),
        m_lineGradient(),

        m_lineCap(CapRound),
        m_lineJoin(JoinRound),
        m_miterLimit(10),
        m_fillGradientFlag(Solid),
        m_lineGradientFlag(Solid),
        m_fillGradientMatrix(),
        m_lineGradientMatrix(),
        m_fillRadialMatrix(),
        m_fillGradientD1(0.0),
        m_lineGradientD1(0.0),
        m_fillGradientD2(100.0),
        m_lineGradientD2(100.0),

        m_fillGradientInterpolator(m_fillGradientMatrix),
        m_lineGradientInterpolator(m_lineGradientMatrix),
        m_interpolator_type(m_fillRadialMatrix),

        m_linearGradientFunction(),
        m_radialGradientFunction(),

        m_lineWidth(1),
        m_evenOddFlag(false),

        m_path(),
        m_transform(),

        m_convCurve(m_path),
        m_convDashCurve(m_convCurve),
        m_convStroke(m_convCurve),
        m_convDashStroke(m_convDashCurve),
        m_pathTransform(m_convCurve, m_transform),
        m_strokeTransform(m_convStroke, m_transform),
        m_dashStrokeTransform(m_convDashStroke, m_transform),
        is_dash(false),
        dashes(nullptr),
        ndashes(0),
        dDashOffset(0),
        shadowColor_(Color(255, 255, 255)),
        shadowOffsetX_(0),
        shadowOffsetY_(0),
        shadowBlurRadius_(0)
    {
        lineCap(m_lineCap);
        lineJoin(m_lineJoin);
    }

    BaseGfxExtendEngine::BaseGfxExtendEngine(const BaseGfxExtendEngine& o) :
        m_scanline(),
        m_rasterizer(),
        m_fillGradientMatrix(o.m_fillGradientMatrix),
        m_lineGradientMatrix(o.m_lineGradientMatrix),
        m_fillGradientInterpolator(m_fillGradientMatrix),
        m_lineGradientInterpolator(m_lineGradientMatrix),
        m_path(o.m_path),
        m_convCurve(m_path),
        m_convDashCurve(m_convCurve),
        m_convStroke(m_convCurve),
        m_convDashStroke(m_convDashCurve),
        m_pathTransform(m_convCurve, m_transform),
        m_strokeTransform(m_convStroke, m_transform),
        m_dashStrokeTransform(m_convDashStroke, m_transform)
    {
        m_rbuf = o.m_rbuf;
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

        m_allocator = o.m_allocator;
        m_clipBox = o.m_clipBox;

        m_blendMode = o.m_blendMode;
        m_imageBlendMode = o.BlendDst;
        m_imageBlendColor = o.m_imageBlendColor;

        m_masterAlpha = o.m_masterAlpha;
        m_antiAliasGamma = o.m_antiAliasGamma;

        m_fillColor = o.m_fillColor;
        m_lineColor = o.m_lineColor;
        m_fillGradient = o.m_fillGradient;
        m_lineGradient = o.m_lineGradient;

        m_lineCap = o.m_lineCap;
        m_lineJoin = o.m_lineJoin;
        m_miterLimit = o.m_miterLimit;
        m_fillGradientFlag = o.Solid;
        m_lineGradientFlag = o.Solid;
        m_fillGradientD1 = o.m_fillGradientD1;
        m_lineGradientD1 = o.m_lineGradientD1;
        m_fillGradientD2 = o.m_fillGradientD2;
        m_lineGradientD2 = o.m_lineGradientD2;

        m_linearGradientFunction = o.m_linearGradientFunction;
        m_radialGradientFunction = o.m_radialGradientFunction;

        m_lineWidth = o.m_lineWidth;
        m_evenOddFlag = o.m_evenOddFlag;
        m_transform = o.m_transform;
        shadowBlurRadius_ = o.shadowBlurRadius_;
        shadowColor_ = o.shadowColor_;
        shadowOffsetX_ = o.shadowOffsetX_;
        shadowOffsetY_ = o.shadowOffsetY_;
        is_dash = o.is_dash;
        // set dashes
        if (is_dash) {
            ndashes = o.ndashes;
            dDashOffset = o.dDashOffset;
            dashes = new float[ndashes];
            if (dashes) {
                for (unsigned int i = 0; i < ndashes; i++) {
                    dashes[i] = o.dashes[i];
                }
            } else {
                //memory alloc error, ignore this dash
                //I think it is never happen.
                ndashes = 0;
                dDashOffset = 0;
                is_dash = false;
            }
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::attach(unsigned char* buf, unsigned width, unsigned height, int stride)
    {
        m_rbuf.attach(buf, width, height, stride);

        m_renBase.ResetClipping(true);
        m_renBaseComp.ResetClipping(true);
        m_renBasePre.ResetClipping(true);
        m_renBaseCompPre.ResetClipping(true);

        resetTransformations();
        lineWidth(1.0),
            lineColor(0, 0, 0);
        fillColor(255, 255, 255);
        clipBox(0, 0, width, height);
        lineCap(CapRound);
        lineJoin(JoinRound);
        m_masterAlpha = 1.0;
        m_antiAliasGamma = 1.0;
        m_rasterizer.GammaFunction(OHOS::GammaNone());
        m_blendMode = BlendAlpha;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::attach(Image& img)
    {
        attach(img.renBuf.GetBuf(), img.renBuf.GetWidth(), img.renBuf.GetHeight(), img.renBuf.GetStride());
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::clipBox(double x1, double y1, double x2, double y2)
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

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::blendMode(BlendMode m)
    {
        m_blendMode = m;
        m_pixFormatComp.CompOp(m);
        m_pixFormatCompPre.CompOp(m);
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::BlendMode BaseGfxExtendEngine::blendMode() const
    {
        return m_blendMode;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::imageBlendMode(BlendMode m)
    {
        m_imageBlendMode = m;
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::BlendMode BaseGfxExtendEngine::imageBlendMode() const
    {
        return m_imageBlendMode;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::imageBlendColor(Color c)
    {
        m_imageBlendColor = c;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::imageBlendColor(unsigned r, unsigned g, unsigned b, unsigned a)
    {
        imageBlendColor(Color(r, g, b, a));
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::Color BaseGfxExtendEngine::imageBlendColor() const
    {
        return m_imageBlendColor;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::masterAlpha(double a)
    {
        if (m_masterAlpha != a) {
            m_masterAlpha = a;
            updateRasterizerGamma();
        }
    }

    //------------------------------------------------------------------------
    double BaseGfxExtendEngine::masterAlpha() const
    {
        return m_masterAlpha;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::antiAliasGamma(double g)
    {
        m_antiAliasGamma = g;
        updateRasterizerGamma();
    }

    //------------------------------------------------------------------------
    double BaseGfxExtendEngine::antiAliasGamma() const
    {
        return m_antiAliasGamma;
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::RectD BaseGfxExtendEngine::clipBox() const
    {
        return m_clipBox;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::clearAll(Color c)
    {
        m_renBase.Clear(c);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::clearAll(unsigned r, unsigned g, unsigned b, unsigned a)
    {
        clearAll(Color(r, g, b, a));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::worldToScreen(double& x, double& y) const
    {
        m_transform.Transform(&x, &y);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::screenToWorld(double& x, double& y) const
    {
        m_transform.InverseTransform(&x, &y);
    }

    //------------------------------------------------------------------------
    double BaseGfxExtendEngine::worldToScreen(double scalar) const
    {
        double x1 = 0;
        double y1 = 0;
        double x2 = scalar;
        double y2 = scalar;
        worldToScreen(x1, y1);
        worldToScreen(x2, y2);
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * 0.7071068;
    }

    //------------------------------------------------------------------------
    double BaseGfxExtendEngine::screenToWorld(double scalar) const
    {
        double x1 = 0;
        double y1 = 0;
        double x2 = scalar;
        double y2 = scalar;
        screenToWorld(x1, y1);
        screenToWorld(x2, y2);
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * 0.7071068;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::alignPoint(double& x, double& y) const
    {
        worldToScreen(x, y);
        x = floor(x) + 0.5;
        y = floor(y) + 0.5;
        screenToWorld(x, y);
    }

    //------------------------------------------------------------------------
    bool BaseGfxExtendEngine::inBox(double worldX, double worldY) const
    {
        worldToScreen(worldX, worldY);
        return m_renBase.Inbox(int(worldX), int(worldY));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::resetTransformations()
    {
        m_transform.Reset();
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::rotate(double angle)
    {
        m_transform *= OHOS::TransAffineRotation(angle);
    }

    void BaseGfxExtendEngine::translate(double x, double y)
    {
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::affine(const Affine& tr)
    {
        m_transform *= tr;
        m_convCurve.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        } else {
            m_convDashStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::affine(const Transformations& tr)
    {
        affine(OHOS::TransAffine(tr.affineMatrix[0], tr.affineMatrix[1], tr.affineMatrix[2],
                                 tr.affineMatrix[3], tr.affineMatrix[4], tr.affineMatrix[5]));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::scale(double sx, double sy)
    {
        m_transform *= OHOS::TransAffineScaling(sx, sy);
        m_convCurve.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        } else {
            m_convDashStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::viewport(double worldX1, double worldY1, double worldX2, double worldY2,
                                       double screenX1, double screenY1, double screenX2, double screenY2,
                                       ViewportOption opt)
    {
        OHOS::TransViewPort vp;
        switch (opt) {
            case Anisotropic: vp.PreserveAspectRatio(0.0, 0.0, OHOS::ASPECT_RATIO_STRETCH); break;
            case XMinYMin: vp.PreserveAspectRatio(0.0, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMidYMin: vp.PreserveAspectRatio(0.5, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMaxYMin: vp.PreserveAspectRatio(1.0, 0.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMinYMid: vp.PreserveAspectRatio(0.0, 0.5, OHOS::ASPECT_RATIO_MEET); break;
            case XMidYMid: vp.PreserveAspectRatio(0.5, 0.5, OHOS::ASPECT_RATIO_MEET); break;
            case XMaxYMid: vp.PreserveAspectRatio(1.0, 0.5, OHOS::ASPECT_RATIO_MEET); break;
            case XMinYMax: vp.PreserveAspectRatio(0.0, 1.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMidYMax: vp.PreserveAspectRatio(0.5, 1.0, OHOS::ASPECT_RATIO_MEET); break;
            case XMaxYMax: vp.PreserveAspectRatio(1.0, 1.0, OHOS::ASPECT_RATIO_MEET); break;
        }
        vp.WorldViewPort(worldX1, worldY1, worldX2, worldY2);
        vp.DeviceViewPort(screenX1, screenY1, screenX2, screenY2);
        m_transform *= vp.ToAffine();
        m_convCurve.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        if (!this->is_dash) {
            m_convStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        } else {
            m_convDashStroke.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::fillColor(Color c)
    {
        m_fillColor = c;
        m_fillGradientFlag = Solid;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::fillColor(unsigned r, unsigned g, unsigned b, unsigned a)
    {
        fillColor(Color(r, g, b, a));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::noFill()
    {
        fillColor(Color(0, 0, 0, 0));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineColor(Color c)
    {
        m_lineColor = c;
        m_lineGradientFlag = Solid;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineColor(unsigned r, unsigned g, unsigned b, unsigned a)
    {
        lineColor(Color(r, g, b, a));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::noLine()
    {
        lineColor(Color(0, 0, 0, 0));
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::Color BaseGfxExtendEngine::fillColor() const
    {
        return m_fillColor;
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::Color BaseGfxExtendEngine::lineColor() const
    {
        return m_lineColor;
    }
    void BaseGfxExtendEngine::fillColor(const OHOS::ColorType& c)
    {
        fillColor(Color(c.red, c.green, c.blue, c.alpha));
    }
    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineColor(const OHOS::ColorType& c)
    {
        lineColor(Color(c.red, c.green, c.blue, c.alpha));
    }

    void BaseGfxExtendEngine::remove_all_color()
    {
        m_fillRadialGradient.RemoveAll();
    }

    void BaseGfxExtendEngine::add_color(double offset, Color c1)
    {
        m_fillRadialGradient.AddColor(offset, c1);
    }

    void BaseGfxExtendEngine::build_lut()
    {
        m_fillRadialGradient.BuildLut();
    }

    void BaseGfxExtendEngine::fillRadialGradient(double start_x, double start_y, double start_r, double end_x, double end_y, double end_r)
    {
        m_fillRadialMatrix.Reset();
        m_fillRadialMatrix *= OHOS::TransAffineTranslation(end_x, end_y);
        m_fillRadialMatrix *= m_transform;
        m_fillRadialMatrix.Invert();
        m_interpolator_type.SetTransformer(m_fillRadialMatrix);
        m_fillGradientD1 = start_r;
        m_fillGradientD2 = end_r;
        m_radialGradientFunction = OHOS::GradientRadialCalculate(end_r, start_x - end_x, start_y - end_y);
        m_fillGradientFlag = Radial;
        m_fillColor = Color(0, 0, 0, 255);
    }

    void BaseGfxExtendEngine::fillLinearGradient(double start_x, double start_y, double end_x, double end_y)
    {
        double angle = atan2(end_y - start_y, end_x - start_x);
        m_fillGradientMatrix.Reset();
        m_fillGradientMatrix *= OHOS::TransAffineRotation(angle);
        m_fillGradientMatrix *= OHOS::TransAffineTranslation(start_x, start_y);
        m_fillGradientMatrix *= m_transform;
        m_fillGradientMatrix.Invert();
        m_fillGradientD1 = 0.0;
        m_fillGradientD2 = sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
        m_fillGradientFlag = Linear;
        m_fillColor = Color(0, 0, 0); // Set some real color
    }
    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineWidth(double w)
    {
        if (w < 0.0) {
            w = abs(w);
        }
        m_lineWidth = w;
        if (!this->is_dash) {
            m_convStroke.Width(w);
        } else {
            m_convDashStroke.Width(w);
        }
    }

    //------------------------------------------------------------------------
    double BaseGfxExtendEngine::lineWidth() const
    {
        return m_lineWidth;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::fillEvenOdd(bool evenOddFlag)
    {
        m_evenOddFlag = evenOddFlag;
        m_rasterizer.FillingRule(evenOddFlag ? OHOS::FILL_EVEN_ODD : OHOS::FILL_NON_ZERO);
    }

    //------------------------------------------------------------------------
    bool BaseGfxExtendEngine::fillEvenOdd() const
    {
        return m_evenOddFlag;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineCap(LineCap cap)
    {
        m_lineCap = cap;
        if (!this->is_dash) {
            m_convStroke.LineCap((OHOS::LineCapEnum)cap);
        } else {
            m_convDashStroke.LineCap((OHOS::LineCapEnum)cap);
        }
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::LineCap BaseGfxExtendEngine::lineCap() const
    {
        return m_lineCap;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineJoin(LineJoin join)
    {
        m_lineJoin = join;
        if (!this->is_dash) {
            m_convStroke.LineJoin((OHOS::LineJoinEnum)join);
        } else {
            m_convDashStroke.LineJoin((OHOS::LineJoinEnum)join);
        }
    }

    //------------------------------------------------------------------------
    BaseGfxExtendEngine::LineJoin BaseGfxExtendEngine::lineJoin() const
    {
        return m_lineJoin;
    }

    void BaseGfxExtendEngine::MiterLimit(double limitValue)
    {
        m_miterLimit = limitValue;
        if (!this->is_dash) {
            m_convStroke.MiterLimit(limitValue);
        } else {
            m_convDashStroke.MiterLimit(limitValue);
        }
    }

    double BaseGfxExtendEngine::MiterLimit() const
    {
        return m_miterLimit;
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::addLine(double x1, double y1, double x2, double y2)
    {
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y2);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::line(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        addLine(x1, y1, x2, y2);
        drawPath(StrokeOnly);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x3, y3);
        m_path.ClosePolygon();
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::rectangle(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x1, y2);
        m_path.ClosePolygon();
        drawPath(FillAndStroke);
    }
    void BaseGfxExtendEngine::rectstroke(double x1, double y1, double x2, double y2)
    {
        m_path.RemoveAll();
        m_path.MoveTo(x1, y1);
        m_path.LineTo(x2, y1);
        m_path.LineTo(x2, y2);
        m_path.LineTo(x1, y2);
        m_path.ClosePolygon();
        stroke();
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2, double r)
    {
        m_path.RemoveAll();
        OHOS::RoundedRect rc(x1, y1, x2, y2, r);
        rc.NormalizeRadius();
        rc.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        // JME audit
        //m_path.add_path(rc, 0, false);
        m_path.ConcatPath(rc, 0);
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2, double rx, double ry)
    {
        m_path.RemoveAll();
        OHOS::RoundedRect rc;
        rc.Rect(x1, y1, x2, y2);
        rc.Radius(rx, ry);
        rc.NormalizeRadius();
        //m_path.add_path(rc, 0, false);
        m_path.ConcatPath(rc, 0); // JME
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2,
                                          double rx_bottom, double ry_bottom,
                                          double rx_top, double ry_top)
    {
        m_path.RemoveAll();
        OHOS::RoundedRect rc;
        rc.Rect(x1, y1, x2, y2);
        rc.Radius(rx_bottom, ry_bottom, rx_top, ry_top);
        rc.NormalizeRadius();
        rc.ApproximationScale(worldToScreen(1.0) * g_approxScale);
        //m_path.add_path(rc, 0, false);
        m_path.ConcatPath(rc, 0); // JME
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::ellipse(double cx, double cy, double rx, double ry)
    {
        m_path.RemoveAll();
        OHOS::BezierArc arc(cx, cy, rx, ry, 0, 2 * pi());
        m_path.ConcatPath(arc, 0); // JME
        m_path.ClosePolygon();
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::arc(double cx, double cy, double rx, double ry, double start, double sweep)
    {
        m_path.RemoveAll();
        OHOS::BezierArc arc(cx, cy, rx, ry, start, sweep);
        //m_path.add_path(arc, 0, false);
        m_path.ConcatPath(arc, 0); // JME
        drawPath(StrokeOnly);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::star(double cx, double cy, double r1, double r2, double startAngle, int numRays)
    {
        m_path.RemoveAll();
        double da = OHOS::PI / double(numRays);
        double a = startAngle;
        int i;
        for (i = 0; i < numRays; i++) {
            double x = cos(a) * r2 + cx;
            double y = sin(a) * r2 + cy;
            if (i)
                m_path.LineTo(x, y);
            else
                m_path.MoveTo(x, y);
            a += da;
            m_path.LineTo(cos(a) * r1 + cx, sin(a) * r1 + cy);
            a += da;
        }
        closePolygon();
        drawPath(FillAndStroke);
    }
    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::polygon(double* xy, int numPoints)
    {
        m_path.RemoveAll();
        //m_path.add_poly(xy, numPoints);
        m_path.ConcatPoly(xy, 0, true); // JME
        closePolygon();
        drawPath(FillAndStroke);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::polyline(double* xy, int numPoints)
    {
        m_path.RemoveAll();
        //m_path.add_poly(xy, numPoints);
        m_path.ConcatPoly(xy, 0, true); // JME
        drawPath(StrokeOnly);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::resetPath()
    {
        m_path.RemoveAll();
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::moveTo(double x, double y)
    {
        m_path.MoveTo(x, y);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::lineTo(double x, double y)
    {
        m_path.LineTo(x, y);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::arcTo(double rx, double ry,
                                    double angle,
                                    bool largeArcFlag,
                                    bool sweepFlag,
                                    double x, double y)
    {
        m_path.ArcTo(rx, ry, angle, largeArcFlag, sweepFlag, x, y);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::closePolygon()
    {
        m_path.ClosePolygon();
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias)
    {
        resetPath();
        moveTo(dstX1, dstY1);
        lineTo(dstX2, dstY1);
        lineTo(dstX2, dstY2);
        lineTo(dstX1, dstY2);
        closePolygon();
        double parallelogram[6] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};
        renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram, isAntiAlias);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImage(const Image& img, double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias)
    {
        resetPath();
        moveTo(dstX1, dstY1);
        lineTo(dstX2, dstY1);
        lineTo(dstX2, dstY2);
        lineTo(dstX1, dstY2);
        closePolygon();
        double parallelogram[6] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};

        renderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram, isAntiAlias);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             const double* parallelogram, bool isAntiAlias)
    {
        resetPath();
        moveTo(parallelogram[0], parallelogram[1]);
        lineTo(parallelogram[2], parallelogram[3]);
        lineTo(parallelogram[4], parallelogram[5]);
        lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
               parallelogram[1] + parallelogram[5] - parallelogram[3]);
        closePolygon();
        renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram, isAntiAlias);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImage(const Image& img, const double* parallelogram, bool isAntiAlias)
    {
        resetPath();
        moveTo(parallelogram[0], parallelogram[1]);
        lineTo(parallelogram[2], parallelogram[3]);
        lineTo(parallelogram[4], parallelogram[5]);
        lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
               parallelogram[1] + parallelogram[5] - parallelogram[3]);
        closePolygon();

        renderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram, isAntiAlias);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImagePath(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                                 double dstX1, double dstY1, double dstX2, double dstY2)
    {
        double parallelogram[6] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};
        renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImagePath(const Image& img, double dstX1, double dstY1, double dstX2, double dstY2)
    {
        double parallelogram[6] = {dstX1, dstY1, dstX2, dstY1, dstX2, dstY2};
        renderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImagePath(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                                 const double* parallelogram)
    {
        renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::transformImagePath(const Image& img, const double* parallelogram)
    {
        renderImage(img, 0, 0, img.renBuf.GetWidth(), img.renBuf.GetHeight(), parallelogram);
    }

    void BaseGfxExtendEngine::drawShadow(double x = 0, double y = 0, double a = 0, double scaleX = 0, double scaleY = 0)
    {
        m_rasterizer.Reset();
        OHOS::TransAffine transform(m_transform.scaleX, m_transform.shearY, m_transform.shearX, m_transform.scaleY, m_transform.translateX, m_transform.translateY);
        PathTransform shadow_trans(m_convCurve, transform);
        transform.Translate(shadowOffsetX_, shadowOffsetY_);
        if (a != 0) {
            transform *= OHOS::TransAffineTranslation(-x, -y);
            transform *= OHOS::TransAffineRotation(a * 3.1415926 / 180.0);
            transform *= OHOS::TransAffineTranslation(x, y);
        }
        if (scaleX != 0 || scaleY != 0) {
            transform *= OHOS::TransAffineTranslation(-x, -y);
            transform *= OHOS::TransAffineScaling(scaleX, scaleY);
            transform *= OHOS::TransAffineTranslation(x, y);
        }
        m_rasterizer.AddPath(shadow_trans);
        OHOS::RenderScanlinesAntiAliasSolid(m_rasterizer, m_scanline, m_renBase, shadowColor_);
        if (shadowBlurRadius_ != 0) {
            RectD bbox;
            bounding_rect_single(0, &bbox, shadow_trans);
            bbox.x1 -= shadowBlurRadius_;
            bbox.y1 -= shadowBlurRadius_;
            bbox.x2 += shadowBlurRadius_;
            bbox.y2 += shadowBlurRadius_;
            RenderingBuffer m_rbuf_window;
            PixFormat pixf2(m_rbuf_window);
            pixf2.Attach(m_pixFormat, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2));
            m_stack_blur.Blur(pixf2, OHOS::Uround(shadowBlurRadius_));
        }
        m_rasterizer.Reset();
    }
    void BaseGfxExtendEngine::drawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry,
                                         double x = 0, double y = 0, double a = 0, double scaleX = 0, double scaleY = 0)
    {
        m_path.RemoveAll();
        OHOS::BezierArc arc(cx, cy, rx, ry, 0, 2 * pi());
        m_path.ConcatPath(arc, 0); // JME
        m_path.ClosePolygon();
        drawShadow(x, y, a, scaleX, scaleY);
    }
    void BaseGfxExtendEngine::rotate(double x, double y, double a)
    {
        m_transform *= OHOS::TransAffineTranslation(-x, -y);
        m_transform *= OHOS::TransAffineRotation(a * 3.1415926 / 180.0);
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }
    void BaseGfxExtendEngine::scale(double x, double y, double scaleX, double scaleY)
    {
        m_transform *= OHOS::TransAffineTranslation(-x, -y);
        m_transform *= OHOS::TransAffineScaling(scaleX, scaleY);
        m_transform *= OHOS::TransAffineTranslation(x, y);
    }
    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::drawPath(DrawPathFlag flag)
    {
        m_rasterizer.Reset();
        switch (flag) {
            case FillOnly:
                if (m_fillColor.alphaValue) {
                    // if (m_shadow_ctrl.GetOffsetX()!=0||m_shadow_ctrl.GetOffsety()!=0) {
                    //     // drawShadow();
                    // }
                    m_rasterizer.AddPath(m_pathTransform);
                    render(true);
                }
                break;

            case StrokeOnly:
                if (m_lineColor.alphaValue && m_lineWidth > 0.0) {
                    if (!this->is_dash) {
                        m_rasterizer.AddPath(m_strokeTransform);
                    } else {
                        for (unsigned int i = 0; i < this->ndashes; i += 2) {
                            m_convDashCurve.AddDash(dashes[i], dashes[i + 1]);
                        }
                        m_convDashCurve.DashStart(dDashOffset);
                        m_rasterizer.AddPath(m_dashStrokeTransform);
                    }
                    render(false);
                }
                break;

            case FillAndStroke:
                if (m_fillColor.alphaValue) {
                    // if (m_shadow_ctrl.GetOffsetX()!=0||m_shadow_ctrl.GetOffsety()!=0) {
                    //     // drawShadow();
                    // }
                    m_rasterizer.AddPath(m_pathTransform);
                    render(true);
                }

                if (m_lineColor.alphaValue && m_lineWidth > 0.0) {
                    if (!this->is_dash) {
                        m_rasterizer.AddPath(m_strokeTransform);
                    } else {
                        for (unsigned int i = 0; i < this->ndashes; i += 2) {
                            m_convDashCurve.AddDash(dashes[i], dashes[i + 1]);
                        }
                        m_convDashCurve.DashStart(dDashOffset);
                        m_rasterizer.AddPath(m_dashStrokeTransform);
                    }
                    render(false);
                }
                break;

            case FillWithLineColor:
                if (m_lineColor.alphaValue) {
                    m_rasterizer.AddPath(m_pathTransform);
                    render(false);
                }
                break;
        }
    }

    void BaseGfxExtendEngine::stroke()
    {
        m_rasterizer.Reset();
        m_rasterizer.AddPath(m_strokeTransform);
        render(false);
    }

    //------------------------------------------------------------------------
    class BaseGfxExtendEngineRenderer {
    public:
        //--------------------------------------------------------------------
        template <class BaseRenderer, class SolidRenderer>
        void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase, SolidRenderer& renSolid, bool fillColor)
        {
            // JME
            typedef OHOS::SpanAllocator<BaseGfxExtendEngine::ColorType> span_allocator_type;
            //- typedef OHOS::RenderScanlinesAntiAlias<BaseRenderer, BaseGfxExtendEngine::LinearGradientSpan> RendererLinearGradient;
            typedef OHOS::RendererScanlineAntiAlias<BaseRenderer,
                                                    span_allocator_type,
                                                    BaseGfxExtendEngine::LinearGradientSpan>
                RendererLinearGradient;
            //- typedef OHOS::RenderScanlinesAntiAlias<BaseRenderer, BaseGfxExtendEngine::RadialGradientSpan> RendererRadialGradient;
            typedef OHOS::RendererScanlineAntiAlias<BaseRenderer,
                                                    span_allocator_type,
                                                    BaseGfxExtendEngine::RadialGradientSpan>
                RendererRadialGradient;

            if (gr.m_fillGradientFlag == BaseGfxExtendEngine::Linear) {
                if (fillColor) {
                    BaseGfxExtendEngine::LinearGradientSpan span(/*gr.m_allocator, */
                                                                 gr.m_fillGradientInterpolator,
                                                                 gr.m_linearGradientFunction,
                                                                 gr.m_fillRadialGradient,
                                                                 gr.m_fillGradientD1,
                                                                 gr.m_fillGradientD2);
                    //-RendererLinearGradient ren(renBase,span);
                    RendererLinearGradient ren(renBase, gr.m_allocator, span);
                    OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, ren);
                } else {
                    BaseGfxExtendEngine::LinearGradientSpan span(/*gr.m_allocator,*/
                                                                 gr.m_fillGradientInterpolator,
                                                                 gr.m_linearGradientFunction,
                                                                 gr.m_fillRadialGradient,
                                                                 gr.m_fillGradientD1,
                                                                 gr.m_fillGradientD2);

                    OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, span);
                }
            } else if (gr.m_fillGradientFlag == BaseGfxExtendEngine::Radial) {
                if (fillColor) {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                        gr.m_interpolator_type,
                        gr.m_radialGradientFunction,
                        gr.m_fillRadialGradient,
                        gr.m_fillGradientD1,
                        gr.m_fillGradientD2);
                    //-RendererRadialGradient ren(renBase, span);
                    RendererRadialGradient ren(renBase, gr.m_allocator, span);
                    OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, ren);
                } else {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                        gr.m_interpolator_type,
                        gr.m_radialGradientFunction,
                        gr.m_fillRadialGradient,
                        gr.m_fillGradientD1,
                        gr.m_fillGradientD2);
                    //-RendererRadialGradient ren(renBase, span);
                    //                    RendererRadialGradient ren(renBase,gr.m_allocator,span);
                    //                    OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, ren);
                    //                    OHOS::scanline_p8 m_sl;
                    OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, span);
                }
            } else {
                renSolid.SetColor(fillColor ? gr.m_fillColor : gr.m_lineColor);
                OHOS::RenderScanlines(gr.m_rasterizer, gr.m_scanline, renSolid);
            }
        }

        //--------------------------------------------------------------------
        class SpanConvImageBlend {
        public:
            SpanConvImageBlend(BaseGfxExtendEngine::BlendMode m, BaseGfxExtendEngine::Color c) :
                m_mode(m), m_color(c)
            {}

            void convert(BaseGfxExtendEngine::Color* span, int x, int y, unsigned len) const
            {
                unsigned l2;
                BaseGfxExtendEngine::Color* s2;
                if (m_mode != BaseGfxExtendEngine::BlendDst) {
                    l2 = len;
                    s2 = span;
                    typedef OHOS::CompOpAdaptorClipToDstRgbaPre<BaseGfxExtendEngine::Color, OHOS::OrderRgba> OpType;
                    do {
                        OpType::BlendPix(m_mode,
                                         (BaseGfxExtendEngine::Color::ValueType*)s2,
                                         m_color.redValue,
                                         m_color.greenValue,
                                         m_color.blueValue,
                                         BaseGfxExtendEngine::Color::FullValue(),
                                         OHOS::cover_full);
                        ++s2;
                    } while (--l2);
                }
                if (!m_color.IsOpaque()) {
                    l2 = len;
                    s2 = span;
                    do {
                        s2->redValue = BaseGfxExtendEngine::Color::Multiply(s2->redValue, m_color.alphaValue);
                        s2->greenValue = BaseGfxExtendEngine::Color::Multiply(s2->greenValue, m_color.alphaValue);
                        s2->blueValue = BaseGfxExtendEngine::Color::Multiply(s2->blueValue, m_color.alphaValue);
                        s2->alphaValue = BaseGfxExtendEngine::Color::Multiply(s2->alphaValue, m_color.alphaValue);
                        ++s2;
                    } while (--l2);
                }
            }

        private:
            BaseGfxExtendEngine::BlendMode m_mode;
            BaseGfxExtendEngine::Color m_color;
        };

        //--------------------------------------------------------------------
        template <class BaseRenderer, class SolidRenderer, class Rasterizer, class Scanline>
        void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase, SolidRenderer& renSolid, Rasterizer& ras, Scanline& sl)
        {
            // JME
            typedef OHOS::SpanAllocator<BaseGfxExtendEngine::ColorType> span_allocator_type;
            typedef OHOS::RendererScanlineAntiAlias<BaseRenderer, span_allocator_type, BaseGfxExtendEngine::LinearGradientSpan> RendererLinearGradient;
            typedef OHOS::RendererScanlineAntiAlias<BaseRenderer, span_allocator_type, BaseGfxExtendEngine::RadialGradientSpan> RendererRadialGradient;

            if (gr.m_fillGradientFlag == BaseGfxExtendEngine::Linear) {
                BaseGfxExtendEngine::LinearGradientSpan span(
                    gr.m_fillGradientInterpolator,
                    gr.m_linearGradientFunction,
                    gr.m_fillRadialGradient,
                    gr.m_fillGradientD1,
                    gr.m_fillGradientD2);
                RendererLinearGradient ren(renBase, gr.m_allocator, span);
                OHOS::RenderScanlines(ras, sl, ren);
            } else {
                if (gr.m_fillGradientFlag == BaseGfxExtendEngine::Radial) {
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

        //--------------------------------------------------------------------
        //! JME - this is where the bulk of the changes have taken place.
        template <class BaseRenderer, class Interpolator>
        static void renderImage(BaseGfxExtendEngine& gr, const BaseGfxExtendEngine::Image& img,
                                BaseRenderer& renBase, Interpolator& interpolator)
        {
            //! JME - have not quite figured which part of this is not const-correct
            // hence the cast.
            BaseGfxExtendEngine::Image& imgc = const_cast<BaseGfxExtendEngine::Image&>(img);
            BaseGfxExtendEngine::PixFormat img_pixf(imgc.renBuf);
            typedef OHOS::ImageAccessorClone<BaseGfxExtendEngine::PixFormat> img_source_type;
            img_source_type source(img_pixf);
            typedef OHOS::SpanImageRgba<img_source_type, Interpolator> SpanGenType;
            SpanGenType sg(source, interpolator);
            OHOS::RenderScanlinesAntiAlias(gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, sg);
        }
    };

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::render(bool fillColor)
    {
        if (m_blendMode == BlendAlpha) {
            BaseGfxExtendEngineRenderer::render(*this, m_renBase, m_renSolid, fillColor);
        } else {
            BaseGfxExtendEngineRenderer::render(*this, m_renBaseComp, m_renSolidComp, fillColor);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::renderImage(const Image& img, int x1, int y1, int x2, int y2,
                                          const double* parl, bool isAntiAlias)
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

        if (m_blendMode == BlendAlpha) {
            if (isAntiAlias) {
                BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBasePre, interpolator);
            } else {
                BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBase, interpolator);
            }
        } else {
            if (isAntiAlias) {
                BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBaseCompPre, interpolator);
            } else {
                BaseGfxExtendEngineRenderer::renderImage(*this, img, m_renBaseComp, interpolator);
            }
        }
    }

    //------------------------------------------------------------------------
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

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::updateRasterizerGamma()
    {
        m_rasterizer.GammaFunction(BaseGfxExtendEngineRasterizerGamma(m_masterAlpha, m_antiAliasGamma));
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::blendImage(Image& img,
                                         int imgX1, int imgY1, int imgX2, int imgY2,
                                         double dstX, double dstY, unsigned alpha)
    {
        worldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        // JME
        //OHOS::rect r(imgX1, imgY1, imgX2, imgY2);
        Rect r(imgX1, imgY1, imgX2, imgY2);
        if (m_blendMode == BlendAlpha) {
            m_renBasePre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
        } else {
            m_renBaseCompPre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::blendImage(Image& img, double dstX, double dstY, unsigned alpha)
    {
        worldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        //m_renBase.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
        m_renBasePre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
        if (m_blendMode == BlendAlpha) {
            m_renBasePre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
        } else {
            m_renBaseCompPre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
        }
    }

    void BaseGfxExtendEngine::patternImageFill(Image& img, double dstX, double dstY, const char* pattternMode)
    {
        worldToScreen(dstX, dstY);
        m_rasterizer.AddPath(m_pathTransform);
        pixfmt img_pixf(img.renBuf); //获取图片
        if (strcmp(pattternMode, "repeat") == 0) {
            img_source_type img_src(img_pixf);
            span_pattern_type_repeat m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-x") == 0) {
            img_source_type_x img_src(img_pixf);
            span_pattern_type_x m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-y") == 0) {
            img_source_type_y img_src(img_pixf);
            span_pattern_type_y m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "no-repeat") == 0) {
            img_source_type_none img_src(img_pixf);
            span_pattern_type_none m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        }
    }

    void BaseGfxExtendEngine::patternImageStroke(Image& img, double dstX, double dstY, const char* pattternMode)
    {
        worldToScreen(dstX, dstY);
        pixfmt img_pixf(img.renBuf); //获取图片

        m_rasterizer.AddPath(m_strokeTransform);

        if (strcmp(pattternMode, "repeat") == 0) {
            img_source_type img_src(img_pixf);
            span_pattern_type_repeat m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-x") == 0) {
            img_source_type_x img_src(img_pixf);
            span_pattern_type_x m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "repeat-y") == 0) {
            img_source_type_y img_src(img_pixf);
            span_pattern_type_y m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        } else if (strcmp(pattternMode, "no-repeat") == 0) {
            img_source_type_none img_src(img_pixf);
            span_pattern_type_none m_spanPatternType(img_src, 0 - dstX, 0 - dstY);
            OHOS::RenderScanlinesAntiAlias(m_rasterizer, m_scanline, m_renBase, m_allocator, m_spanPatternType);
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::copyImage(Image& img,
                                        int imgX1, int imgY1, int imgX2, int imgY2,
                                        double dstX, double dstY)
    {
        worldToScreen(dstX, dstY);
        // JME
        //OHOS::rect r(imgX1, imgY1, imgX2, imgY2);
        Rect r(imgX1, imgY1, imgX2, imgY2);
        m_renBase.CopyFrom(img.renBuf, &r, int(dstX) - imgX1, int(dstY) - imgY1);
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::copyImage(Image& img, double dstX, double dstY)
    {
        worldToScreen(dstX, dstY);
        m_renBase.CopyFrom(img.renBuf, 0, int(dstX), int(dstY));
    }

    void BaseGfxExtendEngine::BlendFromImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                                             double dstX, double dstY, unsigned alpha, bool isAntiAlias)
    {
        worldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        // JME
        //OHOS::rect r(imgX1, imgY1, imgX2, imgY2);
        Rect r(imgX1, imgY1, imgX2, imgY2);
        if (m_blendMode == BlendAlpha) {
            if (isAntiAlias) {
                m_renBasePre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
            } else {
                m_renBase.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
            }
        } else {
            if (isAntiAlias) {
                this->m_renBaseCompPre.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
            } else {
                this->m_renBaseComp.BlendFrom(pixF, &r, int(dstX) - imgX1, int(dstY) - imgY1, alpha);
            }
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::BlendFromImage(Image& img, double dstX, double dstY, unsigned alpha, bool isAntiAlias)
    {
        worldToScreen(dstX, dstY);
        PixFormat pixF(img.renBuf);
        if (isAntiAlias) {
            m_renBasePre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
        }
        if (m_blendMode == BlendAlpha) {
            if (isAntiAlias) {
                m_renBasePre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
            } else {
                m_renBase.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
            }
        } else {
            if (isAntiAlias) {
                m_renBaseCompPre.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
            } else {
                m_renBaseComp.BlendFrom(pixF, 0, int(dstX), int(dstY), alpha);
            }
        }
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::Image::premultiply()
    {
        PixFormat pixf(renBuf);
        pixf.Premultiply();
    }

    //------------------------------------------------------------------------
    void BaseGfxExtendEngine::Image::demultiply()
    {
        PixFormat pixf(renBuf);
        pixf.Demultiply();
    }

    bool BaseGfxExtendEngine::bounding_rect_single(unsigned int path_id, RectD* rect, PathTransform& path)
    {
        return OHOS::BoundingRectSingle(path, path_id, &rect->x1, &rect->y1, &rect->x2, &rect->y2);
    }

} // namespace OHOS
