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
#    define GFX_GRAPGICS_MANAGER_INCLUDED

#    include <gfx_utils/graphics/graphic_depict/graphic_depict_curve.h>
#    include <gfx_utils/graphics/graphic_depict/graphic_depict_dash.h>
#    include <gfx_utils/graphics/graphic_depict/graphic_depict_stroke.h>
#    include <gfx_utils/graphics/graphic_depict/graphic_depict_transform.h>
#    include <gfx_utils/graphics/graphic_filter/graphic_filter_blur.h>
#    include <gfx_utils/graphics/graphic_geometry/graphic_geometry_bounding_rect.h>
#    include <gfx_utils/graphics/graphic_geometry/graphic_geometry_path_storage.h>
#    include <gfx_utils/graphics/graphic_geometry/graphic_geometry_rounded_rect.h>
#    include <gfx_utils/graphics/graphic_rasterizer/graphic_rasterizer_scanline_antialias.h>
#    include <gfx_utils/graphics/graphic_scanline/graphic_geometry_scanline.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_base.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_gradient.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_gradient_lut.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_image_rgba.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_interpolator.h>
#    include <gfx_utils/graphics/graphic_spancolor_fill/graphic_spancolor_fill_pattern_rgba.h>
#    include <gfx_utils/graphics/graphic_transform/graphic_transform_image_accessors.h>
#    include <gfx_utils/graphics/graphic_transform/graphic_transform_viewport.h>
#    include <render/graphic_render_base.h>
#    include <render/graphic_render_pixfmt_rgba.h>
#    include <render/graphic_render_scanline.h>

#    include "gfx_engine_manager.h"
#    include "gfx_utils/color.h"

namespace OHOS {
    class BaseGfxExtendEngine : public BaseGfxEngine {
#    ifdef GRAPHIC_GEOMETYR_ENABLE_FLOAT_FORMAT
        typedef OHOS::rgba32 ColorType;
#    else
        typedef OHOS::Rgba8 ColorType;
#    endif
        // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
        typedef OHOS::OrderBgra ComponentOrder;
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
        typedef OHOS::RgbaBlender<ColorType, ComponentOrder> Blender;
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据comp_op_adaptor_rgba模式处理颜色
        typedef OHOS::CompOpAdaptorRgba<ColorType, ComponentOrder> BlenderComp;
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba_pre模式处理颜色
        typedef OHOS::RgbaPrelerpBlender<ColorType, ComponentOrder> BlenderPre;
        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据comp_op_adaptor_rgba_pre模式处理颜色
        typedef OHOS::CompOpAdaptorRgbaPre<ColorType, ComponentOrder> BlenderCompPre;
        // 根据pixfmt_alpha_blend_rgba的像素处理模式处理RenderingBuffer对应的缓冲区
        typedef OHOS::PixfmtAlphaBlendRgba<Blender, OHOS::RenderingBuffer> PixFormat;
        typedef OHOS::PixfmtCustomBlendRgba<BlenderComp, OHOS::RenderingBuffer> PixFormatComp; //
        typedef OHOS::PixfmtAlphaBlendRgba<BlenderPre, OHOS::RenderingBuffer> PixFormatPre;
        typedef OHOS::PixfmtCustomBlendRgba<BlenderCompPre, OHOS::RenderingBuffer> PixFormatCompPre; //
        typedef OHOS::PixfmtBgra32 pixfmt;

        // 根据像素处理的模板处理基础渲染器
        typedef OHOS::RendererBase<PixFormat> RendererBase;
        typedef OHOS::RendererBase<PixFormatComp> RendererBaseComp;
        typedef OHOS::RendererBase<PixFormatPre> RendererBasePre;
        typedef OHOS::RendererBase<PixFormatCompPre> RendererBaseCompPre;

