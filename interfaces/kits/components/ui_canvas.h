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

/**
 * @addtogroup UI_Components
 * @{
 *
 * @brief Defines UI components such as buttons, texts, images, lists, and progress bars.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file ui_canvas.h
 *
 * @brief Defines the attributes of the canvas component and provides functions for drawing rectangles,
 *        circles, and others.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_LITE_UI_CANVAS_H
#define GRAPHIC_LITE_UI_CANVAS_H

#include <draw/draw_utils.h>
#include <fcntl.h>
#include <gfx_utils/graphics/depiction/graphic_depict_curve.h>
#include <gfx_utils/graphics/depiction/graphic_depict_dash.h>
#include <gfx_utils/graphics/depiction/graphic_depict_stroke.h>
#include <gfx_utils/graphics/imagefilter/graphic_filter_blur.h>
#include <gfx_utils/graphics/rasterizer/graphic_rasterizer_scanline_antialias.h>
#include <gfx_utils/graphics/scanline/graphic_geometry_scanline.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_base.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_gradient.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_gradient_lut.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_interpolator.h>
#include <gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_pattern_rgba.h>
#include <gfx_utils/graphics/transform/graphic_transform_image_accessors.h>
#include <gfx_utils/graphics/vertexprimitive/graphic_geometry_path_storage.h>
#include <render/graphic_render_pixfmt_rgba_blend.h>

#include <stack>

#include "animator/gif_canvas_image_animator.h"
#include "common/image.h"
#include "components/ui_label.h"
#include "gfx_utils/file.h"
#include "gfx_utils/list.h"
#include "gif_lib.h"
#include "render/graphic_render_base.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_scanline.h"

namespace OHOS {
    class UICanvas;
    /**
     * @brief Defines the basic styles of graphs drawn on canvases.
     *
     * @since 1.0
     * @version 1.0
     */
    class Paint : public HeapBase {
    public:
        /**
         * @brief A constructor used to create a <b>Paint</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        Paint()
            : style_(PaintStyle::STROKE_FILL_STYLE),
            fillColor_(Color::Black()),
            strokeColor_(Color::White()),
            opacity_(OPA_OPAQUE),
            strokeWidth_(2),
            changeFlage_(false),
            lineJoin_(LineJoinEnum::ROUND_JOIN),
            lineCap_(LineCapEnum::BUTT_CAP),
            isDashMode_(false),
            dashOffset_(0),
            dashArray_(nullptr),
            ndashes_(0),
            miterLimit_(0),
#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
            shadowColor(Color::Black()),
            haveShadow(false),
#endif
            globalAlpha_(1.0),
            globalCompositeOperation_(SOURCE_OVER),
            rotateAngle_(0),
            haveComposite_(false),
            uiCanvas_(nullptr)
        {}

        Paint(const Paint& paint)
        {
            Init(paint);
        }

        /*
         * 对于数据成员进行初始化
         * style_;       paint style
         * fillColor_;   设置笔的填充颜色
         * strokeColor_; 设置笔的线条颜色
         * opacity_;     设置透明度
         * strokeWidth_; 设置线宽
         * lineCap_;     设置笔帽
         * lineJoin_;    设置笔的路径连接处的风格样式
         * miterLimit_;  设置路径连接处的尖角的间距限制
         * dashOffset;   dash 点偏移量
         * isDrawDash;   是否绘制点划线
         * dashArray;    dash 点数组
         * ndashes;      点划线数量
         * globalAlpha;  设置图元全局alpha
         * shadowBlurRadius;  设置阴影模糊半径
         * shadowOffsetX;     设置阴影横坐标偏移量
         * shadowOffsetY;     设置阴影纵坐标偏移量
         * shadowColor;       设置阴影色彩
         * blendMode;     设置多图元混合渲染模式
         */
        void Init(const Paint& paint)
        {
            style_ = paint.style_;
            fillColor_ = paint.fillColor_;
            strokeColor_ = paint.strokeColor_;
            strokeWidth_ = paint.strokeWidth_;
            opacity_ = paint.opacity_;
            lineCap_ = paint.lineCap_;
            lineJoin_ = paint.lineJoin_;
            isDashMode_ = paint.isDashMode_;
            dashOffset_ = paint.dashOffset_;
            dashArray_ = paint.dashArray_;
            ndashes_ = paint.ndashes_;
            changeFlage_ = paint.changeFlage_;
            if (isDashMode_ && ndashes_ > 0) {
                dashArray_ = new float[ndashes_];
                if (dashArray_) {
                    if (memset_s(dashArray_, ndashes_ * sizeof(float), 0, ndashes_ * sizeof(float)) != EOF) {
                    }
                    for (unsigned int i = 0; i < ndashes_; i++) {
                        dashArray_[i] = paint.dashArray_[i];
                    }
                } else {
                    ndashes_ = 0;
                    dashOffset_ = 0;
                    isDashMode_ = false;
                }
            } else {
                dashArray_ = nullptr;
            }
            miterLimit_ = paint.ndashes_;
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
            linearGradientPoint_ = paint.linearGradientPoint_;
            radialGradientPoint_ = paint.radialGradientPoint_;
            stopAndColors_ = paint.stopAndColors_;
            gradientflag = paint.gradientflag;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
            patternRepeat_ = paint.patternRepeat_;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
            shadowBlurRadius = paint.shadowBlurRadius;
            shadowOffsetX = paint.shadowOffsetX;
            shadowOffsetY = paint.shadowOffsetY;
            shadowColor = paint.shadowColor;
            haveShadow = paint.haveShadow;
#endif
            globalAlpha_ = paint.globalAlpha_;
            globalCompositeOperation_ = paint.globalCompositeOperation_;
            rotateAngle_ = paint.rotateAngle_;
            transfrom_ = paint.transfrom_;
            haveComposite_ = paint.haveComposite_;
            uiCanvas_ = paint.uiCanvas_;
        }

        /**
         * @brief A destructor used to delete the <b>Paint</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual ~Paint()
        {}

        const Paint& operator=(const Paint& paint)
        {
            Init(paint);
            return *this;
        }
        /**
         * @brief Enumerates paint styles of a closed graph. The styles are invalid for non-closed graphs.
         */
        enum PaintStyle {
            /** Stroke only */
            STROKE_STYLE = 1,
            /** Fill only */
            FILL_STYLE,
            /** Stroke and fill */
            STROKE_FILL_STYLE,
            /** 渐变 */
            GRADIENT,
            /** 图像模式 */
            PATTERN
        };

        struct LinearGradientPoint {
            /**  开始点坐标x  */
            double x0;
            /**  开始点坐标y  */
            double y0;
            /**  结束点坐标x  */
            double x1;
            /**  结束点坐标y  */
            double y1;
        };

        struct RadialGradientPoint {
            /**  开始圆点坐标x  */
            double x0;
            /**  开始圆点坐标y  */
            double y0;
            /**  开始圆半径r0  */
            double r0;
            /**  结束圆点坐标x  */
            double x1;
            /**  结束圆点坐标y  */
            double y1;
            /**  开始圆半径r0  */
            double r1;
        };

        struct StopAndColor {
            /** 介于 0.0 与 1.0 之间的值，表示渐变中开始与结束之间的位置。  */
            double stop;
            /** 在结束位置显示的颜色值 */
            ColorType color;
        };
        enum Gradient {
            Linear,
            Radial
        };

        /**
         * repeat 铺满整个画布
         * repeat-x 在画布的x轴重复
         * repeat-y 在画布的y轴重复
         * no-repeat 在画布不重复
         */
        enum PatternRepeatMode {
            REPEAT,
            REPEAT_X,
            REPEAT_Y,
            NO_REPEAT,
        };

        /**
         * @brief Sets the paint style of a closed graph.
         *
         * @param style Indicates the paint style. Stroke and fill are set by default.
         * For details, see {@link PaintStyle}.
         * @see GetStyle
         * @since 1.0
         * @version 1.0
         */
        void SetStyle(PaintStyle style)
        {
            style_ = style;
        }

        void StrokeStyle(ColorType color)
        {
            SetStyle(Paint::STROKE_STYLE);
            SetStrokeColor(color);
        }

        void FillStyle(ColorType color)
        {
            SetStyle(Paint::FILL_STYLE);
            SetFillColor(color);
        }

        void StrokeStyle(PaintStyle style)
        {
            SetStyle(style);
        }
        void FillStyle(PaintStyle style)
        {
            SetStyle(style);
        }

        /**
         * @brief Obtains the paint style of a closed graph.
         *
         * @return Returns the paint style. For details, see {@link PaintStyle}.
         * @see SetStyle
         * @since 1.0
         * @version 1.0
         */
        PaintStyle GetStyle() const
        {
            return style_;
        }

        /**
         * @brief Sets the width of a line or border.
         *
         * @param width Indicates the line width when a line is drawn or the border width when a closed graph is drawn.
         *        The width is extended to both sides.
         * @see GetStrokeWidth
         * @since 1.0
         * @version 1.0
         */
        void SetStrokeWidth(uint16_t width)
        {
            strokeWidth_ = width;
        }

        /**
         * @brief Obtains the width of a line or border.
         *
         * @return Returns the line width if a line is drawn or the border width if a closed graph is drawn.
         * @see SetStrokeWidth
         * @since 1.0
         * @version 1.0
         */
        uint16_t GetStrokeWidth() const
        {
            return strokeWidth_;
        }

        /**
         * @brief Sets the color of a line or border.
         *
         * @param color Indicates the line color when a line is drawn or the border color when a closed graph is drawn.
         * @see GetStrokeColor
         * @since 1.0
         * @version 1.0
         */
        void SetStrokeColor(ColorType color)
        {
            strokeColor_ = color;
            changeFlage_ = true;
        }

        /**
         * @brief Obtains the color of a line or border.
         *
         * @return Returns the line color if a line is drawn or the border color if a closed graph is drawn.
         * @see SetStrokeWidth
         * @since 1.0
         * @version 1.0
         */
        ColorType GetStrokeColor() const
        {
            return strokeColor_;
        }

        /**
         * @brief Sets fill color.
         *
         * This function is valid only for closed graphs.
         *
         * @param color Indicates the fill color to set.
         * @see GetFillColor
         * @since 1.0
         * @version 1.0
         */
        void SetFillColor(ColorType color)
        {
            fillColor_ = color;
            changeFlage_ = true;
        }

        /**
         * @brief Obtains the fill color.
         *
         * @return Returns the fill color.
         * @see SetFillColor
         * @since 1.0
         * @version 1.0
         */
        ColorType GetFillColor() const
        {
            return fillColor_;
        }

        /**
         * @brief Sets the opacity.
         *
         * The setting takes effect for the entire graph, including the border, line color, and fill color.
         *
         * @param opacity Indicates the opacity. The value range is [0, 255].
         * @see GetOpacity
         * @since 1.0
         * @version 1.0
         */
        void SetOpacity(uint8_t opacity)
        {
            opacity_ = opacity;
        }

        /**
         * @brief Obtains the opacity.
         *
         * @return Returns the opacity.
         * @see SetOpacity
         * @since 1.0
         * @version 1.0
         */
        uint8_t GetOpacity() const
        {
            return opacity_;
        }

        bool GetChangeFlag() const
        {
            return changeFlage_ && !transfrom_.IsIdentity();
        }

        /**
         * @brief 设置笔帽类型.
         * @see GetLineCap
         * @since 1.0
         * @version 1.0
         */
        void SetLineCap(LineCapEnum lineCap)
        {
            lineCap_ = lineCap;
            changeFlage_ = true;
        }
        /**
         * @brief 获取笔帽类型.
         * @see SetLineCap
         * @since 1.0
         * @version 1.0
         */
        LineCapEnum GetLineCap() const
        {
            return lineCap_;
        }

        /**
         * @brief 设置笔的路径连接处的风格样式.
         * @see GetLineJoin
         * @since 1.0
         * @version 1.0
         */
        void SetLineJoin(LineJoinEnum lineJoin)
        {
            lineJoin_ = lineJoin;
            changeFlage_ = true;
        }
        /**
         * @brief 设置路径连接处的尖角的间距限制.
         * @see GetMiterLimit
         * @since 1.0
         * @version 1.0
         */
        void SetMiterLimit(double miterLimit)
        {
            miterLimit_ = miterLimit;
            changeFlage_ = true;
        }

        double GetMiterLimit() const
        {
            return miterLimit_;
        }
        /**
         * @brief 获取笔的路径连接处的风格样式.
         * @see SetLineJoin
         * @since 1.0
         * @version 1.0
         */
        LineJoinEnum GetLineJoin() const
        {
            return lineJoin_;
        }

        bool IsLineDash() const
        {
            return isDashMode_;
        }

        /**
         * @brief 设置点划线的数组和数量.
         * @param lineDashs 表示点划线数组,ndash 表示点划线数量
         * @since 1.0
         * @version 1.0
         */
        void SetLineDash(float* lineDashs, const unsigned int ndash)
        {
            if (ndash < 0) {
                GRAPHIC_LOGE("SetLineDash fail,because ndash < =0");
                return;
            }
            ClearLineDash();
            if (lineDashs == nullptr || ndash == 0) {
                return;
            }
            ndashes_ = ndash;
            isDashMode_ = true;
            dashArray_ = new float[ndashes_];
            if (dashArray_) {
                if (memset_s(dashArray_, ndashes_ * sizeof(float), 0, ndashes_ * sizeof(float)) != EOF) {
                }
                for (unsigned int iIndex = 0; iIndex < ndashes_; iIndex++) {
                    dashArray_[iIndex] = lineDashs[iIndex];
                }
            } else {
                ndashes_ = 0;
                dashOffset_ = 0;
                isDashMode_ = false;
            }
            changeFlage_ = true;
        }

        /**
         * @brief 获取dash数组
         * @return
         */
        float* GetLineDash() const
        {
            return dashArray_;
        }

        float GetLineDashOffset() const
        {
            return dashOffset_;
        }

        /**
         * @brief 设置dash模式起点的偏移量
         * @see GetLineDashOffset
         * @since 1.0
         * @version 1.0
         */
        void SetLineDashOffset(float offset)
        {
            dashOffset_ = offset; // dash 点偏移量
            changeFlage_ = true;
            isDashMode_ = true;
        }

        /**
         * @brief 获取dash数组长度
         * @return
         */
        unsigned int GetLineDashCount() const
        {
            return ndashes_;
        }

        /**
         * @brief 清空点划线的，改用实现绘制.
         * @since 1.0
         * @version 1.0
         */
        void ClearLineDash(void)
        {
            dashOffset_ = 0;
            ndashes_ = 0;
            isDashMode_ = false;
            if (dashArray_ != nullptr) {
                delete[] dashArray_;
                dashArray_ = nullptr;
            }
        }

#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        void createLinearGradient(double startx, double starty, double endx, double endy)
        {
            gradientflag = Linear;
            linearGradientPoint_.x0 = startx;
            linearGradientPoint_.y0 = starty;
            linearGradientPoint_.x1 = endx;
            linearGradientPoint_.y1 = endy;
            changeFlage_ = true;
        }

        void addColorStop(double stop, ColorType color)
        {
            StopAndColor stopAndColor;
            stopAndColor.stop = stop;
            stopAndColor.color = color;
            stopAndColors_.PushBack(stopAndColor);
        }

        void createRadialGradient(double start_x, double start_y, double start_r, double end_x, double end_y,
                                  double end_r)
        {
            gradientflag = Radial;
            radialGradientPoint_.x0 = start_x;
            radialGradientPoint_.y0 = start_y;
            radialGradientPoint_.r0 = start_r;
            radialGradientPoint_.x1 = end_x;
            radialGradientPoint_.y1 = end_y;
            radialGradientPoint_.r1 = end_r;
            changeFlage_ = true;
        }

        List<StopAndColor> getStopAndColor() const
        {
            return stopAndColors_;
        }

        LinearGradientPoint GetLinearGradientPoint() const
        {
            return linearGradientPoint_;
        }

        RadialGradientPoint GetRadialGradientPoint() const
        {
            return radialGradientPoint_;
        }

        Gradient GetGradient() const
        {
            return gradientflag;
        }
#endif
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        /*
         * 设置图元用图案填充样式
         * @param img 表示填充的图案，text表示填充样式
         */
        void CreatePattern(const char* img, PatternRepeatMode patternRepeat)
        {
            image = img;
            patternRepeat_ = patternRepeat;
            changeFlage_ = true;
        }

        const char* GetPatternImage() const
        {
            return image;
        }

        PatternRepeatMode GetPatternRepeatMode() const
        {
            return patternRepeat_;
        }
#endif

#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE

        /**
         * @brief 设置阴影模糊级别.
         * @since 1.0
         * @version 1.0
         */
        void SetShadowBlur(double radius)
        {
            shadowBlurRadius = radius;
            changeFlage_ = true;
        }

        /**
         * @brief 获取阴影模糊级别.
         * @since 1.0
         * @version 1.0
         */
        double GetShadowBlur() const
        {
            return shadowBlurRadius;
        }

        /**
         * @brief 获取阴影横坐标偏移量.
         * @since 1.0
         * @version 1.0
         */
        double GetShadowOffsetX() const
        {
            return shadowOffsetX;
        }
        /**
         * @brief 设置阴影横坐标偏移量.
         * @since 1.0
         * @version 1.0
         */
        void SetShadowOffsetX(double offset)
        {
            shadowOffsetX = offset;
            changeFlage_ = true;
        }
        /**
         * @brief 获取阴影纵坐标偏移量.
         * @since 1.0
         * @version 1.0
         */
        double GetShadowOffsetY() const
        {
            return shadowOffsetY;
        }
        /**
         * @brief 设置阴影纵坐标偏移量.
         * @since 1.0
         * @version 1.0
         */
        void SetShadowOffsetY(double offset)
        {
            shadowOffsetY = offset;
            changeFlage_ = true;
        }
        /**
         * @brief 获取阴影的颜色值.
         * @since 1.0
         * @version 1.0
         */
        ColorType GetShadowColor() const
        {
            return shadowColor;
        }
        /**
         * @brief 设置阴影的颜色值.
         * @since 1.0
         * @version 1.0
         */
        void SetShadowColor(ColorType color)
        {
            shadowColor = color;
            changeFlage_ = true;
            haveShadow = true;
        }
        bool HaveShadow() const
        {
            return haveShadow;
        }
#endif
        /**
         * @brief 设置当前绘图的alpha
         */
        void SetGlobalAlpha(float alphaPercentage)
        {
            if (alphaPercentage > 1) {
                globalAlpha_ = 1.0;
                return;
            }
            if (alphaPercentage < 0) {
                globalAlpha_ = 0.0;
                return;
            }
            globalAlpha_ = alphaPercentage;
            changeFlage_ = true;
        }
        /**
         * @brief 返回当前绘图的alpha
         */
        float GetGlobalAlpha() const
        {
            return globalAlpha_;
        }

        /**
         * @brief 设置混合模式
         */
        void SetGlobalCompositeOperation(GlobalCompositeOperation globalCompositeOperation)
        {
            globalCompositeOperation_ = globalCompositeOperation;
            changeFlage_ = true;
            if (globalCompositeOperation != SOURCE_OVER) {
                haveComposite_ = true;
            }
        }

        /**
         * @brief 获取混合模式
         */
        GlobalCompositeOperation GetGlobalCompositeOperation() const
        {
            return globalCompositeOperation_;
        }

        /* 缩放当前绘图至更大或更小 */
        void Scale(float scaleX, float scaleY)
        {
            changeFlage_ = true;

            if (rotateAngle_ > 0.0 || rotateAngle_ < 0) {
                transfrom_.Rotate(-rotateAngle_ * PI / OHOS::BOXER);
                transfrom_.Scale(scaleX, scaleY);
                transfrom_.Rotate(rotateAngle_ * PI / OHOS::BOXER);
            } else {
                transfrom_.Scale(scaleX, scaleY);
            }
        }

        double GetScaleX() const
        {
            return transfrom_.GetData()[0];
        }

        double GetScaleY() const
        {
            return transfrom_.GetData()[INDEX_FOUR];
        }

        /* 旋转当前绘图 */
        void Rotate(float angle)
        {
            changeFlage_ = true;
            transfrom_.Rotate(angle * PI / OHOS::BOXER);
            rotateAngle_ += angle;
        }

        void Rotate(float angle, int16_t x, int16_t y)
        {
            transfrom_.Translate(-x, -y);
            transfrom_.Rotate(angle * PI / OHOS::BOXER);
            rotateAngle_ += angle;
            transfrom_.Translate(x, y);
            changeFlage_ = true;
        }
        /* 重新映射画布上的 (x,y) 位置 */
        void Translate(int16_t x, int16_t y)
        {
            changeFlage_ = true;
            transfrom_.Translate(x, y);
        }

        /* 获取重新映射画布上的x 位置 */
        int16_t GetTranslateX() const
        {
            return transfrom_.GetData()[INDEX_TWO];
        }
        /* 获取重新映射画布上的y 位置 */
        int16_t GetTranslateY() const
        {
            return transfrom_.GetData()[INDEX_FIVE];
        }

        /* 将当前转换重置为单位矩阵。然后运行 transform() */
        void SetTransform(float scaleX, float shearX, float shearY, float scaleY,
                          int16_t transLateX, int16_t transLateY)
        {
            transfrom_.Reset();
            rotateAngle_ = 0;
            Transform(scaleX, shearX, shearY, scaleY, transLateX, transLateY);
            changeFlage_ = true;
        }

        /* 将当前转换重置为单位矩阵。然后运行 transform() */
        void Transform(float scaleX, float shearX, float shearY, float scaleY, int16_t transLateX, int16_t transLateY)
        {
            changeFlage_ = true;
            transLateX += transfrom_.GetData()[INDEX_TWO];
            transLateY += transfrom_.GetData()[INDEX_FIVE];
            transfrom_.Translate(-transfrom_.GetData()[INDEX_TWO], -transfrom_.GetData()[INDEX_FIVE]);
            Scale(scaleX, scaleY);
            transfrom_.Translate(transLateX, transLateY);
            transfrom_.SetData(INDEX_ONE, transfrom_.GetData()[INDEX_ONE] + shearX);
            transfrom_.SetData(INDEX_THREE, transfrom_.GetData()[INDEX_THREE] + shearY);
        }

        double GetshearX() const
        {
            return transfrom_.GetData()[INDEX_ONE];
        }
        double GetshearY() const
        {
            return transfrom_.GetData()[INDEX_THREE];
        }
        TransAffine GetTransAffine() const
        {
            return transfrom_;
        }

        float GetRotateAngle() const
        {
            return rotateAngle_;
        }

        void SetUICanvas(UICanvas* uiCanvas)
        {
            this->uiCanvas_ = uiCanvas;
        }

        UICanvas* GetUICanvas() const
        {
            return uiCanvas_;
        }

        bool HaveComposite() const
        {
            return haveComposite_;
        }

    private:
        PaintStyle style_;
        ColorType fillColor_;
        ColorType strokeColor_;
        uint8_t opacity_;
        uint16_t strokeWidth_;
        bool changeFlage_;
        LineJoinEnum lineJoin_;
        LineCapEnum lineCap_;
        bool isDashMode_;      // 是否是dash模式的线段
        float dashOffset_;     // dash 点偏移量
        float* dashArray_;     // dash 点数组
        unsigned int ndashes_; // dashArray的长度
        double miterLimit_;    // 设置路径连接处的尖角的间距限制
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        LinearGradientPoint linearGradientPoint_;
        RadialGradientPoint radialGradientPoint_;
        List<StopAndColor> stopAndColors_;
        Gradient gradientflag;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        PatternRepeatMode patternRepeat_;
#endif
        const char* image;
#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
        double shadowBlurRadius; // 设置阴影模糊半径
        double shadowOffsetX;    // 设置阴影横坐标偏移量
        double shadowOffsetY;    // 设置阴影纵坐标偏移量。
        ColorType shadowColor;   // 设置阴影色彩
        bool haveShadow;         // 当前是否有阴影
#endif
        float globalAlpha_;                                 // 当前绘图的透明度0-1 百分比
        GlobalCompositeOperation globalCompositeOperation_; // 混合图像方式
        float rotateAngle_;                                 // 旋转角度，单位度数
        TransAffine transfrom_;                             // 矩阵
        bool haveComposite_;
        UICanvas* uiCanvas_;
    };

    /**
 * @brief Defines a canvas, which is used to draw multiple types of 2D graphs.
 *
 * @since 1.0
 * @version 1.0
 */
    class UICanvas : public UIView {
    public:
        typedef Rgba8 Rgba8Color;
        // 颜色数组rgba,的索引位置blue:0,green:1,red:2,alpha:3,
        typedef OrderBgra ComponentOrder;

        // 根据ComponentOrder的索引将颜色填入ComponentOrder规定的位置，根据blender_rgba模式处理颜色
        typedef RgbaBlender<Rgba8Color, ComponentOrder> Blender;
        typedef PixfmtAlphaBlendRgba<Blender, RenderingBuffer> PixFormat;
        typedef RendererBase<PixFormat> RendererBase;

        typedef ScanlineUnPackedContainer Scanline;
        typedef OHOS::SpanFillColorAllocator<Rgba8Color> SpanAllocator;
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        // 设定渐变数组的构造器设定颜色插值器和颜色模板等
        typedef GradientColorCalibration<OHOS::ColorInterpolator<OHOS::Srgba8>> GradientColorMode;
        // 设定放射渐变的算法
        typedef GradientRadialCalculate RadialGradientCalculate;
        // 设定线段插值器
        typedef SpanInterpolatorLinear<> InterpolatorType;
        // 设定线性渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, GradientLinearCalculate,
                GradientColorMode> LinearGradientSpan;
        // 设定放射渐变的线段生成器
        typedef SpanFillColorGradient<Rgba8Color, SpanInterpolatorLinear<>, RadialGradientCalculate,
                GradientColorMode> RadialGradientSpan;
#endif

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
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        //  x,y轴都重复
        typedef OHOS::SpanPatternFillRgba<ImgSourceTypeRepeat> spanPatternTypeRepeat;
        //  x轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatX> spanPatternTypeRepeatX;
        //  y轴重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeRepeatY> spanPatternTypeRepeatY;
        // 不重复
        typedef OHOS::SpanPatternFillRgba<imgSourceTypeNoRepeat> spanPatternTypeNoRepeat;
#endif
        /**
         * @brief A constructor used to create a <b>UICanvas</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        UICanvas()
            : startPoint_( {0, 0}), vertices_(nullptr), gfxMapBuffer_(nullptr)
        {}

        /**
         * @brief A destructor used to delete the <b>UICanvas</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual ~UICanvas();

        /**
         * @brief Obtains the view type.
         *
         * @return Returns the view type. For details, see {@link UIViewType}.
         * @since 1.0
         * @version 1.0
         */
        UIViewType GetViewType() const override
        {
            return UI_CANVAS;
        }

        /**
         * @brief Clears the entire canvas.
         *
         * @since 1.0
         * @version 1.0
         */
        void Clear();

        /**
         * @brief Sets the coordinates of the start point for drawing a line. For example, if <b>startPoint</b> is
         *        set to {50, 50}, the line is drawn from this set of coordinates on the canvas.
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @see GetStartPosition
         * @since 1.0
         * @version 1.0
         */
        void SetStartPosition(const Point& startPoint)
        {
            startPoint_ = startPoint;
        }

        /**
         * @brief Obtains the coordinates of the start point of a line.
         *
         * @return Returns the coordinates of the start point.
         * @see SetStartPosition
         * @since 1.0
         * @version 1.0
         */
        const Point& GetStartPosition() const
        {
            return startPoint_;
        }

        /**
         * @brief Draws a straight line.
         *
         * If {@link SetStartPosition} is not used to set the coordinates of the start point of the line, the drawing
         * starts from the end point of the last line.
         *
         * @param endPoint Indicates the end point of the straight line.
         * @param paint    Indicates the straight line style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawLine(const Point& endPoint, const Paint& paint);

        /**
         * @brief Draws a straight line from the coordinates of the start point.
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @param endPoint   Indicates the coordinates of the end point.
         * @param paint      Indicates the straight line style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawLine(const Point& startPoint, const Point& endPoint, const Paint& paint);

        /**
         * @brief Draws a cubic Bezier curve.
         *
         * If {@link SetStartPosition} is not used to set the coordinates of the start point of the curve,
         * the drawing starts from the end point of the last line.
         * Currently, the opacity cannot be set, and the maximum line width is <b>3</b>.
         *
         * @param control1 Indicates the coordinates of the first control point of the cubic Bezier curve.
         * @param control2 Indicates the coordinates of the second control point of the cubic Bezier curve.
         * @param endPoint Indicates the coordinates of the end point of the cubic Bezier curve.
         * @param paint    Indicates the curve style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawCurve(const Point& control1, const Point& control2, const Point& endPoint, const Paint& paint);

        /**
         * @brief Draws a cubic Bezier curve from the start point coordinates.
         *
         * Currently, the opacity cannot be set, and the maximum line width is <b>3</b>.
         *
         * @param startPoint Indicates the coordinates of the start point of the cubic Bezier curve.
         * @param control1   Indicates the coordinates of the first control point of the cubic Bezier curve.
         * @param control2   Indicates the coordinates of the second control point of the cubic Bezier curve.
         * @param endPoint   Indicates the coordinates of the end point of the cubic Bezier curve.
         * @param paint      Indicates the curve style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawCurve(const Point& startPoint, const Point& control1, const Point& control2,
                       const Point& endPoint, const Paint& paint);

        /**
         * @brief Draws a rectangle.
         *
         * @param startPoint Indicates the coordinates of the point at the upper left corner of the rectangle.
         * @param height     Indicates the height of the rectangle.
         * @param width      Indicates the width of the rectangle.
         * @param paint      Indicates the rectangle style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint);

        /**
         * @brief 绘制矩形路径无填充
         * @param startPoint 起点
         * @param height 高度
         * @param width 宽度
         * @param paint 画笔
         */
        void StrokeRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint);

        /**
         * @brief 清除矩形内的像素
         * @param startPoint 起点
         * @param height 高度
         * @param width 宽度
         * @param paint 画笔
         */
        void ClearRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint);

        /**
         * @brief Draws a circle.
         *
         * @param center Indicates the coordinates of the circle center.
         * @param radius Indicates the radius of the circle.
         * @param paint  Indicates the circle style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawCircle(const Point& center, uint16_t radius, const Paint& paint);

        /**
         * @brief Draws a sector.
         *
         * When the start angle is smaller than the end angle, the sector is drawn clockwise.
         * Otherwise, the sector is drawn counterclockwise.
         *
         * @param center     Indicates the coordinates of the sector's center.
         * @param radius     Indicates the radius of the sector.
         * @param startAngle Indicates the start angle of the sector. Value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @param endAngle   Indicates the end angle of the sector. Value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @param paint      Indicates the sector style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawSector(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle, const Paint& paint);

        /**
         * @brief Draws an arc.
         *
         * Only stroke is supported. \n
         * When the start angle is smaller than the end angle, the sector is drawn clockwise.
         * Otherwise, the sector is drawn counterclockwise. \n
         *
         * @param center     Indicates the coordinates of the arc's center.
         * @param radius     Indicates the radius of the arc.
         * @param startAngle Indicates the start angle of the arc. Value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @param endAngle   Indicates the end angle of the arc. Value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @param paint      Indicates the arc style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawArc(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle, const Paint& paint);

#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWIMAGE
        /**
         * @brief Draws an image.
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @param image      Indicates the pointer to the image source.
         * @param paint      Indicates the image style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawImage(const Point& startPoint, const char* image, const Paint& paint);

        void DrawImage(const Point& startPoint, const char* image, Paint& paint, int16_t width, int16_t height);

        bool IsGif(const char* src);
#endif
        /**
         * @brief Defines the font style.
         */
        struct FontStyle {
            /** Text direction. For details, see {@link UITextLanguageDirect}. */
            UITextLanguageDirect direct;
            /** Text alignment mode. For details, see {@link UITextLanguageAlignment}. */
            UITextLanguageAlignment align;
            /** Font size */
            uint8_t fontSize;
            /** Letter-spacing */
            int16_t letterSpace;
            /** Font name */
            const char* fontName;
        };

        /**
         * @brief Draws text.
         *
         * Only fill is supported. \n
         * If the text length exceeds the value of <b>maxWidth</b>, the text will be truncated. \n
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @param text       Indicates the pointer to the text content.
         * @param maxWidth   Indicates the maximum width of the text that can be displayed. If the maximum width is
         *                   exceeded, the text is truncated.
         * @param fontStyle  Indicates the text layout and font style. For details, see {@link FontStyle}.
         * @param paint      Indicates the text style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawLabel(const Point& startPoint, const char* text, uint16_t maxWidth, const FontStyle& fontStyle,
                       const Paint& paint);

        /**
         * @brief Creates a path.
         *
         * A round corner can be used to join two lines. Currently, miter and bevel joints are not supported.
         * To draw this path, you need to call {@link DrawPath}.
         *
         * @since 3.0
         * @version 5.0
         */
        void BeginPath();

        /**
         * @brief Moves the start point of this path to a specified point.
         *
         * @param point Indicates the specified point to move to.
         * @since 3.0
         * @version 5.0
         */
        void MoveTo(const Point& point);

        /**
         * @brief Creates a straight line from the end point of this path to a specified point.
         *
         * @param point Indicates the coordinates of the specified point.
         * @since 3.0
         * @version 5.0
         */
        void LineTo(const Point& point);

        /**
         * @brief Creates an arc path.
         *
         * @param center     Indicates the coordinates of the arc's center point.
         * @param radius     Indicates the radius of the arc.
         * @param startAngle Indicates the start angle of the arc.
         *                   The value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @param endAngle   Indicates the end angle of the arc.
         *                   The value <b>0</b> indicates the 12-o'clock direction,
         *                   and <b>90</b> indicates the 3-o'clock direction.
         * @since 3.0
         * @version 5.0
         */
        void ArcTo(const Point& center, uint16_t radius, int16_t startAngle, int16_t endAngle);

        /**
         * @brief Creates a rectangular path.
         *
         * @param point  Indicates the coordinates of the rectangle's upper left corner.
         * @param height Indicates the height of the rectangle.
         * @param width  Indicates the width of the rectangle.
         * @since 3.0
         * @version 5.0
         */
        void AddRect(const Point& point, int16_t height, int16_t width);

        /**
         * @brief Closes this path.
         *
         * @since 3.0
         * @version 5.0
         */
        void ClosePath();

        /**
         * @brief Draws this path.
         *
         * @param paint Indicates the path style. For details, see {@link Paint}.
         * @since 3.0
         * @version 5.0
         */
        void DrawPath(const Paint& paint);

        /**
         * @brief 填充多边形路径
         * @param paint
         */
        void FillPath(const Paint& paint);

