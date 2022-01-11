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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED

#include <cmath>
#include <cstring>

#include "engines/gfx/gfx_engine_manager.h"
#include "gfx_utils/heap_base.h"
#include "graphic_render_pixfmt_rgba_gamma.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"
#ifdef ARM_NEON_OPT
#    include "graphic_neon_pipeline.h"
#endif
namespace OHOS {
#define BASE_PIXFMT_BLEND_USING_DEF \
    using RbufType = RenBuf; \
    using RowData = typename RbufType::rowData; \
    using BlenderType = Blender; \
    using ColorType = typename BlenderType::ColorType; \
    using OrderType = typename BlenderType::OrderType; \
    using PixelType = PixelType<uint8_t, OrderType, ColorType> \

#define BLEND_HLINE_MASK \
    if (color.IsOpaque() && cover == COVER_MASK) { \
        for (int16_t iPixel = 0; iPixel < len; ++iPixel) { \
            PixelType pixelValue; \
            pixelValue.SetPixelColor(color); \
            *pPixel = pixelValue; \
            pPixel = pPixel->Next(); \
        } \
    } else { \
        if (cover == COVER_MASK) { \
            for (int16_t iPixel = 0; iPixel < len; ++iPixel) { \
                BlendPix(pPixel, color); \
                pPixel = pPixel->Next(); \
            } \
        } else { \
            for (int16_t iPixel = 0; iPixel < len; ++iPixel) { \
                BlendPix(pPixel, color, cover); \
                pPixel = pPixel->Next(); \
            } \
        } \
    }

    enum {
        NUM_COMPONENTS = 4,
        PIX_STEP = 4
    };

    template <class ColorT, class Order>
    struct RgbaBlender {
        using ColorType = ColorT;
        using OrderType = Order;

#ifdef ARM_NEON_OPT
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t* covers)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg, (uint8_t)cb,
                                                       (uint8_t)alpha, (uint8_t*)covers);
        }
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg, (uint8_t)cb,
                                                       (uint8_t)alpha, (uint8_t)cover);
        }
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                       (uint8_t)cb, (uint8_t)alpha);
        }
        static inline void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t srcCover)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t)srcCover);
        }

        static inline void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t* srcCovers)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t*)srcCovers);
        }
#endif
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void BlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
        }

        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component
         * @since 1.0
         * @version 1.0
         */
        static inline void BlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            pColor[Order::RED] = ColorType::Lerp(pColor[Order::RED], cr, alpha);
            pColor[Order::GREEN] = ColorType::Lerp(pColor[Order::GREEN], cg, alpha);
            pColor[Order::BLUE] = ColorType::Lerp(pColor[Order::BLUE], cb, alpha);
            pColor[Order::ALPHA] = ColorType::Prelerp(pColor[Order::ALPHA], alpha, alpha);
        }
    };

    template <class ColorT, class Order>
    struct RgbaPrelerpBlender {
        using ColorType = ColorT;
        using OrderType = Order;
        using uint8_t = typename ColorType::uint8_t;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;
#ifdef ARM_NEON_OPT
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t* covers)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                          (uint8_t)cb, (uint8_t)alpha, (uint8_t*)covers);
        }
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                          (uint8_t)cb, (uint8_t)alpha, (uint8_t)cover);
        }
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component.
         * @since 1.0
         * @version 1.0
         */
        static inline void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr,
                                                          (uint8_t)cg, (uint8_t)cb, (uint8_t)alpha);
        }

        static inline void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t srcCover)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t)srcCover);
        }

        static inline void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t* srcCovers)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors,
                                                          (uint8_t*)srcCovers);
        }
