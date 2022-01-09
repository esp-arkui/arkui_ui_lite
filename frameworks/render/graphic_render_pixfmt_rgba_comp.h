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
* @version 1.0.
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

#include "gfx_utils/graphic_math.h"
#include "gfx_utils/heap_base.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_pixfmt_rgba_blend.h"
namespace OHOS {
    template <class ColorT, class Order>
    struct CompOpTableRgba {
        using uint8_t = typename ColorT::uint8_t;
        using uint32_t = typename ColorT::uint32_t;
        using CompOpFuncType =
            void (*)(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca, uint8_t cover);
        static CompOpFuncType g_compOpFunc[];
    };

    template <class ColorT, class Order>
    typename CompOpTableRgba<ColorT, Order>::CompOpFuncType CompOpTableRgba<ColorT, Order>::g_compOpFunc[] = {
        0};

    template <class ColorT, class Order>
    struct CompOpAdaptorRgba {
        using ColorType = ColorT;
        using OrderType = Order;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            unsigned op, uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t cover)
        {
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor,
                                                             ColorType::Multiply(r, a),
                                                             ColorType::Multiply(g, a),
                                                             ColorType::Multiply(b, a),
                                                             a, cover);
        }
    };
} // namespace OHOS
#endif
