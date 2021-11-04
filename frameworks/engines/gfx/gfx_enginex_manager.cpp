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
        delete [] dashes;
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
    m_clipBox(0,0,0,0),

    m_blendMode(BlendAlpha),
    m_imageBlendMode(BlendDst),
    m_imageBlendColor(0,0,0),

    m_scanline(),
    m_rasterizer(),

    m_masterAlpha(1.0),
    m_antiAliasGamma(1.0),

    m_fillColor(255, 255, 255),
    m_lineColor(0,   0,   0),
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

    m_imageFilter(Bilinear),
    m_imageResample(NoResample),
    m_imageFilterLut(agg::image_filter_bilinear(), true),

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
    m_dashStrokeTransform(m_convDashStroke,m_transform),
    is_dash(false),
    dashes(nullptr),
    ndashes(0),
    dDashOffset(0)
{
    lineCap(m_lineCap);
    lineJoin(m_lineJoin);
}

BaseGfxExtendEngine::BaseGfxExtendEngine(const BaseGfxExtendEngine &o)
    :m_scanline(),
      m_rasterizer(),
      m_fillGradientMatrix(o.m_fillGradientMatrix),
      m_lineGradientMatrix(o.m_lineGradientMatrix),
      m_imageFilterLut(agg::image_filter_bilinear(), true),
      m_fillGradientInterpolator(m_fillGradientMatrix),
      m_lineGradientInterpolator(m_lineGradientMatrix),
      m_path(o.m_path),
      m_convCurve(m_path),
      m_convDashCurve(m_convCurve),
      m_convStroke(m_convCurve),
      m_convDashStroke(m_convDashCurve),
      m_pathTransform(m_convCurve, m_transform),
      m_strokeTransform(m_convStroke, m_transform),
      m_dashStrokeTransform(m_convDashStroke,m_transform)
{
    m_rbuf=o.m_rbuf;
    m_pixFormat=PixFormat(m_rbuf);
    m_pixFormatComp=PixFormatComp(m_rbuf);
    m_pixFormatPre=PixFormatPre(m_rbuf);
    m_pixFormatCompPre=PixFormatCompPre(m_rbuf);
    m_renBase=RendererBase(m_pixFormat);

    m_renBaseComp=RendererBaseComp(m_pixFormatComp);
    m_renBasePre=RendererBasePre(m_pixFormatPre);
    m_renBaseCompPre=RendererBaseCompPre(m_pixFormatCompPre);
    m_renSolid=RendererSolid(m_renBase);
    m_renSolidComp=RendererSolidComp(m_renBaseComp);

    m_allocator=o.m_allocator;
    m_clipBox=o.m_clipBox;

    m_blendMode=o.m_blendMode;
    m_imageBlendMode=o.BlendDst;
    m_imageBlendColor=o.m_imageBlendColor;

    m_masterAlpha=o.m_masterAlpha;
    m_antiAliasGamma=o.m_antiAliasGamma;

    m_fillColor=o.m_fillColor;
    m_lineColor=o.m_lineColor;
    m_fillGradient=o.m_fillGradient;
    m_lineGradient=o.m_lineGradient;

    m_lineCap=o.m_lineCap;
    m_lineJoin=o.m_lineJoin;
    m_miterLimit=o.m_miterLimit;
    m_fillGradientFlag=o.Solid;
    m_lineGradientFlag=o.Solid;
    m_fillGradientD1=o.m_fillGradientD1;
    m_lineGradientD1=o.m_lineGradientD1;
    m_fillGradientD2=o.m_fillGradientD2;
    m_lineGradientD2=o.m_lineGradientD2;

    m_imageFilter=o.m_imageFilter;
    m_imageResample=o.m_imageResample;


    m_linearGradientFunction=o.m_linearGradientFunction;
    m_radialGradientFunction=o.m_radialGradientFunction;

    m_lineWidth=o.m_lineWidth;
    m_evenOddFlag=o.m_evenOddFlag;
    m_transform=o.m_transform;

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

    m_renBase.reset_clipping(true);
    m_renBaseComp.reset_clipping(true);
    m_renBasePre.reset_clipping(true);
    m_renBaseCompPre.reset_clipping(true);

    resetTransformations();
    lineWidth(1.0),
    lineColor(0,0,0);
    fillColor(255,255,255);
    clipBox(0, 0, width, height);
    lineCap(CapRound);
    lineJoin(JoinRound);
    imageFilter(Bilinear);
    imageResample(NoResample);
    m_masterAlpha = 1.0;
    m_antiAliasGamma = 1.0;
    m_rasterizer.gamma(agg::gamma_none());
    m_blendMode = BlendAlpha;
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::attach(Image& img)
{
    attach(img.renBuf.buf(), img.renBuf.width(), img.renBuf.height(), img.renBuf.stride());
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::clipBox(double x1, double y1, double x2, double y2)
{
    m_clipBox = RectD(x1, y1, x2, y2);
    int rx1 = int(x1);
    int ry1 = int(y1);
    int rx2 = int(x2);
    int ry2 = int(y2);

    m_renBase.clip_box(rx1, ry1, rx2, ry2);
    m_renBaseComp.clip_box(rx1, ry1, rx2, ry2);
    m_renBasePre.clip_box(rx1, ry1, rx2, ry2);
    m_renBaseCompPre.clip_box(rx1, ry1, rx2, ry2);

    m_rasterizer.clip_box(x1, y1, x2, y2);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::blendMode(BlendMode m)
{
    m_blendMode = m;
    m_pixFormatComp.comp_op(m);
    m_pixFormatCompPre.comp_op(m);
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
    m_masterAlpha = a;
    updateRasterizerGamma();
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
    m_renBase.clear(c);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::clearAll(unsigned r, unsigned g, unsigned b, unsigned a)
{
    clearAll(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::clearClipBox(Color c)
{
    m_renBase.copy_bar(0, 0, m_renBase.width(), m_renBase.height(), c);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::clearClipBox(unsigned r, unsigned g, unsigned b, unsigned a)
{
    clearClipBox(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::worldToScreen(double& x, double& y) const
{
    m_transform.transform(&x, &y);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::screenToWorld(double& x, double& y) const
{
    m_transform.inverse_transform(&x, &y);
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
    return m_renBase.inbox(int(worldX), int(worldY));
}


//------------------------------------------------------------------------
BaseGfxExtendEngine::Transformations BaseGfxExtendEngine::transformations() const
{
    Transformations tr;
    m_transform.store_to(tr.affineMatrix);
    return tr;
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformations(const Transformations& tr)
{
    m_transform.load_from(tr.affineMatrix);
    m_convCurve.approximation_scale(worldToScreen(1.0) * g_approxScale);
    if(!this->is_dash) {
        m_convStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    } else {
        m_convDashStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    }
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::resetTransformations()
{
    m_transform.reset();
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::rotate(double angle)          { m_transform *= agg::trans_affine_rotation(angle);   }
void BaseGfxExtendEngine::skew(double sx, double sy)    { m_transform *= agg::trans_affine_skewing(sx, sy);   }
void BaseGfxExtendEngine::translate(double x, double y) { m_transform *= agg::trans_affine_translation(x, y); }


//------------------------------------------------------------------------
void BaseGfxExtendEngine::affine(const Affine& tr)
{
    m_transform *= tr;
    m_convCurve.approximation_scale(worldToScreen(1.0) * g_approxScale);
    if(!this->is_dash) {
        m_convStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    } else {
        m_convDashStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    }
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::affine(const Transformations& tr)
{
    affine(agg::trans_affine(tr.affineMatrix[0], tr.affineMatrix[1], tr.affineMatrix[2],
                             tr.affineMatrix[3], tr.affineMatrix[4], tr.affineMatrix[5]));
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::scale(double sx, double sy)
{
    m_transform *= agg::trans_affine_scaling(sx, sy);
    m_convCurve.approximation_scale(worldToScreen(1.0) * g_approxScale);
    if(!this->is_dash) {
        m_convStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    } else {
        m_convDashStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    }
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::parallelogram(double x1, double y1, double x2, double y2, const double* para)
{
    m_transform *= agg::trans_affine(x1, y1, x2, y2, para);
    m_convCurve.approximation_scale(worldToScreen(1.0) * g_approxScale);
    if(!this->is_dash) {
        m_convStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    } else {
        m_convDashStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    }
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::viewport(double worldX1,  double worldY1,  double worldX2,  double worldY2,
                     double screenX1, double screenY1, double screenX2, double screenY2,
                     ViewportOption opt)
{
    agg::trans_viewport vp;
    switch(opt)
    {
        case Anisotropic: vp.preserve_aspect_ratio(0.0, 0.0, agg::aspect_ratio_stretch); break;
        case XMinYMin:    vp.preserve_aspect_ratio(0.0, 0.0, agg::aspect_ratio_meet);    break;
        case XMidYMin:    vp.preserve_aspect_ratio(0.5, 0.0, agg::aspect_ratio_meet);    break;
        case XMaxYMin:    vp.preserve_aspect_ratio(1.0, 0.0, agg::aspect_ratio_meet);    break;
        case XMinYMid:    vp.preserve_aspect_ratio(0.0, 0.5, agg::aspect_ratio_meet);    break;
        case XMidYMid:    vp.preserve_aspect_ratio(0.5, 0.5, agg::aspect_ratio_meet);    break;
        case XMaxYMid:    vp.preserve_aspect_ratio(1.0, 0.5, agg::aspect_ratio_meet);    break;
        case XMinYMax:    vp.preserve_aspect_ratio(0.0, 1.0, agg::aspect_ratio_meet);    break;
        case XMidYMax:    vp.preserve_aspect_ratio(0.5, 1.0, agg::aspect_ratio_meet);    break;
        case XMaxYMax:    vp.preserve_aspect_ratio(1.0, 1.0, agg::aspect_ratio_meet);    break;
    }
    vp.world_viewport(worldX1,   worldY1,  worldX2,  worldY2);
    vp.device_viewport(screenX1, screenY1, screenX2, screenY2);
    m_transform *= vp.to_affine();
    m_convCurve.approximation_scale(worldToScreen(1.0) * g_approxScale);
    if(!this->is_dash) {
        m_convStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
    } else {
        m_convDashStroke.approximation_scale(worldToScreen(1.0) * g_approxScale);
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

//------------------------------------------------------------------------


void BaseGfxExtendEngine::fillGradientAndStop(Color c1, Color c2, double startscal,double endscal)
{
    m_fillRadialGradient.remove_all();
    m_fillRadialGradient.add_color(0.0, agg::srgba8(0, 255, 0,255));
    m_fillRadialGradient.add_color(0.5, agg::srgba8(120, 0, 0,255));
    m_fillRadialGradient.add_color(1.0, agg::srgba8(0, 0, 255,255));
    m_fillRadialGradient.build_lut();
}

void BaseGfxExtendEngine::remove_all_color(){
    m_fillRadialGradient.remove_all();
}

void BaseGfxExtendEngine::add_color(double offset,  Color c1){
     m_fillRadialGradient.add_color(offset, c1);
}

void BaseGfxExtendEngine::build_lut(){
    m_fillRadialGradient.build_lut();
}





void BaseGfxExtendEngine::fillLinearGradientAndStop(double x1, double y1, double x2, double y2)
{

    double angle = atan2(y2-y1, x2-x1);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_rotation(angle);
    m_fillGradientMatrix *= agg::trans_affine_translation(x1, y1);
    m_fillGradientMatrix *= m_transform;
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0.0;
    m_fillGradientD2 = sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
    m_fillGradientFlag = Linear;
    m_fillColor = Color(0,0,0);  // Set some real color
}


void BaseGfxExtendEngine::fillRadialGradient(double start_x, double start_y,double start_r, double end_x, double end_y,double end_r)
{
    m_fillRadialMatrix.reset();
    m_fillRadialMatrix *= agg::trans_affine_translation(end_x, end_y);
    m_fillRadialMatrix.invert();
    m_interpolator_type.transformer(m_fillRadialMatrix);
    m_fillGradientD1 = start_r;
    m_fillGradientD2 = end_r;
    m_radialGradientFunction  = agg::gradient_radial_focus(end_r,start_x-end_x,start_y-end_y);
    m_fillGradientFlag = Radial;
    m_fillColor = Color(0,0,0);
}

void BaseGfxExtendEngine::fillLinearGradient(double start_x, double start_y,double end_x, double end_y){
    double angle = atan2(end_y-start_y, end_x-start_x);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_rotation(angle);
    m_fillGradientMatrix *= agg::trans_affine_translation(start_x, start_y);
    m_fillGradientMatrix *= m_transform;
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0.0;
    m_fillGradientD2 = sqrt((end_x-start_x) * (end_x-start_x) + (end_y-start_y) * (end_y-start_y));
    m_fillGradientFlag = Linear;
    m_fillColor = Color(0,0,0);  // Set some real color
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::lineWidth(double w)
{
    m_lineWidth = w;
    if(!this->is_dash) {
       m_convStroke.width(w);
    } else {
        m_convDashStroke.width(w);
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
    m_rasterizer.filling_rule(evenOddFlag ? agg::fill_even_odd : agg::fill_non_zero);
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
    if(!this->is_dash) {
        m_convStroke.line_cap((agg::line_cap_e)cap);
    } else {
        m_convDashStroke.line_cap((agg::line_cap_e)cap);
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
    if(!this->is_dash) {
        m_convStroke.line_join((agg::line_join_e)join);
    } else {
        m_convDashStroke.line_join((agg::line_join_e)join);
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
    if(!this->is_dash) {
        m_convStroke.miter_limit(limitValue);
    } else {
        m_convDashStroke.miter_limit(limitValue);
    }
}

double BaseGfxExtendEngine::MiterLimit() const
{
    return m_miterLimit;
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::addLine(double x1, double y1, double x2, double y2)
{
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y2);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::line(double x1, double y1, double x2, double y2)
{
    m_path.remove_all();
    addLine(x1, y1, x2, y2);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::triangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y2);
    m_path.line_to(x3, y3);
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::rectangle(double x1, double y1, double x2, double y2)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y1);
    m_path.line_to(x2, y2);
    m_path.line_to(x1, y2);
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2, double r)
{
    m_path.remove_all();
    agg::rounded_rect rc(x1, y1, x2, y2, r);
    rc.normalize_radius();
    rc.approximation_scale(worldToScreen(1.0) * g_approxScale);
    // JME audit
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0);
    drawPath(FillAndStroke);
}



//------------------------------------------------------------------------
void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2, double rx, double ry)
{
    m_path.remove_all();
    agg::rounded_rect rc;
    rc.rect(x1, y1, x2, y2);
    rc.radius(rx, ry);
    rc.normalize_radius();
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0); // JME
    drawPath(FillAndStroke);
}



//------------------------------------------------------------------------
void BaseGfxExtendEngine::roundedRect(double x1, double y1, double x2, double y2,
                        double rx_bottom, double ry_bottom,
                        double rx_top,    double ry_top)
{
    m_path.remove_all();
    agg::rounded_rect rc;
    rc.rect(x1, y1, x2, y2);
    rc.radius(rx_bottom, ry_bottom, rx_top, ry_top);
    rc.normalize_radius();
    rc.approximation_scale(worldToScreen(1.0) * g_approxScale);
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0); // JME
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::ellipse(double cx, double cy, double rx, double ry)
{
    m_path.remove_all();
    agg::bezier_arc arc(cx, cy, rx, ry, 0, 2*pi());
    //m_path.add_path(arc, 0, false);
    m_path.concat_path(arc,0); // JME
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::arc(double cx, double cy, double rx, double ry, double start, double sweep)
{
    m_path.remove_all();
    agg::bezier_arc arc(cx, cy, rx, ry, start, sweep);
    //m_path.add_path(arc, 0, false);
    m_path.concat_path(arc,0); // JME
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::star(double cx, double cy, double r1, double r2, double startAngle, int numRays)
{
    m_path.remove_all();
    double da = agg::pi / double(numRays);
    double a = startAngle;
    int i;
    for (i = 0; i < numRays; i++)
    {
        double x = cos(a) * r2 + cx;
        double y = sin(a) * r2 + cy;
        if (i) m_path.line_to(x, y);
        else   m_path.move_to(x, y);
        a += da;
        m_path.line_to(cos(a) * r1 + cx, sin(a) * r1 + cy);
        a += da;
    }
    closePolygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::curve(double x1, double y1, double x2, double y2, double x3, double y3)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.curve3(x2, y2, x3, y3);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::curve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.curve4(x2, y2, x3, y3, x4, y4);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::polygon(double* xy, int numPoints)
{
    m_path.remove_all();
    //m_path.add_poly(xy, numPoints);
    m_path.concat_poly(xy,0,true); // JME
    closePolygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::polyline(double* xy, int numPoints)
{
    m_path.remove_all();
    //m_path.add_poly(xy, numPoints);
	m_path.concat_poly(xy,0,true); // JME
    drawPath(StrokeOnly);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::resetPath() { m_path.remove_all(); }

//------------------------------------------------------------------------
void BaseGfxExtendEngine::moveTo(double x, double y)
{
    m_path.move_to(x, y);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::moveRel(double dx, double dy)
{
    m_path.move_rel(dx, dy);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::lineTo(double x, double y)
{
    m_path.line_to(x, y);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::lineRel(double dx, double dy)
{
    m_path.line_rel(dx, dy);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::horLineTo(double x)
{
    m_path.hline_to(x);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::horLineRel(double dx)
{
    m_path.hline_rel(dx);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::verLineTo(double y)
{
    m_path.vline_to(y);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::verLineRel(double dy)
{
    m_path.vline_rel(dy);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::arcTo(double rx, double ry,
                  double angle,
                  bool largeArcFlag,
                  bool sweepFlag,
                  double x, double y)
{
    m_path.arc_to(rx, ry, angle, largeArcFlag, sweepFlag, x, y);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::arcRel(double rx, double ry,
                   double angle,
                   bool largeArcFlag,
                   bool sweepFlag,
                   double dx, double dy)
{
    m_path.arc_rel(rx, ry, angle, largeArcFlag, sweepFlag, dx, dy);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::quadricCurveTo(double xCtrl, double yCtrl,
                           double xTo,   double yTo)
{
    m_path.curve3(xCtrl, yCtrl, xTo, yTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::quadricCurveRel(double dxCtrl, double dyCtrl,
                            double dxTo,   double dyTo)
{
    m_path.curve3_rel(dxCtrl, dyCtrl, dxTo, dyTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::quadricCurveTo(double xTo, double yTo)
{
    m_path.curve3(xTo, yTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::quadricCurveRel(double dxTo, double dyTo)
{
    m_path.curve3_rel(dxTo, dyTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::cubicCurveTo(double xCtrl1, double yCtrl1,
                         double xCtrl2, double yCtrl2,
                         double xTo,    double yTo)
{
    m_path.curve4(xCtrl1, yCtrl1, xCtrl2, yCtrl2, xTo, yTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::cubicCurveRel(double dxCtrl1, double dyCtrl1,
                          double dxCtrl2, double dyCtrl2,
                          double dxTo,    double dyTo)
{
    m_path.curve4_rel(dxCtrl1, dyCtrl1, dxCtrl2, dyCtrl2, dxTo, dyTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::cubicCurveTo(double xCtrl2, double yCtrl2,
                         double xTo,    double yTo)
{
    m_path.curve4(xCtrl2, yCtrl2, xTo, yTo);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::cubicCurveRel(double xCtrl2, double yCtrl2,
                          double xTo,    double yTo)
{
    m_path.curve4_rel(xCtrl2, yCtrl2, xTo, yTo);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::addEllipse(double cx, double cy, double rx, double ry, Direction dir)
{
    agg::bezier_arc arc(cx, cy, rx, ry, 0, (dir == CCW) ? 2*pi() : -2*pi());
    //m_path.add_path(arc, 0, false);
	m_path.concat_path(arc,0); // JME
    m_path.close_polygon();
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::closePolygon()
{
   m_path.close_polygon();
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::imageFilter(ImageFilter f)
{
    m_imageFilter = f;
    switch(f)
    {
        case NoFilter:    break;
        case Bilinear:    m_imageFilterLut.calculate(agg::image_filter_bilinear(),    true); break;
        case Hanning:     m_imageFilterLut.calculate(agg::image_filter_hanning(),     true); break;
        case Hermite:     m_imageFilterLut.calculate(agg::image_filter_hermite(),     true); break;
        case Quadric:     m_imageFilterLut.calculate(agg::image_filter_quadric(),     true); break;
        case Bicubic:     m_imageFilterLut.calculate(agg::image_filter_bicubic(),     true); break;
        case Catrom:      m_imageFilterLut.calculate(agg::image_filter_catrom(),      true); break;
        case Spline16:    m_imageFilterLut.calculate(agg::image_filter_spline16(),    true); break;
        case Spline36:    m_imageFilterLut.calculate(agg::image_filter_spline36(),    true); break;
        case Blackman144: m_imageFilterLut.calculate(agg::image_filter_blackman144(), true); break;
    }
}


//------------------------------------------------------------------------
BaseGfxExtendEngine::ImageFilter BaseGfxExtendEngine::imageFilter() const
{
    return m_imageFilter;
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::imageResample(ImageResample f)
{
    m_imageResample = f;
}


//------------------------------------------------------------------------
BaseGfxExtendEngine::ImageResample BaseGfxExtendEngine::imageResample() const
{
    return m_imageResample;
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImage(const Image& img,    int imgX1,    int imgY1,    int imgX2,    int imgY2,
                                             double dstX1, double dstY1, double dstX2, double dstY2)
{
    resetPath();
    moveTo(dstX1, dstY1);
    lineTo(dstX2, dstY1);
    lineTo(dstX2, dstY2);
    lineTo(dstX1, dstY2);
    closePolygon();
    double parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImage(const Image& img, double dstX1, double dstY1, double dstX2, double dstY2)
{
    resetPath();
    moveTo(dstX1, dstY1);
    lineTo(dstX2, dstY1);
    lineTo(dstX2, dstY2);
    lineTo(dstX1, dstY2);
    closePolygon();
    double parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                           const double* parallelogram)
{
    resetPath();
    moveTo(parallelogram[0], parallelogram[1]);
    lineTo(parallelogram[2], parallelogram[3]);
    lineTo(parallelogram[4], parallelogram[5]);
    lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
           parallelogram[1] + parallelogram[5] - parallelogram[3]);
    closePolygon();
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImage(const Image& img, const double* parallelogram)
{
    resetPath();
    moveTo(parallelogram[0], parallelogram[1]);
    lineTo(parallelogram[2], parallelogram[3]);
    lineTo(parallelogram[4], parallelogram[5]);
    lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
           parallelogram[1] + parallelogram[5] - parallelogram[3]);
    closePolygon();
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImagePath(const Image& img,    int imgX1,    int imgY1,    int imgX2,    int imgY2,
                                                 double dstX1, double dstY1, double dstX2, double dstY2)
{
    double parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::transformImagePath(const Image& img, double dstX1, double dstY1, double dstX2, double dstY2)
{
    double parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
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
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}



//------------------------------------------------------------------------
void BaseGfxExtendEngine::drawPath(DrawPathFlag flag)
{
    m_rasterizer.reset();
    switch(flag)
    {
    case FillOnly:
        if (m_fillColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(true);
        }
        break;

    case StrokeOnly:
        if (m_lineColor.a && m_lineWidth > 0.0)
        {
            if(!this->is_dash) {
                m_rasterizer.add_path(m_strokeTransform);
            } else {
                for (unsigned int i = 0; i < this->ndashes; i += 2) {
                    m_convDashCurve.add_dash(dashes[i], dashes[i+1]);
                }
                m_convDashCurve.dash_start(dDashOffset);
                m_rasterizer.add_path(m_dashStrokeTransform);
            }
            render(false);
        }
        break;

    case FillAndStroke:
        if (m_fillColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(true);
        }

        if (m_lineColor.a && m_lineWidth > 0.0)
        {
            if(!this->is_dash) {
                m_rasterizer.add_path(m_strokeTransform);
            } else {
                for (unsigned int i = 0; i < this->ndashes; i += 2) {
                    m_convDashCurve.add_dash(dashes[i], dashes[i+1]);
                }
                m_convDashCurve.dash_start(dDashOffset);
                m_rasterizer.add_path(m_dashStrokeTransform);
            }
            render(false);
        }
        break;

    case FillWithLineColor:
        if (m_lineColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(false);
        }
        break;
    }
}



//------------------------------------------------------------------------
class BaseGfxExtendEngineRenderer
{
public:
    //--------------------------------------------------------------------
    template<class BaseRenderer, class SolidRenderer>
    void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase, SolidRenderer& renSolid, bool fillColor)
    {
		// JME
        typedef agg::span_allocator<BaseGfxExtendEngine::ColorType> span_allocator_type;
        //- typedef agg::renderer_scanline_aa<BaseRenderer, BaseGfxExtendEngine::LinearGradientSpan> RendererLinearGradient;
        typedef agg::renderer_scanline_aa<BaseRenderer,
										span_allocator_type,
                                        BaseGfxExtendEngine::LinearGradientSpan> RendererLinearGradient;
        //- typedef agg::renderer_scanline_aa<BaseRenderer, BaseGfxExtendEngine::RadialGradientSpan> RendererRadialGradient;
		typedef agg::renderer_scanline_aa<BaseRenderer,
										span_allocator_type,
                                        BaseGfxExtendEngine::RadialGradientSpan> RendererRadialGradient;

        if ((fillColor && gr.m_fillGradientFlag == BaseGfxExtendEngine::Linear) ||
           (!fillColor && gr.m_lineGradientFlag == BaseGfxExtendEngine::Linear))
        {
            if (fillColor)
            {
                BaseGfxExtendEngine::LinearGradientSpan span(/*gr.m_allocator, */
                                               gr.m_fillGradientInterpolator,
                                               gr.m_linearGradientFunction,
                                               gr.m_fillRadialGradient,
                                               gr.m_fillGradientD1,
                                               gr.m_fillGradientD2);
				//-RendererLinearGradient ren(renBase,span);
                RendererLinearGradient ren(renBase,gr.m_allocator,span);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ren);
            }
            else
            {
                BaseGfxExtendEngine::LinearGradientSpan span(/*gr.m_allocator,*/
                                               gr.m_lineGradientInterpolator,
                                               gr.m_linearGradientFunction,
                                               gr.m_fillRadialGradient,
                                               gr.m_lineGradientD1,
                                               gr.m_lineGradientD2);
                //- RendererLinearGradient ren(renBase, span);
                RendererLinearGradient ren(renBase,gr.m_allocator,span);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ren);
            }
        }
        else
        {
            if ((fillColor && gr.m_fillGradientFlag == BaseGfxExtendEngine::Radial) ||
               (!fillColor && gr.m_lineGradientFlag == BaseGfxExtendEngine::Radial))
            {
                if (fillColor)
                {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                                                                gr.m_interpolator_type,
                                                                gr.m_radialGradientFunction,
                                                                gr.m_fillRadialGradient,
                                                                gr.m_fillGradientD1,
                                                                gr.m_fillGradientD2);
                    //-RendererRadialGradient ren(renBase, span);
                    RendererRadialGradient ren(renBase,gr.m_allocator,span);
                    agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ren);
                }
                else
                {
                    BaseGfxExtendEngine::RadialGradientSpan span(
                                                                gr.m_interpolator_type,
                                                                gr.m_radialGradientFunction,
                                                                gr.m_fillRadialGradient,
                                                                gr.m_fillGradientD1,
                                                                gr.m_fillGradientD2);
                    //-RendererRadialGradient ren(renBase, span);
                    RendererRadialGradient ren(renBase,gr.m_allocator,span);
                    agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ren);
                }
            }
            else
            {
                renSolid.color(fillColor ? gr.m_fillColor : gr.m_lineColor);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, renSolid);
            }
        }
    }


    //--------------------------------------------------------------------
    class SpanConvImageBlend
    {
    public:
        SpanConvImageBlend(BaseGfxExtendEngine::BlendMode m, BaseGfxExtendEngine::Color c) :
            m_mode(m), m_color(c)
        {}

        void convert(BaseGfxExtendEngine::Color* span, int x, int y, unsigned len) const
        {
            unsigned l2;
            BaseGfxExtendEngine::Color* s2;
            if(m_mode != BaseGfxExtendEngine::BlendDst)
            {
                l2 = len;
                s2 = span;
                typedef agg::comp_op_adaptor_clip_to_dst_rgba_pre<BaseGfxExtendEngine::Color, agg::order_rgba> OpType;
                do
                {
                    OpType::blend_pix(m_mode,
                                      (BaseGfxExtendEngine::Color::value_type*)s2,
                                      m_color.r,
                                      m_color.g,
                                      m_color.b,
                                      BaseGfxExtendEngine::Color::full_value(),
                                      agg::cover_full);
                    ++s2;
                }
                while(--l2);
            }
            if(!m_color.is_opaque())
            {
                l2 = len;
                s2 = span;
                do
                {
                    s2->r = BaseGfxExtendEngine::Color::multiply(s2->r, m_color.a);
                    s2->g = BaseGfxExtendEngine::Color::multiply(s2->g, m_color.a);
                    s2->b = BaseGfxExtendEngine::Color::multiply(s2->b, m_color.a);
                    s2->a = BaseGfxExtendEngine::Color::multiply(s2->a, m_color.a);
                    ++s2;
                }
                while(--l2);
            }
        }

    private:
        BaseGfxExtendEngine::BlendMode m_mode;
        BaseGfxExtendEngine::Color     m_color;
    };




    //--------------------------------------------------------------------
    template<class BaseRenderer, class SolidRenderer, class Rasterizer, class Scanline>
    void static render(BaseGfxExtendEngine& gr, BaseRenderer& renBase, SolidRenderer& renSolid, Rasterizer& ras, Scanline& sl)
    {
		// JME
        typedef agg::span_allocator<BaseGfxExtendEngine::ColorType> span_allocator_type;
        typedef agg::renderer_scanline_aa<BaseRenderer,span_allocator_type,BaseGfxExtendEngine::LinearGradientSpan> RendererLinearGradient;
        typedef agg::renderer_scanline_aa<BaseRenderer,span_allocator_type,BaseGfxExtendEngine::RadialGradientSpan> RendererRadialGradient;

        if(gr.m_fillGradientFlag == BaseGfxExtendEngine::Linear)
        {
            BaseGfxExtendEngine::LinearGradientSpan span(
                                           gr.m_fillGradientInterpolator,
                                           gr.m_linearGradientFunction,
                                           gr.m_fillRadialGradient,
                                           gr.m_fillGradientD1,
                                           gr.m_fillGradientD2);
            RendererLinearGradient ren(renBase,gr.m_allocator,span);
            agg::render_scanlines(ras, sl, ren);
        }
        else
        {
            if(gr.m_fillGradientFlag == BaseGfxExtendEngine::Radial)
            {

                BaseGfxExtendEngine::RadialGradientSpan span(
                                               gr.m_interpolator_type,
                                               gr.m_radialGradientFunction,
                                               gr.m_fillRadialGradient,
                                               gr.m_fillGradientD1,
                                               gr.m_fillGradientD2);
                RendererRadialGradient ren(renBase,gr.m_allocator,span);
                agg::render_scanlines(ras, sl, ren);
            }
            else
            {
                renSolid.color(gr.m_fillColor);
                agg::render_scanlines(ras, sl, renSolid);
            }
        }
    }



    //--------------------------------------------------------------------
    //! JME - this is where the bulk of the changes have taken place.
    template<class BaseRenderer, class Interpolator>
    static void renderImage(BaseGfxExtendEngine& gr, const BaseGfxExtendEngine::Image& img,
                            BaseRenderer& renBase, Interpolator& interpolator)
    {
		//! JME - have not quite figured which part of this is not const-correct
		// hence the cast.
        BaseGfxExtendEngine::Image& imgc = const_cast<BaseGfxExtendEngine::Image&>(img);
        BaseGfxExtendEngine::PixFormat img_pixf(imgc.renBuf);
        typedef agg::image_accessor_clone<BaseGfxExtendEngine::PixFormat> img_source_type;
		img_source_type source(img_pixf);

        SpanConvImageBlend blend(gr.m_imageBlendMode, gr.m_imageBlendColor);
        if (gr.m_imageFilter == BaseGfxExtendEngine::NoFilter)
        {

			typedef agg::span_image_filter_rgba_nn<img_source_type,Interpolator> SpanGenType;
			typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
            typedef agg::renderer_scanline_aa<BaseRenderer,BaseGfxExtendEngine::SpanAllocator,SpanGenType> RendererType;

			SpanGenType sg(source,interpolator);
			SpanConvType sc(sg, blend);
			RendererType ri(renBase,gr.m_allocator,sg);
			agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
        }
        else
        {
            bool resample = (gr.m_imageResample == BaseGfxExtendEngine::ResampleAlways);
            if(gr.m_imageResample == BaseGfxExtendEngine::ResampleOnZoomOut)
            {
                double sx, sy;
                interpolator.transformer().scaling_abs(&sx,&sy);
                if (sx > 1.125 || sy > 1.125)
                {
					resample = true;
                }
            }

            if (resample)
            {
                typedef agg::span_image_resample_rgba_affine<img_source_type> SpanGenType;
                typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                typedef agg::renderer_scanline_aa<BaseRenderer,BaseGfxExtendEngine::SpanAllocator,SpanGenType> RendererType;

                SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                SpanConvType sc(sg, blend);
                RendererType ri(renBase,gr.m_allocator,sg);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
            }
            else
            {
                // this is the BaseGfxExtendEngine default
                if (gr.m_imageFilter == BaseGfxExtendEngine::Bilinear)
                {
                    typedef agg::span_image_filter_rgba_bilinear<img_source_type,Interpolator> SpanGenType;
                    typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                    typedef agg::renderer_scanline_aa<BaseRenderer,BaseGfxExtendEngine::SpanAllocator,SpanGenType> RendererType;

					SpanGenType sg(source,interpolator);
					SpanConvType sc(sg, blend);
					RendererType ri(renBase,gr.m_allocator,sg);
					agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                }
                else
                {
                    if(gr.m_imageFilterLut.diameter() == 2)
                    {
                        typedef agg::span_image_filter_rgba_2x2<img_source_type,Interpolator> SpanGenType;
                        typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                        typedef agg::renderer_scanline_aa<BaseRenderer,BaseGfxExtendEngine::SpanAllocator,SpanGenType> RendererType;

                        SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                        SpanConvType sc(sg,blend);
                        RendererType ri(renBase,gr.m_allocator,sg);
                        agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                    }
                    else
                    {
                        typedef agg::span_image_filter_rgba<img_source_type,Interpolator> SpanGenType;
                        typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                        typedef agg::renderer_scanline_aa<BaseRenderer,BaseGfxExtendEngine::SpanAllocator,SpanGenType> RendererType;
                        SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                        SpanConvType sc(sg, blend);
						RendererType ri(renBase,gr.m_allocator,sg);
                        agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                    }
                }
            }
        }
    }
};


//------------------------------------------------------------------------
void BaseGfxExtendEngine::render(bool fillColor)
{
    if(m_blendMode == BlendAlpha)
    {
        BaseGfxExtendEngineRenderer::render(*this, m_renBase, m_renSolid, fillColor);
    }
    else
    {
        BaseGfxExtendEngineRenderer::render(*this, m_renBaseComp, m_renSolidComp, fillColor);
    }
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::renderImage(const Image& img, int x1, int y1, int x2, int y2,
                        const double* parl)
{
    agg::trans_affine mtx((double)x1,
                          (double)y1,
                          (double)x2,
                          (double)y2,
                          parl);
    mtx *= m_transform;
    mtx.invert();

    m_rasterizer.reset();
    m_rasterizer.add_path(m_pathTransform);

    typedef agg::span_interpolator_linear<agg::trans_affine> Interpolator;
    Interpolator interpolator(mtx);

    if(m_blendMode == BlendAlpha)
    {
		// JME audit -
        BaseGfxExtendEngineRenderer::renderImage(*this,img, m_renBasePre, interpolator);
    }
    else
    {
        BaseGfxExtendEngineRenderer::renderImage(*this,img, m_renBaseCompPre, interpolator);
    }
}

//------------------------------------------------------------------------
struct BaseGfxExtendEngineRasterizerGamma
{

    BaseGfxExtendEngineRasterizerGamma(double alpha, double gamma) :
        m_alpha(alpha), m_gamma(gamma) {}

    double operator() (double x) const
    {
        return m_alpha(m_gamma(x));
    }
    agg::gamma_multiply m_alpha;
    agg::gamma_power    m_gamma;
};

//------------------------------------------------------------------------
void BaseGfxExtendEngine::updateRasterizerGamma()
{
    m_rasterizer.gamma(BaseGfxExtendEngineRasterizerGamma(m_masterAlpha, m_antiAliasGamma));
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::blendImage(Image& img,
                       int imgX1, int imgY1, int imgX2, int imgY2,
                       double dstX, double dstY, unsigned alpha)
{
    worldToScreen(dstX, dstY);
    PixFormat pixF(img.renBuf);
    // JME
    //agg::rect r(imgX1, imgY1, imgX2, imgY2);
    Rect r(imgX1, imgY1, imgX2, imgY2);
    if(m_blendMode == BlendAlpha)
    {
        m_renBasePre.blend_from(pixF, &r, int(dstX)-imgX1, int(dstY)-imgY1, alpha);
    }
    else
    {
        m_renBaseCompPre.blend_from(pixF, &r, int(dstX)-imgX1, int(dstY)-imgY1, alpha);
    }
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::blendImage(Image& img, double dstX, double dstY, unsigned alpha)
{
    worldToScreen(dstX, dstY);
    PixFormat pixF(img.renBuf);
    m_renBasePre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    if(m_blendMode == BlendAlpha)
    {
        m_renBasePre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    }
    else
    {
        m_renBaseCompPre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    }
}


//------------------------------------------------------------------------
void BaseGfxExtendEngine::copyImage(Image& img,
                      int imgX1, int imgY1, int imgX2, int imgY2,
                      double dstX, double dstY)
{
    worldToScreen(dstX, dstY);
    // JME
    //agg::rect r(imgX1, imgY1, imgX2, imgY2);
    Rect r(imgX1, imgY1, imgX2, imgY2);
    m_renBase.copy_from(img.renBuf, &r, int(dstX)-imgX1, int(dstY)-imgY1);
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::copyImage(Image& img, double dstX, double dstY)
{
    worldToScreen(dstX, dstY);
    m_renBase.copy_from(img.renBuf, 0, int(dstX), int(dstY));
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::Image::premultiply()
{
    PixFormat pixf(renBuf);
    pixf.premultiply();
}

//------------------------------------------------------------------------
void BaseGfxExtendEngine::Image::demultiply()
{
    PixFormat pixf(renBuf);
    pixf.demultiply();
}

}
