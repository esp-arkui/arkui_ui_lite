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
        typedef OHOS::pixfmt_alpha_blend_rgba<Blender, OHOS::RenderingBuffer> PixFormat;
        typedef OHOS::pixfmt_custom_blend_rgba<BlenderComp, OHOS::RenderingBuffer> PixFormatComp;
        typedef OHOS::pixfmt_alpha_blend_rgba<BlenderPre, OHOS::RenderingBuffer> PixFormatPre;
        typedef OHOS::pixfmt_custom_blend_rgba<BlenderCompPre, OHOS::RenderingBuffer> PixFormatCompPre;
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
        typedef OHOS::conv_curve<OHOS::PathStorage> ConvCurve;
        typedef OHOS::conv_dash<ConvCurve> ConvDashCurve;
        typedef OHOS::conv_stroke<ConvCurve> ConvStroke;
        typedef OHOS::conv_stroke<ConvDashCurve> ConvDashStroke;
        typedef OHOS::conv_transform<ConvCurve> PathTransform;
        typedef OHOS::conv_transform<ConvStroke> StrokeTransform;
        typedef OHOS::conv_transform<ConvDashStroke> DashStrokeTransform;
        typedef OHOS::StackBlur<ColorType, OHOS::StackBlurCalcRGBA<>> StackBlur;
        typedef OHOS::RenderingBuffer RenderingBuffer;
        typedef OHOS::ImageAccessorWrap<pixfmt, OHOS::WrapModeRepeat, OHOS::WrapModeRepeat> img_source_type;
        typedef OHOS::ImageAccessorRepeatX<pixfmt, OHOS::WrapModeRepeat> img_source_type_x;
        typedef OHOS::ImageAccessorRepeatY<pixfmt, OHOS::WrapModeRepeat> img_source_type_y;
        typedef OHOS::ImageAccessorNoRepeat<pixfmt> img_source_type_none;
        typedef OHOS::SpanPatternRgba<img_source_type> span_pattern_type_repeat;
        typedef OHOS::SpanPatternRgba<img_source_type_x> span_pattern_type_x;
        typedef OHOS::SpanPatternRgba<img_source_type_y> span_pattern_type_y;
        typedef OHOS::SpanPatternRgba<img_source_type_none> span_pattern_type_none;

    public:
        friend class BaseGfxExtendEngineRenderer;
        typedef OHOS::srgba8 Color;
        typedef OHOS::rect_i Rect;
        typedef OHOS::rect_d RectD;
        typedef OHOS::TransAffine Affine;

        /**
         * 渐变的模式
         */
        enum Gradient
        {
            /** 单色 */
            SOLID,
            /** 线性渐变 */
            LINEAR,
            /** 放射渐变 */
            RADIAL
        };

        /**
         * @brief 两条线相交时，所创建的拐角类型
         */
        enum LineJoin
        {
            JOINNONE = -1,
            /** 创建尖角 */
            JOINMITER = OHOS::MITER_JOIN,
            /** 创建圆角 */
            JOINROUND = OHOS::ROUND_JOIN,
            /** 创建斜角 */
            JOINBEVEL = OHOS::BEVEL_JOIN
        };

        /**
         * @brief 线条末端线帽的样式。
         */
        enum LineCap
        {
            CAPNONE = -1,
            /** 向线条的每个末端添加平直的边缘 */
            CAPBUTT = OHOS::BUTT_CAP,
            /** 向线条的每个末端添加正方形线帽 */
            CAPSQUARE = OHOS::SQUARE_CAP,
            /** 向线条的每个末端添加圆形线帽 */
            CAPROUND = OHOS::ROUND_CAP
        };

        /**
         * @brief 绘制的类型
         */
        enum DrawPathFlag
        {
            /** 填充路径绘制的区域 */
            FILLONLY,
            /** 绘制路径 */
            STROKEONLY,
            /** 填充路径绘制区域和绘制路径 */
            FILLANDSTROKE,
            /** 填充线条颜色 */
            FILLWITHLINECOLOR
        };

        /**
         * @brief viewPort的模式
         */
        enum ViewportOption
        {
            /** 以0.0, 0.0为基准，拉伸 */
            ANISOTROPIC,
            /** 以0.0, 0.0为基准，铺满 */
            XMINYMIN,
            /** 以0.5, 0.0为基准，铺满 */
            XMIDYMIN,
            /** 以1.0, 0.0为基准，铺满 */
            XMAXYMIN,
            /** 以0.0, 0.5为基准，铺满 */
            XMINYMID,
            /** 以0.5, 0.5为基准，铺满 */
            XMIDYMID,
            /** 以1.0, 0.5为基准，铺满 */
            XMAXYMID,
            /** 以0.0, 1.0为基准，铺满 */
            XMINYMAX,
            /** 以0.5, 1.0为基准，铺满 */
            XMIDYMAX,
            /** 以1.0, 1.0为基准，铺满 */
            XMAXYMAX
        };

        /**
         * repeat|repeat-x|repeat-y|no-repeat
         */
        enum PatternRepeat
        {
            /** 铺满，x,y轴都重复 */
            REPEAT,
            /** x轴上重复，y轴上无 */
            REPEAT_X,
            /** y轴上重复，x轴上无 */
            REPEAT_Y,
            /** 单个图片，x,y都不重复 */
            NO_REPEAT,
        };

        /**
         * 混合模式
         */
        enum BlendMode
        {
            /** 不混合 */
            BLENDNONE = -1,
            /** 混合透明度 */
            BLENDALPHA = OHOS::end_of_comp_op_e,
            BLENDCLEAR = OHOS::comp_op_clear,
            BLENDSRC = OHOS::comp_op_src,
            BLENDDST = OHOS::comp_op_dst,
            /** 默认。在目标图像上显示源图像。 */
            BLENDSRCOVER = OHOS::comp_op_src_over,
            /** 在源图像上显示目标图像。 */
            BLENDDSTOVER = OHOS::comp_op_dst_over,
            /** 在目标图像中显示源图像。只有目标图像之内的源图像部分会显示，目标图像是透明的。 */
            BLENDSRCIN = OHOS::comp_op_src_in,
            /** 在源图像中显示目标图像。只有源图像之内的目标图像部分会被显示，源图像是透明的。 */
            BLENDDSTIN = OHOS::comp_op_dst_in,
            /** 在目标图像之外显示源图像。只有目标图像之外的源图像部分会显示，目标图像是透明的。 */
            BLENDSRCOUT = OHOS::comp_op_src_out,
            /** 在源图像之外显示目标图像。只有源图像之外的目标图像部分会被显示，源图像是透明的。 */
            BLENDDSTOUT = OHOS::comp_op_dst_out,
            /** 在目标图像顶部显示源图像。源图像位于目标图像之外的部分是不可见的。 */
            BLENDSRCATOP = OHOS::comp_op_src_atop,
            /** 在源图像顶部显示目标图像。目标图像位于源图像之外的部分是不可见的。 */
            BLENDDSTATOP = OHOS::comp_op_dst_atop,
            /** 使用异或操作对源图像与目标图像进行组合。 */
            BLENDXOR = OHOS::comp_op_xor,
            BLENDADD = OHOS::comp_op_plus,
            BLENDMULTIPLY = OHOS::comp_op_multiply,
            BLENDSCREEN = OHOS::comp_op_screen,
            BLENDOVERLAY = OHOS::comp_op_overlay,
            BLENDDARKEN = OHOS::comp_op_darken,
            BLENDLIGHTEN = OHOS::comp_op_lighten,
            BLENDCOLORDODGE = OHOS::comp_op_color_dodge,
            BLENDCOLORBURN = OHOS::comp_op_color_burn,
            /**  显示源图像 + 目标图像 -硬 */
            BLENDHARDLIGHT = OHOS::comp_op_hard_light,
            /** 显示源图像 + 目标图像 -软 */
            BLENDSOFTLIGHT = OHOS::comp_op_soft_light,
            BLENDDIFFERENCE = OHOS::comp_op_difference,
            BLENDEXCLUSION = OHOS::comp_op_exclusion
        };

        struct Transformations {
            double affineMatrix[6];
        };

        /**
         * @brief 图像的结构体
         */
        struct Image {
            OHOS::RenderingBuffer renBuf;
            Image()
            {}
            /**
             * @brief 图像相关信息入参
             * @param buf 缓冲区
             * @param width 图片的宽度
             * @param height 图片的高度
             * @param stride 图片的步幅
             */
            Image(unsigned char* buf, unsigned width, unsigned height, int stride) :
                renBuf(buf, width, height, stride)
            {}

            /**
             * @brief 图像相关信息入参
             * @param buf 缓冲区
             * @param width 图片的宽度
             * @param height 图片的高度
             * @param stride 图片的步幅
             */
            void attach(unsigned char* buf, unsigned width, unsigned height, int stride)
            {
                renBuf.Attach(buf, width, height, stride);
            }
            /**
             * @brief 获取缓冲区的图片宽的
             */
            int GetWidth() const
            {
                return renBuf.GetWidth();
            }
            /**
             * @brief 获取缓冲区的高度
             */
            int GetHeight() const
            {
                return renBuf.GetHeight();
            }
        };
        ~BaseGfxExtendEngine();
        BaseGfxExtendEngine();
        BaseGfxExtendEngine(const BaseGfxExtendEngine& o);

        /**
         * @brief 初始化参数
         * @param buf 缓冲区首地址
         * @param width 缓冲区的宽度
         * @param height 缓冲区的高度
         * @param stride 缓冲区的步幅
         */
        void Attach(unsigned char* buf, unsigned width, unsigned height, int stride);
        /**
         * @brief  传入图片的结构体
         * @param img
         */
        void Attach(Image& img);
        /**
         * @brief 裁剪出矩形盒子
         * @param x1 矩形左上角的x坐标
         * @param y1 矩形左上角的y坐标
         * @param x2 矩形右下角的x坐标
         * @param y2 矩形右下角的y坐标
         */
        void ClipBox(double x1, double y1, double x2, double y2);
        /**
         * @brief 获得裁剪的矩形
         */
        RectD GetClipBox() const;

        /**
         * @brief 使用颜色color覆盖缓冲区的对应矩形的区域颜色
         */
        void ClearAll(Color color);

        /**
         * @brief 使用颜色(red,green,blue,alpha)覆盖缓冲区的对应矩形的区域颜色
         * @param red 红色
         * @param green 绿色
         * @param blue 蓝色
         * @param alpha 透明度
         */
        void ClearAll(unsigned red, unsigned green, unsigned blue, unsigned alpha = 255);

        /**
         * @brief 将画布坐标转换到屏幕坐标
         * @param x 入参为画布坐标x
         * @param y 入参为画布坐标y
         */
        void WorldToScreen(double& x, double& y) const;
        /**
         * @brief 将屏幕坐标转换为画布坐标
         * @param x 屏幕坐标x
         * @param y 屏幕坐标y
         */
        void ScreenToWorld(double& x, double& y) const;
        /**
         * @brief 将画布上的长度scalar转换成屏幕上的长度
         * @param scalar 画布上的长度
         * @return
         */
        double WorldToScreen(double scalar) const;
        /**
         * @brief 将屏幕上的长度scalar转换成画布上的长度
         * @param scalar
         * @return
         */
        double ScreenToWorld(double scalar) const;
        /**
         * @brief 设置混合的模式
         * @param bMode 混合模式
         */
        void SetBlendMode(BlendMode bMode);
        /**
         * @brief 获取混合模式
         */
        BlendMode GetBlendMode() const;
        /**
         * @brief 设置透明度
         * @param alpha
         */
        void SetMasterAlpha(double alpha);
        /**
         * @brief 获取透明度
         * @return
         */
        double GetMasterAlpha() const;
        /**
         * @brief 设置区域填充的颜色，单色模式
         * @param color 颜色
         */
        void SetFillColor(Color color);
        void SetFillColor(const OHOS::ColorType& color);
        void SetFillColor(unsigned read, unsigned green, unsigned blue, unsigned alpha = 255);
        /**
         * @brief 返回区域填充颜色
         */
        Color GetFillColor() const;

        /**
         * @brief 将填充区域设置为透明
         */
        void NoFill();
        void LineColor(Color color);
        void LineColor(const OHOS::ColorType& color);
        void LineColor(unsigned read, unsigned green, unsigned blue, unsigned alpha = 255);
        void NoLine();

        Color LineColor() const;
        void RemoveAllColor();
        void AddColor(double offset, Color c1);
        void BuildLut();

        /**
         * @brief fillRadialGradient 根据开始圆和结束圆控制放射渐变。
         * @param start_x 开始圆圆心坐标x
         * @param start_y 开始圆圆心坐标y
         * @param start_r 开始圆半径
         * @param end_x 结束圆圆心坐标x
         * @param end_y 结束圆圆心坐标y
         * @param end_r 结束圆半径
         */
        void SetRadialGradient(double start_x, double start_y, double start_r, double end_x, double end_y, double end_r);
        void SetLinearGradient(double start_x, double start_y, double end_x, double end_y);
        void SetLineWidth(double w);
        double GetLineWidth() const;
        void SetLineCap(LineCap cap);
        LineCap GetLineCap() const;
        void SetLineJoin(LineJoin join);
        LineJoin GetLineJoin() const;
        void SetMiterLimit(double limitValue);
        double GetMiterLimit() const;
        void ResetTransformations();
        void SetAffine(const Affine& tr);
        void SetAffine(const Transformations& tr);
        void Rotate(double angle);
        void Rotate(double x, double y, double a);
        void Scale(double sx, double sy);
        void Scale(double x, double y, double scaleX, double scaleY);
        void Translate(double x, double y);
        void Viewport(double worldX1, double worldY1, double worldX2, double worldY2,
                      double screenX1, double screenY1, double screenX2, double screenY2,
                      ViewportOption opt = XMIDYMID);
        void Line(double x1, double y1, double x2, double y2);
        void Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
        void Rectangle(double x1, double y1, double x2, double y2);
        void Rectstroke(double x1, double y1, double x2, double y2);
        void Ellipse(double cx, double cy, double rx, double ry);
        void Arc(double cx, double cy, double rx, double ry, double start, double sweep);
        void ResetPath();
        void MoveTo(double x, double y);
        void LineTo(double x, double y);
        void ArcTo(double rx, double ry,
                   double angle,
                   bool largeArcFlag,
                   bool sweepFlag,
                   double x, double y);
        void ClosePolygon();
        void DrawShadow(double x, double y, double a, double scaleX, double scaleY);
        void DrawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry, double x, double y, double a, double scaleX, double scaleY);
        void DrawPath(DrawPathFlag flag = FILLANDSTROKE);
        void Stroke();
        void TransformImage(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias = true);
        void TransformImage(const Image& img,
                            double dstX1, double dstY1, double dstX2, double dstY2, bool isAntiAlias = true);
        void TransformImage(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            const double* parallelogram, bool isAntiAlias = true);
        void TransformImage(const Image& img, const double* parallelogram, bool isAntiAlias = true);
        void BlendImage(Image& img,
                        int imgX1, int imgY1, int imgX2, int imgY2,
                        double dstX, double dstY, unsigned alpha = 255);
        void BlendImage(Image& img, double dstX, double dstY, unsigned alpha = 255);
        void PatternImageFill(Image& img, double dstX, double dstY, const char* pattternMode);
        void PatternImageStroke(Image& img, double dstX, double dstY, const char* pattternMode);
        static double Pi()
        {
            return OHOS::pi;
        }
        static double Deg2Rad(double v)
        {
            return v * OHOS::pi / 180.0;
        }

        void SetLineDashOffset(float dDashOffset)
        {
            this->dDashOffset = dDashOffset;
        }

        float GetLineDashOffset() const
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
        OHOS::RenderingBuffer GetRenderBuffer() const
        {
            return m_rbuf;
        }

        void SetShadowColor(int r, int g, int b, int a)
        {
            shadowColor_ = Color(r, g, b, a);
        }
        void SetShadowOffsetX(double x)
        {
            shadowOffsetX_ = x;
        }
        void SetShadowOffsetY(double y)
        {
            shadowOffsetY_ = y;
        }
        void SetShadowOffset(double x, double y)
        {
            shadowOffsetX_ = x;
            shadowOffsetY_ = y;
        }
        void SetShadowBlurRadius(double radius)
        {
            shadowBlurRadius_ = radius;
        }
        bool bounding_rect_single(unsigned int path_id, RectD* rect, PathTransform& path);
        void BlendFromImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                            double dstX, double dstY, unsigned alpha, bool isAntiAlias = false);
        void BlendFromImage(Image& img, double dstX, double dstY, unsigned alpha, bool isAntiAlias = false);

    private:
        void Render(bool fillColor);
        void AddLine(double x1, double y1, double x2, double y2);
        void UpdateRasterizerGamma();
        void RenderImage(const Image& img, int x1, int y1, int x2, int y2, const double* parl, bool isAntiAlias = true);

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
        OHOS::RenderingBuffer m_rbuf;
        OHOS::scanline_u8 m_scanline;
        OHOS::rasterizer_scanline_aa<> m_rasterizer;
        OHOS::TransAffine m_fillGradientMatrix;
        OHOS::TransAffine m_lineGradientMatrix;
        OHOS::TransAffine m_fillRadialMatrix;
        OHOS::SpanInterpolatorLinear<> m_fillGradientInterpolator;
        OHOS::SpanInterpolatorLinear<> m_lineGradientInterpolator;
        OHOS::GradientLinearCalculate m_linearGradientFunction;
        OHOS::PathStorage m_path;
        OHOS::TransAffine m_transform;
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
        StackBlur m_stack_blur;
        ColorType shadowColor_;
        double m_masterAlpha;
        double m_antiAliasGamma;
        double m_miterLimit;
        double m_fillGradientD1;
        double m_lineGradientD1;
        double m_fillGradientD2;
        double m_lineGradientD2;
        double m_lineWidth;
        double shadowOffsetY_;
        double shadowOffsetX_;
        double shadowBlurRadius_;
        bool m_evenOddFlag;
        bool is_dash;
        unsigned int ndashes;
        float* dashes;
        float dDashOffset;
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
