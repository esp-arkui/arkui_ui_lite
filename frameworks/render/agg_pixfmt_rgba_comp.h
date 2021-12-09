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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_MULTI_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_MULTI_INCLUDED

#include <cmath>
#include <cstring>

#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"
#include "heap_base.h"
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
/**
 * @brief 颜色分量裁剪.
 *
 * @since 1.0
 * @version 1.0
 */
inline Rgba& Clip(Rgba& color)
{
    if (color.a > 1) {
        color.a = 1;
    } else if (color.a < 0) {
        color.a = 0;
    }

    if (color.b > color.a) {
        color.b = color.a;
    } else if (color.b < 0) {
        color.b = 0;
    }

    if (color.r > color.a) {
        color.r = color.a;
    } else if (color.r < 0) {
        color.r = 0;
    }

    if (color.g > color.a) {
        color.g = color.a;
    } else if (color.g < 0) {
        color.g = 0;
    }
    return color;
}



template <class ColorT, class Order>
struct CompOpRgbaSrc : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;
    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        if (cover >= COVER_FULL) {
            Set(pColor, r, g, b, a);
        } else {
            Rgba s = Get(r, g, b, a, cover);
            Rgba d = Get(p, COVER_FULL - cover);
            d.r += s.r;
            d.g += s.g;
            d.b += s.b;
            d.a += s.a;
            Set(pColor, d);
        }
    }
};



template <class ColorT, class Order>
struct CompOpRgbaMinus : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            d.a += s.a - s.a * d.a;
            d.r = SdMax(d.r - s.r, 0.0);
            d.g = SdMax(d.g - s.g, 0.0);
            d.b = SdMax(d.b - s.b, 0.0);
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaScreen : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            d.r += s.r - s.r * d.r;
            d.g += s.g - s.g * d.g;
            d.b += s.b - s.b * d.b;
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};


template <class ColorT, class Order>
struct CompOpRgbaOverlay : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE double Calc(double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
    {
        return (2 * dca <= da) ? 2 * sca * dca + sca * d1a + dca * s1a : sada - 2 * (da - dca) * (sa - sca) + sca * d1a + dca * s1a;
    }

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            double d1a = 1 - d.a;
            double s1a = 1 - s.a;
            double sada = s.a * d.a;
            d.r = Calc(d.r, s.r, d.a, s.a, sada, d1a, s1a);
            d.g = Calc(d.g, s.g, d.a, s.a, sada, d1a, s1a);
            d.b = Calc(d.b, s.b, d.a, s.a, sada, d1a, s1a);
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDarken : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            double d1a = 1 - d.a;
            double s1a = 1 - s.a;
            d.r = sd_min(s.r * d.a, d.r * s.a) + s.r * d1a + d.r * s1a;
            d.g = sd_min(s.g * d.a, d.g * s.a) + s.g * d1a + d.g * s1a;
            d.b = sd_min(s.b * d.a, d.b * s.a) + s.b * d1a + d.b * s1a;
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaLighten : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            double d1a = 1 - d.a;
            double s1a = 1 - s.a;
            d.r = sd_max(s.r * d.a, d.r * s.a) + s.r * d1a + d.r * s1a;
            d.g = sd_max(s.g * d.a, d.g * s.a) + s.g * d1a + d.g * s1a;
            d.b = sd_max(s.b * d.a, d.b * s.a) + s.b * d1a + d.b * s1a;
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaColorDodge : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE double Calc(double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
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
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            if (d.a > 0) {
                double sada = s.a * d.a;
                double s1a = 1 - s.a;
                double d1a = 1 - d.a;
                d.r = Calc(d.r, s.r, d.a, s.a, sada, d1a, s1a);
                d.g = Calc(d.g, s.g, d.a, s.a, sada, d1a, s1a);
                d.b = Calc(d.b, s.b, d.a, s.a, sada, d1a, s1a);
                d.a += s.a - s.a * d.a;
                Set(pColor, Clip(d));
            } else {
                Set(pColor, s);
            }
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaColorBurn : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE double Calc(double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
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
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            if (d.a > 0) {
                double sada = s.a * d.a;
                double s1a = 1 - s.a;
                double d1a = 1 - d.a;
                d.r = Calc(d.r, s.r, d.a, s.a, sada, d1a, s1a);
                d.g = Calc(d.g, s.g, d.a, s.a, sada, d1a, s1a);
                d.b = Calc(d.b, s.b, d.a, s.a, sada, d1a, s1a);
                d.a += s.a - sada;
                Set(pColor, Clip(d));
            } else {
                Set(pColor, s);
            }
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaHardLight : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE double Calc(double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
    {
        return (2 * sca < sa) ? 2 * sca * dca + sca * d1a + dca * s1a : sada - 2 * (da - dca) * (sa - sca) + sca * d1a + dca * s1a;
    }
    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            double d1a = 1 - d.a;
            double s1a = 1 - s.a;
            double sada = s.a * d.a;
            d.r = Calc(d.r, s.r, d.a, s.a, sada, d1a, s1a);
            d.g = Calc(d.g, s.g, d.a, s.a, sada, d1a, s1a);
            d.b = Calc(d.b, s.b, d.a, s.a, sada, d1a, s1a);
            d.a += s.a - sada;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaSoftLight : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE double Calc(double dca, double sca, double da, double sa, double sada, double d1a, double s1a)
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
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            if (d.a > 0) {
                double sada = s.a * d.a;
                double s1a = 1 - s.a;
                double d1a = 1 - d.a;
                d.r = Calc(d.r, s.r, d.a, s.a, sada, d1a, s1a);
                d.g = Calc(d.g, s.g, d.a, s.a, sada, d1a, s1a);
                d.b = Calc(d.b, s.b, d.a, s.a, sada, d1a, s1a);
                d.a += s.a - sada;
                Set(pColor, Clip(d));
            } else {
                Set(pColor, s);
            }
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDifference : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;
    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            d.r += s.r - 2 * SdMin(s.r * d.a, d.r * s.a);
            d.g += s.g - 2 * SdMin(s.g * d.a, d.g * s.a);
            d.b += s.b - 2 * SdMin(s.b * d.a, d.b * s.a);
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaExclusion : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;
    /**
     * @brief 用颜色分量及覆盖率混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            double d1a = 1 - d.a;
            double s1a = 1 - s.a;
            d.r = (s.r * d.a + d.r * s.a - 2 * s.r * d.r) + s.r * d1a + d.r * s1a;
            d.g = (s.g * d.a + d.g * s.a - 2 * s.g * d.g) + s.g * d1a + d.g * s1a;
            d.b = (s.b * d.a + d.b * s.a - 2 * s.b * d.b) + s.b * d1a + d.b * s1a;
            d.a += s.a - s.a * d.a;
            Set(pColor, Clip(d));
        }
    }
};

template <class ColorT, class Order>
struct CompOpTableRgba {
    using ValueType = typename ColorT::ValueType ValueType;
    using CalcType = typename ColorT::CalcType;
    using CompOpFuncType = void (*)(ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType ca, CoverType cover);
    static CompOpFuncType g_compOpFunc[];
};

template <class ColorT, class Order>
typename CompOpTableRgba<ColorT, Order>::CompOpFuncType CompOpTableRgba<ColorT, Order>::g_compOpFunc[] =
    {
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
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(unsigned op, ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
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
     *
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
        ValueType da = p[Order::A];
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

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        ValueType da = p[Order::A];
        CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor,
                                                         ColorType::Multiply(r, da),
                                                         ColorType::Multiply(g, da),
                                                         ColorType::Multiply(b, da),
                                                         ColorType::Multiply(a, da), cover);
    }
};

}