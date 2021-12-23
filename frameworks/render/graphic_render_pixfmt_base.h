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

#include "gfx_utils/graphics/graphic_color/graphic_color_rgba.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    /**
     *
     * @brief Defines Blenderbase.
     * @since 1.0
     * @version 1.0
     */
    template <class ColorT, class Order = void>
    struct BlenderBase {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;

        /**
         * @brief 把颜色分量设置给颜色.
         * @param r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static void Set(ValueType* pColor, ValueType r, ValueType g, ValueType b, ValueType a)
        {
            pColor[OrderType::RED] = r;
            pColor[OrderType::GREEN] = g;
            pColor[OrderType::BLUE] = b;
            pColor[OrderType::ALPHA] = a;
        }

        /**
         * @brief 把颜色分量设置给颜色.
         * @param pColor 颜色,color像素颜色分量
         * @since 1.0
         * @version 1.0
         */
        static void Set(ValueType* pColor, const Rgba& color)
        {
            pColor[OrderType::RED] = ColorType::FromDouble(color.redValue);
            pColor[OrderType::GREEN] = ColorType::FromDouble(color.greenValue);
            pColor[OrderType::BLUE] = ColorType::FromDouble(color.blueValue);
            pColor[OrderType::ALPHA] = ColorType::FromDouble(color.alphaValue);
        }

        /**
         * @brief 通过颜色分量获取颜色.
         * @return 返回颜色
         * @param r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static Rgba Get(ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover = COVER_FULL)
        {
            if (cover > COVER_NONE) {
                Rgba c(ColorType::ToDouble(r), ColorType::ToDouble(g), ColorType::ToDouble(b), ColorType::ToDouble(a));

                if (cover < COVER_FULL) {
                    double x = double(cover) / COVER_FULL;
                    c.redValue *= x;
                    c.greenValue *= x;
                    c.blueValue *= x;
                    c.alphaValue *= x;
                }

                return c;
            } else {
                return Rgba::NoColor();
            }
        }

        /**
         * @brief 通过颜色分量获取颜色.
         * @return 返回颜色
         * @param pColor 颜色,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static Rgba Get(const ValueType* pColor, CoverType cover = COVER_FULL)
        {
            return Get(pColor[OrderType::RED], pColor[OrderType::GREEN],
                       pColor[OrderType::BLUE], pColor[OrderType::ALPHA], cover);
        }
    };
} // namespace OHOS

#endif
