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
    using PixelType = PixelType<uint8_t, OrderType, ColorType>; \

// 把像素附加到绘制区.
#define ATTACH_FUNCTION_DEF \
    virtual void Attach(RbufType& rb) \
    { \
        rbuf_ = &rb; \
    } \
\
    template <class PixFmt> \
    bool Attach(PixFmt& pixf, int x1, int y1, int x2, int y2) \
    { \
        RectI r(x1, y1, x2, y2); \
        if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) { \
            int stride = pixf.Stride(); \
            rbuf_->Attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1), \
                          (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride); \
            return true; \
        } \
        return false; \
    }

    // 获取每屏幕（绘制缓冲区）大小
#define GET_SIZE_FRUNCTION_DEF \
    virtual GRAPHIC_GEOMETRY_INLINE unsigned Width() const \
    { \
        return rbuf_->GetWidth(); \
    } \
\
    virtual GRAPHIC_GEOMETRY_INLINE unsigned Height() const \
    { \
        return rbuf_->GetHeight(); \
    }

// 指针转为像素类型指针
#define PIX_PTR_FUNCTION_DEF \
    virtual GRAPHIC_GEOMETRY_INLINE uint8_t* PixPtr(int x, int y) \
    { \
        return rbuf_->RowPtr(y) + sizeof(uint8_t) * (x * PIX_STEP); \
    } \
\
    virtual GRAPHIC_GEOMETRY_INLINE const uint8_t* PixPtr(int x, int y) const \
    { \
        return rbuf_->RowPtr(y) + sizeof(uint8_t) * (x * PIX_STEP); \
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
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t* covers)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg, (uint8_t)cb,
                                                       (uint8_t)alpha, (uint8_t*)covers);
        }
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg, (uint8_t)cb,
                                                       (uint8_t)alpha, (uint8_t)cover);
        }
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                       (uint8_t)cb, (uint8_t)alpha);
        }
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t srcCover)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t)srcCover);
        }

        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t* srcCovers)
        {
            BaseGfxEngine::GetInstance()->BlendLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t*)srcCovers);
        }
#endif
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
        }

        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
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
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t* covers)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                          (uint8_t)cb, (uint8_t)alpha, (uint8_t*)covers);
        }
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr, (uint8_t)cg,
                                                          (uint8_t)cb, (uint8_t)alpha, (uint8_t)cover);
        }
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(
            uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)pColor, (uint8_t)cr,
                                                          (uint8_t)cg, (uint8_t)cb, (uint8_t)alpha);
        }

        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t srcCover)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors, (uint8_t)srcCover);
        }

        static GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t* srcCovers)
        {
            BaseGfxEngine::GetInstance()->BlendPreLerpPix((uint8_t*)dstColors, (uint8_t*)srcColors,
                                                          (uint8_t*)srcCovers);
        }