#endif
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component,cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        static inline void
            BlendPix(uint8_t* pColor,
                     uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BlendPix(pColor,
                     ColorType::MultCover(cr, cover),
                     ColorType::MultCover(cg, cover),
                     ColorType::MultCover(cb, cover),
                     ColorType::MultCover(alpha, cover));
        }
        /**
         * @brief Mix the pixels with the color component.
         * @param pColor color,cr,cg,cb,alpha Color Component.
         * @since 1.0
         * @version 1.0
         */
        static inline void BlendPix(uint8_t* pColor,
                                                     uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            pColor[Order::RED] = ColorType::Prelerp(pColor[Order::RED], cr, alpha);
            pColor[Order::GREEN] = ColorType::Prelerp(pColor[Order::GREEN], cg, alpha);
            pColor[Order::BLUE] = ColorType::Prelerp(pColor[Order::BLUE], cb, alpha);
            pColor[Order::ALPHA] = ColorType::Prelerp(pColor[Order::ALPHA], alpha, alpha);
        }
    };

    template <class uint8_t, class OrderType, class ColorType>
    struct PixelType {
        uint8_t colors[NUM_COMPONENTS];
        /**
         * @brief Set Colors.
         * @param r,g,b,a Color Component.
         * @since 1.0
         * @version 1.0
         */
        void SetPixelColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue, uint8_t alphaValue)
        {
            colors[OrderType::RED] = redValue;
            colors[OrderType::GREEN] = greenValue;
            colors[OrderType::BLUE] = blueValue;
            colors[OrderType::ALPHA] = alphaValue;
        }
        /**
         * @brief Set Colors.
         * @param color Colors.
         * @since 1.0
         * @version 1.0
         */
        void SetPixelColor(const ColorType& color)
        {
            SetPixelColor(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
        }
        /**
         * @brief Get Colors.
         * @param r,g,b,a Color Component.
         * @since 1.0
         * @version 1.0
         */
        void GetPixelColor(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const
        {
            red = colors[OrderType::RED];
            green = colors[OrderType::GREEN];
            blue = colors[OrderType::BLUE];
            alpha = colors[OrderType::ALPHA];
        }
        /**
         * @brief Get Colors.
         * @return Colors
         * @since 1.0
         * @version 1.0
         */
        ColorType GetPixelColor() const
        {
            return ColorType(colors[OrderType::RED], colors[OrderType::GREEN],
                             colors[OrderType::BLUE], colors[OrderType::ALPHA]);
        }
        /**
        * @brief Gets the color component of the next pixel.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Next()
        {
            return (PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief Gets the first address of the color component of the next pixel.
        *
        * @since 1.0
        * @version 1.0
        */
        const PixelType* Next() const
        {
            return (const PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief Gets the first address of the color component of the nth pixe.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Advance(int n)
        {
            return (PixelType*)(colors + n * PIX_STEP);
        }
        /**
        * @brief Gets the first address of the color component of the nth pixe.
        *
        * @since 1.0
        * @version 1.0
        */
        const PixelType* Advance(int n) const
        {
            return (const PixelType*)(colors + n * PIX_STEP);
        }
    };

    template <class Blender, class RenBuf>
    class GraphicRenderPixfmtRgbaBlend : public HeapBase {
    public:
        BASE_PIXFMT_BLEND_USING_DEF;
        enum {
            PIX_WIDTH = sizeof(uint8_t) * PIX_STEP
        };

    public:
        GraphicRenderPixfmtRgbaBlend() : rbuf_(0) {}
        explicit GraphicRenderPixfmtRgbaBlend(RbufType& rb) :  rbuf_(&rb) {}

        /**
        * @brief Attach pixels to the drawing area.
        *
        * @since 1.0
        * @version 1.0
        */
        virtual void Attach(RbufType& rb)
        {
            rbuf_ = &rb;
        }

        template <class PixFmt>
        bool Attach(PixFmt& pixf, int x1, int y1, int x2, int y2)
        {
            RectI r(x1, y1, x2, y2);
            if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) {
                int stride = pixf.Stride(); \
                rbuf_->Attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1),
                              (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
                return true;
            }
            return false;
        }

        /**
        * @brief Get the size of each screen (draw buffer).
        *
        * @since 1.0
        * @version 1.0
        */
        virtual inline unsigned Width() const
        {
            return rbuf_->GetWidth();
        }

        virtual inline unsigned Height() const
        {
            return rbuf_->GetHeight();
        }

        /**
          * @brief Returns the number of bytes in a row of the window.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual inline int Stride() const
        {
            return rbuf_->GetStride();
        }

        /**
          * @brief Returns the row address of a window row.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual inline uint8_t* RowPtr(int y)
        {
            return rbuf_->RowPtr(y);
        }

        /**
          * @brief Returns the row address of a window row.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual inline const uint8_t* RowPtr(int y) const
        {
            return rbuf_->RowPtr(y);
        }

        /**
          * @brief Returns the row data of one row of the window.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual inline RowData Row(int y) const
        {
            return rbuf_->Row(y);
        }

        virtual inline uint8_t* PixPtr(int x, int y)
        {
            return rbuf_->RowPtr(y) + sizeof(uint8_t) * (x * PIX_STEP);
        }

        virtual inline const uint8_t* PixPtr(int x, int y) const
        {
            return rbuf_->RowPtr(y) + sizeof(uint8_t) * (x * PIX_STEP);
        }

        /**
         * @brief Pointer to pixel type pointer.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline PixelType* PixValuePtr(int x, int y, unsigned len)
        {
            return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(uint8_t) * (x * PIX_STEP));
        }

        /**
         * @brief Gets the pixel address of the specified left.
         *
         * @since 1.0
         * @version 1.0.
         */
        virtual inline const PixelType* PixValuePtr(int x, int y) const
        {
            uint8_t* pixelPtr = rbuf_->RowPtr(y);
            return pixelPtr ? (PixelType*)(pixelPtr + sizeof(uint8_t) * (x * PIX_STEP)) : nullptr;
        }

        /**
         * @brief Pointer to pixel type pointer.
         *
         * @since 1.0
         * @version 1.0
         */
        inline static PixelType* PixValuePtr(void* pixelPtr)
        {
            return (PixelType*)pixelPtr;
        }

        /**
         * @brief Pointer to pixel type pointer.
         *
         * @since 1.0
         * @version 1.0
         */
        inline static const PixelType* PixValuePtr(const void* pixelPtr)
        {
            return (const PixelType*)pixelPtr;
        }

        /**
         * @brief Set a color for an address.
         *
         * @since 1.0
         * @version 1.0
         */
        inline static void SetPixelColor(uint8_t* pixelPtr, const ColorType& color)
        {
            ((PixelType*)pixelPtr)->SetPixelColor(color);
        }

        /**
         * @brief Gets the color of a pixel.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline ColorType Pixel(int x, int y) const
        {
            if (const PixelType* pixelPtr = PixValuePtr(x, y)) {
                return pixelPtr->GetPixelColor();
            }
            return ColorType::NoColor();
        }

        /**
         * @brief Copy color to pixel.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline void CopyPixel(int x, int y, const ColorType& color)
        {
            PixValuePtr(x, y, 1)->SetPixelColor(color);
        }

        /**
        * @brief Mix pixels (x, y) with the transparency of cover (coverage = transparency) with color color.
        *
        * @since 1.0
        * @version 1.0
        */
        virtual inline void BlendPixel(int x, int y, const ColorType& color, uint8_t cover)
        {
            CopyOrBlendPix(PixValuePtr(x, y, 1), color, cover);
        }
        /**
         * @brief Set the pixels of len length in horizontal order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void CopyHline(int x, int y,
                                      unsigned len,
                                      const ColorType& color)
        {
            PixelType vPixelValue;
            vPixelValue.SetPixelColor(color);
            PixelType* pixelPtr = PixValuePtr(x, y, len);
#ifdef NEON_ARM_OPT
            int16_t step = NEON_STEP_8 * PIX_STEP;
            while (len >= NEON_STEP_8) {
                OHOS::SetPixelColor_ARGB8888(pixelPtr->colors, color->redValue,
                                             colors->greenValue, colors->blueValue,
                                             colors->alphaValue);
                pixelPtr = pixelPtr->colors + step;
                len -= NEON_STEP_8;
            };
#endif
            for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                *pixelPtr = vPixelValue;
                pixelPtr = pixelPtr->Next();
            }
        }

        /**
         * @brief Pixels of len length are mixed horizontally from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void BlendHline(int x, int y,
                                unsigned len,
                                const ColorType& color,
                                uint8_t cover)
        {
            if (!color.IsTransparent()) {
                PixelType* pPixel = PixValuePtr(x, y, len);
#ifdef NEON_ARM_OPT
                int16_t step = NEON_STEP_8 * PIX_STEP;
                while (len >= NEON_STEP_8) {
                    NeonBlendPix(pixelPtr, color, cover);
                    pixelPtr = pixelPtr->colors + step;
                    len -= NEON_STEP_8;
                };
#endif
                BLEND_HLINE_MASK
            }
        }

        /**
         * @brief Mix a series of colors of len length in horizontal order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void BlendSolidHspan(int x, int y,
                                     unsigned len,
                                     const ColorType& color,
                                     const uint8_t* covers)
        {
            if (!color.IsTransparent()) {
                PixelType* pixelPtr = PixValuePtr(x, y, len);

#ifdef NEON_ARM_OPT
                int16_t step = NEON_STEP_8 * PIX_STEP;
                while (len >= NEON_STEP_8) {
                    NeonBlendPix(pixelPtr->colors, color, covers);
                    pixelPtr = pixelPtr->colors + step;
                    covers += NEON_STEP_8;
                    len -= NEON_STEP_8;
                };
#endif

                for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                    if (color.IsOpaque() && *covers == COVER_MASK) {
                        pixelPtr->SetPixelColor(color);
                    } else {
                        BlendPix(pixelPtr, color, *covers);
                    }
                    pixelPtr = pixelPtr->Next();
                    ++covers;
                }
            }
        }
        /**
         * @brief Mix a series of colors of len length in vertical order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void BlendSolidVspan(int x, int y,
                                     unsigned len,
                                     const ColorType& color,
                                     const uint8_t* covers)
        {
            if (!color.IsTransparent()) {
                do {
                    PixelType* pixelPtr = PixValuePtr(x, y++, 1);
                    if (color.IsOpaque() && *covers == COVER_MASK) {
                        pixelPtr->SetPixelColor(color);
                    } else {
                        BlendPix(pixelPtr, color, *covers);
                    }
                    ++covers;
                } while (--len);
            }
        }

        /**
         * @brief Set the color of len length in horizontal order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void CopyColorHspan(int x, int y,
                                    unsigned len,
                                    const ColorType* colors)
        {
            PixelType* pixelPtr = PixValuePtr(x, y, len);
#ifdef NEON_ARM_OPT
            int16_t step = NEON_STEP_8 * PIX_STEP;
            const int16_t NEON_STEP_COMPONENTS = NEON_STEP_8 * NUM_COMPONENTS;
            uint8_t mColors[NEON_STEP_COMPONENTS];
            while (len >= NEON_STEP_8) {
                memset_s(mColors, size_t(NEON_STEP_COMPONENTS), 0, size_t(NEON_STEP_COMPONENTS));
                NeonMemcpy(mColors, NEON_STEP_COMPONENTS, colors, NEON_STEP_COMPONENTS);

                OHOS::SetPixelColor_ARGB8888(pixelPtr->colors, mColors);
                pixelPtr = pixelPtr->colors + step;
                colors += NEON_STEP_8;
                len -= NEON_STEP_8;
            };
#endif
            for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                pixelPtr->SetPixelColor(*colors++);
                pixelPtr = pixelPtr->Next();
            }
        }

        /**
         * @brief Set the color of len length in vertical order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void CopyColorVspan(int x, int y,
                                    unsigned len,
                                    const ColorType* colors)
        {
            do {
                PixValuePtr(x, y++, 1)->SetPixelColor(*colors++);
            } while (--len);
        }

        /**
         * @brief Mix a series of colors of len length in horizontal order starting from (x, y).
         *
         * @since 1.0
         * @version 1.0
         */
        virtual void BlendColorHspan(int x, int y,
                                     unsigned len,
                                     const ColorType* colors,
                                     const uint8_t* covers,
                                     uint8_t cover)
        {
            PixelType* pixelPtr = PixValuePtr(x, y, len);
            if (covers) {
#ifdef NEON_ARM_OPT
                int16_t step = NEON_STEP_8 * PIX_STEP;
                const int16_t NEON_STEP_COMPONENTS = NEON_STEP_8 * NUM_COMPONENTS;
                uint8_t mColors[NEON_STEP_COMPONENTS];
                while (len >= NEON_STEP_8) {
                    memset_s(mColors, size_t(NEON_STEP_COMPONENTS), 0, size_t(NEON_STEP_COMPONENTS));
                    NeonMemcpy(mColors, NEON_STEP_COMPONENTS, colors, NEON_STEP_COMPONENTS);

                    NeonBlendPix(pixelPtr->colors, mColors, covers);
                    pixelPtr = pixelPtr->colors + step;
                    colors += NEON_STEP_8;
                    covers += NEON_STEP_8;
                    len -= NEON_STEP_8;
                };
#endif
                for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                    CopyOrBlendPix(pixelPtr, *colors++, *covers++);
                    pixelPtr = pixelPtr->Next();
                }
            } else {
#ifdef NEON_ARM_OPT
                int16_t step = NEON_STEP_8 * PIX_STEP;
                const int16_t NEON_STEP_COMPONENTS = NEON_STEP_8 * NUM_COMPONENTS;
                uint8_t mColors[NEON_STEP_COMPONENTS];
                while (len >= NEON_STEP_8) {
                    memset_s(mColors, size_t(NEON_STEP_COMPONENTS), 0, size_t(NEON_STEP_COMPONENTS));
                    NeonMemcpy(mColors, NEON_STEP_COMPONENTS, colors, NEON_STEP_COMPONENTS);

                    NeonBlendPix(pixelPtr->colors, mColors, cover);
                    pixelPtr = pixelPtr->colors + step;
                    colors += NEON_STEP_8;
                    len -= NEON_STEP_8;
                };
#endif
                if (cover == COVER_MASK) {
                    for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                        CopyOrBlendPix(pixelPtr, *colors++);
                        pixelPtr = pixelPtr->Next();
                    }
                } else {
                    for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                        CopyOrBlendPix(pixelPtr, *colors++, cover);
                        pixelPtr = pixelPtr->Next();
                    }
                }
            }
        }

        /**
         * @brief Mix source pixels and coverage into rbuf_.
         * @param from Source pixel buffer.
         * @param xdst,ydst Destination buffer start position.
         * @param xsrc,ysrc Source buffer start position.
         * @param len Length to blend.
         * @param cover Coverage.
         * @since 1.0
         * @version 1.0
         */
        template <class SrcPixelFormatRenderer>
        void BlendFrom(const SrcPixelFormatRenderer& from,
                       int xdst, int ydst,
                       int xsrc, int ysrc,
                       unsigned len,
                       uint8_t cover)
        {
            using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

            if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
                PixelType* pdst = PixValuePtr(xdst, ydst, len);

                int srcinc = 1;
                int dstinc = 1;

#ifdef NEON_ARM_OPT
                srcinc = dstinc = NEON_STEP_8;
                if (xdst > xsrc) {
                    psrc = psrc->Advance(len - NEON_STEP_8);
                    pdst = pdst->Advance(len - NEON_STEP_8);
                    srcinc = -NEON_STEP_8;
                    dstinc = -NEON_STEP_8;
                }

                while (len >= NEON_STEP_8) {
                    NeonBlendPix(pdst->colors, psrc->colors, cover);
                    psrc = psrc->Advance(srcinc);
                    pdst = pdst->Advance(dstinc);
                    len -= NEON_STEP_8;
                };
#endif

                if (xdst > xsrc) {
                    psrc = psrc->Advance(len - 1);
                    pdst = pdst->Advance(len - 1);
                    srcinc = -1;
                    dstinc = -1;
                }
                if (cover == COVER_MASK) {
                    for (int16_t i = 0; i < (int)len; ++i) {
                        CopyOrBlendPix(pdst, psrc->GetPixelColor());
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    }
                } else {
                    for (int16_t i = 0; i < (int)len; ++i) {
                        CopyOrBlendPix(pdst, psrc->GetPixelColor(), cover);
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    }
                }
            }
        }

    protected:
#ifdef ARM_NEON_OPT
        virtual inline void NeonBlendPix(PixelType* pixelColors, const ColorType& color)
        {
            blender_.NeonBlendPix(pixelColors->colors, color.redValue, color.greenValue,
                                  color.blueValue, color.alphaValue);
        }
        virtual inline void NeonBlendPix(PixelType* pixelColors,
                                                          const ColorType& color, uint8_t cover)
        {
            blender_.NeonBlendPix(pixelColors->colors, color.redValue, color.greenValue,
                                  color.blueValue, color.alphaValue, cover);
        }
        virtual inline void NeonBlendPix(uint8_t* dstColors,
                                                          uint8_t* srcColors, uint8_t srcCover)
        {
            blender_.NeonBlendPix(dstColors, srcColors, srcCover);
        }
        virtual inline void NeonBlendPix(uint8_t* dstColors,
                                                          uint8_t* srcColors, uint8_t* srcCovers)
        {
            blender_.NeonBlendPix(dstColors, srcColors, srcCovers);
        }
        virtual inline void NeonBlendPix(uint8_t* dstColors, const ColorType& srcColors,
                                                          uint8_t* srcCovers)
        {
            blender_.NeonBlendPix(dstColors, srcColors.redValue, srcColors.greenValue, srcColors.blueValue,
                                  srcColors.alphaValue, srcCovers);
        }