        typedef OHOS::RendererScanlineAntiAliasSolid<RendererBase> RendererSolid;
        typedef OHOS::RendererScanlineAntiAliasSolid<RendererBaseComp> RendererSolidComp;
        // 设定线段分配器
        typedef OHOS::SpanFillColorAllocator<ColorType> SpanAllocator;
        // 设定渐变数组的构造器设定颜色插值器和颜色模板等
        typedef OHOS::GradientColorCalibration<OHOS::ColorInterpolator<OHOS::Srgba8>, 1024> color_func_type;
        // 设定放射渐变的算法
        typedef OHOS::GradientRadialCalculate gradient_func_type;
        // 设定线段插值器
        typedef OHOS::SpanInterpolatorLinear<> interpolator_type;
        // 设定线性渐变的线段生成器
        typedef OHOS::SpanFillColorGradient<ColorType, OHOS::SpanInterpolatorLinear<>, OHOS::GradientLinearCalculate,
                                            color_func_type>
            LinearGradientSpan;
        // 设定放射渐变的线段生成器
        typedef OHOS::SpanFillColorGradient<ColorType, OHOS::SpanInterpolatorLinear<>, gradient_func_type, color_func_type>
            RadialGradientSpan;

        typedef OHOS::DepictCurve<OHOS::PathStorage> ConvCurve;
        typedef OHOS::DepictStroke<ConvCurve> ConvStroke;
        typedef OHOS::DepictDash<ConvCurve> ConvDashCurve;
        typedef OHOS::DepictTransform<ConvCurve> PathTransform;
        typedef OHOS::DepictStroke<ConvDashCurve> ConvDashStroke;
        typedef OHOS::DepictTransform<ConvStroke> StrokeTransform;
        typedef OHOS::DepictTransform<ConvDashStroke> DashStrokeTransform;
        typedef OHOS::StackBlur<ColorType, OHOS::StackBlurCalcRGBA<>> StackBlur;
        // 渲染器缓冲区
        typedef OHOS::RenderingBuffer RenderingBuffer;
        // 设定图像观察器的模式为Wrap设定X,Y轴上WrapModeRepeat模式，即X,Y上都重复图片
        typedef OHOS::ImageAccessorWrap<pixfmt, OHOS::WrapModeRepeat, OHOS::WrapModeRepeat> imgSourceTypeRepeat;
        // 设定图像观察器的模式为RepeatX设定X轴上WrapModeRepeat模式，即X上都重复图片
        typedef OHOS::ImageAccessorRepeatX<pixfmt, OHOS::WrapModeRepeat> imgSourceTypeRepeatX;
        // 设定图像观察器的模式为RepeatY设定Y轴上WrapModeRepeat模式，即Y上都重复图片
        typedef OHOS::ImageAccessorRepeatY<pixfmt, OHOS::WrapModeRepeat> imgSourceTypeRepeatY;
        // 设定图像观察器的模式为NoRepeat即X,Y轴上都不重复，只有一张原本的图片
        typedef OHOS::ImageAccessorNoRepeat<pixfmt> imgSourceTypeNoRepeat;
        // 通过线段生成器SpanPatternRgba设定相应的图像观察器对应的模式生成相应线段
        //  x,y轴都重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeat> spanPatternTypeRepeat;
        //  x轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatX> spanPatternTypeRepeatX;
        //  y轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatY> spanPatternTypeRepeatY;
        // 不重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeNoRepeat> spanPatternTypeNoRepeat;

    public:
        friend class BaseGfxExtendEngineRenderer;
        typedef OHOS::Srgba8 Color;
        typedef OHOS::RectI Rect;
        typedef OHOS::RectD RectD;
        typedef OHOS::TransAffine Affine;

