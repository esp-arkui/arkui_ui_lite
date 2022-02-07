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
 * @brief Defines The operation method of pixel and color component conversion is defined.
 *
 * @since 1.0
 * @version 1.0.
 */

#ifndef GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_BASE_INCLUDED

#include "gfx_utils/color.h"
#include "gfx_utils/diagram/common/graphic_common_basics.h"

namespace OHOS {
    /**
     *
     * @brief Defines Blenderbase.
     * @since 1.0
     * @version 1.0
     */
    template <class ColorT, class Order = OrderBgra>
    struct BlenderBase {
        using ColorType = ColorT;
        using OrderType = Order;

        /**
         * @brief Set the color component to the color.
         * @param r,g,b,a Color Component, cover Coverage.
         * @since 1.0
         * @version 1.0.
         */
        static void SetBlendColor(uint8_t* pColor, uint8_t redValue, uint8_t greenValue, uint8_t blueValue,
                                  uint8_t alphaValue)
        {
            pColor[OrderType::RED] = redValue;
            pColor[OrderType::GREEN] = greenValue;
            pColor[OrderType::BLUE] = blueValue;
            pColor[OrderType::ALPHA] = alphaValue;
        }

        /**
         * @brief Set the color component to the color.
         * @param pColor Color,color Pixel color component
         * @since 1.0
         * @version 1.0
         */
        static void SetBlendColor(uint8_t* pColor, const Rgba& color)
        {
            pColor[OrderType::RED] = ColorType::FromFloat(color.redValue);
            pColor[OrderType::GREEN] = ColorType::FromFloat(color.greenValue);
            pColor[OrderType::BLUE] = ColorType::FromFloat(color.blueValue);
            pColor[OrderType::ALPHA] = ColorType::FromFloat(color.alphaValue);
        }

        /**
         * @brief Get the color from the color component.
         * @return Return color.
         * @param r,g,b,a Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0.
         */
        static Rgba GetBlendColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue,
                                  uint8_t alphaValue, uint8_t cover = COVER_FULL)
        {
            if (cover > COVER_NONE) {
                Rgba resultColor(ColorType::ToFloat(redValue), ColorType::ToFloat(greenValue),
                                 ColorType::ToFloat(blueValue), ColorType::ToFloat(alphaValue));

                if (cover < COVER_FULL) {
                    float coverX = float(cover) / COVER_FULL;
                    resultColor.redValue *= coverX;
                    resultColor.greenValue *= coverX;
                    resultColor.blueValue *= coverX;
                    resultColor.alphaValue *= coverX;
                }

                return resultColor;
            } else {
                return Rgba::NoColor();
            }
        }

        /**
         * @brief Get the color from the color component.
         * @return Return color.
         * @param pColor Color,cover Coverage
         * @since 1.0
         * @version 1.0
         */
        static Rgba GetBlendColor(const uint8_t* pColor, uint8_t cover = COVER_FULL)
        {
            return GetBlendColor(pColor[OrderType::RED], pColor[OrderType::GREEN],
                                 pColor[OrderType::BLUE], pColor[OrderType::ALPHA], cover);
        }
    };
} // namespace OHOS

#endif
