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

#include <gfx_utils/graphics/graphic_color/agg_color_rgba.h>
#include <gfx_utils/graphics/graphic_depict/agg_conv_curve.h>
#include <gfx_utils/graphics/graphic_depict/agg_conv_dash.h>
#include <gfx_utils/graphics/graphic_depict/agg_conv_stroke.h>
#include <gfx_utils/graphics/graphic_depict/agg_conv_transform.h>
#include <gfx_utils/graphics/graphic_filter/agg_blur.h>
#include <gfx_utils/graphics/graphic_geometry/agg_bounding_rect.h>
#include <gfx_utils/graphics/graphic_geometry/agg_path_storage.h>
#include <gfx_utils/graphics/graphic_geometry/agg_rounded_rect.h>
#include <gfx_utils/graphics/graphic_rasterizer/agg_rasterizer_scanline_aa.h>
#include <gfx_utils/graphics/graphic_scanline/agg_scanline_u.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/gradient_lut.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/span_base.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/span_gradient.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/span_image_rgba.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/span_interpolator.h>
#include <gfx_utils/graphics/graphic_spancolor_fill/span_pattern_rgba.h>
#include <gfx_utils/graphics/graphic_transform/agg_image_accessors.h>
#include <gfx_utils/graphics/graphic_transform/agg_polygon_ctrl.h>
#include <gfx_utils/graphics/graphic_transform/agg_trans_viewport.h>
#include <render/agg_pixfmt_rgba.h>
#include <render/renderer_base.h>
#include <render/renderer_scanline.h>

#include "gfx_engine_manager.h"

namespace OHOS {
    class BaseGfxExtendEngine : public BaseGfxEngine {
#ifdef BaseGfxExtendEngine_USE_FLOAT_FORMAT
        typedef OHOS::rgba32 ColorType;
#else
        typedef OHOS::rgba8 ColorType;
#endif
        typedef OHOS::order_bgra ComponentOrder;
        typedef OHOS::blender_rgba<ColorType, ComponentOrder> Blender;
        typedef OHOS::comp_op_adaptor_rgba<ColorType, ComponentOrder> BlenderComp;
        typedef OHOS::blender_rgba_pre<ColorType, ComponentOrder> BlenderPre;
        typedef OHOS::comp_op_adaptor_rgba_pre<ColorType, ComponentOrder> BlenderCompPre;
        typedef OHOS::pixfmt_alpha_blend_rgba<Blender, OHOS::rendering_buffer> PixFormat;
        typedef OHOS::pixfmt_custom_blend_rgba<BlenderComp, OHOS::rendering_buffer> PixFormatComp;
        typedef OHOS::pixfmt_alpha_blend_rgba<BlenderPre, OHOS::rendering_buffer> PixFormatPre;
        typedef OHOS::pixfmt_custom_blend_rgba<BlenderCompPre, OHOS::rendering_buffer> PixFormatCompPre;
        typedef OHOS::pixfmt_bgra32 pixfmt;
        typedef OHOS::RendererBase<PixFormat> RendererBase;
        typedef OHOS::RendererBase<PixFormatComp> RendererBaseComp;
        typedef OHOS::RendererBase<PixFormatPre> RendererBasePre;
        typedef OHOS::RendererBase<PixFormatCompPre> RendererBaseCompPre;
        typedef OHOS::RendererScanlineAntiAliasSolid<RendererBase> RendererSolid;
        typedef OHOS::RendererScanlineAntiAliasSolid<RendererBaseComp> RendererSolidComp;
        typedef OHOS::SpanAllocator<ColorType> SpanAllocator;
        typedef OHOS::pod_auto_array<ColorType, 256> GradientArray;
        typedef OHOS::GradientLut<OHOS::ColorInterpolator<OHOS::srgba8>, 1024> color_func_type;
        typedef OHOS::GradientRadialCalculate gradient_func_type;
        typedef OHOS::SpanInterpolatorLinear<> interpolator_type;
        typedef OHOS::SpanGradient<ColorType, OHOS::SpanInterpolatorLinear<>, OHOS::GradientLinearCalculate, color_func_type> LinearGradientSpan;
        typedef OHOS::SpanGradient<ColorType, OHOS::SpanInterpolatorLinear<>, gradient_func_type, color_func_type> RadialGradientSpan;
        typedef OHOS::conv_curve<OHOS::path_storage> ConvCurve;
        typedef OHOS::conv_dash<ConvCurve> ConvDashCurve;
        typedef OHOS::conv_stroke<ConvCurve> ConvStroke;
        typedef OHOS::conv_stroke<ConvDashCurve> ConvDashStroke;
        typedef OHOS::conv_transform<ConvCurve> PathTransform;
        typedef OHOS::conv_transform<ConvStroke> StrokeTransform;
        typedef OHOS::conv_transform<ConvDashStroke> DashStrokeTransform;
        typedef OHOS::shadow_ctrl<ColorType> ShadowCtrl;
        typedef OHOS::stack_blur<ColorType, OHOS::stack_blur_calc_rgba<>> StackBlur;
        typedef OHOS::recursive_blur<ColorType, OHOS::recursive_blur_calc_rgb<>> RecursiveBlur;
        typedef OHOS::rendering_buffer RenderingBuffer;
        typedef OHOS::image_accessor_wrap<pixfmt, OHOS::wrap_mode_repeat, OHOS::wrap_mode_repeat> img_source_type;
        typedef OHOS::image_accessor_repeat_x<pixfmt, OHOS::wrap_mode_repeat> img_source_type_x;
        typedef OHOS::image_accessor_repeat_y<pixfmt, OHOS::wrap_mode_repeat> img_source_type_y;
        typedef OHOS::image_accessor_norepeat<pixfmt> img_source_type_none;
        typedef OHOS::SpanPatternRgba<img_source_type> span_pattern_type_repeat;
        typedef OHOS::SpanPatternRgba<img_source_type_x> span_pattern_type_x;
        typedef OHOS::SpanPatternRgba<img_source_type_y> span_pattern_type_y;
        typedef OHOS::SpanPatternRgba<img_source_type_none> span_pattern_type_none;