#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWTEXT
        /*  在画布上绘制文本 */
        void StrokeText(const char* text, const Point& point, const FontStyle& fontStyle, const Paint& paint);
#endif

        /* 返回包含指定文本宽度的对象 */
        Point MeasureText(const char* text, const FontStyle& fontStyle, const Paint& paint);

        /* 保存历史状态 */
        void Save(Paint& paint)
        {
            PaintStack.push(paint);
        }

        /* 恢复到上次历史状态 */
        Paint Restore()
        {
            Paint paint;
            if (PaintStack.empty()) {
                return paint;
            }
            paint = PaintStack.top();
            PaintStack.pop();
            return paint;
        }

        void OnBlendDraw(BufferInfo& gfxDstBuffer, const Rect& trunc);

        void OnDraw(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea) override;

    protected:
        constexpr static uint8_t MAX_CURVE_WIDTH = 3;

        struct LineParam : public HeapBase {
            Point start;
            Point end;
        };

        struct CurveParam : public HeapBase {
            Point start;
            Point control1;
            Point control2;
            Point end;
        };

        struct RectParam : public HeapBase {
            Point start;
            int16_t height;
            int16_t width;
        };

        struct CircleParam : public HeapBase {
            Point center;
            uint16_t radius;
        };

        struct ArcParam : public HeapBase {
            Point center;
            uint16_t radius;
            int16_t startAngle;
            int16_t endAngle;
        };

        struct PathParam : public HeapBase {
            UICanvasVertices* vertices;
            ImageParam* imageParam = nullptr;
            bool isStroke;
        };

        struct TextParam : public HeapBase {
            const char* text;
            Point position;
            Color32 fontColor;
            uint8_t fontOpa;
            FontStyle fontStyle;
            Text* textComment;
            TextParam()
            {
                textComment = new Text;
            }

            ~TextParam()
            {
                if (textComment) {
                    delete textComment;
                    textComment = nullptr;
                }
            };
        };

        struct DrawCmd : public HeapBase {
            Paint paint;
            void* param;
            void (*DrawGraphics)(BufferInfo&, void*, const Paint&, const Rect&, const Rect&, const Style&);
            void (*DeleteParam)(void*);
        };

        Point startPoint_;
        UICanvasVertices* vertices_;
        List<DrawCmd> drawCmdList_;
        // 保存Paint的历史修改信息
        std::stack<Paint> PaintStack;
        BufferInfo* gfxMapBuffer_;

        static void DeleteLineParam(void* param)
        {
            LineParam* lineParam = static_cast<LineParam*>(param);
            delete lineParam;
        }

        static void DeleteCurveParam(void* param)
        {
            CurveParam* curveParam = static_cast<CurveParam*>(param);
            delete curveParam;
        }

        static void DeleteRectParam(void* param)
        {
            RectParam* rectParam = static_cast<RectParam*>(param);
            delete rectParam;
        }

        static void DeleteCircleParam(void* param)
        {
            CircleParam* circleParam = static_cast<CircleParam*>(param);
            delete circleParam;
        }

        static void DeleteArcParam(void* param)
        {
            ArcParam* arcParam = static_cast<ArcParam*>(param);
            delete arcParam;
        }

        static void DeleteImageParam(void* param)
        {
            ImageParam* imageParam = static_cast<ImageParam*>(param);
            if (imageParam->image != nullptr) {
                delete imageParam->image;
                imageParam->image = nullptr;
            }
            if (imageParam->gifImageAnimator != nullptr) {
                imageParam->gifImageAnimator->Stop();
                delete imageParam->gifImageAnimator;
                imageParam->gifImageAnimator = nullptr;
            }
            delete imageParam;
        }

        static void DeleteLabel(void* param)
        {
            UILabel* label = static_cast<UILabel*>(param);
            delete label;
        }

        static void DeletePathParam(void* param)
        {
            PathParam* pathParam = static_cast<PathParam*>(param);
            if (pathParam->vertices != nullptr) {
                pathParam->vertices->FreeAll();
                pathParam->vertices = nullptr;
            }
            if (pathParam->imageParam != nullptr) {
                DeleteImageParam(pathParam->imageParam);
            }
            delete pathParam;
            pathParam = nullptr;
        }

        static void DeleteTextParam(void* param)
        {
            TextParam* textParam = static_cast<TextParam*>(param);
            delete textParam;
        }

        static void DoDrawLine(BufferInfo& gfxDstBuffer,
                               void* param,
                               const Paint& paint,
                               const Rect& rect,
                               const Rect& invalidatedArea,
                               const Style& style);
        static void DoDrawCurve(BufferInfo& gfxDstBuffer,
                                void* param,
                                const Paint& paint,
                                const Rect& rect,
                                const Rect& invalidatedArea,
                                const Style& style);
        static void DoDrawRect(BufferInfo& gfxDstBuffer,
                               void* param,
                               const Paint& paint,
                               const Rect& rect,
                               const Rect& invalidatedArea,
                               const Style& style);
        static void DoFillRect(BufferInfo& gfxDstBuffer,
                               void* param,
                               const Paint& paint,
                               const Rect& rect,
                               const Rect& invalidatedArea,
                               const Style& style);
        static void DoDrawCircle(BufferInfo& gfxDstBuffer,
                                 void* param,
                                 const Paint& paint,
                                 const Rect& rect,
                                 const Rect& invalidatedArea,
                                 const Style& style);
        static void DoDrawArc(BufferInfo& gfxDstBuffer,
                              void* param,
                              const Paint& paint,
                              const Rect& rect,
                              const Rect& invalidatedArea,
                              const Style& style);
