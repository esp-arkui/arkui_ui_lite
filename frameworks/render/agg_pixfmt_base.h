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
 * @brief Defines Blenderbase.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_pixfmt_base.h
 *
 * @brief Defines 定义了像素与颜色分量转换的操作方法.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_color/agg_color_gray.h"
#include "gfx_utils/graphics/graphic_color/agg_color_rgba.h"

namespace OHOS {
 /**
 *
 * @brief Defines Blenderbase.
 * @since 1.0
 * @version 1.0
 */
template<class ColorT, class Order = void> 
struct BlenderBase
{
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;

    /**
     * @brief 把颜色分量设置给颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    static void Set(ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a)
    {
        pColor[OrderType::R] = r;
        pColor[OrderType::G] = g;
        pColor[OrderType::B] = b;
        pColor[OrderType::A] = a;
    }

    /**
     * @brief 把颜色分量设置给颜色.
     *
     * @since 1.0
     * @version 1.0
     */
    static void Set(ValueType* pColor, const Rgba& color)
    {
        pColor[OrderType::R] = ColorType::fromDouble(color.r);
        pColor[OrderType::G] = ColorType::fromDouble(color.g);
        pColor[OrderType::B] = ColorType::fromDouble(color.b);
        pColor[OrderType::A] = ColorType::fromDouble(color.a);
    }

    /**
     * @brief 通过颜色分量获取颜色.
     * @return 返回颜色
     * @since 1.0
     * @version 1.0
     */
    static Rgba Get(ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover = COVER_FULL)
    {
        if (cover > COVER_NONE) {
            Rgba c(ColorType::ToDouble(r), ColorType::ToDouble(g), ColorType::ToDouble(b), ColorType::ToDouble(a));

            if (cover < COVER_FULL) {
                double x = double(cover) / COVER_FULL;
                c.r *= x;
                c.g *= x;
                c.b *= x;
                c.a *= x;
            }

            return c;
        } else {
            return Rgba::NoColor();
        }
                
    }

    /**
     * @brief 通过颜色分量获取颜色.
     * @return 返回颜色
     * @since 1.0
     * @version 1.0
     */
    static Rgba Get(const ValueType* pColor, CoverType cover = COVER_FULL)
    {
        return Get(pColor[OrderType::R], pColor[OrderType::G], pColor[OrderType::B], pColor[OrderType::A], cover);
    }

};
}

#endif
