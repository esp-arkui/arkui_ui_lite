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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_GAMMA_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_GAMMA_INCLUDED

#include <cmath>
#include <cstring>

#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"
#include "heap_base.h"
namespace OHOS {


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

} // namespace OHOS