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
* @addtogroup GraphicGeometry
* @{
*
* @brief Defines .
*
* @since 1.0
* @version 1.0
*/

/**
* @file graphic_geometry_pixfmt_rgba.h
*
* @brief Defines 颜色分量gamma操作类.
*
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_COMP_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_COMP_INCLUDED

#include <cmath>
#include <cstring>

#include "gfx_utils/heap_base.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_pixfmt_rgba_blend.h"
#include "render/graphic_render_pixfmt_rgba_multi.h"
namespace OHOS {
    /**
     * @brief 求两个数的最小值.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    inline T SdMin(T val1, T val2)
    {
        return (val1 < val2) ? val1 : val2;
    }

    /**
     * @brief 求两个数的最大值.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    inline T SdMax(T val1, T val2)
    {
        return (val1 > val2) ? val1 : val2;
    }

    template <class ColorT, class Order>
    struct CompOpRgbaSrc : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            if (cover >= COVER_FULL) {
                SetBlendColor(pColor, r, g, b, a);
            } else {
                Rgba s = GetBlendColor(r, g, b, a, cover);
                Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
                d.redValue += s.redValue;
                d.greenValue += s.greenValue;
                d.blueValue += s.blueValue;
                d.alphaValue += s.alphaValue;
                SetBlendColor(pColor, d);
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaMinus : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba srcColor = GetBlendColor(r, g, b, a, cover);
            if (srcColor.alphaValue > 0) {
                Rgba destColor = GetBlendColor(pColor);
                destColor.alphaValue += srcColor.alphaValue - srcColor.alphaValue * destColor.alphaValue;
                destColor.redValue = SdMax(destColor.redValue - srcColor.redValue, 0.0f);
                destColor.greenValue = SdMax(destColor.greenValue - srcColor.greenValue, 0.0f);
                destColor.blueValue = SdMax(destColor.blueValue - srcColor.blueValue, 0.0f);
                SetBlendColor(pColor, Clip(destColor));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaScreen : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                d.redValue += s.redValue - s.redValue * d.redValue;
                d.greenValue += s.greenValue - s.greenValue * d.greenValue;
                d.blueValue += s.blueValue - s.blueValue * d.blueValue;
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaOverlay : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        static GRAPHIC_GEOMETRY_INLINE double Calc(
            double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
        {
            return (2 * dca <= da) ?
                       2 * sca * dca + sca * d1a + dca * s1a :
                       sada - 2 * (da - dca) * (sa - sca) + sca * d1a + dca * s1a;
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                double d1a = 1 - d.alphaValue;
                double s1a = 1 - s.alphaValue;
                double sada = s.alphaValue * d.alphaValue;
                d.redValue = Calc(d.redValue, s.redValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.greenValue = Calc(d.greenValue, s.greenValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.blueValue = Calc(d.blueValue, s.blueValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDarken : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                double d1a = 1 - d.alphaValue;
                double s1a = 1 - s.alphaValue;
                d.redValue = SdMin(s.redValue * d.alphaValue, d.redValue * s.alphaValue) +
                             s.redValue * d1a + d.redValue * s1a;
                d.greenValue = SdMin(s.greenValue * d.alphaValue, d.greenValue * s.alphaValue) +
                               s.greenValue * d1a + d.greenValue * s1a;
                d.blueValue = SdMin(s.blueValue * d.alphaValue, d.blueValue * s.alphaValue) +
                              s.blueValue * d1a + d.blueValue * s1a;
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaLighten : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                double d1a = 1 - d.alphaValue;
                double s1a = 1 - s.alphaValue;
                d.redValue = SdMax(s.redValue * d.alphaValue, d.redValue * s.alphaValue) +
                             s.redValue * d1a + d.redValue * s1a;
                d.greenValue = SdMax(s.greenValue * d.alphaValue, d.greenValue * s.alphaValue) +
                               s.greenValue * d1a + d.greenValue * s1a;
                d.blueValue = SdMax(s.blueValue * d.alphaValue, d.blueValue * s.alphaValue) +
                              s.blueValue * d1a + d.blueValue * s1a;
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaColorDodge : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        static GRAPHIC_GEOMETRY_INLINE double Calc(
            double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
        {
            if (sca < sa) {
                return sada * SdMin(1.0, (dca / da) * sa / (sa - sca)) + sca * d1a + dca * s1a;
            }
            if (dca > 0) {
                return sada + sca * d1a + dca * s1a;
            }
            return sca * d1a;
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                if (d.alphaValue > 0) {
                    double sada = s.alphaValue * d.alphaValue;
                    double s1a = 1 - s.alphaValue;
                    double d1a = 1 - d.alphaValue;
                    d.redValue = Calc(d.redValue, s.redValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.greenValue = Calc(d.greenValue, s.greenValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.blueValue = Calc(d.blueValue, s.blueValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                    SetBlendColor(pColor, Clip(d));
                } else {
                    SetBlendColor(pColor, s);
                }
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaColorBurn : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        static GRAPHIC_GEOMETRY_INLINE double Calc(
            double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
        {
            if (sca > 0) {
                return sada * (1 - SdMin(1.0, (1 - dca / da) * sa / sca)) + sca * d1a + dca * s1a;
            }
            if (dca > da) {
                return sada + dca * s1a;
            }
            return dca * s1a;
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                if (d.alphaValue > 0) {
                    double sada = s.alphaValue * d.alphaValue;
                    double s1a = 1 - s.alphaValue;
                    double d1a = 1 - d.alphaValue;
                    d.redValue = Calc(d.redValue, s.redValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.greenValue = Calc(d.greenValue, s.greenValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.blueValue = Calc(d.blueValue, s.blueValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.alphaValue += s.alphaValue - sada;
                    SetBlendColor(pColor, Clip(d));
                } else {
                    SetBlendColor(pColor, s);
                }
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaHardLight : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        static GRAPHIC_GEOMETRY_INLINE double Calc(
            double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
        {
            return (2 * sca < sa) ?
                       2 * sca * dca + sca * d1a + dca * s1a :
                       sada - 2 * (da - dca) * (sa - sca) + sca * d1a + dca * s1a;
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                double d1a = 1 - d.alphaValue;
                double s1a = 1 - s.alphaValue;
                double sada = s.alphaValue * d.alphaValue;
                d.redValue = Calc(d.redValue, s.redValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.greenValue = Calc(d.greenValue, s.greenValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.blueValue = Calc(d.blueValue, s.blueValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                d.alphaValue += s.alphaValue - sada;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSoftLight : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        static GRAPHIC_GEOMETRY_INLINE double Calc(
            double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
        {
            double dcasa = dca * sa;
            if (2 * sca <= sa) {
                return dcasa - (sada - 2 * sca * da) * dcasa * (sada - dcasa) + sca * d1a + dca * s1a;
            }
            if (4 * dca <= da) {
                return dcasa + (2 * sca * da - sada) * ((((16 * dcasa - 12) * dcasa + 4) * dca * da) - dca * da) +
                       sca * d1a + dca * s1a;
            }
            return dcasa + (2 * sca * da - sada) * (std::sqrt(dcasa) - dcasa) + sca * d1a + dca * s1a;
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                if (d.alphaValue > 0) {
                    double sada = s.alphaValue * d.alphaValue;
                    double s1a = 1 - s.alphaValue;
                    double d1a = 1 - d.alphaValue;
                    d.redValue = Calc(d.redValue, s.redValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.greenValue = Calc(d.greenValue, s.greenValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.blueValue = Calc(d.blueValue, s.blueValue, d.alphaValue, s.alphaValue, sada, d1a, s1a);
                    d.alphaValue += s.alphaValue - sada;
                    SetBlendColor(pColor, Clip(d));
                } else {
                    SetBlendColor(pColor, s);
                }
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDifference : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                d.redValue += s.redValue - 2 * SdMin(s.redValue * d.alphaValue, d.redValue * s.alphaValue);
                d.greenValue += s.greenValue - 2 * SdMin(s.greenValue * d.alphaValue, d.greenValue * s.alphaValue);
                d.blueValue += s.blueValue - 2 * SdMin(s.blueValue * d.alphaValue, d.blueValue * s.alphaValue);
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaExclusion : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                double d1a = 1 - d.alphaValue;
                double s1a = 1 - s.alphaValue;
                d.redValue = (s.redValue * d.alphaValue + d.redValue * s.alphaValue - 2 * s.redValue * d.redValue) +
                             s.redValue * d1a + d.redValue * s1a;
                d.greenValue = (s.greenValue * d.alphaValue + d.greenValue * s.alphaValue -
                                2 * s.greenValue * d.greenValue) +
                               s.greenValue * d1a + d.greenValue * s1a;
                d.blueValue = (s.blueValue * d.alphaValue + d.blueValue * s.alphaValue -
                               2 * s.blueValue * d.blueValue) +
                              s.blueValue * d1a + d.blueValue * s1a;
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaPlus : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                d.alphaValue = SdMin(d.alphaValue + s.alphaValue, 1.0f);
                d.redValue = SdMin(d.redValue + s.redValue, d.alphaValue);
                d.greenValue = SdMin(d.greenValue + s.greenValue, d.alphaValue);
                d.blueValue = SdMin(d.blueValue + s.blueValue, d.alphaValue);
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaXor : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            double s1a = 1 - s.alphaValue;
            double d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.redValue = s.redValue * d1a + d.redValue * s1a;
            d.greenValue = s.greenValue * d1a + d.greenValue * s1a;
            d.blueValue = s.blueValue * d1a + d.blueValue * s1a;
            d.alphaValue = s.alphaValue + d.alphaValue - 2 * s.alphaValue * d.alphaValue;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDstAtop : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba sc = GetBlendColor(r, g, b, a, cover);
            Rgba dc = GetBlendColor(pColor, cover);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            double sa = ColorT::ToFloat(a);
            double d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.redValue += dc.redValue * sa + sc.redValue * d1a;
            d.greenValue += dc.greenValue * sa + sc.greenValue * d1a;
            d.blueValue += dc.blueValue * sa + sc.blueValue * d1a;
            d.alphaValue += sc.alphaValue;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcAtop : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            double s1a = 1 - s.alphaValue;
            d.redValue = s.redValue * d.alphaValue + d.redValue * s1a;
            d.greenValue = s.greenValue * d.alphaValue + d.greenValue * s1a;
            d.blueValue = s.blueValue * d.alphaValue + d.greenValue * s1a;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstOut : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType, ValueType, ValueType, ValueType a, CoverType cover)
        {
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            Rgba dc = GetBlendColor(pColor, cover);
            double s1a = 1 - ColorT::ToFloat(a);
            d.redValue += dc.redValue * s1a;
            d.greenValue += dc.greenValue * s1a;
            d.blueValue += dc.blueValue * s1a;
            d.alphaValue += dc.alphaValue * s1a;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcOut : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            double d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.alphaValue += s.alphaValue * d1a;
            d.redValue += s.redValue * d1a;
            d.greenValue += s.greenValue * d1a;
            d.blueValue += s.blueValue * d1a;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstIn : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType, ValueType, ValueType, ValueType a, CoverType cover)
        {
            double sa = ColorT::ToFloat(a);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            Rgba d2 = GetBlendColor(pColor, cover);
            d.redValue += d2.redValue * sa;
            d.greenValue += d2.greenValue * sa;
            d.blueValue += d2.blueValue * sa;
            d.alphaValue += d2.alphaValue * sa;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaSrcIn : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            double da = ColorT::ToFloat(pColor[Order::ALPHA]);
            if (da > 0) {
                Rgba s = GetBlendColor(r, g, b, a, cover);
                Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
                d.redValue += s.redValue * da;
                d.greenValue += s.greenValue * da;
                d.blueValue += s.blueValue * da;
                d.alphaValue += s.alphaValue * da;
                SetBlendColor(pColor, d);
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcOver : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0.
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            BlenderRgbaPre<ColorT, Order>::BlendPix(pColor, r, g, b, a, cover);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstOver : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            double d1a = 1 - d.alphaValue;
            d.redValue += s.redValue * d1a;
            d.greenValue += s.greenValue * d1a;
            d.blueValue += s.blueValue * d1a;
            d.alphaValue += s.alphaValue * d1a;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDst : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType*, ValueType, ValueType, ValueType, ValueType, CoverType)
        {
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaClear : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* p, ValueType, ValueType, ValueType, ValueType, CoverType cover)
        {
            if (cover >= COVER_FULL) {
                p[0] = p[1] = p[2] = p[3] = ColorType::EmptyValue();
            } else if (cover > COVER_NONE) {
                SetBlendColor(p, GetBlendColor(p, COVER_FULL - cover));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpTableRgba {
        using ValueType = typename ColorT::ValueType;
        using CalcType = typename ColorT::CalcType;
        using CompOpFuncType =
            void (*)(ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType ca, CoverType cover);
        static CompOpFuncType g_compOpFunc[];
    };

    template <class ColorT, class Order>
    typename CompOpTableRgba<ColorT, Order>::CompOpFuncType CompOpTableRgba<ColorT, Order>::g_compOpFunc[] = {
        CompOpRgbaClear<ColorT, Order>::BlendPix,
        CompOpRgbaSrc<ColorT, Order>::BlendPix,
        CompOpRgbaDst<ColorT, Order>::BlendPix,
        CompOpRgbaSrcOver<ColorT, Order>::BlendPix,
        CompOpRgbaDstOver<ColorT, Order>::BlendPix,
        CompOpRgbaSrcIn<ColorT, Order>::BlendPix,
        CompOpRgbaDstIn<ColorT, Order>::BlendPix,
        CompOpRgbaSrcOut<ColorT, Order>::BlendPix,
        CompOpRgbaDstOut<ColorT, Order>::BlendPix,
        CompOpRgbaSrcAtop<ColorT, Order>::BlendPix,
        CompOpRgbaDstAtop<ColorT, Order>::BlendPix,
        CompOpRgbaXor<ColorT, Order>::BlendPix,
        CompOpRgbaPlus<ColorT, Order>::BlendPix,
        CompOpRgbaMultiply<ColorT, Order>::BlendPix,
        CompOpRgbaScreen<ColorT, Order>::BlendPix,
        CompOpRgbaOverlay<ColorT, Order>::BlendPix,
        CompOpRgbaDarken<ColorT, Order>::BlendPix,
        CompOpRgbaLighten<ColorT, Order>::BlendPix,
        CompOpRgbaColorDodge<ColorT, Order>::BlendPix,
        CompOpRgbaColorBurn<ColorT, Order>::BlendPix,
        CompOpRgbaHardLight<ColorT, Order>::BlendPix,
        CompOpRgbaSoftLight<ColorT, Order>::BlendPix,
        CompOpRgbaDifference<ColorT, Order>::BlendPix,
        CompOpRgbaExclusion<ColorT, Order>::BlendPix,
        0};

    template <class ColorT, class Order>
    struct CompOpAdaptorRgba {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            unsigned op, ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor,
                                                             ColorType::Multiply(r, a),
                                                             ColorType::Multiply(g, a),
                                                             ColorType::Multiply(b, a),
                                                             a, cover);
        }
    };

    template <class ColorT, class Order>
    struct CompOpAdaptorClipToDstRgba {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
            BlendPix(unsigned op, ValueType* pColor,
                     ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            r = ColorType::Multiply(r, a);
            g = ColorType::Multiply(g, a);
            b = ColorType::Multiply(b, a);
            ValueType da = pColor[Order::A];
            CompOpTableRgba<ColorT, Order>::gCompOpFunc[op](pColor,
                                                            ColorType::Multiply(r, da),
                                                            ColorType::Multiply(g, da),
                                                            ColorType::Multiply(b, da),
                                                            ColorType::Multiply(a, da), cover);
        }
    };

    template <class ColorT, class Order>
    struct CompOpAdaptorRgbaPre {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
            BlendPix(unsigned op, ValueType* pColor,
                     ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor, r, g, b, a, cover);
        }
    };

    template <class ColorT, class Order>
    struct CompOpAdaptorClipToDstRgbaPre {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
            BlendPix(unsigned op, ValueType* pColor,
                     ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
        {
            ValueType da = pColor[Order::ALPHA];
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor,
                                                             ColorType::Multiply(r, da),
                                                             ColorType::Multiply(g, da),
                                                             ColorType::Multiply(b, da),
                                                             ColorType::Multiply(a, da), cover);
        }
    };

} // namespace OHOS
#endif