        /**
         * 渐变的模式
         */
        enum Gradient {
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
        enum LineJoin {
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
        enum LineCap {
            CAPNONE = -1,
            /** 向线条的每个末端添加平直的边缘 */
            CAPBUTT = OHOS::BUTT_CAP,
            /** 向线条的每个末端添加正方形线帽 */
            CAPSQUARE = OHOS::SQUARE_CAP,
            /** 向线条的每个末端添加圆形线帽 */
            CAPROUND = OHOS::ROUND_CAP
        };
#endif
        /**
         * @brief 绘制的类型
         */
        enum DrawPathFlag {
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
        enum ViewportOption {
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
        enum PatternRepeat {
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
        enum BlendMode {
            /** 不混合 */
            BLENDNONE = -1,
            /** 混合透明度 */
            BLENDALPHA = OHOS::END_OF_COMP_OP_E,
            BLENDSRC = OHOS::COMP_OP_SRC,
            BLENDDST = OHOS::COMP_OP_DST,
            /** 默认。在目标图像上显示源图像。 */
            BLENDSRCOVER = OHOS::COMP_OP_SRC_OVER,
            /** 在源图像上显示目标图像。 */
            BLENDDSTOVER = OHOS::COMP_OP_DST_OVER,
            /** 在目标图像中显示源图像。只有目标图像之内的源图像部分会显示，目标图像是透明的。 */
            BLENDSRCIN = OHOS::COMP_OP_SRC_IN,
            /** 在源图像中显示目标图像。只有源图像之内的目标图像部分会被显示，源图像是透明的。 */
            BLENDDSTIN = OHOS::COMP_OP_DST_IN,
            /** 在目标图像之外显示源图像。只有目标图像之外的源图像部分会显示，目标图像是透明的。 */
            BLENDSRCOUT = OHOS::COMP_OP_SRC_OUT,
            /** 在源图像之外显示目标图像。只有源图像之外的目标图像部分会被显示，源图像是透明的。 */
            BLENDDSTOUT = OHOS::COMP_OP_DST_OUT,
            /** 在目标图像顶部显示源图像。源图像位于目标图像之外的部分是不可见的。 */
            BLENDSRCATOP = OHOS::COMP_OP_SRC_ATOP,
            /** 在源图像顶部显示目标图像。目标图像位于源图像之外的部分是不可见的。 */
            BLENDDSTATOP = OHOS::COMP_OP_DST_ATOP,
            /** 使用异或操作对源图像与目标图像进行组合。 */
            BLENDXOR = OHOS::COMP_OP_XOR,
            /** 显示源图像 + 目标图像。 */
            BLENDLIGHTEN = OHOS::COMP_OP_LIGHTEN,
            BLENDCOPY = 1000
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
        virtual ~BaseGfxExtendEngine();
        BaseGfxExtendEngine();
        BaseGfxExtendEngine(const BaseGfxExtendEngine& baseGfxExtendEngine);

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
        /**
         * @brief 设置线条颜色
         * @param color
         */
        void SetLineColor(Color color);
        void SetLineColor(const OHOS::ColorType& color);
        void SetLineColor(unsigned read, unsigned green, unsigned blue, unsigned alpha = 255);
        /**
         * @brief 返回线条颜色
         */
        Color GetLineColor() const;
        /**
         * @brief 将线条颜色设置为透明，即不显示线条
         */
        void NoLine();

        /**
         * @brief 颜色渐变，删除所有颜色
         */
        void RemoveAllColor();
        /**
         * @brief AddColor 颜色渐变，添加渐变过程颜色
         * @param offset （0-1）
         * @param color  offset所在位置的颜色
         */
        void AddColor(double offset, Color color);

        /**
         * @brief 根据渐变颜色构建color_type数组
         * 数组长度0-255
         * 数组内容根据渐变颜色分布在数组上
         */
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
        void SetRadialGradient(double start_x, double start_y, double start_r, double end_x, double end_y,
                               double end_r);
        /**
         * @brief fillRadialGradient 根据线性起点和终点控制线性渐变
         * @param start_x 线性起点坐标x
         * @param start_y 线性起点坐标y
         * @param end_x 线性终点坐标x
         * @param end_y 线性终点坐标y
         */
        void SetLinearGradient(double start_x, double start_y, double end_x, double end_y);
        /**
         * @brief 设置线条宽度
         */
        void SetLineWidth(double w);
        /**
         * @brief 获取线条宽度
         * @return
         */
        double GetLineWidth() const;
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        /**
         * @brief 设置线条末端线帽样式
         * @param cap 具体线帽样式
         */
        void SetLineCap(LineCap cap);
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        /**
         * @brief 设置交点拐角类型
         */
        void SetLineJoin(LineJoin join);
        void SetMiterLimit(double limitValue);
#endif
        /**
         * @brief 获取线条末端线帽样式
         */
        LineCap GetLineCap() const;
        /**
        * @brief 返回线条拐点的样式
        */
        LineJoin GetLineJoin() const;
        /**
         * @brief 返回最大斜接长度
         */
        double GetMiterLimit() const;
        /**
        * @brief 设置最大斜接长度
        */
        /**
         * @brief 重置转换
         */
        void ResetTransformations();

