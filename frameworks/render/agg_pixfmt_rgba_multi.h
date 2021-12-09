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
template <class ColorT, class Order>
struct CompOpRgbaMultiply : BlenderBase<ColorT, Order> {
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

}