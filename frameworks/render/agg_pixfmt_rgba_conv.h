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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_CONV_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_CONV_INCLUDED

#include <cmath>
#include <cstring>

#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"
#include "heap_base.h"
namespace OHOS {
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
}