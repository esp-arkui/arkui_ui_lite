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
* @brief Defines 颜色分量操作类.
*
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_INCLUDED

#include <cmath>
#include <cstring>

#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"

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

/**
 * @file graphic_geometry_pixfmt_rgba.h
 *
 * @brief Defines 颜色分量倍增器.
 *
 * @since 1.0
 * @version 1.0
 */
template <class ColorT, class Order>
struct MultiplierRgba {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    /**
     * @brief 颜色分量与Alpha相乘.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void Premultiply(ValueType* pColor)
    {
        ValueType a = pColor[Order::A];
        pColor[Order::B] = ColorType::Multiply(p[Order::B], a);
        pColor[Order::R] = ColorType::Multiply(p[Order::R], a);
        pColor[Order::G] = ColorType::Multiply(p[Order::G], a);
    }

    /**
     * @brief 颜色分量与Alpha解复用.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void Demultiply(ValueType* pColor)
    {
        ValueType a = pColor[Order::A];
        pColor[Order::B] = ColorType::Demultiply(pColor[Order::B], a);
        pColor[Order::R] = ColorType::Demultiply(pColor[Order::R], a);
        pColor[Order::G] = ColorType::Demultiply(pColor[Order::G], a);
    }
};

template <class ColorT, class Order, class GammaLut>
class ApplyGammaDirRgba : public HeapBase {
public:
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;

    ApplyGammaDirRgba(const GammaLut& gamma)
        : gamma_(gamma)
    {}

    GRAPHIC_GEOMETRY_INLINE void operator()(ValueType* pColor)
    {
        pColor[Order::R] = gamma_.Dir(pColor[Order::R]);
        pColor[Order::B] = gamma_.Dir(pColor[Order::B]);
        pColor[Order::G] = gamma_.Dir(pColor[Order::G]);
    }

private:
    const GammaLut& gamma_;
};

template <class ColorT, class Order, class GammaLut>
class ApplyGammaInvRgba : public HeapBase {
public:
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;

    ApplyGammaInvRgba(const GammaLut& gamma)
        : gamma_(gamma)
    {}

    GRAPHIC_GEOMETRY_INLINE void operator()(ValueType* pColor)
    {
        pColor[Order::B] = gamma_.Inv(pColor[Order::B]);
        pColor[Order::R] = gamma_.Inv(pColor[Order::R]);
        pColor[Order::G] = gamma_.Inv(pColor[Order::G]);
    }

private:
    const GammaLut& gamma_;
};

template <class ColorT, class Order>
struct ConvRgbaPre {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;

    static GRAPHIC_GEOMETRY_INLINE void SetPlainColor(ValueType* pColor, ColorType color)
    {
        color.Premultiply();
        pColor[Order::A] = color.a;
        pColor[Order::R] = color.r;
        pColor[Order::B] = color.b;
        pColor[Order::G] = color.g;
    }

    static GRAPHIC_GEOMETRY_INLINE ColorType GetPlainColor(const ValueType* p)
    {
        return ColorType(p[Order::R], p[Order::G], p[Order::B], p[Order::A])
            .Demultiply();
    }
};

template <class ColorT, class Order>
struct ConvRgbaPlain {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;

    static GRAPHIC_GEOMETRY_INLINE void SetPlainColor(ValueType* pColor, ColorType color)
    {
        pColor[Order::R] = color.r;
        pColor[Order::G] = color.g;
        pColor[Order::B] = color.b;
        pColor[Order::A] = color.a;
    }

    static GRAPHIC_GEOMETRY_INLINE ColorType GetPlainColor(const ValueType* pColor)
    {
        return ColorType(pColor[Order::R], pColor[Order::G], pColor[Order::B], pColor[Order::A]);
    }
};

template <class ColorT, class Order>
struct BlenderRgba : ConvRgbaPre<ColorT, Order> {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    /**
     * @brief 用颜色分量混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
    {
        BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
    }
    /**
     * @brief 用颜色分量混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        pColor[Order::R] = ColorType::Lerp(pColor[Order::R], cr, alpha);
        pColor[Order::G] = ColorType::Lerp(pColor[Order::G], cg, alpha);
        pColor[Order::B] = ColorType::Lerp(pColor[Order::B], cb, alpha);
        pColor[Order::A] = ColorType::Prelerp(pColor[Order::A], alpha, alpha);
    }
};

template <class ColorT, class Order>
struct BlenderRgbaPre : ConvRgbaPre<ColorT, Order> {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    /**
     * @brief 用颜色分量混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(ValueType* pColor,
             ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
    {
        BlendPix(pColor,
                 ColorType::MultCover(cr, cover),
                 ColorType::MultCover(cg, cover),
                 ColorType::MultCover(cb, cover),
                 ColorType::MultCover(alpha, cover));
    }
    /**
     * @brief 用颜色分量混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        pColor[Order::R] = ColorType::Prelerp(pColor[Order::R], cr, alpha);
        pColor[Order::G] = ColorType::Prelerp(pColor[Order::G], cg, alpha);
        pColor[Order::B] = ColorType::Prelerp(pColor[Order::B], cb, alpha);
        pColor[Order::A] = ColorType::Prelerp(pColor[Order::A], alpha, alpha);
    }
};

template <class ColorT, class Order>
struct BlenderRgbaPlain : ConvRgbaPlain<ColorT, Order> {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    /**
     * @brief 用颜色分量混合像素.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
    {
        BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
    }

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        if (alpha > ColorType::EmptyValue()) {
            CalcType a = pColor[Order::A];
            CalcType r = ColorType::Multiply(pColor[Order::R], a);
            CalcType g = ColorType::Multiply(pColor[Order::G], a);
            CalcType b = ColorType::Multiply(pColor[Order::B], a);
            pColor[Order::A] = ColorType::Prelerp(a, alpha, alpha);
            pColor[Order::R] = ColorType::Lerp(r, cr, alpha);
            pColor[Order::G] = ColorType::Lerp(g, cg, alpha);
            pColor[Order::B] = ColorType::Lerp(b, cb, alpha);
            MultiplierRgba<ColorT, Order>::Demultiply(pColor);
        }
    }
};

//TODO  没有被用到
template <class ColorT, class Order>
struct CompPpRgbaClear : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType, ValueType, ValueType, ValueType, CoverType cover)
    {
        if (cover >= COVER_FULL) {
            pColor[0] = pColor[1] = pColor[2] = pColor[3] = ColorType::EmptyValue();
        } else if (cover > COVER_NONE) {
            Set(pColor, Get(pColor, COVER_FULL - cover));
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaSrc : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

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
struct CompOpRgbaDst : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType*,
                                                 ValueType, ValueType, ValueType, ValueType, CoverType)
    {
    }
};

template <class ColorT, class Order>
struct CompOpRgbaSrcOver : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        BlenderRgbaPre<ColorT, Order>::BlendPix(pColor, r, g, b, a, cover);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDstOver : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        Rgba d = Get(pColor);
        double d1a = 1 - d.a;
        d.r += s.r * d1a;
        d.g += s.g * d1a;
        d.b += s.b * d1a;
        d.a += s.a * d1a;
        Set(pColor, d);
    }
};
// TODO  没有用到
template <class ColorT, class Order>
struct CompOpRgbaSrcIn : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        double da = ColorT::ToDouble(pColor[Order::A]);
        if (da > 0) {
            Rgba s = Get(r, g, b, a, cover);
            Rgba d = Get(p, COVER_FULL - cover);
            d.r += s.r * da;
            d.g += s.g * da;
            d.b += s.b * da;
            d.a += s.a * da;
            Set(pColor, d);
        }
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDstIn : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType, ValueType, ValueType, ValueType a, CoverType cover)
    {
        double sa = ColorT::ToDouble(a);
        Rgba d = Get(pColor, COVER_FULL - cover);
        Rgba d2 = Get(pColor, cover);
        d.r += d2.r * sa;
        d.g += d2.g * sa;
        d.b += d2.b * sa;
        d.a += d2.a * sa;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaSrcOut : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        Rgba d = Get(p, COVER_FULL - cover);
        double d1a = 1 - ColorT::ToDouble(pColor[Order::A]);
        d.a += s.a * d1a;
        d.r += s.r * d1a;
        d.g += s.g * d1a;
        d.b += s.b * d1a;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDstOut : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType, ValueType, ValueType, ValueType a, CoverType cover)
    {
        Rgba d = Get(pColor, COVER_FULL - cover);
        Rgba dc = Get(pColor, cover);
        double s1a = 1 - ColorT::ToDouble(a);
        d.r += dc.r * s1a;
        d.g += dc.g * s1a;
        d.b += dc.b * s1a;
        d.a += dc.a * s1a;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaSrcAtop : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        Rgba d = Get(pColor);
        double s1a = 1 - s.a;
        d.r = s.r * d.a + d.r * s1a;
        d.g = s.g * d.a + d.g * s1a;
        d.b = s.b * d.a + d.g * s1a;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaDstAtop : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba sc = Get(r, g, b, a, cover);
        Rgba dc = Get(pColor, cover);
        Rgba d = Get(pColor, COVER_FULL - cover);
        double sa = ColorT::ToDouble(a);
        double d1a = 1 - ColorT::ToDouble(p[Order::A]);
        d.r += dc.r * sa + sc.r * d1a;
        d.g += dc.g * sa + sc.g * d1a;
        d.b += dc.b * sa + sc.b * d1a;
        d.a += sc.a;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaXor : BlenderBase<ColorT, Order> {
    using ColorType = ColorT;
    using ValueType = typename ColorType::ValueType;
    using BlenderBase<ColorT, Order>::Get;
    using BlenderBase<ColorT, Order>::Set;

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        Rgba d = Get(pColor);
        double s1a = 1 - s.a;
        double d1a = 1 - ColorT::ToDouble(pColor[Order::A]);
        d.r = s.r * d1a + d.r * s1a;
        d.g = s.g * d1a + d.g * s1a;
        d.b = s.b * d1a + d.b * s1a;
        d.a = s.a + d.a - 2 * s.a * d.a;
        Set(pColor, d);
    }
};

template <class ColorT, class Order>
struct CompOpRgbaPlus : BlenderBase<ColorT, Order> {
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
            d.a = SdMin(d.a + s.a, 1.0);
            d.r = SdMin(d.r + s.r, d.a);
            d.g = SdMin(d.g + s.g, d.a);
            d.b = SdMin(d.b + s.b, d.a);
            Set(pColor, Clip(d));
        }
    }
};
//TODO 没有用到
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
struct CompOpRgbaMultiply : BlenderBase<ColorT, Order> {
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
            double s1a = 1 - s.a;
            double d1a = 1 - d.a;
            d.r = s.r * d.r + s.r * d1a + d.r * s1a;
            d.g = s.g * d.g + s.g * d1a + d.g * s1a;
            d.b = s.b * d.b + s.b * d1a + d.b * s1a;
            d.a += s.a - s.a * d.a;
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

//TODO 没有用到
template <class ColorT, class Order>
struct CompOpRgba_overlay : BlenderBase<ColorT, Order> {
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
            return sada * (1 - sd_min(1.0, (1 - dca / da) * sa / sca)) + sca * d1a + dca * s1a;
        }
        if (dca > da) {
            return sada + dca * s1a;
        }
        return dca * s1a;
    }

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

    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        Rgba s = Get(r, g, b, a, cover);
        if (s.a > 0) {
            Rgba d = Get(pColor);
            d.r += s.r - 2 * sd_min(s.r * d.a, d.r * s.a);
            d.g += s.g - 2 * sd_min(s.g * d.a, d.g * s.a);
            d.b += s.b - 2 * sd_min(s.b * d.a, d.b * s.a);
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

enum CompOpEnum
{
    COMP_OP_CLEAR,
    COMP_OP_SRC,
    COMP_OP_DST,
    COMP_OP_SRC_OVER,
    COMP_OP_DST_OVER,
    COMP_OP_SRC_IN,
    COMP_OP_DST_IN,
    COMP_OP_SRC_OUT,
    COMP_OP_DST_OUT,
    COMP_OP_SRC_ATOP,
    COMP_OP_DST_ATOP,
    COMP_OP_XOR,
    COMP_OP_PLUS,
    COMP_OP_MULTIPLY,
    COMP_OP_SCREEN,
    COMP_OP_OVERLAY,
    COMP_OP_DARKEN,
    COMP_OP_LIGHTEN,
    COMP_OP_COLOR_DODGE,
    COMP_OP_COLOR_BURN,
    COMP_OP_HARD_LIGHT,
    COMP_OP_SOFT_LIGHT,
    COMP_OP_DIFFERENCE,
    COMP_OP_EXCLUSION,
    END_OF_COMP_OP_E
};

template <class ColorT, class Order>
struct CompOpAdaptorRgba {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
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

// TODO 没有用到
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
// TODO 没有用到
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

//TODO 没有用到
template <class ColorT, class Order>
struct CompOpAdaptorRgbaPlain {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        MultiplierRgba<ColorT, Order>::Premultiply(pColor);
        CompOpAdaptorRgba<ColorT, Order>::BlendPix(op, pColor, r, g, b, a, cover);
        MultiplierRgba<ColorT, Order>::Demultiply(pColor);
    }
};

template <class ColorT, class Order>
struct CompOpAdaptorClipToDstRgbaPlain {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        MultiplierRgba<ColorT, Order>::Premultiply(pColor);
        CompOpAdaptorClipToDstRgba<ColorT, Order>::BlendPix(op, pColor, r, g, b, a, cover);
        MultiplierRgba<ColorT, Order>::Demultiply(pColor);
    }
};

template <class BlenderPre>
struct CompAdaptorRgba {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        BlenderPre::BlendPix(pColor,
                             ColorType::Multiply(r, a),
                             ColorType::Multiply(g, a),
                             ColorType::Multiply(b, a),
                             a, cover);
    }
};

template <class BlenderPre>
struct CompAdaptorClipToDstRgba {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        r = ColorType::Multiply(r, a);
        g = ColorType::Multiply(g, a);
        b = ColorType::Multiply(b, a);
        ValueType da = p[OrderType::A];
        BlenderPre::BlendPix(pColor,
                             ColorType::Multiply(r, da),
                             ColorType::Multiply(g, da),
                             ColorType::Multiply(b, da),
                             ColorType::Multiply(a, da), cover);
    }
};

template <class BlenderPre>
struct CompAdaptorRgbaPre {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        BlenderPre::BlendPix(pColor, r, g, b, a, cover);
    }
};

template <class BlenderPre>
struct CompAdaptorClipToDstRgbaPre {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        unsigned da = p[OrderType::A];
        BlenderPre::BlendPix(pColor,
                             ColorType::Multiply(r, da),
                             ColorType::Multiply(g, da),
                             ColorType::Multiply(b, da),
                             ColorType::Multiply(a, da),
                             cover);
    }
};

template <class BlenderPre>
struct CompAdaptorRgbaPlain {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        MultiplierRgba<ColorType, OrderType>::Premultiply(pColor);
        CompAdaptorRgba<BlenderPre>::BlendPix(op, pColor, r, g, b, a, cover);
        MultiplierRgba<ColorType, OrderType>::Demultiply(pColor);
    }
};

template <class BlenderPre>
struct CompAdaptorClipToDstRgbaPlain {
    using ColorType = typename BlenderPre::ColorType;
    using OrderType = typename BlenderPre::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    static GRAPHIC_GEOMETRY_INLINE void
    BlendPix(unsigned op, ValueType* pColor,
             ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover)
    {
        MultiplierRgba<ColorType, OrderType>::Premultiply(pColor);
        CompAdaptorClipToDstRgba<BlenderPre>::BlendPix(op, pColor, r, g, b, a, cover);
        MultiplierRgba<ColorType, OrderType>::Demultiply(pColor);
    }
};

template <class Blender, class RenBuf>
class PixfmtAlphaBlendRgba : public HeapBase {
public:
    using PixfmtCategory = PixfmtRgbaTag;
    using RbufType = RenBuf;
    using RowData = typename RbufType::RowData;
    using BlenderType = Blender;
    using ColorType = typename BlenderType::ColorType;
    using OrderType = typename BlenderType::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType

        enum {
            NUM_COMPONENTS = 4,
            PIX_STEP = 4,
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP,
        };

    struct PixelType {
        ValueType c[NUM_COMPONENTS];

        void Set(ValueType r, ValueType g, ValueType b, ValueType a)
        {
            c[OrderType::R] = r;
            c[OrderType::G] = g;
            c[OrderType::B] = b;
            c[OrderType::A] = a;
        }

        void Set(const ColorType& color)
        {
            Set(color.r, color.g, color.b, color.a);
        }

        void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
        {
            r = c[OrderType::R];
            g = c[OrderType::G];
            b = c[OrderType::B];
            a = c[OrderType::A];
        }

        ColorType Get() const
        {
            return ColorType(c[OrderType::R], c[OrderType::G], c[OrderType::B], c[OrderType::A]);
        }

        PixelType* Next()
        {
            return (PixelType*)(c + PIX_STEP);
        }

        const PixelType* Next() const
        {
            return (const PixelType*)(c + PIX_STEP);
        }

        PixelType* Advance(int n)
        {
            return (PixelType*)(c + n * PIX_STEP);
        }

        const PixelType* Advance(int n) const
        {
            return (const PixelType*)(c + n * PIX_STEP);
        }
    };

private:
    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover)
    {
        blender_.BlendPix(p->c, c.r, c.g, c.b, c.a, cover);
    }

    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c)
    {
        blender_.BlendPix(p->c, c.r, c.g, c.b, c.a);
    }

    GRAPHIC_GEOMETRY_INLINE void copy_or_BlendPix(PixelType* p, const ColorType& c, unsigned cover)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque() && cover == COVER_MASK) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                blender_.BlendPix(p->c, c.r, c.g, c.b, c.a, cover);
            }
        }
    }

    GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque()) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                blender_.BlendPix(p->c, c.r, c.g, c.b, c.a);
            }
        }
    }

public:
    PixfmtAlphaBlendRgba()
        : rbuf_(0)
    {}
    explicit PixfmtAlphaBlendRgba(RbufType& rb)
        : rbuf_(&rb)
    {}
    void Attach(RbufType& rb)
    {
        rbuf_ = &rb;
    }

    template <class PixFmt>
    bool Attach(PixFmt& pixf, int x1, int y1, int x2, int y2)
    {
        RectI r(x1, y1, x2, y2);
        if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) {
            int stride = pixf.Stride();
            rbuf_->Attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
            return true;
        }
        return false;
    }

    /**
     * @brief 返回窗口的宽.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE unsigned Width() const
    {
        return rbuf_->Width();
    }

    /**
      * @brief 返回窗口的高.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE unsigned Height() const
    {
        return rbuf_->Height();
    }

    /**
      * @brief 返回窗口一行的字节数.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int Stride() const
    {
        return rbuf_->Stride();
    }

    /**
      * @brief 返回窗口一行的行地址.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
    {
        return rbuf_->RowPtr(y);
    }

    /**
      * @brief 返回窗口一行的行地址.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
    {
        return rbuf_->RowPtr(y);
    }

    /**
      * @brief 返回窗口一行的行数据.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE row_data Row(int y) const
    {
        return rbuf_->Row(y);
    }

    /**
      * @brief 指针转为像素类型指针.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    /**
     * @brief 指针转为像素类型指针.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    /**
     * @brief 指针转为像素类型指针.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
    {
        return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
    }

    /**
     * @brief 获取指定左边的像素地址.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
    {
        int8u* p = rbuf_->RowPtr(y);
        return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
    }

    /**
     * @brief 指针转为像素类型指针.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
    {
        return (PixelType*)p;
    }

    /**
     * @brief 指针转为像素类型指针.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
    {
        return (const PixelType*)p;
    }

    /**
     * @brief 把纯色写入一个地址.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static void WritePlainColor(void* p, ColorType c)
    {
        BlenderType::SetPlainColor(PixValuePtr(p)->c, c);
    }

    /**
     * @brief 获取一个地址的纯色.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static ColorType ReadPlainColor(const void* p)
    {
        return BlenderType::GetPlainColor(PixValuePtr(p)->c);
    }

    /**
     * @brief 给一个地址设置颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u* p, const ColorType& c)
    {
        ((PixelType*)p)->Set(c);
    }

    /**
     * @brief 获取一个像素的颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
    {
        if (const PixelType* p = PixValuePtr(x, y)) {
            return p->Get();
        }
        return ColorType::NoColor();
    }

    /**
     * @brief 拷贝颜色到像素.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
    {
        PixValuePtr(x, y, 1)->Set(c);
    }

    /**
    * @brief 用颜色c以cover（覆盖率 = 透明度）的透明度混合像素(x, y).
    *
    * @since 1.0
    * @version 1.0
    */
    GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
    {
        CopyOrBlendPix(PixValuePtr(x, y, 1), c, cover);
    }
    /**
     * @brief 从(x, y)开始打横顺序设置len长度的像素.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                           unsigned len,
                                           const ColorType& c)
    {
        PixelType v;
        v.Set(c);
        PixelType* p = PixValuePtr(x, y, len);
        do {
            *p = v;
            p = p->Next();
        } while (--len);
    }

    /**
     * @brief 从(x, y)开始打竖顺序设置len长度的像素.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyVline(int x, int y,
                                           unsigned len,
                                           const ColorType& c)
    {
        PixelType v;
        v.Set(c);
        do {
            *PixValuePtr(x, y++, 1) = v;
        } while (--len);
    }

    /**
     * @brief 从(x, y)开始打横顺序混合len长度的像素.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendHline(int x, int y,
                    unsigned len,
                    const ColorType& c,
                    int8u cover)
    {
        if (!c.IsTransparent()) {
            PixelType* p = PixValuePtr(x, y, len);
            if (c.IsOpaque() && cover == COVER_MASK) {
                PixelType v;
                v.Set(c);
                do {
                    *p = v;
                    p = p->Next();
                } while (--len);
            } else {
                if (cover == COVER_MASK) {
                    do {
                        BlendPix(p, c);
                        p = p->Next();
                    } while (--len);
                } else {
                    do {
                        BlendPix(p, c, cover);
                        p = p->Next();
                    } while (--len);
                }
            }
        }
    }

    /**
     * @brief 从(x, y)开始打竖顺序混合len长度的像素.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendVline(int x, int y,
                    unsigned len,
                    const ColorType& c,
                    int8u cover)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque() && cover == COVER_MASK) {
                PixelType v;
                v.Set(c);
                do {
                    *PixValuePtr(x, y++, 1) = v;
                } while (--len);
            } else {
                if (cover == COVER_MASK) {
                    do {
                        BlendPix(PixValuePtr(x, y++, 1), c, c.a);
                    } while (--len);
                } else {
                    do {
                        BlendPix(PixValuePtr(x, y++, 1), c, cover);
                    } while (--len);
                }
            }
        }
    }

    /**
     * @brief 从(x, y)开始打横顺序混合len长度的一系列颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendSolidHspan(int x, int y,
                         unsigned len,
                         const ColorType& c,
                         const int8u* covers)
    {
        if (!c.IsTransparent()) {
            PixelType* p = PixValuePtr(x, y, len);
            do {
                if (c.IsOpaque() && *covers == cover_mask) {
                    p->Set(c);
                } else {
                    BlendPix(p, c, *covers);
                }
                p = p->Next();
                ++covers;
            } while (--len);
        }
    }
    /**
     * @brief 从(x, y)开始打竖顺序混合len长度的一系列颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendSolidVspan(int x, int y,
                         unsigned len,
                         const ColorType& c,
                         const int8u* covers)
    {
        if (!c.IsTransparent()) {
            do {
                PixelType* p = PixValuePtr(x, y++, 1);
                if (c.IsOpaque() && *covers == COVER_MASK) {
                    p->Set(c);
                } else {
                    BlendPix(p, c, *covers);
                }
                ++covers;
            } while (--len);
        }
    }

    /**
     * @brief 从(x, y)开始打横顺序设置len长度的颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void CopyColorHspan(int x, int y,
                        unsigned len,
                        const ColorType* colors)
    {
        PixelType* p = PixValuePtr(x, y, len);
        do {
            p->Set(*colors++);
            p = p->Next();
        } while (--len);
    }

    /**
     * @brief 从(x, y)开始打竖顺序设置len长度的颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void CopyColorVspan(int x, int y,
                        unsigned len,
                        const ColorType* colors)
    {
        do {
            PixValuePtr(x, y++, 1)->Set(*colors++);
        } while (--len);
    }

    /**
     * @brief 从(x, y)开始打横顺序混合len长度的一系列颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendColorHspan(int x, int y,
                         unsigned len,
                         const ColorType* colors,
                         const int8u* covers,
                         int8u cover)
    {
        PixelType* p = PixValuePtr(x, y, len);
        if (covers) {
            do {
                CopyOrBlendPix(p, *colors++, *covers++);
                p = p->Next();
            } while (--len);
        } else {
            if (cover == COVER_MASK) {
                do {
                    CopyOrBlendPix(p, *colors++);
                    p = p->Next();
                } while (--len);
            } else {
                do {
                    CopyOrBlendPix(p, *colors++, cover);
                    p = p->Next();
                } while (--len);
            }
        }
    }

    /**
     * @brief 从(x, y)开始打竖顺序混合len长度的一系列颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    void BlendColorVspan(int x, int y,
                         unsigned len,
                         const ColorType* colors,
                         const int8u* covers,
                         int8u cover)
    {
        if (covers) {
            do {
                CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++, *covers++);
            } while (--len);
        } else {
            if (cover == COVER_MASK) {
                do {
                    CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++);
                } while (--len);
            } else {
                do {
                    CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++, cover);
                } while (--len);
            }
        }
    }

    /**
     * @brief 每一像素执行一遍Function函数.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class Function>
    void ForEachPixel(Function f)
    {
        for (unsigned y = 0; y < height(); ++y) {
            RowData r = rbuf_->Row(y);
            if (r.ptr) {
                unsigned len = r.x2 - r.x1 + 1;
                PixelType* p = PixValuePtr(r.x1, y, len);
                do {
                    f(p->c);
                    p = p->Next();
                } while (--len);
            }
        }
    }

    void Premultiply()
    {
        ForEachPixel(MultiplierRgba<ColorType, OrderType>::Premultiply);
    }

    void Demultiply()
    {
        ForEachPixel(MultiplierRgba<ColorType, OrderType>::Demultiply);
    }

    template <class GammaLut>
    void ApplyGammaDir(const GammaLut& g)
    {
        ForEachPixel(ApplyGammaDirRgba<ColorType, OrderType, GammaLut>(g));
    }

    template <class GammaLut>
    void ApplyGammaInv(const GammaLut& g)
    {
        ForEachPixel(ApplyGammaInvRgba<ColorType, OrderType, GammaLut>(g));
    }

    template <class RenBuf2>
    void CopyFrom(const RenBuf2& from,
                  int xdst, int ydst,
                  int xsrc, int ysrc,
                  unsigned len)
    {
        if (const int8u* p = from.RowPtr(ysrc)) {
            std::memmove(rbuf_->RowPtr(xdst, ydst, len) + xdst * PIX_WIDTH, p + xsrc * PIX_WIDTH, len * PIX_WIDTH);
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFrom(const SrcPixelFormatRenderer& from,
                   int xdst, int ydst,
                   int xsrc, int ysrc,
                   unsigned len,
                   int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);
            int srcinc = 1;
            int dstinc = 1;

            if (xdst > xsrc) {
                psrc = psrc->Advance(len - 1);
                pdst = pdst->Advance(len - 1);
                srcinc = -1;
                dstinc = -1;
            }

            if (cover == COVER_MASK) {
                do {
                    CopyOrBlendPix(pdst, psrc->Get());
                    psrc = psrc->Advance(srcinc);
                    pdst = pdst->Advance(dstinc);
                } while (--len);
            } else {
                do {
                    CopyOrBlendPix(pdst, psrc->Get(), cover);
                    psrc = psrc->Advance(srcinc);
                    pdst = pdst->Advance(dstinc);
                } while (--len);
            }
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFromColor(const SrcPixelFormatRenderer& from,
                        const ColorType& color,
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;
        using SrcColorType = typename SrcPixelFormatRenderer::ColorType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);

            do {
                CopyOrBlendPix(pdst, color, SrcColorType::ScaleCover(cover, psrc->c[0]));
                psrc = psrc->Next();
                pdst = pdst->Next();
            } while (--len);
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFromLut(const SrcPixelFormatRenderer& from,
                      const ColorType* colorLut,
                      int xdst, int ydst,
                      int xsrc, int ysrc,
                      unsigned len,
                      int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);

            if (cover == COVER_MASK) {
                do {
                    CopyOrBlendPix(pdst, colorLut[psrc->c[0]]);
                    psrc = psrc->Next();
                    pdst = pdst->Next();
                } while (--len);
            } else {
                do {
                    CopyOrBlendPix(pdst, colorLut[psrc->c[0]], cover);
                    psrc = psrc->Next();
                    pdst = pdst->Next();
                } while (--len);
            }
        }
    }

private:
    RbufType* rbuf_;
    Blender blender_;
};

template <class Blender, class RenBuf>
class PixfmtCustomBlendRgba public HeapBase
{
public:
    using PixfmtCategory = PixfmtRgbaTag;
    using RbufType = RenBuf;
    using RowData = typename RbufType::RowData;
    using BlenderType = Blender;
    using ColorType = typename BlenderType::ColorType;
    using OrderType = typename BlenderType::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    enum
    {
        NUM_COMPONENTS = 4,
        PIX_STEP = 4,
        PIX_WIDTH = sizeof(ValueType) * PIX_STEP,
    };
    struct PixelType {
        ValueType c[NUM_COMPONENTS];

        void Set(ValueType r, ValueType g, ValueType b, ValueType a)
        {
            c[OrderType::R] = r;
            c[OrderType::G] = g;
            c[OrderType::B] = b;
            c[OrderType::A] = a;
        }

        void Set(const ColorType& color)
        {
            Set(color.r, color.g, color.b, color.a);
        }

        void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
        {
            r = c[OrderType::R];
            g = c[OrderType::G];
            b = c[OrderType::B];
            a = c[OrderType::A];
        }

        ColorType Get() const
        {
            return ColorType(
                c[OrderType::R],
                c[OrderType::G],
                c[OrderType::B],
                c[OrderType::A]);
        }

        PixelType* Next()
        {
            return (PixelType*)(c + PIX_STEP);
        }

        const PixelType* Next() const
        {
            return (const PixelType*)(c + PIX_STEP);
        }

        PixelType* Advance(int n)
        {
            return (PixelType*)(c + n * PIX_STEP);
        }

        const PixelType* Advance(int n) const
        {
            return (const PixelType*)(c + n * PIX_STEP);
        }
    };

private:
    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType * p, const ColorType& c, unsigned cover = COVER_FULL)
    {
        blender_.BlendPix(compOp_, p->c, c.r, c.g, c.b, c.a, cover);
    }

    GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType * p, const ColorType& c, unsigned cover = COVER_FULL)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque() && cover == COVER_MASK) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                BlendPix(p, c, cover);
            }
        }
    }

public:
    PixfmtCustomBlendRgba()
        : rbuf_(0), compOp_(3)
    {}
    explicit PixfmtCustomBlendRgba(rbuf_type & rb, unsigned compOp = 3)
        : rbuf_(&rb), compOp_(compOp)
    {}
    void Attach(RbufType & rb)
    {
        rbuf_ = &rb;
    }

    template <class PixFmt>
    bool Attach(PixFmt & pixf, int x1, int y1, int x2, int y2)
    {
        RectI r(x1, y1, x2, y2);
        if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) {
            int stride = pixf.Stride();
            rbuf_->Attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
            return true;
        }
        return false;
    }

    void CompOp(unsigned op)
    {
        compOp_ = op;
    }

    unsigned CompOp() const
    {
        return compOp_;
    }

    GRAPHIC_GEOMETRY_INLINE unsigned Width() const
    {
        return rbuf_->Width();
    }

    GRAPHIC_GEOMETRY_INLINE unsigned Height() const
    {
        return rbuf_->Height();
    }

    GRAPHIC_GEOMETRY_INLINE int Stride() const
    {
        return rbuf_->Stride();
    }

    GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
    {
        return rbuf_->RowPtr(y);
    }
    GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
    {
        return rbuf_->RowPtr(y);
    }
    GRAPHIC_GEOMETRY_INLINE RowData Row(int y) const
    {
        return rbuf_->Row(y);
    }

    GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
    {
        return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
    }

    GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
    {
        int8u* p = rbuf_->RowPtr(y);
        return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
    }

    GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
    {
        return (PixelType*)p;
    }

    GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
    {
        return (const PixelType*)p;
    }

    GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u * p, const ColorType& c)
    {
        ((PixelType*)p)->Set(c);
    }

    GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
    {
        if (const PixelType* p = PixValuePtr(x, y)) {
            return p->Get();
        }
        return ColorType::NoColor();
    }

    GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
    {
        MakePix(PixValuePtr(x, y, 1), c);
    }

    GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
    {
        BlendPix(PixValuePtr(x, y, 1), c, cover);
    }

    GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                           unsigned len,
                                           const ColorType& c)
    {
        PixelType v;
        v.Set(c);
        PixelType* p = PixValuePtr(x, y, len);
        do {
            *p = v;
            p = p->Next();
        } while (--len);
    }

    GRAPHIC_GEOMETRY_INLINE void CopyVline(int x, int y,
                                           unsigned len,
                                           const ColorType& c)
    {
        PixelType v;
        v.Set(c);
        do {
            *PixValuePtr(x, y++, 1) = v;
        } while (--len);
    }

    void BlendHline(int x, int y, unsigned len,
                    const ColorType& c, int8u cover)
    {
        PixelType* p = PixValuePtr(x, y, len);
        do {
            BlendPix(p, c, cover);
            p = p->Next();
        } while (--len);
    }

    void BlendVline(int x, int y, unsigned len,
                    const ColorType& c, int8u cover)
    {
        do {
            BlendPix(PixValuePtr(x, y++, 1), c, cover);
        } while (--len);
    }

    void BlendSolidHspan(int x, int y, unsigned len,
                         const ColorType& c, const int8u* covers)
    {
        PixelType* p = PixValuePtr(x, y, len);

        do {
            BlendPix(p, c, *covers++);
            p = p->Next();
        } while (--len);
    }

    void BlendSolidVspan(int x, int y, unsigned len,
                         const ColorType& c, const int8u* covers)
    {
        do {
            BlendPix(PixValuePtr(x, y++, 1), c, *covers++);
        } while (--len);
    }

    void CopyColorHspan(int x, int y,
                        unsigned len,
                        const ColorType* colors)
    {
        PixelType* p = PixValuePtr(x, y, len);

        do {
            p->Set(*colors++);
            p = p->Next();
        } while (--len);
    }

    void CopyColorVspan(int x, int y,
                        unsigned len,
                        const ColorType* colors)
    {
        do {
            PixValuePtr(x, y++, 1)->Set(*colors++);
        } while (--len);
    }

    void BlendColorHspan(int x, int y, unsigned len,
                         const ColorType* colors,
                         const int8u* covers,
                         int8u cover)
    {
        PixelType* p = PixValuePtr(x, y, len);

        do {
            BlendPix(p, *colors++, covers ? *covers++ : cover);
            p = p->Next();
        } while (--len);
    }

    void BlendColorVspan(int x, int y, unsigned len,
                         const ColorType* colors,
                         const int8u* covers,
                         int8u cover)
    {
        do {
            BlendPix(PixValuePtr(x, y++, 1), *colors++, covers ? *covers++ : cover);
        } while (--len);
    }

    /**
     * @brief 每一像素执行一遍Function函数.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class Function>
    void ForEachPixel(Function f)
    {
        unsigned y;
        for (y = 0; y < Height(); ++y) {
            row_data r = rbuf_->row(y);
            if (r.ptr) {
                unsigned len = r.x2 - r.x1 + 1;
                PixelType* p = PixValuePtr(r.x1, y, len);
                do {
                    f(p->c);
                    p = p->Next();
                } while (--len);
            }
        }
    }

    void Premultiply()
    {
        ForEachPixel(MultiplierRgba<ColorType, OrderType>::Premultiply);
    }

    void Demultiply()
    {
        ForEachPixel(MultiplierRgba<ColorType, OrderType>::Demultiply);
    }

    template <class GammaLut>
    void ApplyGammaDir(const GammaLut& g)
    {
        ForEachPixel(ApplyGammaDirRgba<ColorType, OrderType, GammaLut>(g));
    }

    template <class GammaLut>
    void ApplyGammaInv(const GammaLut& g)
    {
        ForEachPixel(ApplyGammaInvRgba<ColorType, OrderType, GammaLut>(g));
    }

    template <class RenBuf2>
    void CopyFrom(const RenBuf2& from,
                  int xdst, int ydst,
                  int xsrc, int ysrc,
                  unsigned len)
    {
        if (const int8u* p = from.RowPtr(ysrc)) {
            std::memmove(rbuf_->RowPtr(xdst, ydst, len) + xdst * PIX_IDTH, p + xsrc * PIX_IDTH, len * PIX_IDTH);
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFrom(const SrcPixelFormatRenderer& from,
                   int xdst, int ydst,
                   int xsrc, int ysrc,
                   unsigned len,
                   int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);
            int srcinc = 1;
            int dstinc = 1;

            if (xdst > xsrc) {
                psrc = psrc->Advance(len - 1);
                pdst = pdst->Advance(len - 1);
                srcinc = -1;
                dstinc = -1;
            }

            do {
                BlendPix(pdst, psrc->Get(), cover);
                psrc = psrc->Advance(srcinc);
                pdst = pdst->Advance(dstinc);
            } while (--len);
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFromColor(const SrcPixelFormatRenderer& from,
                        const ColorType& color,
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;
        using SrcColorType = typename SrcPixelFormatRenderer::ColorType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);

            do {
                BlendPix(pdst, color, SrcColorType::ScaleCover(cover, psrc->c[0]));
                psrc = psrc->Next();
                pdst = pdst->Next();
            } while (--len);
        }
    }

    template <class SrcPixelFormatRenderer>
    void BlendFromLut(const SrcPixelFormatRenderer& from,
                      const ColorType* colorLut,
                      int xdst, int ydst,
                      int xsrc, int ysrc,
                      unsigned len,
                      int8u cover)
    {
        using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

        if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
            PixelType* pdst = PixValuePtr(xdst, ydst, len);

            do {
                BlendPix(pdst, colorLut[psrc->c[0]], cover);
                psrc = psrc->Next();
                pdst = pdst->Next();
            } while (--len);
        }
    }

private:
    RbufType* rbuf_;
    Blender blender_;
    unsigned compOp_;
};

using BlenderRgba32 = BlenderRgba<Rgba8, OrderRgba>;
using BlenderArgb32 = BlenderRgba<Rgba8, OrderArgb>;
using BlenderAbgr32 = BlenderRgba<Rgba8, OrderAbgr>;
using BlenderBgra32 = BlenderRgba<Rgba8, OrderBgra>;

using BlenderSrgba32 = BlenderRgba<Srgba8, OrderRgba>;
using BlenderSargb32 = BlenderRgba<Srgba8, OrderArgb>;
using BlenderSabgr32 = BlenderRgba<Srgba8, OrderAbgr>;
using BlenderSbgra32 = BlenderRgba<Srgba8, OrderBgra>;

using BlenderRgba32Pre = BlenderRgbaPre<Rgba8, OrderRgba>;
using BlenderArgb32Pre = BlenderRgbaPre<Rgba8, OrderArgb>;
using BlenderAbgr32Pre = BlenderRgbaPre<Rgba8, OrderAbgr>;
using BlenderBgra32Pre = BlenderRgbaPre<Rgba8, OrderBgra>;

using BlenderSrgba32Pre = BlenderRgbaPre<Srgba8, OrderRgba>;
using BlenderSargb32Pre = BlenderRgbaPre<Srgba8, OrderArgb>;
using BlenderSabgr32Pre = BlenderRgbaPre<Srgba8, OrderAbgr>;
using BlenderSbgra32Pre = BlenderRgbaPre<Srgba8, OrderBgra>;

using BlenderRgba32Plain = BlenderRgbaPlain<Rgba8, OrderRgba>;
using BlenderArgb32Plain = BlenderRgbaPlain<Rgba8, OrderArgb>;
using BlenderAbgr32Plain = BlenderRgbaPlain<Rgba8, OrderAbgr>;
using BlenderBgra32Plain = BlenderRgbaPlain<Rgba8, OrderBgra>;

using BlenderSrgba32Plain = BlenderRgbaPlain<Srgba8, OrderRgba>;
using BlenderSargb32Plain = BlenderRgbaPlain<Srgba8, OrderArgb>;
using BlenderSabgr32Plain = BlenderRgbaPlain<Srgba8, OrderAbgr>;
using BlenderSbgra32Plain = BlenderRgbaPlain<Srgba8, OrderBgra>;

using BlenderRgba64 = BlenderRgba<Rgba16, OrderRgba>;
using BlenderArgb64 = BlenderRgba<Rgba16, OrderArgb>;
using BlenderAbgr64 = BlenderRgba<Rgba16, OrderAbgr>;
using BlenderBgra64 = BlenderRgba<Rgba16, OrderBgra>;

using BlenderRgba64Pre = BlenderRgbaPre<Rgba16, OrderRgba>;
using BlenderArgb64Pre = BlenderRgbaPre<Rgba16, OrderArgb>;
using BlenderAbgr64Pre = BlenderRgbaPre<Rgba16, OrderAbgr>;
using BlenderBgra64Pre = BlenderRgbaPre<Rgba16, OrderBgra>;

using BlenderRgba64Plain = BlenderRgbaPlain<Rgba16, OrderRgba>;
using BlenderArgb64Plain = BlenderRgbaPlain<Rgba16, OrderArgb>;
using BlenderAbgr64Plain = BlenderRgbaPlain<Rgba16, OrderAbgr>;
using BlenderBgra64Plain = BlenderRgbaPlain<Rgba16, OrderBgra>;

using BlenderRgba128 = BlenderRgba<Rgba32, OrderRgba>;
using BlenderArgb128 = BlenderRgba<Rgba32, OrderArgb>;
using BlenderAbgr128 = BlenderRgba<Rgba32, OrderAbgr>;
using BlenderBgra128 = BlenderRgba<Rgba32, OrderBgra>;

using BlenderRgba128Pre = BlenderRgbaPre<Rgba32, OrderRgba>;
using BlenderArgb128Pre = BlenderRgbaPre<Rgba32, OrderArgb>;
using BlenderAbgr128Pre = BlenderRgbaPre<Rgba32, OrderAbgr>;
using BlenderBgra128Pre = BlenderRgbaPre<Rgba32, OrderBgra>;

using BlenderRgba128Plain = BlenderRgbaPlain<Rgba32, OrderRgba>;
using BlenderArgb128Plain = BlenderRgbaPlain<Rgba32, OrderArgb>;
using BlenderAbgr128Plain = BlenderRgbaPlain<Rgba32, OrderAbgr>;
using BlenderBgra128Plain = BlenderRgbaPlain<Rgba32, OrderBgra>;

using PixfmtRgba32 = PixfmtAlphaBlendRgba<BlenderRgba32, RenderingBuffer>;
using PixfmtArgb32 = PixfmtAlphaBlendRgba<BlenderArgb32, RenderingBuffer>;
using PixfmtAbgr32 = PixfmtAlphaBlendRgba<BlenderAbgr32, RenderingBuffer>;
using PixfmtBgra32 = PixfmtAlphaBlendRgba<BlenderBgra32, RenderingBuffer>;

using PixfmtSrgba32 = PixfmtAlphaBlendRgba<BlenderSrgba32, RenderingBuffer>;
using PixfmtSargb32 = PixfmtAlphaBlendRgba<BlenderSargb32, RenderingBuffer>;
using PixfmtSabgr32 = PixfmtAlphaBlendRgba<BlenderSabgr32, RenderingBuffer>;
using PixfmtSbgra32 = PixfmtAlphaBlendRgba<BlenderSbgra32, RenderingBuffer>;

using PixfmtRgba32Pre = PixfmtAlphaBlendRgba<BlenderRgba32Pre, RenderingBuffer>;
using PixfmtArgb32Pre = PixfmtAlphaBlendRgba<BlenderArgb32Pre, RenderingBuffer>;
using PixfmtAbgr32Pre = PixfmtAlphaBlendRgba<BlenderAbgr32Pre, RenderingBuffer>;
using PixfmtBgra32Pre = PixfmtAlphaBlendRgba<BlenderBgra32Pre, RenderingBuffer>;

using PixfmtSrgba32Pre = PixfmtAlphaBlendRgba<BlenderSrgba32Pre, RenderingBuffer>;
using PixfmtSargb32Pre = PixfmtAlphaBlendRgba<BlenderSargb32Pre, RenderingBuffer>;
using PixfmtSabgr32Pre = PixfmtAlphaBlendRgba<BlenderSabgr32Pre, RenderingBuffer>;
using PixfmtSbgra32Pre = PixfmtAlphaBlendRgba<BlenderSbgra32Pre, RenderingBuffer>;

using PixfmtRgba32Plain = PixfmtAlphaBlendRgba<BlenderRgba32Plain, RenderingBuffer>;
using PixfmtArgb32Plain = PixfmtAlphaBlendRgba<BlenderArgb32Plain, RenderingBuffer>;
using PixfmtAbgr32Plain = PixfmtAlphaBlendRgba<BlenderAbgr32Plain, RenderingBuffer>;
using PixfmtBgra32Plain = PixfmtAlphaBlendRgba<BlenderBgra32Plain, RenderingBuffer>;

using PixfmtSrgba32Plain = PixfmtAlphaBlendRgba<BlenderSrgba32Plain, RenderingBuffer>;
using PixfmtSargb32Plain = PixfmtAlphaBlendRgba<BlenderSargb32Plain, RenderingBuffer>;
using PixfmtSabgr32Plain = PixfmtAlphaBlendRgba<BlenderSabgr32Plain, RenderingBuffer>;
using PixfmtSbgra32Plain = PixfmtAlphaBlendRgba<BlenderSbgra32Plain, RenderingBuffer>;

using PixfmtRgba64 = PixfmtAlphaBlendRgba<BlenderRgba64, RenderingBuffer>;
using PixfmtArgb64 = PixfmtAlphaBlendRgba<BlenderArgb64, RenderingBuffer>;
using PixfmtAbgr64 = PixfmtAlphaBlendRgba<BlenderAbgr64, RenderingBuffer>;
using PixfmtBgra64 = PixfmtAlphaBlendRgba<BlenderBgra64, RenderingBuffer>;

using PixfmtRgba64Pre = PixfmtAlphaBlendRgba<BlenderRgba64Pre, RenderingBuffer>;
using PixfmtArgb64Pre = PixfmtAlphaBlendRgba<BlenderArgb64Pre, RenderingBuffer>;
using PixfmtAbgr64Pre = PixfmtAlphaBlendRgba<BlenderAbgr64Pre, RenderingBuffer>;
using PixfmtBgra64Pre = PixfmtAlphaBlendRgba<BlenderBgra64Pre, RenderingBuffer>;

using PixfmtRgba64Plain = PixfmtAlphaBlendRgba<BlenderRgba64Plain, RenderingBuffer>;
using PixfmtArgb64Plain = PixfmtAlphaBlendRgba<BlenderArgb64Plain, RenderingBuffer>;
using PixfmtAbgr64Plain = PixfmtAlphaBlendRgba<BlenderAbgr64Plain, RenderingBuffer>;
using PixfmtBgra64Plain = PixfmtAlphaBlendRgba<BlenderBgra64Plain, RenderingBuffer>;

using PixfmtRgba128 = PixfmtAlphaBlendRgba<BlenderRgba128, RenderingBuffer>;
using PixfmtArgb128 = PixfmtAlphaBlendRgba<BlenderArgb128, RenderingBuffer>;
using PixfmtAbgr128 = PixfmtAlphaBlendRgba<BlenderAbgr128, RenderingBuffer>;
using PixfmtBgra128 = PixfmtAlphaBlendRgba<BlenderBgra128, RenderingBuffer>;

using PixfmtRgba128Pre = PixfmtAlphaBlendRgba<BlenderRgba128Pre, RenderingBuffer>;
using PixfmtArgb128Pre = PixfmtAlphaBlendRgba<BlenderArgb128Pre, RenderingBuffer>;
using PixfmtAbgr128Pre = PixfmtAlphaBlendRgba<BlenderAbgr128Pre, RenderingBuffer>;
using PixfmtBgra128Pre = PixfmtAlphaBlendRgba<BlenderBgra128Pre, RenderingBuffer>;

using PixfmtRgba128Plain = PixfmtAlphaBlendRgba<BlenderRgba128Plain, RenderingBuffer>;
using PixfmtArgb128Plain = PixfmtAlphaBlendRgba<BlenderArgb128Plain, RenderingBuffer>;
using PixfmtAbgr128Plain = PixfmtAlphaBlendRgba<BlenderAbgr128Plain, RenderingBuffer>;
using PixfmtBgra128Plain = PixfmtAlphaBlendRgba<BlenderBgra128Plain, RenderingBuffer>;

} // namespace OHOS

#endif