        void SetAffine(const Affine& tr);
        void SetAffine(const Transformations& tr);

        /**
         * @brief Rotate 以（0,0）为基准旋转一定范围内矩形
         * @param angle 旋转角度
         */
        void Rotate(double angle);
        /**
         * @brief Rotate 以（x,y）为基准旋转一定范围内矩形
         * @param x
         * @param y
         * @param angle 旋转角度
         */
        void Rotate(double x, double y, double angle);

        void Scale(double sx, double sy);
        void Scale(double x, double y, double scaleX, double scaleY);

        /**
         * @brief Translate 将坐标转换为
         * @param x
         * @param y
         */
        void Translate(double x, double y);

        /**
         * @brief Viewport 将画布映射到屏幕上
         * @param worldX1 画布左上角坐标x
         * @param worldY1 画布左上角坐标y
         * @param worldX2 画布右下角角坐标x
         * @param worldY2 画布右下角坐标y
         * @param screenX1 屏幕左上角坐标x
         * @param screenY1 屏幕左上角坐标y
         * @param screenX2 屏幕右下角角坐标x
         * @param screenY2 屏幕右下角坐标y
         * @param opt
         */
        void Viewport(double worldX1, double worldY1, double worldX2, double worldY2, double screenX1, double screenY1,
                      double screenX2, double screenY2, ViewportOption opt = XMIDYMID);
        /**
         * @brief 设置线条的起止位置
         * @param x1 起点x
         * @param y1 起点y
         * @param x2 终点x
         * @param y2 终点y
         */
        void Line(double x1, double y1, double x2, double y2);
        /**
         * @brief Triangle 绘制三角形
         * @param x1 第一个点x坐标
         * @param y1 第一个点y坐标
         * @param x2 第二个点x坐标
         * @param y2 第二个点y坐标
         * @param x3 第三个点x坐标
         * @param y3 第三个点y坐标
         */
        void Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
        /**
         * @brief 绘制矩形
         * @param x1 矩形左上角x
         * @param y1 矩形左上角y
         * @param x2 矩形右下角x
         * @param y2 矩形右下角y
         */
        void Rectangle(double x1, double y1, double x2, double y2);

        /**
         * @brief Rectstroke 绘制矩形的路径
         * @param x1 矩形左上角x
         * @param y1 矩形左上角y
         * @param x2 矩形右下角x
         * @param y2 矩形右下角y
         */
        void Rectstroke(double x1, double y1, double x2, double y2);
        /**
         * @brief Round 绘制圆
         * @param cx 圆心x坐标
         * @param cy 圆心y坐标
         * @param radius 半径
         */
        void Round(double cx, double cy, double radius);
        /**
         * @brief 重置路径
         */
        void ResetPath();
        /**
         * @brief 起点
         * @param x 起点坐标x
         * @param y 起点坐标y
         */
        void MoveTo(double x, double y);
        /**
         * @brief 直线连接上一个点到当前点。
         * @param x 当前点坐标x
         * @param y 当前点坐标y
         */
        void LineTo(double x, double y);
        /**
         * @brief 弧线连接上一个点到当前点。
         * @param rx 长短半轴
         * @param ry 长短半轴
         * @param angle 角度
         * @param largeArcFlag 优劣弧
         * @param sweepFlag 顺逆时针
         * @param x 终点x轴坐标
         * @param y 终点y轴坐标
         */
        void ArcTo(double rx, double ry, double angle, bool largeArcFlag, bool sweepFlag, double x, double y);
        /**
         * @brief 关闭多边形路径
         */
        void ClosePolygon();

