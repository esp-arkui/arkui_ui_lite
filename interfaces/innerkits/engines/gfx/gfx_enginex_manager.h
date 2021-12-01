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

#ifndef GFX_GRAPGICS_MANAGER_INCLUDED
#define GFX_GRAPGICS_MANAGER_INCLUDED

// With this define uncommented you can use FreeType font engine
//#define BaseGfxExtendEngine_USE_FREETYPE

// With this define uncommented you can use floating-point pixel format
//#define BaseGfxExtendEngine_USE_FLOAT_FORMAT


#include "gfx_engine_manager.h"
//#include "graphics/include/ctrl/agg_polygon_ctrl.h"

//#include <agg_rendering_buffer.h>



#include <graphic_transform/agg_image_accessors.h>
#include <graphic_transform/agg_trans_affine.h>

#include <graphic_spancolor_fill/agg_gradient_lut.h>
#include <graphic_spancolor_fill/agg_span_allocator.h>
#include <graphic_spancolor_fill/agg_span_gradient.h>
#include <graphic_spancolor_fill/agg_span_interpolator_linear.h>
#include <graphic_spancolor_fill/agg_span_pattern_rgba.h>

#include <graphic_depict/agg_conv_curve.h>
#include <graphic_depict/agg_conv_dash.h>
#include <graphic_depict/agg_conv_stroke.h>
#include <graphic_depict/agg_conv_transform.h>

#include <graphic_renderer/agg_renderer_base.h>
#include <graphic_renderer/agg_renderer_scanline.h>

#include <graphic_pixfmt/agg_pixfmt_rgba.h>
#include <graphic_geometry/agg_path_storage.h>
#include <graphic_color/agg_color_rgba.h>
#include <graphic_filter/agg_blur.h>
namespace OHOS {
class BaseGfxExtendEngine : public BaseGfxEngine
{
#ifdef BaseGfxExtendEngine_USE_FLOAT_FORMAT
    typedef agg::rgba32 ColorType;
#else
    typedef agg::rgba8  ColorType;
#endif
    typedef agg::order_bgra ComponentOrder; // Platform dependent!
    typedef agg::blender_rgba<ColorType, ComponentOrder>             Blender;
    typedef agg::comp_op_adaptor_rgba<ColorType, ComponentOrder>     BlenderComp;
    typedef agg::blender_rgba_pre<ColorType, ComponentOrder>         BlenderPre;
    typedef agg::comp_op_adaptor_rgba_pre<ColorType, ComponentOrder> BlenderCompPre;

    typedef agg::pixfmt_alpha_blend_rgba<Blender, agg::rendering_buffer>         PixFormat;
    typedef agg::pixfmt_custom_blend_rgba<BlenderComp, agg::rendering_buffer>    PixFormatComp;
    typedef agg::pixfmt_alpha_blend_rgba<BlenderPre, agg::rendering_buffer>      PixFormatPre;
    typedef agg::pixfmt_custom_blend_rgba<BlenderCompPre, agg::rendering_buffer> PixFormatCompPre;
    typedef agg::pixfmt_bgra32 pixfmt;


    typedef agg::renderer_base<PixFormat>        RendererBase;
    typedef agg::renderer_base<PixFormatComp>    RendererBaseComp;
    typedef agg::renderer_base<PixFormatPre>     RendererBasePre;
    typedef agg::renderer_base<PixFormatCompPre> RendererBaseCompPre;

    typedef agg::renderer_scanline_aa_solid<RendererBase>     RendererSolid;
    typedef agg::renderer_scanline_aa_solid<RendererBaseComp> RendererSolidComp;

    typedef agg::span_allocator<ColorType> SpanAllocator;
    typedef agg::pod_auto_array<ColorType, 256> GradientArray;
    typedef agg::gradient_lut<agg::color_interpolator<agg::srgba8>, 1024> color_func_type;

    typedef agg::gradient_radial_focus gradient_func_type;
    typedef agg::span_interpolator_linear<> interpolator_type;

    typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>, agg::gradient_x,      color_func_type> LinearGradientSpan;
//    typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>, agg::gradient_circle, GradientArray> RadialGradientSpan;
    typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>, gradient_func_type, color_func_type> RadialGradientSpan;

//  typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>,gradient_func_type,color_func_type> span_gradient_type;

