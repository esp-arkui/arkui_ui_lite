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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_color/agg_color_gray.h"
#include "gfx_utils/graphics/graphic_color/agg_color_rgba.h"

namespace OHOS {
struct PixfmtGrayTag
{
};

struct PixfmtRgbTag
{
};

struct PixfmtRgbaRag
{
};

template<class ColorT, class Order = void> 
struct blenderbase
{
    using ColorType = ColorT;
    using OrderType = Order;
    typedef typename ColorType::ValueType ValueType;

    static void Set(ValueType* p, ValueType r, ValueType g, ValueType b, ValueType a)
    {
        p[OrderType::R] = r;
        p[OrderType::G] = g;
        p[OrderType::B] = b;
        p[OrderType::A] = a;
    }

    static void set(value_type* p, const rgba& c)
    {
        p[OrderType::R] = ColorType::fromDouble(c.r);
        p[OrderType::G] = ColorType::fromDouble(c.g);
        p[OrderType::B] = ColorType::fromDouble(c.b);
        p[OrderType::A] = ColorType::fromDouble(c.a);
    }

    static Rgba Get(ValueType r, ValueType g, ValueType b, ValueType a, CoverType cover = COVER_FULL)
    {
        if (cover > COVER_NONE)
        {
            Rgba c(ColorType::toDouble(r), 
                ColorType::toDouble(g), 
                ColorType::toDouble(b), 
                ColorType::toDouble(a));

            if (cover < COVER_FULL)
            {
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

    static rgba Get(const ValueType* p, CoverType cover = COVER_FULL)
    {
        return Get(
            p[OrderType::R], 
            p[OrderType::G], 
            p[OrderType::B], 
            p[OrderType::A], 
            cover);
    }

};
}

#endif