        /**
         * @brief 绘制阴影
         * @param x
         * @param y
         * @param a
         * @param scaleX
         * @param scaleY
         */
        void DrawShadow(double x, double y, double angle, double scaleX, double scaleY, double transLateX = 0, double transLateY = 0);
        void DrawShadow(int16_t cx, int16_t cy, int16_t rx, int16_t ry, double x, double y, double angle, double scaleX,
                        double scaleY, double transLateX = 0, double transLateY = 0);

        /**
         * @brief 根据路径和flag确认绘制路径还是填充路径内区域，或者两者兼备
         * @param flag 绘制模式 默认填充和绘制路径都有
         */
        void DrawPath(DrawPathFlag flag = FILLANDSTROKE);
        /**
         * @brief 绘制路径
         */
        void Stroke();

        /**
         * @brief 变换图像
         * @param img 图像
         * @param imgX1 图像左上角x
         * @param imgY1 图像左上角y
         * @param imgX2 图像右下角x
         * @param imgY2 图像右下角y
         * @param dstX1 合法区域的左上角x
         * @param dstY1 合法区域的左上角y
         * @param dstX2 合法区域的右下角x
         * @param dstY2 合法区域的右下角y
         * @param isComposite 是否抗锯齿
         */
        void TransformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2, double dstX1, double dstY1,
                            double dstX2, double dstY2);
        void TransformImage(const Image& img, double dstX1, double dstY1, double dstX2, double dstY2);
        void TransformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2, const double* parallelogram);
        void TransformImage(const Image& img, const double* parallelogram);

        void BlendImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2, double dstX, double dstY,
                        unsigned alpha = 255);

        void BlendFromImage(Image& img, int imgX1, int imgY1, int imgX2, int imgY2, double dstX, double dstY,
                            unsigned alpha);
        void BlendFromImage(Image& img, double dstX, double dstY, unsigned alpha);

        /**
         * @brief 根据pattternMode模式在指定的方向内重复指定的元素 填充图形
         * @param img 图像相关信息
         * @param dstX x方向偏移量
         * @param dstY y方向偏移量
         * @param pattternMode 有四种模式
         */
        void PatternImageFill(Image& img, double offsetX, double offsetY, const char* pattternMode);
        /**
         * @brief 根据pattternMode模式在指定的方向内重复指定的元素 填充路径
         * @param img 图像相关信息
         * @param dstX x方向偏移量
         * @param dstY y方向偏移量
         * @param pattternMode 有四种模式
         */
        void PatternImageStroke(Image& img, double offsetX, double offsetY, const char* pattternMode);
        /**
         * @brief 返回 π
         */
        static double Pi()
        {
            return OHOS::PI;
        }

        /**
         * @brief 角度转换为弧度
         * @param  Angle 规定要转换的角度。
         * @return 返回弧度
         */
        static double Deg2Rad(double Angle)
        {
            return Angle * OHOS::PI / OHOS::BOXER;
        }

        /**
         * @brief 设置lineDash的起始位置偏移量
         * @param dDashOffset 要偏移的位置
         */
        void SetLineDashOffset(float dDashOffset)
        {
            this->dDashOffset = dDashOffset;
        }
        /**
         * @brief 获得lineDash的起始位置偏移量
         */
        float GetLineDashOffset() const
        {
            return dDashOffset;
        }

        /**
         * @brief 设置lineDash的虚实线的长度
         * @param dashArray 虚实线长度的数组
         * @param ndash 数组长度
         */
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

        /**
         * @brief 返回是否是dash的划线模式
         */
        bool IsLineDash() const
        {
            return is_dash;
        }

        /**
         * @brief 获取lineDash的虚实线长度数组
         */
        float* GetLineDash() const
        {
            return dashes;
        }

        /**
         * @brief 获取lineDash的虚实线长度数组的长度
         */
        unsigned int GetLineDashCount() const
        {
            return ndashes;
        }