#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWIMAGE
        static void DoDrawImage(BufferInfo& gfxDstBuffer,
                                void* param,
                                const Paint& paint,
                                const Rect& rect,
                                const Rect& invalidatedArea,
                                const Style& style);
#endif
        static void DoRenderImage(RenderingBuffer& renderBuffer,
                                  const Paint& paint,
                                  const Rect& invalidatedArea,
                                  TransAffine& transform,
                                  RenderingBuffer& imageBuffer);
        static void DoDrawLabel(BufferInfo& gfxDstBuffer,
                                void* param,
                                const Paint& paint,
                                const Rect& rect,
                                const Rect& invalidatedArea,
                                const Style& style);
        static void DoDrawPath(BufferInfo& gfxDstBuffer,
                               void* param,
                               const Paint& paint,
                               const Rect& rect,
                               const Rect& invalidatedArea,
                               const Style& style);
        static void GetAbsolutePosition(const Point& prePoint, const Rect& rect, const Style& style, Point& point);
        static void DoDrawLineJoin(BufferInfo& gfxDstBuffer,
                                   const Point& center,
                                   const Rect& invalidatedArea,
                                   const Paint& paint);

        static void InitRendAndTransform(BufferInfo& gfxDstBuffer, RenderingBuffer& renderBuffer, const Rect& rect,
                                         TransAffine& transform, const Style& style, const Paint& paint);

        static void DoFillPath(BufferInfo& gfxDstBuffer,
                               void* param,
                               const Paint& paint,
                               const Rect& rect,
                               const Rect& invalidatedArea,
                               const Style& style);

        static void SetRasterizer(UICanvasVertices& vertices,
                                  const Paint& paint,
                                  RasterizerScanlineAntiAlias<>& rasterizer,
                                  TransAffine& transform,
                                  const bool& isStroke);

        static void DoRender(BufferInfo& gfxDstBuffer,
                             void* param,
                             const Paint& paint,
                             const Rect& rect,
                             const Rect& invalidatedArea,
                             const Style& style,
                             const bool& isStroke);