        enum Gradient
        {
            Solid,
            Linear,
            Radial
        };

    public:
        friend class BaseGfxExtendEngineRenderer;

        typedef OHOS::srgba8 Color;
        typedef OHOS::rect_i Rect;
        typedef OHOS::rect_d RectD;
        typedef OHOS::trans_affine Affine;

        enum LineJoin
        {
            JoinNone = -1,
            JoinMiter = OHOS::MITER_JOIN,
            JoinRound = OHOS::ROUND_JOIN,
            JoinBevel = OHOS::BEVEL_JOIN
        };

        enum LineCap
        {
            CapNone = -1,
            CapButt = OHOS::BUTT_CAP,
            CapSquare = OHOS::SQUARE_CAP,
            CapRound = OHOS::ROUND_CAP
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

        /**
        * repeat|repeat-x|repeat-y|no-repeat
        */
        enum PatternRepeat
        {
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
            BlendNone = -1,
            BlendAlpha = OHOS::end_of_comp_op_e,
            BlendClear = OHOS::comp_op_clear,
            BlendSrc = OHOS::comp_op_src,
            BlendDst = OHOS::comp_op_dst,
            BlendSrcOver = OHOS::comp_op_src_over,
            BlendDstOver = OHOS::comp_op_dst_over,
            BlendSrcIn = OHOS::comp_op_src_in,
            BlendDstIn = OHOS::comp_op_dst_in,
            BlendSrcOut = OHOS::comp_op_src_out,
            BlendDstOut = OHOS::comp_op_dst_out,
            BlendSrcAtop = OHOS::comp_op_src_atop,
            BlendDstAtop = OHOS::comp_op_dst_atop,
            BlendXor = OHOS::comp_op_xor,
            BlendAdd = OHOS::comp_op_plus,
            BlendMultiply = OHOS::comp_op_multiply,
            BlendScreen = OHOS::comp_op_screen,
            BlendOverlay = OHOS::comp_op_overlay,
            BlendDarken = OHOS::comp_op_darken,
            BlendLighten = OHOS::comp_op_lighten,
            BlendColorDodge = OHOS::comp_op_color_dodge,
            BlendColorBurn = OHOS::comp_op_color_burn,
            BlendHardLight = OHOS::comp_op_hard_light,
            BlendSoftLight = OHOS::comp_op_soft_light,
            BlendDifference = OHOS::comp_op_difference,
            BlendExclusion = OHOS::comp_op_exclusion
        };