    typedef agg::conv_curve<agg::path_storage>    ConvCurve;
    typedef agg::conv_dash<ConvCurve>             ConvDashCurve;
    typedef agg::conv_stroke<ConvCurve>           ConvStroke;
    typedef agg::conv_stroke<ConvDashCurve>       ConvDashStroke;
    typedef agg::conv_transform<ConvCurve>        PathTransform;
    typedef agg::conv_transform<ConvStroke>       StrokeTransform;
//    typedef agg::conv_stroke<agg::path_storage>       t_conv_stroke;

    typedef agg::conv_transform<ConvDashStroke>   DashStrokeTransform;
    typedef agg::shadow_ctrl<ColorType> ShadowCtrl;
    typedef agg::stack_blur<ColorType, agg::stack_blur_calc_rgba<>> StackBlur;
    typedef agg::recursive_blur<ColorType, agg::recursive_blur_calc_rgb<> > RecursiveBlur;
    typedef agg::rendering_buffer RenderingBuffer;

    typedef agg::ImageAccessorWrap<pixfmt, agg::WrapModeRepeat, agg::WrapModeRepeat> img_source_type;

    typedef agg::ImageAccessorRepeatX<pixfmt, agg::WrapModeRepeat> img_source_type_x;
    typedef agg::ImageAccessorRepeatY<pixfmt, agg::WrapModeRepeat> img_source_type_y;
    typedef agg::ImageAccessorNoRepeat<pixfmt> img_source_type_none;

    typedef agg::span_pattern_rgba<img_source_type> span_pattern_type_repeat;
    typedef agg::span_pattern_rgba<img_source_type_x> span_pattern_type_x;
    typedef agg::span_pattern_rgba<img_source_type_y> span_pattern_type_y;
    typedef agg::span_pattern_rgba<img_source_type_none> span_pattern_type_none;


//    typedef AGG_INT8U  int8u;
//    typedef row_accessor<int8u> rendering_buffer;


    enum Gradient
    {
        Solid,
        Linear,
        Radial
    };

public:
    friend class BaseGfxExtendEngineRenderer;

    // Use srgba8 as the "user" color type, even though the underlying color type 
    // might be something else, such as rgba32. This allows code based on 
    // 8-bit sRGB values to carry on working as before.
    typedef agg::srgba8       Color;
    typedef agg::rect_i       Rect;
    typedef agg::rect_d       RectD;
    typedef agg::trans_affine Affine;

    enum LineJoin
    {
        JoinNone  = -1,
        JoinMiter = agg::miter_join,
        JoinRound = agg::round_join,
        JoinBevel = agg::bevel_join
    };

    enum LineCap
    {
        CapNone   = -1,
        CapButt   = agg::butt_cap,
        CapSquare = agg::square_cap,
        CapRound  = agg::round_cap
    };

    enum DrawPathFlag
    {
        FillOnly,
        StrokeOnly,
        FillAndStroke,
        FillWithLineColor
    };

    enum ViewportOption
    {
        Anisotropic,
        XMinYMin,
        XMidYMin,
        XMaxYMin,
        XMinYMid,
        XMidYMid,
        XMaxYMid,
        XMinYMax,
        XMidYMax,
        XMaxYMax
    };

    struct Transformations
    {
        double affineMatrix[6];
    };


    struct Image
    {
        agg::rendering_buffer renBuf;
        Image() {}
        Image(unsigned char* buf, unsigned width, unsigned height, int stride) :
            renBuf(buf, width, height, stride) {}
        void attach(unsigned char* buf, unsigned width, unsigned height, int stride)
        {
            renBuf.attach(buf, width, height, stride);
        }
        int width()  const { return renBuf.width(); }
        int height() const { return renBuf.height(); }
        void premultiply();
        void demultiply();
    };

    /**
     * repeat|repeat-x|repeat-y|no-repeat
     */
    enum PatternRepeat {
        REPEAT,
        REPEAT_X,
        REPEAT_Y,
        NO_REPEAT,
    };

    enum ImageFilter
    {
        NoFilter,
        Bilinear,
        Hanning,
        Hermite,
        Quadric,
        Bicubic,
        Catrom,
        Spline16,
        Spline36,
        Blackman144
    };