#if GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE
        static void DoDrawShadow(BufferInfo& gfxDstBuffer,
                                 void* param,
                                 const Paint& paint,
                                 const Rect& rect,
                                 const Rect& invalidatedArea,
                                 const Style& style,
                                 const bool& isStroke);
#endif

#if GRAPHIC_GEOMETYR_ENABLE_HAMONY_DRAWTEXT
        static void DoDrawText(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
#endif
        /**
         * @brief CopyBuffer 复制buffer
         * @param gfxMapBuffer 复制后的buffer
         * @param gfxDstBuffer 原buffer
         */
        static void CopyBuffer(BufferInfo& gfxMapBuffer, BufferInfo& gfxDstBuffer);

        /**
         * 组装参数设置线宽，LineCap，LineJoin
         */
        template <class LineStyle>
        static void LineStyleCalc(DepictStroke<LineStyle>& strokeLineStyle, const Paint& paint)
        {
            strokeLineStyle.Width(paint.GetStrokeWidth()); // 线条样式相关
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
            strokeLineStyle.LineCap(paint.GetLineCap());
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
            strokeLineStyle.LineJoin(paint.GetLineJoin());
            if (paint.GetMiterLimit() > 0) {
                strokeLineStyle.MiterLimit(paint.GetMiterLimit());
            }
#endif
        };

        /**
         * 设置linedash样式
         */
        template <class LineDashStyle>
        static void LineDashStyleCalc(DepictDash<LineDashStyle>& dashStyle, const Paint& paint)
        {
            for (unsigned int i = 0; i < paint.GetLineDashCount(); i += TWO_STEP) {
                dashStyle.AddDash(paint.GetLineDash()[i], paint.GetLineDash()[i + 1]);
            }
            dashStyle.DashStart(paint.GetLineDashOffset());
        };

        /**
         * 渲染单色的多边形路径和填充
         */
        template <class Pixfmt>
        static void RenderSolid(const Paint& paint,
                                RasterizerScanlineAntiAlias<>& rasterizer,
                                OHOS::RendererBase<Pixfmt>& renBase,
                                const bool& isStroke)
        {
            Scanline scanline;
            Rgba8Color color;
            RenderBlendSolid(paint, color, isStroke);
            RenderScanlinesAntiAliasSolid(rasterizer, scanline, renBase, color);
        }

        static void RenderBlendSolid(const Paint& paint,
                                     Rgba8Color& color,
                                     const bool& isStroke)
        {
            if (isStroke) {
                if (paint.GetStyle() == Paint::STROKE_STYLE ||
                    paint.GetStyle() == Paint::STROKE_FILL_STYLE) {
                    ChangeColor(color, paint.GetStrokeColor(), paint.GetStrokeColor().alpha * paint.GetGlobalAlpha());
                }
            } else {
                if (paint.GetStyle() == Paint::FILL_STYLE ||
                    paint.GetStyle() == Paint::STROKE_FILL_STYLE) {
                    ChangeColor(color, paint.GetFillColor(), paint.GetFillColor().alpha * paint.GetGlobalAlpha());
                }
            }
        }

        static bool isSoild(const Paint& paint)
        {
            if (paint.GetStyle() == Paint::STROKE_STYLE ||
               paint.GetStyle() == Paint::FILL_STYLE ||
               paint.GetStyle() == Paint::STROKE_FILL_STYLE) {
                return true;
            }
            return false;
        }

#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        /**
         * 渲染渐变
         */
        template <class Pixfmt, class color>
        static void RenderGradient(const Paint& paint,
                                   RasterizerScanlineAntiAlias<>& rasterizer,
                                   TransAffine& transform,
                                   OHOS::RendererBase<Pixfmt>& renBase,
                                   RenderingBuffer& renderBuffer,
                                   SpanFillColorAllocator<color>& allocator,
                                   const Rect& invalidatedArea)
        {
            Scanline scanline;

            PixFormat pixFormatComp(renderBuffer);
            RendererBase m_renBaseComp(pixFormatComp);

            m_renBaseComp.ResetClipping(true);
            m_renBaseComp.ClipBox(invalidatedArea.GetLeft(), invalidatedArea.GetTop(),
                                  invalidatedArea.GetRight(), invalidatedArea.GetBottom());
            TransAffine gradientMatrix;
            InterpolatorType interpolatorType(gradientMatrix);
            GradientColorMode gradientColorMode;
            BuildGradientColor(paint, gradientColorMode);
            if (paint.GetGradient() == Paint::Linear) {
                Paint::LinearGradientPoint linearPoint = paint.GetLinearGradientPoint();

                double angle = atan2(linearPoint.y1 - linearPoint.y0, linearPoint.x1 - linearPoint.x0);
                gradientMatrix.Reset();
                gradientMatrix *= OHOS::TransAffineRotation(angle);
                gradientMatrix *= OHOS::TransAffineTranslation(linearPoint.x0, linearPoint.y0);
                gradientMatrix *= transform;
                gradientMatrix.Invert();

                double distance = sqrt((linearPoint.x1 - linearPoint.x0) * (linearPoint.x1 - linearPoint.x0) +
                                       (linearPoint.y1 - linearPoint.y0) * (linearPoint.y1 - linearPoint.y0));
                GradientLinearCalculate gradientLinearCalculate;
                LinearGradientSpan span(interpolatorType, gradientLinearCalculate, gradientColorMode, 0, distance);
                RenderScanlinesAntiAlias(rasterizer, scanline, renBase, allocator, span);
            }

            if (paint.GetGradient() == Paint::Radial) {
                Paint::RadialGradientPoint radialPoint = paint.GetRadialGradientPoint();
                gradientMatrix.Reset();
                gradientMatrix *= OHOS::TransAffineTranslation(radialPoint.x1, radialPoint.y1);
                gradientMatrix *= transform;
                gradientMatrix.Invert();
                double startRadius = radialPoint.r0;
                double endRadius = radialPoint.r1;
                GradientRadialCalculate gradientRadialCalculate(radialPoint.r1, radialPoint.x0 - radialPoint.x1,
                                                                radialPoint.y0 - radialPoint.y1);
                RadialGradientSpan span(interpolatorType, gradientRadialCalculate, gradientColorMode,
                                        startRadius, endRadius);
                RenderScanlinesAntiAlias(rasterizer, scanline, renBase, allocator, span);
            }
        }

        static void BuildGradientColor(const Paint& paint, GradientColorMode& gradientColorMode)
        {
            gradientColorMode.RemoveAll();
            ListNode<Paint::StopAndColor>* iter = paint.getStopAndColor().Begin();
            uint16_t count = 0;
            for (; count < paint.getStopAndColor().Size(); count++) {
                ColorType stopColor = iter->data_.color;
                Srgba8 srgba8Color;
                ChangeColor(srgba8Color, stopColor, stopColor.alpha * paint.GetGlobalAlpha());
                gradientColorMode.AddColor(iter->data_.stop, srgba8Color);
                iter = iter->next_;
            }
            gradientColorMode.BuildLut();
        }

        static void BuildLineGradientMatrix(const Paint& paint,
                                        TransAffine& gradientMatrix,
                                        TransAffine& transform,
                                        double& distance)
        {
            Paint::LinearGradientPoint linearPoint = paint.GetLinearGradientPoint();
            double angle = atan2(linearPoint.y1 - linearPoint.y0, linearPoint.x1 - linearPoint.x0);
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineRotation(angle);
            gradientMatrix *= OHOS::TransAffineTranslation(linearPoint.x0, linearPoint.y0);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            distance = sqrt((linearPoint.x1 - linearPoint.x0) * (linearPoint.x1 - linearPoint.x0) +
                            (linearPoint.y1 - linearPoint.y0) * (linearPoint.y1 - linearPoint.y0));
        }

        static void BuildRadialGradientMatrix(const Paint& paint,
                                        TransAffine& gradientMatrix,
                                        TransAffine& transform,
                                        double& startRadius,
                                        double& endRadius)
        {
            Paint::RadialGradientPoint radialPoint = paint.GetRadialGradientPoint();
            gradientMatrix.Reset();
            gradientMatrix *= OHOS::TransAffineTranslation(radialPoint.x1, radialPoint.y1);
            gradientMatrix *= transform;
            gradientMatrix.Invert();
            startRadius = radialPoint.r0;
            endRadius = radialPoint.r1;
        }

        template <class Pixfmt, class color>
        static void SetGradient(const Paint& paint,
                                   RasterizerScanlineAntiAlias<>& rasterizer,
                                   TransAffine& transform,
                                   OHOS::RendererBase<Pixfmt>& renBase,
                                   RenderingBuffer& renderBuffer,
                                   SpanFillColorAllocator<color>& allocator,
                                   const Rect& invalidatedArea)
        {
            Scanline scanline;
            PixFormat pixFormatComp(renderBuffer);
            RendererBase m_renBaseComp(pixFormatComp);
            m_renBaseComp.ResetClipping(true);
            m_renBaseComp.ClipBox(invalidatedArea.GetLeft(), invalidatedArea.GetTop(),
                                  invalidatedArea.GetRight(), invalidatedArea.GetBottom());
            TransAffine gradientMatrix;
            InterpolatorType interpolatorType(gradientMatrix);
            GradientColorMode gradientColorMode;
            BuildGradientColor(paint, gradientColorMode);
            if (paint.GetGradient() == Paint::Linear) {
                double distance = 0;
                BuildLineGradientMatrix(paint, gradientMatrix, transform, distance);
                GradientLinearCalculate gradientLinearCalculate;
                LinearGradientSpan span(interpolatorType, gradientLinearCalculate, gradientColorMode, 0, distance);
                RenderScanlinesAntiAlias(rasterizer, scanline, renBase, allocator, span);
            }

            if (paint.GetGradient() == Paint::Radial) {
                Paint::RadialGradientPoint radialPoint = paint.GetRadialGradientPoint();
                double startRadius = 0;
                double endRadius = 0;
                BuildRadialGradientMatrix(paint, gradientMatrix, transform, startRadius, endRadius);
                GradientRadialCalculate gradientRadialCalculate(endRadius, radialPoint.x0 - radialPoint.x1,
                                                                radialPoint.y0 - radialPoint.y1);
                RadialGradientSpan span(interpolatorType, gradientRadialCalculate, gradientColorMode,
                                        startRadius, endRadius);
                RenderScanlinesAntiAlias(rasterizer, scanline, renBase, allocator, span);
            }
        }
#endif
        template<class SpanGen, class Pixfmt>
        static void BlendRaster(const Paint& paint,
                                void* param,
                                RasterizerScanlineAntiAlias<>& blendRasterizer,
                                RasterizerScanlineAntiAlias<>& rasterizer,
                                OHOS::RendererBase<Pixfmt>& renBase,
                                TransAffine& transform,
                                SpanGen& spanGen,
                                const Rect& rect,
                                bool isStroke)
        {
            TransAffine gradientMatrixBlend;
            typedef SpanSoildColor<Rgba8Color> SpanSoildColor;

            Scanline scanline1;
            Scanline scanline2;
            SpanAllocator allocator1;
            SpanAllocator allocator2;

            if (isSoild(paint)) {
                Rgba8Color blendColor;
                RenderBlendSolid(paint, blendColor, isStroke);
                SpanSoildColor spanBlendSoildColor(blendColor);
                BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                              scanline1, scanline2, renBase, allocator1, spanBlendSoildColor, allocator2, spanGen);
            }
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
            InterpolatorType interpolatorTypeBlend(gradientMatrixBlend);
            GradientColorMode gradientColorModeBlend;

            if (paint.GetStyle() == Paint::GRADIENT) {
                BuildGradientColor(paint, gradientColorModeBlend);
                if (paint.GetGradient() == Paint::Linear) {
                    double distance = 0;
                    BuildLineGradientMatrix(paint, gradientMatrixBlend, transform, distance);
                    GradientLinearCalculate gradientLinearCalculate;
                    LinearGradientSpan span(interpolatorTypeBlend, gradientLinearCalculate,
                                            gradientColorModeBlend, 0, distance);
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, span, allocator2, spanGen);
                }

                if (paint.GetGradient() == Paint::Radial) {
                    Paint::RadialGradientPoint radialPoint = paint.GetRadialGradientPoint();
                    double startRadius = 0;
                    double endRadius = 0;
                    BuildRadialGradientMatrix(paint, gradientMatrixBlend, transform, startRadius, endRadius);
                    GradientRadialCalculate gradientRadialCalculate(endRadius, radialPoint.x0 - radialPoint.x1,
                                                                    radialPoint.y0 - radialPoint.y1);
                    RadialGradientSpan span(interpolatorTypeBlend, gradientRadialCalculate, gradientColorModeBlend,
                                            startRadius, endRadius);
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, span, allocator2, spanGen);
                }
            }