        enum Direction
        {
            CW,
            CCW
        };

        struct Transformations {
            double affineMatrix[6];
        };

        struct Image {
            OHOS::rendering_buffer renBuf;
            Image()
            {
            }
            Image(unsigned char* buf, unsigned width, unsigned height, int stride) :
                renBuf(buf, width, height, stride)
            {
            }
            void attach(unsigned char* buf, unsigned width, unsigned height, int stride)
            {
                renBuf.attach(buf, width, height, stride);
            }
            int width() const
            {
                return renBuf.GetWidth();
            }
            int height() const
            {
                return renBuf.GetHeight();
            }
            void premultiply();
            void demultiply();
        };

        ~BaseGfxExtendEngine();
        BaseGfxExtendEngine();
        BaseGfxExtendEngine(const BaseGfxExtendEngine& baseGfxExtendEngine);

        void attach(unsigned char* buf, unsigned width, unsigned height, int stride);
        void attach(Image& img);

        void clipBox(double x1, double y1, double x2, double y2);
        RectD clipBox() const;
        void clearAll(Color c);
        void clearAll(unsigned r, unsigned g, unsigned b, unsigned a = 255);
        void worldToScreen(double& x, double& y) const;
        void screenToWorld(double& x, double& y) const;
        double worldToScreen(double scalar) const;
        double screenToWorld(double scalar) const;
        void alignPoint(double& x, double& y) const;
        bool inBox(double worldX, double worldY) const;
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
        void add_color(double offset, Color c1);
        void build_lut();

