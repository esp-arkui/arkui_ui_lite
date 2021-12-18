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

#include <fcntl.h>

#include <memory>
#include <stack>

#include "animator/gif_canvas_image_animator.h"
#include "common/image.h"
#include "components/ui_label.h"
#include "draw/clip_utils.h"
#include "engines/gfx/gfx_enginex_manager.h"
#include "gfx_utils/file.h"
#include "gfx_utils/list.h"
#include "ui_image_view.h"
namespace OHOS {
    /**
     * @brief Defines the basic styles of graphs drawn on canvases.
     *
     * @since 1.0
     * @version 1.0
     */

    class GradientControl {
    public:
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
            Solid,
            Linear,
            Radial
        } gradientflag;

        void createLinearGradient(double startx, double starty, double endx, double endy)
        {
            gradientflag = Linear;
            linearGradientPoint_.x0 = startx;
            linearGradientPoint_.y0 = starty;
            linearGradientPoint_.x1 = endx;
            linearGradientPoint_.y1 = endy;
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
        }

        List<StopAndColor> getStopAndColor() const
        {
            return stopAndColors_;
        }

        LinearGradientPoint getLinearGradientPoint() const
        {
            return linearGradientPoint_;
        }

        RadialGradientPoint getRadialGradientPoint() const
        {
            return radialGradientPoint_;
        }

        GradientControl& operator=(GradientControl gradientControl)
        {
            gradientflag = gradientControl.gradientflag;
            linearGradientPoint_ = gradientControl.getLinearGradientPoint();
            radialGradientPoint_ = gradientControl.getRadialGradientPoint();
            stopAndColors_ = gradientControl.getStopAndColor();
            return *this;
        }