#endif
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
            if (paint.GetStyle() == Paint::PATTERN) {
                if (param == nullptr) {
                    return;
                }

                PathParam* pathParam = static_cast<PathParam*>(param);

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

                if (paint.GetPatternRepeatMode() == Paint::REPEAT) {
                    ImgSourceTypeRepeat img_src(img_pixf);
                    spanPatternTypeRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, m_spanPatternType, allocator2, spanGen);
                }
                if (paint.GetPatternRepeatMode() == Paint::REPEAT_X) {
                    imgSourceTypeRepeatX img_src(img_pixf);
                    spanPatternTypeRepeatX m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, m_spanPatternType, allocator2, spanGen);
                }
                if (paint.GetPatternRepeatMode() == Paint::REPEAT_Y) {
                    imgSourceTypeRepeatY img_src(img_pixf);
                    spanPatternTypeRepeatY m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, m_spanPatternType, allocator2, spanGen);
                }
                if (paint.GetPatternRepeatMode() == Paint::NO_REPEAT) {
                    imgSourceTypeNoRepeat img_src(img_pixf);
                    spanPatternTypeNoRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                    BlendScanLine(paint.GetGlobalCompositeOperation(), blendRasterizer, rasterizer,
                                  scanline1, scanline2, renBase, allocator1, m_spanPatternType, allocator2, spanGen);
                }
            }