#endif
        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
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
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(uint8_t* pColor,
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
         * @brief 设置颜色.
         * @param r，g，b，a 颜色分量
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
         * @brief 设置颜色.
         * @param color 颜色
         * @since 1.0
         * @version 1.0
         */
        void SetPixelColor(const ColorType& color)
        {
            SetPixelColor(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
        }
        /**
         * @brief 获取颜色.
         * @param r，g，b，a 颜色分量
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
         * @brief 获取颜色.
         * @return 颜色
         * @since 1.0
         * @version 1.0
         */
        ColorType GetPixelColor() const
        {
            return ColorType(colors[OrderType::RED], colors[OrderType::GREEN],
                             colors[OrderType::BLUE], colors[OrderType::ALPHA]);
        }
        /**
        * @brief 获取下一个像素的颜色分量.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Next()
        {
            return (PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief 获取下一个像素的颜色分量首地址.
        *
        * @since 1.0
        * @version 1.0
        */
        const PixelType* Next() const
        {
            return (const PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief 获取第n个像素的颜色分量首地址.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Advance(int n)
        {
            return (PixelType*)(colors + n * PIX_STEP);
        }
        /**
        * @brief 获取第n个像素的颜色分量首地址.
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
    class PixfmtAlphaBlendRgba : public HeapBase {
    public:
        BASE_PIXFMT_BLEND_USING_DEF
        enum {
            PIX_WIDTH = sizeof(uint8_t) * PIX_STEP
        };

    public:
        PixfmtAlphaBlendRgba() :
            rbuf_(0)
        {}
        explicit PixfmtAlphaBlendRgba(RbufType& rb) :
            rbuf_(&rb)
        {}

        ATTACH_FUNCTION_DEF

        GET_SIZE_FRUNCTION_DEF

        /**
          * @brief 返回窗口一行的字节数.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual GRAPHIC_GEOMETRY_INLINE int Stride() const
        {
            return rbuf_->GetStride();
        }

        /**
          * @brief 返回窗口一行的行地址.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual GRAPHIC_GEOMETRY_INLINE uint8_t* RowPtr(int y)
        {
            return rbuf_->RowPtr(y);
        }

        /**
          * @brief 返回窗口一行的行地址.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual GRAPHIC_GEOMETRY_INLINE const uint8_t* RowPtr(int y) const
        {
            return rbuf_->RowPtr(y);
        }

        /**
          * @brief 返回窗口一行的行数据.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual GRAPHIC_GEOMETRY_INLINE RowData Row(int y) const
        {
            return rbuf_->Row(y);
        }

        PIX_PTR_FUNCTION_DEF

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
        {
            return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(uint8_t) * (x * PIX_STEP));
        }

        /**
         * @brief 获取指定左边的像素地址.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
        {
            uint8_t* pixelPtr = rbuf_->RowPtr(y);
            return pixelPtr ? (PixelType*)(pixelPtr + sizeof(uint8_t) * (x * PIX_STEP)) : 0;
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* pixelPtr)
        {
            return (PixelType*)pixelPtr;
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* pixelPtr)
        {
            return (const PixelType*)pixelPtr;
        }

        /**
         * @brief 给一个地址设置颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static void SetPixelColor(uint8_t* pixelPtr, const ColorType& color)
        {
            ((PixelType*)pixelPtr)->SetPixelColor(color);
        }

        /**
         * @brief 获取一个像素的颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
        {
            if (const PixelType* pixelPtr = PixValuePtr(x, y)) {
                return pixelPtr->GetPixelColor();
            }
            return ColorType::NoColor();
        }

        /**
         * @brief 拷贝颜色到像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& color)
        {
            PixValuePtr(x, y, 1)->SetPixelColor(color);
        }

        /**
        * @brief 用颜色c以cover（覆盖率 = 透明度）的透明度混合像素(x, y).
        *
        * @since 1.0
        * @version 1.0
        */
        virtual GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& color, uint8_t cover)
        {
            CopyOrBlendPix(PixValuePtr(x, y, 1), color, cover);
        }
        /**
         * @brief 从(x, y)开始打横顺序设置len长度的像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
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
         * @brief 从(x, y)开始打横顺序混合len长度的像素.
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
                if (color.IsOpaque() && cover == COVER_MASK) {
                    for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                        PixelType pixelValue;
                        pixelValue.SetPixelColor(color);
                        *pPixel = pixelValue;
                        pPixel = pPixel->Next();
                    }
                } else {
                    if (cover == COVER_MASK) {
                        for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                            BlendPix(pPixel, color);
                            pPixel = pPixel->Next();
                        }
                    } else {
                        for (int16_t iPixel = 0; iPixel < len; ++iPixel) {
                            BlendPix(pPixel, color, cover);
                            pPixel = pPixel->Next();
                        }
                    }
                }
            }
        }

        /**
         * @brief 从(x, y)开始打横顺序混合len长度的一系列颜色.
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
         * @brief 从(x, y)开始打竖顺序混合len长度的一系列颜色.
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
         * @brief 从(x, y)开始打横顺序设置len长度的颜色.
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
                memset_s(mColors, NEON_STEP_COMPONENTS, 0, NEON_STEP_COMPONENTS);
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
         * @brief 从(x, y)开始打竖顺序设置len长度的颜色.
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
         * @brief 从(x, y)开始打横顺序混合len长度的一系列颜色.
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
                    memset_s(mColors, NEON_STEP_COMPONENTS, 0, NEON_STEP_COMPONENTS);
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
                    memset_s(mColors, NEON_STEP_COMPONENTS, 0, NEON_STEP_COMPONENTS);
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
         * @brief 把源像素及覆盖率混合到rbuf_.
         * @param from 源像素缓存区,xdst,ydst 目的缓冲区起始位置,xsrc,ysrc 源缓冲区起始位置,
         *        len 要混合的长度 cover 覆盖率.
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
        virtual GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(PixelType* pixelColors, const ColorType& color)
        {
            blender_.NeonBlendPix(pixelColors->colors, color.redValue, color.greenValue,
                                  color.blueValue, color.alphaValue);
        }
        virtual GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(PixelType* pixelColors,
                                                          const ColorType& color, uint8_t cover)
        {
            blender_.NeonBlendPix(pixelColors->colors, color.redValue, color.greenValue,
                                  color.blueValue, color.alphaValue, cover);
        }
        virtual GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors,
                                                          uint8_t* srcColors, uint8_t srcCover)
        {
            blender_.NeonBlendPix(dstColors, srcColors, srcCover);
        }
        virtual GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors,
                                                          uint8_t* srcColors, uint8_t* srcCovers)
        {
            blender_.NeonBlendPix(dstColors, srcColors, srcCovers);
        }
        virtual GRAPHIC_GEOMETRY_INLINE void NeonBlendPix(uint8_t* dstColors, const ColorType& srcColors,
                                                          uint8_t* srcCovers)
        {
            blender_.NeonBlendPix(dstColors, srcColors.redValue, srcColors.greenValue, srcColors.blueValue,
                                  srcColors.alphaValue, srcCovers);
        }
#endif

        /**
         * @brief 用颜色及覆盖率混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* pixelPtr, const ColorType& color, unsigned cover)
        {
            blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue,
                              color.blueValue, color.alphaValue, cover);
        }

        /**
         * @brief 用颜色混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* pixelPtr, const ColorType& color)
        {
            blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue, color.blueValue, color.alphaValue);
        }
        /**
         * @brief 用颜色及覆盖率设置或混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* pixelPtr,
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
         * @brief 用颜色设置或混合到指定像素.
         *
         * @since 1.0
         * @version 1.0.
         */
        virtual GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* pixelPtr, const ColorType& color)
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

    template <class Blender, class RenBuf>
    class PixfmtCustomBlendRgba : public HeapBase {
    public:
        BASE_PIXFMT_BLEND_USING_DEF
        enum {
            PIX_WIDTH = sizeof(uint8_t) * PIX_STEP,
            COMP_OP_VALUE = 3
        };

    private:
        GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover = COVER_FULL)
        {
            PixfmtCustomBlendRgba<Blender, RenBuf>::blender_.BlendPix(compOp_, p->colors, c.redValue, c.greenValue,
                                                                      c.blueValue, c.alphaValue, cover);
        }

    public:
        PixfmtCustomBlendRgba() :
            rbuf_(0), compOp_(COMP_OP_VALUE)
        {}
        explicit PixfmtCustomBlendRgba(RbufType& rb, unsigned compOp = 3) :
            rbuf_(&rb),
            compOp_(compOp)
        {}

        ATTACH_FUNCTION_DEF
        GET_SIZE_FRUNCTION_DEF

        void CompOp(unsigned op)
        {
            compOp_ = op;
        }

        unsigned CompOp() const
        {
            return compOp_;
        }

        /**
         * @brief 获取每一行的像素占用的内存.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE int Stride() const
        {
            return rbuf_->Stride();
        }
        /**
         * @brief 通过纵坐标获取行地址.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE uint8_t* RowPtr(int y)
        {
            return rbuf_->row_ptr(y);
        }
        /**
         * @brief 通过纵坐标获取行地址.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const uint8_t* RowPtr(int y) const
        {
            return rbuf_->RowPtr(y);
        }
        /**
         * @brief 通过纵坐标获取行数据.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE RowData Row(int y) const
        {
            return rbuf_->Row(y);
        }

        PIX_PTR_FUNCTION_DEF

        /**
         * @brief 像素坐标转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len = 1)
        {
            return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(uint8_t) * (x * PIX_STEP));
        }

        GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                               unsigned len,
                                               const ColorType& covers)
        {
            PixelType vPixelType;
            vPixelType.SetPixelColor(covers);
            PixelType* pPixelType = PixValuePtr(x, y, len);
            do {
                *pPixelType = vPixelType;
                pPixelType = pPixelType->Next();
            } while (--len);
        }

        GRAPHIC_GEOMETRY_INLINE void ReplaceHlineColor(int x, int y,
                                                       unsigned len,
                                                       const ColorType& covers)
        {
            PixelType vPixelType;
            vPixelType.SetPixelColor(covers);
            PixelType* pPixelType = PixValuePtr(x, y, len);
            do {
                if (pPixelType->colors[OrderType::ALPHA] > 0) {
                    *pPixelType = vPixelType;
                }
                pPixelType = pPixelType->Next();
            } while (--len);
        }

        /**
         * @brief 从(x, y)横向开始混合len长度的线性颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendHline(int x, int y, unsigned len,
                        const ColorType& color, uint8_t cover)
        {
            PixelType* p = PixValuePtr(x, y, len);
            do {
                BlendPix(p, color, cover);
                p = p->Next();
            } while (--len);
        }

        /**
         * @brief 从(x, y)横向开始混合len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendSolidHspan(int x, int y, unsigned len,
                             const ColorType& color, const uint8_t* covers)
        {
            PixelType* pPixel = PixValuePtr(x, y, len);

            do {
                BlendPix(pPixel, color, *covers++);
                pPixel = pPixel->Next();
            } while (--len);
        }

        /**
         * @brief 从(x, y)开始横向混合len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendColorHspan(int x, int y, unsigned len,
                             const ColorType* colors,
                             const uint8_t* covers,
                             uint8_t cover)
        {
            PixelType* p = PixValuePtr(x, y, len);

            do {
                BlendPix(p, *colors++, covers ? *covers++ : cover);
                p = p->Next();
            } while (--len);
        }

        /**
         * @brief 把源像素及覆盖率混合到rbuf_.
         * @param from 源像素缓存区,xdst,ydst 目的缓冲区起始位置,xsrc,ysrc 源缓冲区起始位置,
         *        len 要混合的长度 cover 覆盖率
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

                if (xdst > xsrc) {
                    psrc = psrc->Advance(len - 1);
                    pdst = pdst->Advance(len - 1);
                    srcinc = -1;
                    dstinc = -1;
                }

                do {
                    BlendPix(pdst, psrc->GetPixelColor(), cover);
                    psrc = psrc->Advance(srcinc);
                    pdst = pdst->Advance(dstinc);
                } while (--len);
            }
        }

    private:
        RbufType* rbuf_;
        Blender blender_;
        unsigned compOp_;
    };
} // namespace OHOS
#endif