        /**
     * @brief fillRadialGradient 根据开始圆和结束圆控制放射渐变。
     * @param start_x 开始圆圆心坐标x
     * @param start_y 开始圆圆心坐标y
     * @param start_r 开始圆半径
     * @param end_x 结束圆圆心坐标x
     * @param end_y 结束圆圆心坐标y
     * @param end_r 结束圆半径
     */
        void fillRadialGradient(double start_x, double start_y, double start_r, double end_x, double end_y, double end_r);
        void fillLinearGradient(double start_x, double start_y, double end_x, double end_y);
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
        void viewport(double worldX1, double worldY1, double worldX2, double worldY2,
                      double screenX1, double screenY1, double screenX2, double screenY2,
                      ViewportOption opt = XMidYMid);
        void line(double x1, double y1, double x2, double y2);
        void triangle(double x1, double y1, double x2, double y2, double x3, double y3);
        void rectangle(double x1, double y1, double x2, double y2);
        void rectstroke(double x1, double y1, double x2, double y2);
        void roundedRect(double x1, double y1, double x2, double y2, double r);
        void roundedRect(double x1, double y1, double x2, double y2, double rx, double ry);
        void roundedRect(double x1, double y1, double x2, double y2,
                         double rxBottom, double ryBottom,
                         double rxTop, double ryTop);
        void ellipse(double cx, double cy, double rx, double ry);
        void arc(double cx, double cy, double rx, double ry, double start, double sweep);
        void star(double cx, double cy, double r1, double r2, double startAngle, int numRays);
        void curve(double x1, double y1, double x2, double y2, double x3, double y3);
        void curve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
        void polygon(double* xy, int numPoints);
        void polyline(double* xy, int numPoints);
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
                            double xTo, double yTo);
        void quadricCurveRel(double dxCtrl, double dyCtrl,
                             double dxTo, double dyTo);
        void quadricCurveTo(double xTo, double yTo);
        void quadricCurveRel(double dxTo, double dyTo);
        void cubicCurveTo(double xCtrl1, double yCtrl1,
                          double xCtrl2, double yCtrl2,
                          double xTo, double yTo);
        void cubicCurveRel(double dxCtrl1, double dyCtrl1,
                           double dxCtrl2, double dyCtrl2,
                           double dxTo, double dyTo);
        void cubicCurveTo(double xCtrl2, double yCtrl2,
                          double xTo, double yTo);
        void cubicCurveRel(double xCtrl2, double yCtrl2,
                           double xTo, double yTo);
        void addEllipse(double cx, double cy, double rx, double ry, Direction dir);
        void closePolygon();
        void drawShadow(double x, double y, double a, double scaleX, double scaleY);
        void drawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry, double x, double y, double a, double scaleX, double scaleY);
        void rotate(double x, double y, double a);
        void scale(double x, double y, double scaleX, double scaleY);
        void drawPath(DrawPathFlag flag = FillAndStroke);
        void drawPathNoTransform(DrawPathFlag flag = FillAndStroke);
        void stroke();
        void imageFilter(ImageFilter f);
        ImageFilter imageFilter() const;
        void imageResample(ImageResample f);
        ImageResample imageResample() const;
        void transformImage(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias = true);
        void transformImage(const Image& img,
                            double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias = true);
        void transformImage(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            const double* parallelogram, bool isAntiAlias = true);
        void transformImage(const Image& img, const double* parallelogram, bool isAntiAlias = true);
        void transformImagePath(const Image& img,
                                int imgX1, int imgY1, int imgX2, int imgY2,
                                double dstX1, double dstY1, double dstX2, double dstY2);
        void transformImagePath(const Image& img,
                                double dstX1, double dstY1, double dstX2, double dstY2);
        void transformImagePath(const Image& img,
                                int imgX1, int imgY1, int imgX2, int imgY2,
                                const double* parallelogram);
        void transformImagePath(const Image& img, const double* parallelogram);
        void blendImage(Image& img,
                        int imgX1, int imgY1, int imgX2, int imgY2,
                        double dstX, double dstY, unsigned alpha = 255);
        void blendImage(Image& img, double dstX, double dstY, unsigned alpha = 255);
        void patternImageFill(Image& img, double dstX, double dstY, const char* pattternMode);
        void patternImageStroke(Image& img, double dstX, double dstY, const char* pattternMode);
        void copyImage(Image& img,
                       int imgX1, int imgY1, int imgX2, int imgY2,
                       double dstX, double dstY);
        void copyImage(Image& img, double dstX, double dstY);
        static double pi()
        {
            return OHOS::pi;
        }
        static double deg2Rad(double v)
        {
            return v * OHOS::pi / 180.0;
        }
        static double rad2Deg(double v)
        {
            return v * 180.0 / OHOS::pi;
        }

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
            if (dashArray == nullptr || ndash == 0) {
                return;
            }
            is_dash = true;
            //dDashOffset = dashOffset;
            ndashes = (ndash + 1) & ~1;
            dashes = new float[ndashes];
            if (dashes) {
                memset(dashes, 0, ndashes * sizeof(float));
                for (unsigned int i = 0; i < ndash; i++) {
                    dashes[i] = dashArray[i];
                }
            } else {
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
        OHOS::rendering_buffer GetRenderBuffer() const
        {
            return m_rbuf;
        }

        void SetShadowColor(int r, int g, int b, int a)
        {
            m_shadow_ctrl.fill_color(Color(r, g, b, a));
        }
        void SetShadowOffsetX(double x)
        {
            m_shadow_ctrl.SetOffsetX(x);
        }
        void SetShadowOffsetY(double y)
        {
            m_shadow_ctrl.SetOffsetY(y);
        }
        void SetShadowOffset(double x, double y)
        {
            m_shadow_ctrl.SetOffsetX(x);
            m_shadow_ctrl.SetOffsetY(y);
        }
        void SetShadowBlurRadius(double radius)
        {
            m_shadow_ctrl.SetRadius(radius);
            m_shadow_ctrl.SetIsBlur(true);
        }
        bool bounding_rect_single(unsigned int path_id, RectD* rect, PathTransform& path);
        void blend_from(const Image& img, Rect srcRect, Rect dstRect);
        void BlendFromImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                            double dstX, double dstY, unsigned alpha, bool isAntiAlias = false);
        void BlendFromImage(Image& img, double dstX, double dstY, unsigned alpha, bool isAntiAlias = false);