#endif
        }

#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
        /**
         * 渲染Pattern模式
         */
        template <class Pixfmt, class color>
        static void RenderPattern(const Paint& paint,
                                  void* param,
                                  RasterizerScanlineAntiAlias<>& rasterizer,
                                  OHOS::RendererBase<Pixfmt>& renBase,
                                  SpanFillColorAllocator<color>& allocator,
                                  const Rect& rect)
        {
            if (param == nullptr) {
                return;
            }
            ImageParam* imageParam = static_cast<ImageParam*>(param);

            if (imageParam->image == nullptr) {
                return;
            }

            ScanlineUnPackedContainer m_scanline;
            PatternBuffer patternBuffer;
            uint8_t pxSize = DrawUtils::GetPxSizeByColorMode(imageParam->image->GetImageInfo()->header.colorMode);
            patternBuffer.Attach((unsigned char*)imageParam->image->GetImageInfo()->data,
                                 imageParam->width,
                                 imageParam->height,
                                 imageParam->width * (pxSize >> OHOS::PXSIZE2STRIDE_FACTOR));
            PixFormat img_pixf(patternBuffer); // 获取图片

            if (paint.GetPatternRepeatMode() == Paint::REPEAT) {
                ImgSourceTypeRepeat img_src(img_pixf);
                spanPatternTypeRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                RenderScanlinesAntiAlias(rasterizer, m_scanline, renBase, allocator, m_spanPatternType);
            }
            if (paint.GetPatternRepeatMode() == Paint::REPEAT_X) {
                imgSourceTypeRepeatX img_src(img_pixf);
                spanPatternTypeRepeatX m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                RenderScanlinesAntiAlias(rasterizer, m_scanline, renBase, allocator, m_spanPatternType);
            }
            if (paint.GetPatternRepeatMode() == Paint::REPEAT_Y) {
                imgSourceTypeRepeatY img_src(img_pixf);
                spanPatternTypeRepeatY m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                RenderScanlinesAntiAlias(rasterizer, m_scanline, renBase, allocator, m_spanPatternType);
            }
            if (paint.GetPatternRepeatMode() == Paint::NO_REPEAT) {
                imgSourceTypeNoRepeat img_src(img_pixf);
                spanPatternTypeNoRepeat m_spanPatternType(img_src, 0 - rect.GetLeft(), 0 - rect.GetTop());
                RenderScanlinesAntiAlias(rasterizer, m_scanline, renBase, allocator, m_spanPatternType);
            }
        }
#endif
        template <class Color>
        static void ChangeColor(Color& color, ColorType colorType, uint8_t alpha)
        {
            color.redValue = colorType.red;
            color.greenValue = colorType.green;
            color.blueValue = colorType.blue;
            color.alphaValue = alpha;
        }

        void InitGfxMapBuffer(const BufferInfo& srcBuff, const Rect& rect);
        BufferInfo* UpdateMapBufferInfo(const BufferInfo& srcBuff, const Rect& rect);
        void DestroyMapBufferInfo();
    };
} // namespace OHOS
#endif // GRAPHIC_LITE_UI_CANVAS_H