    enum ImageResample
    {
        NoResample,
        ResampleAlways,
        ResampleOnZoomOut
    };

    enum BlendMode
    {
        BlendNone       =-1,
        BlendAlpha      = agg::end_of_comp_op_e,
        BlendClear      = agg::comp_op_clear,
        BlendSrc        = agg::comp_op_src,
        BlendDst        = agg::comp_op_dst,
        BlendSrcOver    = agg::comp_op_src_over,
        BlendDstOver    = agg::comp_op_dst_over,
        BlendSrcIn      = agg::comp_op_src_in,
        BlendDstIn      = agg::comp_op_dst_in,
        BlendSrcOut     = agg::comp_op_src_out,
        BlendDstOut     = agg::comp_op_dst_out,
        BlendSrcAtop    = agg::comp_op_src_atop,
        BlendDstAtop    = agg::comp_op_dst_atop,
        BlendXor        = agg::comp_op_xor,
        BlendAdd        = agg::comp_op_plus,
        BlendMultiply   = agg::comp_op_multiply,
        BlendScreen     = agg::comp_op_screen,
        BlendOverlay    = agg::comp_op_overlay,
        BlendDarken     = agg::comp_op_darken,
        BlendLighten    = agg::comp_op_lighten,
        BlendColorDodge = agg::comp_op_color_dodge,
        BlendColorBurn  = agg::comp_op_color_burn,
        BlendHardLight  = agg::comp_op_hard_light,
        BlendSoftLight  = agg::comp_op_soft_light,
        BlendDifference = agg::comp_op_difference,
        BlendExclusion  = agg::comp_op_exclusion
    };

    enum Direction
    {
        CW, CCW
    };

    ~BaseGfxExtendEngine();
    BaseGfxExtendEngine();
    BaseGfxExtendEngine(const BaseGfxExtendEngine& o);
    // Setup
    //-----------------------
    void  attach(unsigned char* buf, unsigned width, unsigned height, int stride);
    void  attach(Image& img);

    void  clipBox(double x1, double y1, double x2, double y2);
    RectD clipBox() const;