#endif

        /**
         * @brief Blend color and coverage to specified pixels.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline void BlendPix(PixelType* pixelPtr, const ColorType& color, unsigned cover)
        {
            blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue,
                              color.blueValue, color.alphaValue, cover);
        }

        /**
         * @brief Blend colors to specified pixels.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline void BlendPix(PixelType* pixelPtr, const ColorType& color)
        {
            blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue, color.blueValue, color.alphaValue);
        }
        /**
         * @brief Set or blend to specified pixels with color and coverage.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual inline void CopyOrBlendPix(PixelType* pixelPtr,
                                                            const ColorType& color, unsigned cover)
        {
            if (!color.IsTransparent()) {
                if (color.IsOpaque() && cover == COVER_MASK) {
                    pixelPtr->SetPixelColor(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
                } else {
                    blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue,
                                      color.blueValue, color.alphaValue, cover);
                }
            }
        }

        /**
         * @brief Sets or blends the color to the specified pixel.
         *
         * @since 1.0
         * @version 1.0.
         */
        virtual inline void CopyOrBlendPix(PixelType* pixelPtr, const ColorType& color)
        {
            if (!color.IsTransparent()) {
                if (color.IsOpaque()) {
                    pixelPtr->SetPixelColor(color);
                } else {
                    blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue,
                                      color.blueValue, color.alphaValue);
                }
            }
        }
    protected:
        RbufType* rbuf_;
        Blender blender_;
    };
} // namespace OHOS
#endif