        /**
         * @brief 返回渲染器
         */
        OHOS::RenderingBuffer GetRenderBuffer() const
        {
            return m_rbuf;
        }

        /**
         * @brief 设置阴影的颜色
         * @param red 红色
         * @param green 绿色
         * @param blue 蓝色
         * @param alpha 透明度
         */
        void SetShadowColor(unsigned red, unsigned green, unsigned blue, unsigned alpha = 255)
        {
            shadowColor_ = Color(red, green, blue, alpha);
        }
        /**
         * @brief 设置阴影的x轴偏移量
         */
        void SetShadowOffsetX(double x)
        {
            shadowOffsetX_ = x;
        }
        /**
         * @brief 设置阴影的y轴偏移量
         */
        void SetShadowOffsetY(double y)
        {
            shadowOffsetY_ = y;
        }
        /**
         * @brief 设置阴影的偏移量
         * @param x x轴偏移量
         * @param y y轴偏移量
         */
        void SetShadowOffset(double x, double y)
        {
            shadowOffsetX_ = x;
            shadowOffsetY_ = y;
        }
        /**
         * @brief 设置阴影模糊半径
         */
        void SetShadowBlurRadius(double radius)
        {
            shadowBlurRadius_ = radius;
        }

        /**
         * @brief 设置单个矩形边界
         */
        bool BoundingRectSingle(unsigned int path_id, RectD* rect, PathTransform& path);

    private:
        /**
         * @brief 开始渲染
         * @param fillColor 是否填充颜色
         */
        void Render(bool fillColor);
        /**
         * @brief 添加一条直线
         * @param x1 直线的起点x
         * @param y1 直线的起点y
         * @param x2 直线的终点x
         * @param y2 直线的终点y
         */
        void AddLine(double x1, double y1, double x2, double y2);
        /**
         * @brief 更新光栅器的gamma
         */
        void UpdateRasterizerGamma();
        /**
         * @brief 渲染图像
         * @param img
         * @param x1
         * @param y1
         * @param x2
         * @param y2
         * @param parl
         * @param isComposite
         */
        void RenderImage(const Image& img, int x1, int y1, int x2, int y2, const double* parl);

        /**
         * @brief 重置LineDash相关配置
         */
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
        OHOS::RenderingBuffer m_rbuf;                     // 渲染器缓冲区
        OHOS::ScanlineUnPackedContainer m_scanline;       // 扫描线不合并相同扫描线
        OHOS::RasterizerScanlineAntiAlias<> m_rasterizer; // 光栅
        OHOS::TransAffine m_fillGradientMatrix;           // 放射渐变的矩阵
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
        Color m_imageBlendColor; // 图像混合颜色
        Color m_fillColor;       // 图像需要填充颜色
        Color m_lineColor;       // 线条需要填充颜色
        color_func_type m_fillRadialGradient;
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        LineCap m_lineCap;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        LineJoin m_lineJoin;
#endif
        Gradient m_fillGradientFlag;
        Gradient m_lineGradientFlag;
        interpolator_type m_interpolator_type;
        gradient_func_type m_radialGradientFunction;
        ConvCurve m_convCurve;
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
        ConvDashCurve m_convDashCurve;
        ConvDashStroke m_convDashStroke;
        DashStrokeTransform m_dashStrokeTransform;
#endif
        ConvStroke m_convStroke;

        PathTransform m_pathTransform;
        StrokeTransform m_strokeTransform;

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

    /**
     * @brief operator ==重写两个颜色相比是否相等的比较过程
     */
    inline bool operator==(const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
    {
        return c1.redValue == c2.redValue && c1.greenValue == c2.greenValue && c1.blueValue == c2.blueValue &&
               c1.alphaValue == c2.alphaValue;
    }

    /**
     * @brief operator != 重写两个颜色相比是否不相等的比较过程
     */
    inline bool operator!=(const BaseGfxExtendEngine::Color& c1, const BaseGfxExtendEngine::Color& c2)
    {
        return !(c1 == c2);
    }
} // namespace OHOS
