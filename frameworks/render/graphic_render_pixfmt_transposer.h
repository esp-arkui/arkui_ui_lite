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
 * limitations under the License..
 */
#ifndef AGG_PIXFMT_TRANSPOSER_INCLUDED
#define AGG_PIXFMT_TRANSPOSER_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
namespace OHOS {
    template <class PixFmt>
    class GraphicRenderPixfmtTransposer : public HeapBase {
    public:
        using PixfmtType = PixFmt;
        using ColorType = typename PixfmtType::ColorType;
        using RowData = typename PixfmtType::RowData;

        GraphicRenderPixfmtTransposer() : pixf_(0) {}

        /**
         * @brief Construct a pixfmttransposer instance to initialize pixf_ Members.
         * @Param pixf Screen buffer.
         * @since 1.0
         * @version 1.0
         */
        explicit GraphicRenderPixfmtTransposer(PixfmtType& pixf) : pixf_(&pixf) {}

        /**
         * @brief Additional screen.
         * @Param pixf Screen buffer.
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixfmtType& pixf)
        {
            pixf_ = &pixf;
        }

        /**
         * @brief Get screen width.
         * @return width.
         * @since 1.0
         * @version 1.0
         */
        inline unsigned Width() const
        {
            return pixf_->Height();
        }

        /**
         * @brief Get screen height.
         * @return height.
         * @since 1.0
         * @version 1.0
         */
        inline unsigned Height() const
        {
            return pixf_->Width();
        }

        /**
         * @brief Gets the color of the specified coordinates.
         * @return Color.
         * @since 1.0
         * @version 1.0
         */
        inline ColorType Pixel(int x, int y) const
        {
            return pixf_->Pixel(y, x);
        }

        /**
         * @brief Sets the color of the specified coordinates.
         *
         * @since 1.0
         * @version 1.0.
         */
        inline void CopyPixel(int x, int y, const ColorType& c)
        {
            pixf_->CopyPixel(y, x, c);
        }

        /**
         * @brief Blend colors from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendPixel(int x, int y,
                                                const ColorType& color,
                                                uint8_t cover)
        {
            pixf_->BlendPixel(y, x, color, cover);
        }

        /**
         * @brief Copy the color of len length horizontally from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void CopyHline(int x, int y,
                                               unsigned len,
                                               const ColorType& color)
        {
            pixf_->copy_vline(y, x, len, color);
        }

        /**
         * @brief Copy the color of len length vertically from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void CopyVline(int x, int y,
                                               unsigned len,
                                               const ColorType& color)
        {
            pixf_->CopyHline(y, x, len, color);
        }

        /**
         * @brief Blends the color of len length horizontally from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendHline(int x, int y,
                                                unsigned len,
                                                const ColorType& color, uint8_t cover)
        {
            pixf_->BlendVline(y, x, len, color, cover);
        }

        /**
         * @brief Blends the color of the len length vertically from the specified x, y
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendVline(int x, int y,
                                                unsigned len,
                                                const ColorType& color,
                                                uint8_t cover)
        {
            pixf_->BlendHline(y, x, len, color, cover);
        }

        /**
         * @brief Horizontally blends a series of colors of len length from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendSolidHspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType& c,
                                                     const uint8_t* covers)
        {
            pixf_->BlendSolidVspan(y, x, len, c, covers);
        }

        /**
         * @brief Blends a series of colors of len length vertically from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendSolidVspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType& c,
                                                     const uint8_t* covers)
        {
            pixf_->BlendSolidHspan(y, x, len, c, covers);
        }

        /**
         * @brief Copies a series of colors of len length horizontally from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void CopyColorHspan(int x, int y,
                                                    unsigned len,
                                                    const ColorType* colors)
        {
            pixf_->CopyColorVspan(y, x, len, colors);
        }

        /**
         * @brief Copies a series of colors of len length vertically from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void CopyColorVspan(int x, int y,
                                                    unsigned len,
                                                    const ColorType* colors)
        {
            pixf_->CopyColorHspan(y, x, len, colors);
        }

        /**
         * @brief Horizontally blends a series of colors of len length from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendColorHspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType* colors,
                                                     const uint8_t* covers,
                                                     uint8_t cover)
        {
            pixf_->BlendColorVspan(y, x, len, colors, covers, cover);
        }

        /**
         * @brief Blends a series of colors of len length vertically from the specified x, y.
         *
         * @since 1.0
         * @version 1.0
         */
        inline void BlendColorVspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType* colors,
                                                     const uint8_t* covers,
                                                     uint8_t cover)
        {
            pixf_->BlendColorHspan(y, x, len, colors, covers, cover);
        }

    private:
        PixfmtType* pixf_;
    };
} // namespace OHOS

#endif
