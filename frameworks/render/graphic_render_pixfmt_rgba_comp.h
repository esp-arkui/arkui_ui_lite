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
    /**
     * @brief 颜色分量裁剪.
     *
     * @since 1.0
     * @version 1.0.
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
    struct CompOpRgbaSrc : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            if (cover >= COVER_FULL) {
                SetBlendColor(pColor, r, g, b, a);
            } else {
                Rgba s = GetBlendColor(r, g, b, a, cover);
                Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
                d.redValue += s.redValue;
                d.greenValue += s.greenValue;
                d.blueValue += s.blueValue;
                d.alphaValue += s.alphaValue;
                SetBlendColor(pColor, d);
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaMinus : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba srcColor = GetBlendColor(r, g, b, a, cover);
            if (srcColor.alphaValue > 0) {
                Rgba destColor = GetBlendColor(pColor);
                destColor.alphaValue += srcColor.alphaValue - srcColor.alphaValue * destColor.alphaValue;
                destColor.redValue = MATH_MAX(destColor.redValue - srcColor.redValue, 0.0f);
                destColor.greenValue = MATH_MAX(destColor.greenValue - srcColor.greenValue, 0.0f);
                destColor.blueValue = MATH_MAX(destColor.blueValue - srcColor.blueValue, 0.0f);
                SetBlendColor(pColor, Clip(destColor));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaLighten : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            if (s.alphaValue > 0) {
                Rgba d = GetBlendColor(pColor);
                float d1a = 1 - d.alphaValue;
                float s1a = 1 - s.alphaValue;
                d.redValue = MATH_MAX(s.redValue * d.alphaValue, d.redValue * s.alphaValue) +
                             s.redValue * d1a + d.redValue * s1a;
                d.greenValue = MATH_MAX(s.greenValue * d.alphaValue, d.greenValue * s.alphaValue) +
                               s.greenValue * d1a + d.greenValue * s1a;
                d.blueValue = MATH_MAX(s.blueValue * d.alphaValue, d.blueValue * s.alphaValue) +
                              s.blueValue * d1a + d.blueValue * s1a;
                d.alphaValue += s.alphaValue - s.alphaValue * d.alphaValue;
                SetBlendColor(pColor, Clip(d));
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaXor : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            float s1a = 1 - s.alphaValue;
            float d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.redValue = s.redValue * d1a + d.redValue * s1a;
            d.greenValue = s.greenValue * d1a + d.greenValue * s1a;
            d.blueValue = s.blueValue * d1a + d.blueValue * s1a;
            d.alphaValue = s.alphaValue + d.alphaValue - 2 * s.alphaValue * d.alphaValue;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDstAtop : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba sc = GetBlendColor(r, g, b, a, cover);
            Rgba dc = GetBlendColor(pColor, cover);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            float sa = ColorT::ToFloat(a);
            float d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.redValue += dc.redValue * sa + sc.redValue * d1a;
            d.greenValue += dc.greenValue * sa + sc.greenValue * d1a;
            d.blueValue += dc.blueValue * sa + sc.blueValue * d1a;
            d.alphaValue += sc.alphaValue;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcAtop : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            float s1a = 1 - s.alphaValue;
            d.redValue = s.redValue * d.alphaValue + d.redValue * s1a;
            d.greenValue = s.greenValue * d.alphaValue + d.greenValue * s1a;
            d.blueValue = s.blueValue * d.alphaValue + d.greenValue * s1a;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstOut : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t, uint8_t, uint8_t, uint8_t a, CoverType cover)
        {
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            Rgba dc = GetBlendColor(pColor, cover);
            float s1a = 1 - ColorT::ToFloat(a);
            d.redValue += dc.redValue * s1a;
            d.greenValue += dc.greenValue * s1a;
            d.blueValue += dc.blueValue * s1a;
            d.alphaValue += dc.alphaValue * s1a;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcOut : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            float d1a = 1 - ColorT::ToFloat(pColor[Order::ALPHA]);
            d.alphaValue += s.alphaValue * d1a;
            d.redValue += s.redValue * d1a;
            d.greenValue += s.greenValue * d1a;
            d.blueValue += s.blueValue * d1a;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstIn : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t, uint8_t, uint8_t, uint8_t a, CoverType cover)
        {
            float sa = ColorT::ToFloat(a);
            Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
            Rgba d2 = GetBlendColor(pColor, cover);
            d.redValue += d2.redValue * sa;
            d.greenValue += d2.greenValue * sa;
            d.blueValue += d2.blueValue * sa;
            d.alphaValue += d2.alphaValue * sa;
            SetBlendColor(pColor, d);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaSrcIn : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            float da = ColorT::ToFloat(pColor[Order::ALPHA]);
            if (da > 0) {
                Rgba s = GetBlendColor(r, g, b, a, cover);
                Rgba d = GetBlendColor(pColor, COVER_FULL - cover);
                d.redValue += s.redValue * da;
                d.greenValue += s.greenValue * da;
                d.blueValue += s.blueValue * da;
                d.alphaValue += s.alphaValue * da;
                SetBlendColor(pColor, d);
            }
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaSrcOver : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0..
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            RgbaPrelerpBlender<ColorT, Order>::BlendPix(pColor, r, g, b, a, cover);
        }
    };
    template <class ColorT, class Order>
    struct CompOpRgbaDstOver : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        using BlenderBase<ColorT, Order>::GetBlendColor;
        using BlenderBase<ColorT, Order>::SetBlendColor;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            Rgba s = GetBlendColor(r, g, b, a, cover);
            Rgba d = GetBlendColor(pColor);
            float d1a = 1 - d.alphaValue;
            d.redValue += s.redValue * d1a;
            d.greenValue += s.greenValue * d1a;
            d.blueValue += s.blueValue * d1a;
            d.alphaValue += s.alphaValue * d1a;
            SetBlendColor(pColor, d);
        }
    };

    template <class ColorT, class Order>
    struct CompOpRgbaDst : BlenderBase<ColorT, Order> {
        using ColorType = ColorT;
        using uint8_t = typename ColorType::uint8_t;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, CoverType)
        {
        }
    };

    template <class ColorT, class Order>
    struct CompOpTableRgba {
        using uint8_t = typename ColorT::uint8_t;
        using uint32_t = typename ColorT::uint32_t;
        using CompOpFuncType =
            void (*)(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca, CoverType cover);
        static CompOpFuncType g_compOpFunc[];
    };

    template <class ColorT, class Order>
    typename CompOpTableRgba<ColorT, Order>::CompOpFuncType CompOpTableRgba<ColorT, Order>::g_compOpFunc[] = {
        CompOpRgbaSrcOver<ColorT, Order>::BlendPix,
        CompOpRgbaSrcAtop<ColorT, Order>::BlendPix,
        CompOpRgbaSrcIn<ColorT, Order>::BlendPix,
        CompOpRgbaSrcOut<ColorT, Order>::BlendPix,
        CompOpRgbaDstOver<ColorT, Order>::BlendPix,
        CompOpRgbaDstAtop<ColorT, Order>::BlendPix,
        CompOpRgbaDstIn<ColorT, Order>::BlendPix,
        CompOpRgbaDstOut<ColorT, Order>::BlendPix,
        CompOpRgbaLighten<ColorT, Order>::BlendPix,
        CompOpRgbaSrc<ColorT, Order>::BlendPix,
        CompOpRgbaXor<ColorT, Order>::BlendPix,
        CompOpRgbaDst<ColorT, Order>::BlendPix,
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
            unsigned op, uint8_t* pColor, uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor,
                                                             ColorType::Multiply(r, a),
                                                             ColorType::Multiply(g, a),
                                                             ColorType::Multiply(b, a),
                                                             a, cover);
        }
    };

    template <class ColorT, class Order>
    struct CompOpAdaptorRgbaPre {
        using ColorType = ColorT;
        using OrderType = Order;
        using uint8_t = typename ColorType::uint8_t;
        using uint32_t = typename ColorType::uint32_t;
        using int32_t = typename ColorType::int32_t;
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         * @param op 函数表保存了各个子操作的函数的index, pColor 像素 r,g,b,a 颜色分量,cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
            BlendPix(unsigned op, uint8_t* pColor,
                     uint8_t r, uint8_t g, uint8_t b, uint8_t a, CoverType cover)
        {
            CompOpTableRgba<ColorT, Order>::g_compOpFunc[op](pColor, r, g, b, a, cover);
        }
    };
} // namespace OHOS
#endif