    void  clearAll(Color c);
    void  clearAll(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    void  clearClipBox(Color c);
    void  clearClipBox(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    // Conversions
    //-----------------------
    void   worldToScreen(double& x, double& y) const;
    void   screenToWorld(double& x, double& y) const;
    double worldToScreen(double scalar) const;
    double screenToWorld(double scalar) const;
    void   alignPoint(double& x, double& y) const;
    bool   inBox(double worldX, double worldY) const;

    // General Attributes
    //-----------------------
    void blendMode(BlendMode m);
    BlendMode blendMode() const;

    void imageBlendMode(BlendMode m);
    BlendMode imageBlendMode() const;

    void imageBlendColor(Color c);
    void imageBlendColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    Color imageBlendColor() const;

    void masterAlpha(double a);
    double masterAlpha() const;

    void antiAliasGamma(double g);
    double antiAliasGamma() const;

    void fillColor(Color c);
    void fillColor(const OHOS::ColorType& c);
    void fillColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noFill();

    void lineColor(Color c);
    void lineColor(const OHOS::ColorType& c);
    void lineColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noLine();

    Color fillColor() const;
    Color lineColor() const;

    void remove_all_color();
    void add_color(double offset,  Color c1);
    void build_lut();

    void fillGradientAndStop(Color c1, Color c2, double startscal=0.0,double endscal=1.0);
    void fillLinearGradientAndStop(double x1, double y1, double x2, double y2);


    /**
     * @brief fillRadialGradient 根据开始圆和结束圆控制放射渐变。
     * @param start_x 开始圆圆心坐标x
     * @param start_y 开始圆圆心坐标y
     * @param start_r 开始圆半径
     * @param end_x 结束圆圆心坐标x
     * @param end_y 结束圆圆心坐标y
     * @param end_r 结束圆半径
     */
    void fillRadialGradient(double start_x, double start_y,double start_r, double end_x, double end_y,double end_r);

    void fillLinearGradient(double start_x, double start_y,double end_x, double end_y);

    void fillRadialGradient(double x, double y, double r, Color c1, Color c2, Color c3);


    void lineWidth(double w);
    double lineWidth() const;

    void lineCap(LineCap cap);
    LineCap lineCap() const;

    void lineJoin(LineJoin join);
    LineJoin lineJoin() const;
    void MiterLimit(double limitValue);
    double MiterLimit() const;

    void fillEvenOdd(bool evenOddFlag);
    bool fillEvenOdd() const;

    // Transformations-
    //-----------------------
    Transformations transformations() const;
    void transformations(const Transformations& tr);
    void resetTransformations();
    void affine(const Affine& tr);
    void affine(const Transformations& tr);
    void rotate(double angle);
    void scale(double sx, double sy);
    void skew(double sx, double sy);
    void translate(double x, double y);
    void parallelogram(double x1, double y1, double x2, double y2, const double* para);
    void viewport(double worldX1,  double worldY1,  double worldX2,  double worldY2,
                  double screenX1, double screenY1, double screenX2, double screenY2,
                  ViewportOption opt=XMidYMid);

    // Basic Shapes
    //-----------------------
    void line(double x1, double y1, double x2, double y2);
    void triangle(double x1, double y1, double x2, double y2, double x3, double y3);
    void rectangle(double x1, double y1, double x2, double y2);
    void rectstroke(double x1, double y1, double x2, double y2);
    void roundedRect(double x1, double y1, double x2, double y2, double r);
    void roundedRect(double x1, double y1, double x2, double y2, double rx, double ry);
    void roundedRect(double x1, double y1, double x2, double y2,
                     double rxBottom, double ryBottom,
                     double rxTop,    double ryTop);
    void ellipse(double cx, double cy, double rx, double ry);
    void arc(double cx, double cy, double rx, double ry, double start, double sweep);
    void star(double cx, double cy, double r1, double r2, double startAngle, int numRays);
    void curve(double x1, double y1, double x2, double y2, double x3, double y3);
    void curve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    void polygon(double* xy, int numPoints);
    void polyline(double* xy, int numPoints);

    // Path commands
    //-----------------------
    void resetPath();

    void moveTo(double x, double y);
    void moveRel(double dx, double dy);

    void lineTo(double x, double y);
    void lineRel(double dx, double dy);

    void horLineTo(double x);
    void horLineRel(double dx);

    void verLineTo(double y);
    void verLineRel(double dy);

    void arcTo(double rx, double ry,
               double angle,
               bool largeArcFlag,
               bool sweepFlag,
               double x, double y);

    void arcRel(double rx, double ry,
                double angle,
                bool largeArcFlag,
                bool sweepFlag,
                double dx, double dy);

    void quadricCurveTo(double xCtrl, double yCtrl,
                         double xTo,   double yTo);
    void quadricCurveRel(double dxCtrl, double dyCtrl,
                         double dxTo,   double dyTo);
    void quadricCurveTo(double xTo, double yTo);
    void quadricCurveRel(double dxTo, double dyTo);

    void cubicCurveTo(double xCtrl1, double yCtrl1,
                      double xCtrl2, double yCtrl2,
                      double xTo,    double yTo);

    void cubicCurveRel(double dxCtrl1, double dyCtrl1,
                       double dxCtrl2, double dyCtrl2,
                       double dxTo,    double dyTo);

    void cubicCurveTo(double xCtrl2, double yCtrl2,
                      double xTo,    double yTo);

    void cubicCurveRel(double xCtrl2, double yCtrl2,
                       double xTo,    double yTo);

    void addEllipse(double cx, double cy, double rx, double ry, Direction dir);
    void closePolygon();
    void drawShadow(double x, double y, double a,double scaleX, double scaleY);
    void drawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry,double x, double y, double a,double scaleX, double scaleY);
    void rotate(double x, double y, double a);
    void scale(double x, double y,double scaleX, double scaleY);
    void drawPath(DrawPathFlag flag = FillAndStroke);
    void drawPathNoTransform(DrawPathFlag flag = FillAndStroke);


    void stroke();

    // Image Transformations
    //-----------------------
    void imageFilter(ImageFilter f);
    ImageFilter imageFilter() const;

    void imageResample(ImageResample f);
    ImageResample imageResample() const;

//    void transformImage(const Image& img,
//                           int imgX1,    int imgY1,    int imgX2,    int imgY2,
//                        double dstX1, double dstY1, double dstX2, double dstY2);

//    void transformImage(const Image& img,
//                        double dstX1, double dstY1, double dstX2, double dstY2);

//    void transformImage(const Image& img,
//                        int imgX1, int imgY1, int imgX2, int imgY2,
//                        const double* parallelogram);

//    void transformImage(const Image& img, const double* parallelogram);

    void transformImage(const Image& img,
                               int imgX1,    int imgY1,    int imgX2,    int imgY2,
                            double dstX1, double dstY1, double dstX2, double dstY2,bool isAntiAlias=true);

    void transformImage(const Image& img,
                        double dstX1, double dstY1, double dstX2, double dstY2,bool isAntiAlias=true);

    void transformImage(const Image& img,
                        int imgX1, int imgY1, int imgX2, int imgY2,
                        const double* parallelogram,bool isAntiAlias=true);

    void transformImage(const Image& img, const double* parallelogram,bool isAntiAlias=true);


    void transformImagePath(const Image& img,
                               int imgX1,    int imgY1,    int imgX2,    int imgY2,
                            double dstX1, double dstY1, double dstX2, double dstY2);

    void transformImagePath(const Image& img,
                            double dstX1, double dstY1, double dstX2, double dstY2);

    void transformImagePath(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            const double* parallelogram);

    void transformImagePath(const Image& img, const double* parallelogram);


    // Image Blending (no transformations available)
    void blendImage(Image& img,
                    int imgX1, int imgY1, int imgX2, int imgY2,
                    double dstX, double dstY, unsigned alpha=255);
    void blendImage(Image& img, double dstX, double dstY, unsigned alpha=255);
    void patternImageFill(Image& img, double dstX, double dstY,const char* pattternMode);
    void patternImageStroke(Image& img, double dstX, double dstY,const char* pattternMode);

    // Copy image directly, together with alpha-channel
    void copyImage(Image& img,
                   int imgX1, int imgY1, int imgX2, int imgY2,
                   double dstX, double dstY);
    void copyImage(Image& img, double dstX, double dstY);


    // Auxiliary
    //-----------------------
    static double pi() { return agg::pi; }
    static double deg2Rad(double v) { return v * agg::pi / 180.0; }
    static double rad2Deg(double v) { return v * 180.0 / agg::pi; }


    void lineDashOffset(float dDashOffset)
    {
        this->dDashOffset = dDashOffset;
    }

    float lineDashOffset() const
    {
        return dDashOffset;
    }

    void SetLineDash(const float* dashArray, unsigned int ndash)
    {
        ClearLineDash();
        if(dashArray == nullptr || ndash == 0) {
            return;
        }
        is_dash = true;
        //dDashOffset = dashOffset;
        ndashes = (ndash+1)&~1;
        dashes = new float[ndashes];
        if (dashes) {
            memset(dashes, 0, ndashes * sizeof(float));
            for (unsigned int i = 0; i < ndash; i++) {
                dashes[i] = dashArray[i];
            }
        } else {
            //memory alloc error, ignore this dash
            //I think it is never happen.
            ndashes = 0;
            dDashOffset = 0;
            is_dash = false;
        }
    }

    bool IsLineDash() const
    {
        return is_dash;
    }

    float* GetLineDash() const
    {
        return dashes;
    }

    unsigned int GetLineDashCount() const
    {
        return ndashes;
    }
    agg::rendering_buffer GetRenderBuffer() const
    {
        return m_rbuf;
    }

    void SetShadowColor(int r, int g, int b, int a){
        m_shadow_ctrl.fill_color(Color(r, g, b, a));

    }
    void SetShadowOffsetX(double x){
        m_shadow_ctrl.SetOffsetX(x);

    }
    void SetShadowOffsetY(double y){
        m_shadow_ctrl.SetOffsetY(y);

    }
    void SetShadowOffset(double x,double y){
        m_shadow_ctrl.SetOffsetX(x);
        m_shadow_ctrl.SetOffsetY(y);

    }
    void SetShadowBlurRadius(double radius){
        m_shadow_ctrl.SetRadius(radius);
        m_shadow_ctrl.SetIsBlur(true);
    }
    bool bounding_rect_single(unsigned int path_id,RectD* rect ,PathTransform &path);

    void blend_from(const Image& img, Rect srcRect,Rect dstRect);

    void BlendFromImage(Image &img, int imgX1, int imgY1, int imgX2, int imgY2,
                            double dstX, double dstY, unsigned alpha,bool isAntiAlias=false);
    void BlendFromImage(Image &img, double dstX, double dstY, unsigned alpha,bool isAntiAlias=false);

private:
    void render(bool fillColor);
    void addLine(double x1, double y1, double x2, double y2);
    void updateRasterizerGamma();
    void renderImage(const Image& img, int x1, int y1, int x2, int y2, const double* parl,bool isAntiAlias=true);

    void ClearLineDash(void)
    {
        dDashOffset = 0;
        ndashes = 0;
        is_dash = false;
        if (dashes) {
            delete [] dashes;
            dashes = NULL;
        }
    }
    agg::rendering_buffer           m_rbuf;
    PixFormat                       m_pixFormat;
    PixFormatComp                   m_pixFormatComp;
    PixFormatPre                    m_pixFormatPre;
    PixFormatCompPre                m_pixFormatCompPre;
    RendererBase                    m_renBase;
    RendererBaseComp                m_renBaseComp;
    RendererBasePre                 m_renBasePre;
    RendererBaseCompPre             m_renBaseCompPre;
    RendererSolid                   m_renSolid;
    RendererSolidComp               m_renSolidComp;

    SpanAllocator                   m_allocator;
    RectD                           m_clipBox;

    BlendMode                       m_blendMode;
    BlendMode                       m_imageBlendMode;
    Color                           m_imageBlendColor;

    agg::scanline_u8                m_scanline;
    agg::rasterizer_scanline_aa<>   m_rasterizer;

    double                          m_masterAlpha;
    double                          m_antiAliasGamma;

    Color                           m_fillColor;
    Color                           m_lineColor;
    GradientArray                   m_fillGradient;
    GradientArray                   m_lineGradient;

    color_func_type                 m_fillRadialGradient;

    LineCap                         m_lineCap;
    LineJoin                        m_lineJoin;
    double                          m_miterLimit;
    Gradient                        m_fillGradientFlag;
    Gradient                        m_lineGradientFlag;
    agg::trans_affine               m_fillGradientMatrix;
    agg::trans_affine               m_lineGradientMatrix;
    agg::trans_affine               m_fillRadialMatrix;


    interpolator_type               m_interpolator_type  ;


    double                          m_fillGradientD1;
    double                          m_lineGradientD1;
    double                          m_fillGradientD2;
    double                          m_lineGradientD2;

    ImageFilter                     m_imageFilter;
    ImageResample                   m_imageResample;
    agg::image_filter_lut           m_imageFilterLut;

    agg::span_interpolator_linear<> m_fillGradientInterpolator;
    agg::span_interpolator_linear<> m_lineGradientInterpolator;

    agg::gradient_x                 m_linearGradientFunction;
//    agg::gradient_circle            m_radialGradientFunction;
    gradient_func_type              m_radialGradientFunction;//TODO：m_fillRadialMatrix

    double                          m_lineWidth;
    bool                            m_evenOddFlag;

    agg::path_storage               m_path;
//    typedef agg::conv_stroke<agg::path_storage> cs_ps(m_path);
    agg::trans_affine               m_transform;

    ConvCurve                       m_convCurve;
    ConvDashCurve                   m_convDashCurve;
    ConvStroke                      m_convStroke;
    ConvDashStroke                  m_convDashStroke;
    PathTransform                   m_pathTransform;
    StrokeTransform                 m_strokeTransform;
    DashStrokeTransform             m_dashStrokeTransform;
//    t_conv_stroke m_conv_stroke;
    ShadowCtrl m_shadow_ctrl;
    StackBlur m_stack_blur;
    RecursiveBlur m_recursive_blur;
    //dash
    bool is_dash;
    float* dashes;
    unsigned int ndashes;
    float dDashOffset;


};

inline bool operator == (const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
{
   return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

inline bool operator != (const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
{
   return !(c1 == c2);
}

}
#endif


