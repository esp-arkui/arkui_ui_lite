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
* @file graphic_geometry_pixfmt_rgba.h
*
* @brief Defines 颜色分量gamma操作类.
*
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_CONV_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_CONV_INCLUDED

#include <cmath>
#include <cstring>

#include "gfx_utils/heap_base.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_buffer.h"
namespace OHOS {
    template <class ColorT, class Order>
    struct ConvRgbaPre {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        /**
         * @brief 设置素色
         *
         * @param pColor 像素指针，color指定的颜色
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void SetPlainColor(ValueType* pColor, ColorType color)
        {
            color.Premultiply();
            pColor[Order::ALPHA] = color.alphaValue;
            pColor[Order::RED] = color.redValue;
            pColor[Order::BLUE] = color.blueValue;
            pColor[Order::GREEN] = color.greenValue;
        }
        /**
         * @brief 获取素色
         *
         * @param pColor 像素指针
         * @return color指定的颜色
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE ColorType GetPlainColor(const ValueType* p)
        {
            return ColorType(p[Order::RED], p[Order::GREEN], p[Order::BLUE], p[Order::ALPHA])
                .Demultiply();
        }
    };

    template <class ColorT, class Order>
    struct ConvRgbaPlain {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        /**
         * @brief 设置素色
         *
         * @param pColor 像素指针，color指定的颜色
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void SetPlainColor(ValueType* pColor, ColorType color)
        {
            pColor[Order::ALPHA] = color.alphaValue;
            pColor[Order::RED] = color.redValue;
            pColor[Order::BLUE] = color.blueValue;
            pColor[Order::GREEN] = color.greenValue;
        }
        /**
         * @brief 获取素色
         *
         * @param pColor 像素指针
         * @return color指定的颜色
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE ColorType GetPlainColor(const ValueType* pColor)
        {
            return ColorType(pColor[Order::RED], pColor[Order::GREEN], pColor[Order::BLUE], pColor[Order::ALPHA]);
        }
    };
} // namespace OHOS
#endif