    private:
        void render(bool fillColor);
        void addLine(double x1, double y1, double x2, double y2);
        void updateRasterizerGamma();
        void renderImage(const Image& img, int x1, int y1, int x2, int y2, const double* parl, bool isAntiAlias = true);
        void ClearLineDash(void)
        {
            dDashOffset = 0;
            ndashes = 0;
            is_dash = false;
            if (dashes) {
                delete[] dashes;
                dashes = NULL;
            }
        }

        double m_masterAlpha;
        double m_antiAliasGamma;
        double m_miterLimit;
        double m_fillGradientD1;
        double m_lineGradientD1;
        double m_fillGradientD2;
        double m_lineGradientD2;
        double m_lineWidth;
        bool is_dash;
        bool m_evenOddFlag;
        float* dashes;
        float dDashOffset;
        unsigned int ndashes;

        OHOS::rendering_buffer m_rbuf;
        OHOS::scanline_u8 m_scanline;
        OHOS::rasterizer_scanline_aa<> m_rasterizer;
        OHOS::trans_affine m_fillGradientMatrix;
        OHOS::trans_affine m_lineGradientMatrix;
        OHOS::trans_affine m_fillRadialMatrix;
        OHOS::SpanInterpolatorLinear<> m_fillGradientInterpolator;
        OHOS::SpanInterpolatorLinear<> m_lineGradientInterpolator;
        OHOS::GradientLinearCalculate m_linearGradientFunction;
        OHOS::path_storage m_path;
        OHOS::trans_affine m_transform;

        PixFormat m_pixFormat;
        PixFormatComp m_pixFormatComp;
        PixFormatPre m_pixFormatPre;
        PixFormatCompPre m_pixFormatCompPre;
        RendererBase m_renBase;
        RendererBaseComp m_renBaseComp;
        RendererBasePre m_renBasePre;
        RendererBaseCompPre m_renBaseCompPre;
        RendererSolid m_renSolid;
        RendererSolidComp m_renSolidComp;
        SpanAllocator m_allocator;
        RectD m_clipBox;
        BlendMode m_blendMode;
        BlendMode m_imageBlendMode;
        Color m_imageBlendColor;
        Color m_fillColor;
        Color m_lineColor;
        GradientArray m_fillGradient;
        GradientArray m_lineGradient;
        color_func_type m_fillRadialGradient;
        LineCap m_lineCap;
        LineJoin m_lineJoin;
        Gradient m_fillGradientFlag;
        Gradient m_lineGradientFlag;
        interpolator_type m_interpolator_type;
        gradient_func_type m_radialGradientFunction;
        ConvCurve m_convCurve;
        ConvDashCurve m_convDashCurve;
        ConvStroke m_convStroke;
        ConvDashStroke m_convDashStroke;
        PathTransform m_pathTransform;
        StrokeTransform m_strokeTransform;
        DashStrokeTransform m_dashStrokeTransform;
        ShadowCtrl m_shadow_ctrl;
        StackBlur m_stack_blur;
        RecursiveBlur m_recursive_blur;
    };

    inline bool operator==(const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
    {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
    }

    inline bool operator!=(const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
    {
        return !(c1 == c2);
    }

} // namespace OHOS
#endif