    private:
        LinearGradientPoint linearGradientPoint_;
        RadialGradientPoint radialGradientPoint_;
        List<StopAndColor> stopAndColors_;
    };

    class Paint : public HeapBase {
    public:
        /**
         * @brief A constructor used to create a <b>Paint</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        Paint() :
            style_(PaintStyle::STROKE_FILL_STYLE), fillColor_(Color::Black()), strokeColor_(Color::White()),
            opacity_(OPA_OPAQUE), strokeWidth_(2), lineCap_(BaseGfxExtendEngine::LineCap::CAPBUTT),
            lineJoin_(BaseGfxExtendEngine::LineJoin::JOINMITER), miterLimit_(10.0), dashOffset(0.0), isDrawDash(false),
            dashArray(nullptr), ndashes(0), globalAlpha(1.0f), shadowBlurRadius(0), shadowOffsetX(0), shadowOffsetY(0),
            shadowColor(Color::Black()), blendMode(BaseGfxExtendEngine::BlendMode::BLENDSRCOVER), transformCenterX(0),
            transformCenterY(0.0), rotateAngle(0.0), scaleX(0.0), scaleY(0.0), transLateX(0.0), transLateY(0.0)
        {
            m_graphics = nullptr;
            m_transform.Reset();
        }
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
            m_graphics = paint.m_graphics;
            strokeColor_ = paint.strokeColor_;
            opacity_ = paint.opacity_;
            strokeWidth_ = paint.strokeWidth_;
            lineCap_ = paint.lineCap_;
            lineJoin_ = paint.lineJoin_;
            miterLimit_ = paint.miterLimit_;
            globalAlpha = paint.globalAlpha;
            dashOffset = paint.dashOffset;
            isDrawDash = paint.isDrawDash;
            shadowColor = paint.shadowColor;
            shadowOffsetX = paint.shadowOffsetX;
            shadowOffsetY = paint.shadowOffsetY;
            shadowBlurRadius = paint.shadowBlurRadius;
            ndashes = (paint.ndashes + 1) & ~1;
            blendMode = paint.blendMode;
            m_transform = paint.m_transform;

            transformCenterX = paint.transformCenterX;
            transformCenterY = paint.transformCenterY;
            rotateAngle = paint.rotateAngle;
            scaleX = paint.scaleX;
            scaleY = paint.scaleY;
            transLateX = paint.transLateX;
            transLateY = paint.transLateY;
            if (isDrawDash && ndashes > 0) {
                dashArray = new float[ndashes];
                if (dashArray) {
                    if (memset_s(dashArray, ndashes * sizeof(float), 0, ndashes * sizeof(float)) != EOF) {
                    }
                    for (unsigned int i = 0; i < ndashes; i++) {
                        dashArray[i] = paint.dashArray[i];
                    }
                } else {
                    /*memory alloc error, ignore this dash
                     *I think it is never happen.
                     */
                    ndashes = 0;
                    dashOffset = 0;
                    isDrawDash = false;
                }
            } else {
                dashArray = nullptr;
            }
            gradientControl = paint.getGradientControl();
            patternRepeat = paint.patternRepeat;
        }
        const Paint& operator=(const Paint& paint)
        {
            if (dashArray != nullptr) {
                delete[] dashArray;
                dashArray = nullptr;
            }
            Init(paint);
            return *this;
        }
        /**
         * @brief A destructor used to delete the <b>Paint</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual ~Paint()
        {
            if (dashArray != nullptr) {
                delete[] dashArray;
                dashArray = nullptr;
            }
        }

        /**
         * @brief Enumerates paint styles of a closed graph. The styles are invalid for non-closed graphs.
         */
        enum PaintStyle {
            /** Stroke only */
            STROKE_STYLE = 1,
            /** Fill only */
            FILL_STYLE = 2,
            /** Stroke and fill */
            STROKE_FILL_STYLE = 3,
            PATTERN = 4,
            STROKE_GRADIENT = 8,
            FILL_GRADIENT = 16,
        };

        /**
         * @brief 线性渐变所需要的起止点
         */

        /**
         * repeat|repeat-x|repeat-y|no-repeat
         */
        enum PatternRepeat {
            REPEAT,
            REPEAT_X,
            REPEAT_Y,
            NO_REPEAT,
        } patternRepeat;

        const char* image;

        /**
         * @brief Sets the paint style of a closed graph.
         *
         * @param style Indicates the paint style. Stroke and fill are set by default. For details, see {@link
         * PaintStyle}.
         * @see GetStyle
         * @since 1.0
         * @version 1.0
         */
        void SetStyle(PaintStyle style)
        {
            style_ = style;
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
         * @param color Indicates the line color when a line is drawn or the border color when a closed graph is drawn.
         * @see GetStrokeColor
         * @since 1.0
         * @version 1.0
         */
        void SetStrokeColor(ColorType color)
        {
            strokeColor_ = color;
        }

        /**
         * @brief Obtains the color of a line or border.
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
            gradientControl.gradientflag = GradientControl::Solid;
            fillColor_ = color;
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
         * The setting takes effect for the entire graph, including the border, line color, and fill color.
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

        /**
         * @brief 设置路径连接处的尖角的间距限制.
         * @see GetMiterLimit
         * @since 1.0
         * @version 1.0
         */
        void SetMiterLimit(double miterLimit)
        {
            miterLimit_ = miterLimit;
        }

        double GetMiterLimit() const
        {
            return miterLimit_;
        }
        /**
         * @brief 设置笔帽类型.
         * @see GetLineCap
         * @since 1.0
         * @version 1.0
         */
        void SetLineCap(BaseGfxExtendEngine::LineCap lineCap)
        {
            lineCap_ = lineCap;
        }
        /**
         * @brief 获取笔帽类型.
         * @see SetLineCap
         * @since 1.0
         * @version 1.0
         */
        BaseGfxExtendEngine::LineCap GetLineCap() const
        {
            return lineCap_;
        }
        /**
         * @brief 设置笔的路径连接处的风格样式.
         * @see GetLineJoin
         * @since 1.0
         * @version 1.0
         */
        void SetLineJoin(BaseGfxExtendEngine::LineJoin lineJoin)
        {
            lineJoin_ = lineJoin;
        }
        /**
         * @brief 获取笔的路径连接处的风格样式.
         * @see SetLineJoin
         * @since 1.0
         * @version 1.0
         */
        BaseGfxExtendEngine::LineJoin GetLineJoin() const
        {
            return lineJoin_;
        }
        /**
         * @brief 设置点划线的偏移量.
         * @see GetLineDashOffset
         * @since 1.0
         * @version 1.0
         */
        void SetLineDashOffset(float dashOffset)
        {
            this->dashOffset = dashOffset; //dash 点偏移量
        }
        /**
         * @brief 获取点划线的偏移量.
         * @since 1.0
         * @version 1.0
         */
        float GetLineDashOffset() const
        {
            return dashOffset;
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
            ndashes = ndash;
            isDrawDash = true;
            dashArray = new float[ndashes];
            if (dashArray) {
                if (memset_s(dashArray, ndashes * sizeof(float), 0, ndashes * sizeof(float)) != EOF) {
                }
                for (unsigned int iIndex = 0; iIndex < ndashes; iIndex++) {
                    dashArray[iIndex] = lineDashs[iIndex];
                }
            } else {
                // memory alloc error, ignore this dash
                // I think it is never happen.
                ndashes = 0;
                dashOffset = 0;
                isDrawDash = false;
            }
        }
        /**
         * @brief 清空点划线的，改用实现绘制.
         * @since 1.0
         * @version 1.0
         */
        void ClearLineDash(void)
        {
            dashOffset = 0;
            ndashes = 0;
            isDrawDash = false;
            if (dashArray != nullptr) {
                delete[] dashArray;
                dashArray = nullptr;
            }
        }

        bool IsLineDash() const
        {
            return isDrawDash;
        }

        float* GetLineDash() const
        {
            return dashArray;
        }

        unsigned int GetLineDashCount() const
        {
            return ndashes;
        }
        /**
         * @brief 设置全局alpha度.
         * @since 1.0
         * @version 1.0
         */
        void SetGlobalAlpha(float globalAlpha)
        {
            this->globalAlpha = globalAlpha;
        }
        /**
         * @brief 获取全局alpha度.
         * @since 1.0
         * @version 1.0
         */
        float GetGlobalAlpha() const
        {
            return globalAlpha;
        }
        /**
         * @brief 获取阴影模糊半径.
         * @since 1.0
         * @version 1.0
         */
        double GetShadowBlurRadius() const
        {
            return shadowBlurRadius;
        }
        /**
         * @brief 设置阴影模糊半径.
         * @since 1.0
         * @version 1.0
         */
        void SetShadowBlurRadius(double radius)
        {
            shadowBlurRadius = radius;
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
        }

        void SetTransformCenterX(double x)
        {
            transformCenterX = x;
        }

        double GetTransformCenterX() const
        {
            return transformCenterX;
        }

        void SetTransformCenterY(double y)
        {
            transformCenterY = y;
        }

        double GetTransformCenterY() const
        {
            return transformCenterY;
        }

        void SetTransformCenter(double x, double y)
        {
            transformCenterX = x;
            transformCenterY = y;
        }

        double GetRotateAngle() const
        {
            return rotateAngle;
        }
        double GetScaleX() const
        {
            return scaleX;
        }

        double GetScaleY() const
        {
            return scaleY;
        }

        double GetTransLateX() const
        {
            return transLateX;
        }

        double GetTransLateY() const
        {
            return transLateY;
        }

        /*
         * 设置图元混合渲染模式
         * @param BaseGfxExtendEngine::BlendMode 表示图元混合渲染模式
         */
        void SetGlobalCompositeOperation(BaseGfxExtendEngine::BlendMode blendMode)
        {
            this->blendMode = blendMode;
        }

        BaseGfxExtendEngine::BlendMode GetGlobalCompositeOperation() const
        {
            return this->blendMode;
        }
        /*
         * 设置图元用图案填充样式
         * @param img 表示填充的图案，text表示填充样式
         */
        void CreatePattern(const char* img, const char* text)
        {
            image = img;
            patternRepeat = NO_REPEAT;
            if (strcmp(text, "repeat") == 0) {
                patternRepeat = REPEAT;
            } else if (strcmp(text, "repeat-x") == 0) {
                patternRepeat = REPEAT_X;
            } else if (strcmp(text, "repeat-y") == 0) {
                patternRepeat = REPEAT_Y;
            } else if (strcmp(text, "no-repeat") == 0) {
                patternRepeat = NO_REPEAT;
            }
        }

        /*
         * 设置图元填充样式
         * @param GradientControl表示渐变控制器
         */
        void FillStyle(GradientControl& ctrl)
        {
            gradientControl = ctrl;
        }
        /*
         * 设置图元填充样式颜色
         * @param ColorType表示颜色值类型
         */
        void FillStyle(ColorType color)
        {
            SetFillColor(color);
        }

        /* 缩放当前绘图至更大或更小 */
        void Scale(float scaleX, float scaleY)
        {
            this->scaleX += scaleX;
            this->scaleY += scaleY;
            m_transform *= OHOS::TransAffineScaling(scaleX, scaleY);
        }
        /* 重新设置矩阵，形成单位阵 */
        void ResetTransForm()
        {
            rotateAngle = 0;
            this->scaleX = 0;
            this->scaleY = 0;
            transLateX = 0;
            transLateY = 0;
            transformCenterX = 0;
            transformCenterY = 0;
            m_transform.Reset();
        }

        /* 旋转当前绘图 */
        void Rotate(float angle)
        {
            rotateAngle += angle;
            m_transform *= OHOS::TransAffineRotation(BaseGfxExtendEngine::Deg2Rad(angle));
        }

        /* 重新映射画布上的 (x,y) 位置 */
        void Translate(int16_t x, int16_t y)
        {
            transLateX += x;
            transLateY += y;
            m_transform *= OHOS::TransAffineTranslation(x, y);
        }

        /* 替换绘图的当前转换矩阵 */
        void Transform(float sx, float shy, float shx, float sy, float tx, float ty)
        {
            m_transform = OHOS::TransAffine(sx, shy, shx, sy, tx, ty);
        }

        /* 获取当前变换矩阵 */
        const OHOS::TransAffine& GetTransform() const
        {
            return m_transform;
        }

        /* 将当前转换重置为单位矩阵。然后运行 transform() */
        void SetTransform(float sx, float shy, float shx, float sy, float tx, float ty)
        {
            m_transform.Reset();
            Transform(sx, shy, shx, sy, tx, ty);
        }
        /* 是否经过变换，即是不是单位矩阵 */
        bool IsTransform() const
        {
            return !m_transform.IsIdentity();
        }

        GradientControl getGradientControl() const
        {
            return gradientControl;
        }

        BaseGfxExtendEngine* GetDrawGraphicsContext() const
        {
            return m_graphics;
        }
        void SetDrawGraphicsContext(BaseGfxExtendEngine* m_graphics)
        {
            this->m_graphics = m_graphics;
        }

    private:
        PaintStyle style_;
        ColorType fillColor_;
        ColorType strokeColor_;
        uint8_t opacity_;
        uint16_t strokeWidth_;                   //设置线宽
        BaseGfxExtendEngine::LineCap lineCap_;   //设置笔帽
        BaseGfxExtendEngine::LineJoin lineJoin_; //设置笔的路径连接处的风格样式
        double miterLimit_;                      //设置路径连接处的尖角的间距限制
        float dashOffset;                        //dash 点偏移量
        bool isDrawDash;
        float* dashArray; //dash 点数组
        unsigned int ndashes;
        float globalAlpha;                        //设置图元全局alpha
        double shadowBlurRadius;                  //设置阴影模糊半径
        double shadowOffsetX;                     //设置阴影横坐标偏移量
        double shadowOffsetY;                     //设置阴影纵坐标偏移量
        ColorType shadowColor;                    //设置阴影色彩
        BaseGfxExtendEngine::BlendMode blendMode; //设置多图元混合渲染模式
        BaseGfxExtendEngine* m_graphics;
        /* 用于操作变换矩阵 */
        OHOS::TransAffine m_transform;
        GradientControl gradientControl;
        double transformCenterX;
        double transformCenterY;
        double rotateAngle;
        double scaleX;
        double scaleY;
        double transLateX;
        double transLateY;
    };

    /**
     * @brief Defines a canvas, which is used to draw multiple types of 2D graphs.
     *
     * @since 1.0
     * @version 1.0
     */
    class UICanvas : public UIView {
    public:
        /**
         * @brief A constructor used to create a <b>UICanvas</b> instance.
         *
         * @since 1.0
         * @version 1.0
         */
        UICanvas() :
            startPoint_({0, 0}), path_(nullptr)
        {
        }

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
        void DrawCurve(const Point& startPoint, const Point& control1, const Point& control2, const Point& endPoint,
                       const Paint& paint);

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
         * @brief Draws a rectangle not fill.
         *
         * @param startPoint Indicates the coordinates of the point at the upper left corner of the rectangle.
         * @param height     Indicates the height of the rectangle.
         * @param width      Indicates the width of the rectangle.
         * @param paint      Indicates the rectangle style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void StrokeRect(const Point& startPoint, int16_t height, int16_t width, const Paint& paint);
        /**
         * @brief Clear a rectangle not fill.
         *
         * @param startPoint Indicates the coordinates of the point at the upper left corner of the rectangle.
         * @param height     Indicates the height of the rectangle.
         * @param width      Indicates the width of the rectangle.
         * @param paint      Indicates the rectangle style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void ClearRect(const Point& clearRect, int clearHeight, int clearWidth, const Paint& paint);
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

        /**
         * @brief Draws an image.
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @param image      Indicates the pointer to the image source.
         * @param paint      Indicates the image style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawImage(const Point& startPoint, const char* image, const Paint& paint,int16_t sizeWidth = -1, int16_t sizeHeight = -1);

        /**
         * @brief Draws an image.
         *
         * @param startPoint Indicates the coordinates of the start point.
         * @param image      Indicates the pointer to the image source.
         * @param paint      Indicates the image style. For details, see {@link Paint}.
         * @since 1.0
         * @version 1.0
         */
        void DrawImage(const Point& startPoint, const ImageInfo* image, const Paint& paint);

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

        /** 点精度double */
        struct PointD {
            double x;

            double y;
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
         * @param startAngle Indicates the start angle of the arc. The value <b>0</b> indicates the 12-o'clock
         * direction, and <b>90</b> indicates the 3-o'clock direction.
         * @param endAngle   Indicates the end angle of the arc. The value <b>0</b> indicates the 12-o'clock direction,
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

        void AddRect(const Point& point, int16_t height, int16_t width, const Paint& paint);
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
         * @brief FIlls this path.
         *
         * @param paint Indicates the path style. For details, see {@link Paint}.
         * @since 3.0
         * @version 5.0
         */
        void FillPath(const Paint& paint);
        void OnDraw(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea) override;
        /**
         * @brief 设置点划线的数组和数量.
         * @param lineDashs 表示点划线数组,ndash 表示点划线数量
         * @since 1.0
         * @version 1.0
         */
        void SetLineDash(float* dashArray, unsigned int ndash, Paint&);
        /**
         * @brief 设置全局alpha.
         * @since 1.0
         * @version 1.0
         */
        void GlobalAlpha(float globalAlpha, Paint& paint)
        {
            if (globalAlpha < 0.0f) {
                globalAlpha = 0.0f;
            } else if (globalAlpha > 1.0f) {
                globalAlpha = 1.0f;
            }
            paint.SetGlobalAlpha(globalAlpha);
        }
        /**
         * @brief 获取点划线的数组和数量.
         * @since 1.0
         * @version 1.0
         */
        float* GetLineDash(const Paint& paint, unsigned& nDashes)
        {
            nDashes = paint.GetLineDashCount();
            return paint.GetLineDash();
        }
        /**
         * @brief 设置点划线的偏移量.
         * @param dashOffset 表示点划线偏移量
         * @since 1.0
         * @version 1.0
         */
        void LineDashOffset(float dashOffset, Paint& paint)
        {
            paint.SetLineDashOffset(dashOffset);
        }
        /**
         * @brief 设置线的宽度.
         * @param lineWidth 表示线的宽度
         * @since 1.0
         * @version 1.0
         */
        void LineWidth(uint16_t lineWidth, Paint& paint)
        {
            paint.SetStrokeWidth(lineWidth);
        }

        /**
         * @brief 填充渐变和pattern
         */
        void Fill(const Paint& paint);
        /**
         * @brief 绘制路径渐变和pattern模式
         */
        void Stroke(const Paint& paint);
        /**
         * @brief 根据设置模式去绘制路径或者填充
         */
        void Gradient(const Paint& paint);
        /**
         * @brief 根据pattern模式填充路径绘制的区域
         */
        void FillPattern(const Paint& paint);
        /**
         * @brief 根据pattern模式填充绘制的路径
         */
        void StrokePattern(const Paint& paint);

        /*  在画布上绘制文本 */
        void StrokeText(const char* text, const Point& point, const FontStyle& fontStyle, const Paint& paint);

        /* 缩放当前绘图至更大或更小 */
        void SetScale(float x, float y, Paint& paint)
        {
            paint.Scale(x, y);
        }

        double GetTransformCenterX(const Paint& paint) const
        {
            return paint.GetTransformCenterX();
        }

        double GetTransformCenterY(const Paint& paint) const
        {
            return paint.GetTransformCenterY();
        }

        void SetTransformCenter(double x, double y, Paint& paint)
        {
            paint.SetTransformCenter(x, y);
        }
        /* 旋转当前绘图 */
        void SetRotate(float angle, Paint& paint)
        {
            paint.Rotate(angle);
        }

        /* 重新映射画布上的 (x,y) 位置 */
        void SetTranslate(int16_t x, int16_t y, Paint& paint)
        {
            paint.Translate(x, y);
        }
        /* 重新设置矩阵，形成单位阵 */
        void ResetTransForm(Paint& paint)
        {
            paint.ResetTransForm();
        }
        /* 替换绘图的当前转换矩阵 */
        void Transform(float sx, float shy, float shx, float sy, float tx, float ty, Paint& paint)
        {
            paint.Transform(sx, shy, shx, sy, tx, ty);
        }

        /* 获取当前变换矩阵 */
        const OHOS::TransAffine& GetTransform(const Paint& paint) const
        {
            return paint.GetTransform();
        }

        /* 将当前转换重置为单位矩阵。然后运行 transform() */
        void SetTransform(float sx, float shy, float shx, float sy, float tx, float ty, Paint& paint)
        {
            paint.SetTransform(sx, shy, shx, sy, tx, ty);
        }
        /**
         * @brief 设置点划线的偏移量.
         * @see GetLineDashOffset
         * @since 1.0
         * @version 1.0
         */
        void SetLineDashOffset(float dashOffset, Paint& paint)
        {
            //m_graphics->SetLineDashOffset(dashOffset);
            paint.SetLineDashOffset(dashOffset);
        }
        /**
         * @brief 获取点划线的偏移量.
         * @since 1.0
         * @version 1.0
         */
        float GetLineDashOffset(const Paint& paint) const
        {
            //return m_graphics->GetLineDashOffset();
            return paint.GetLineDashOffset();
        }
        void SetDrawGraphicsContext(Paint& paint)
        {
            paint.SetDrawGraphicsContext(&this->m_graphics);
        }
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
        struct Images_ {
            Point startp;
            UIImageView* img;
        };

    protected:
        bool InitDrawEnvironment(const BufferInfo& gfxDstBuffer, const Rect& fillArea, const Rect& worldRect,
                                 const Rect& screenRect);

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

        struct TextParam : public HeapBase {
            const char* text;
            Point position;
            Color32 fontColor;
            uint8_t fontOpa;
            FontStyle fontStyle;
            Text* textComment;
            TransformMap drawTransMap;
            bool isDrawTrans;

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
        enum PathCmd {
            CMD_MOVE_TO,
            CMD_LINE_TO,
            CMD_ARC,
            CMD_CLOSE,
        };

        class UICanvasPath : public HeapBase {
        public:
            UICanvasPath() :
                startPos_({0, 0}), strokeCount_(0){};
            ~UICanvasPath();
            List<Point> points_;
            List<PathCmd> cmd_;
            List<ArcParam> arcParam_;
            Point startPos_;
            uint16_t strokeCount_;
        };

        struct PathParam : public HeapBase {
            UICanvasPath* path;
            uint16_t count;
        };

        struct DrawCmd : public HeapBase {
            Paint paint;
            void* param;
            void (*DrawGraphics)(BufferInfo&, void*, const Paint&, const Rect&, const Rect&, const Style&);
            void (*DeleteParam)(void*);
        };

        List<Images_> imageList_;
        Point startPoint_;
        UICanvasPath* path_;
        List<DrawCmd> drawCmdList_;
        // 保存Paint的历史修改信息
        std::stack<Paint> PaintStack;
        BaseGfxExtendEngine m_graphics;
        static void DeleteTextParam(void* param)
        {
            TextParam* textParam = static_cast<TextParam*>(param);
            delete textParam;
        }
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

        static void DeletePairRectParam(void* param)
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
            if(imageParam->path != nullptr){
                delete[] imageParam->path;
                imageParam->path = nullptr;
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
            pathParam->path->strokeCount_--;
            if (pathParam->path->strokeCount_ == 0) {
                delete pathParam->path;
            }
            delete pathParam;
        }

        static void DoDrawLine(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        static void DoDrawCurve(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style);

        static void DoClearRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style);

        static void DoStrokeRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                 const Rect& invalidatedArea, const Style& style);

        static void DoDrawRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        static void DoFillRect(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        static void DoDrawCircle(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                 const Rect& invalidatedArea, const Style& style);
        static void DoDrawArc(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                              const Rect& invalidatedArea, const Style& style);
        static void DoDrawImage(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style);

        /* 绘制图元时，开始执行变换操作 */
        static void StartTransform(const Rect& rect, const Rect& invalidatedArea, const Paint& paint);

        static void DoDrawPattern(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                  const Rect& invalidatedArea, const Style& style);
        static void DoStrokePattern(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                    const Rect& invalidatedArea, const Style& style);
        static void DoDrawLabel(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                                const Rect& invalidatedArea, const Style& style);

        static void DoDrawText(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        /* 返回包含指定文本宽度的对象 */
        Point MeasureText(const char* text, const FontStyle& fontStyle, const Paint& paint);
        static void DoDrawPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        static void DoFillPath(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);
        static void GetAbsolutePosition(const Point& prePoint, const Rect& rect, const Style& style, Point& point);
        static void DoDrawLineJoin(BufferInfo& gfxDstBuffer, const Point& center, const Rect& invalidatedArea,
                                   const Paint& paint);

        static void addColorGradient(BaseGfxExtendEngine& m_graphics,
                                     List<GradientControl::StopAndColor>& stopAndColors);

        static void setGradient(BaseGfxExtendEngine& m_graphics, const Paint& paint, const Rect& rect,
                                const Style& style);
        static void DoGradient(BufferInfo& gfxDstBuffer, void* param, const Paint& paint, const Rect& rect,
                               const Rect& invalidatedArea, const Style& style);

        static void BlitMapBuffer(BufferInfo& gfxDstBuffer, BufferInfo& gfxMapBuffer, Rect& curViewRect,
                                  TransformMap& transMap, const Rect& invalidatedArea);
        static bool IsGif(const char* src);
    };
} // namespace OHOS
#endif // GRAPHIC_LITE_UI_CANVAS_H
