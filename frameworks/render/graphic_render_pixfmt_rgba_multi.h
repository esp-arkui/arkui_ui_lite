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

#include "gfx_utils/heap_base.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"
namespace OHOS {
    /**
     * @brief 颜色分量裁剪.
     *
     * @since 1.0
     * @version 1.0
     */
    static Rgba& Clip(Rgba& color)
    {
        if (color.alphaValue > 1) {
            color.alphaValue = 1;
        } else if (color.alphaValue < 0) {
            color.alphaValue = 0;
        }

        if (color.blueValue > color.alphaValue) {
            color.blueValue = color.alphaValue;
        } else if (color.blueValue < 0) {
            color.blueValue = 0;
        }

        if (color.redValue > color.alphaValue) {
            color.redValue = color.alphaValue;
        } else if (color.redValue < 0) {
            color.redValue = 0;
        }

        if (color.greenValue > color.alphaValue) {
            color.greenValue = color.alphaValue;
        } else if (color.greenValue < 0) {
            color.greenValue = 0;
        }
        return color;
    }

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
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType vRed, ValueType vGreen, ValueType vBlue, ValueType vAlpha, CoverType cover)
        {
            Rgba sColor = Get(vRed, vGreen, vBlue, vAlpha, cover);
            if (sColor.alphaValue > 0) {
                Rgba dColor = Get(pColor);
                double s1a = 1 - sColor.alphaValue;
                double d1a = 1 - dColor.alphaValue;
                dColor.redValue = sColor.redValue * dColor.redValue + sColor.redValue * d1a + dColor.redValue * s1a;
                dColor.greenValue = sColor.greenValue * dColor.greenValue + sColor.greenValue * d1a + dColor.greenValue * s1a;
                dColor.blueValue = sColor.blueValue * dColor.blueValue + sColor.blueValue * d1a + dColor.blueValue * s1a;
                dColor.alphaValue += sColor.alphaValue - sColor.alphaValue * dColor.alphaValue;
                Set(pColor, Clip(dColor));
            }
        }
    };
} // namespace OHOS
#endif
