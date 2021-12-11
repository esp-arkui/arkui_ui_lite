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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED

#include <cmath>
#include <cstring>

#include "agg_pixfmt_rgba_conv.h"
#include "agg_pixfmt_rgba_gamma.h"
#include "gfx_utils/heap_base.h"
#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"
namespace OHOS {

    /**
     * @file graphic_geometry_pixfmt_rgba.h
     *
     * @brief Defines 颜色分量倍增器.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class ColorT, class Order>
    struct MultiplierRgba {
        using ColorType = ColorT;
        using ValueType = typename ColorType::ValueType;
        /**
         * @brief 颜色分量与Alpha相乘.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void Premultiply(ValueType* pColor)
        {
            ValueType a = pColor[Order::ALPHA];
            pColor[Order::BLUE] = ColorType::Multiply(pColor[Order::BLUE], a);
            pColor[Order::RED] = ColorType::Multiply(pColor[Order::RED], a);
            pColor[Order::GREEN] = ColorType::Multiply(pColor[Order::GREEN], a);
        }

        /**
         * @brief 颜色分量与Alpha解复用.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void Demultiply(ValueType* pColor)
        {
            ValueType a = pColor[Order::ALPHA];
            pColor[Order::BLUE] = ColorType::Demultiply(pColor[Order::BLUE], a);
            pColor[Order::RED] = ColorType::Demultiply(pColor[Order::RED], a);
            pColor[Order::GREEN] = ColorType::Demultiply(pColor[Order::GREEN], a);
        }
    };

    template <class ColorT, class Order>
    struct BlenderRgba : ConvRgbaPre<ColorT, Order> {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;

        /**
         * @brief 用颜色分量混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
        {
            BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
        }
        /**
         * @brief 用颜色分量混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
        {
            pColor[Order::RED] = ColorType::Lerp(pColor[Order::RED], cr, alpha);
            pColor[Order::GREEN] = ColorType::Lerp(pColor[Order::GREEN], cg, alpha);
            pColor[Order::BLUE] = ColorType::Lerp(pColor[Order::BLUE], cb, alpha);
            pColor[Order::ALPHA] = ColorType::Prelerp(pColor[Order::ALPHA], alpha, alpha);
        }
    };

    template <class ColorT, class Order>
    struct BlenderRgbaPre : ConvRgbaPre<ColorT, Order> {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;

        /**
         * @brief 用颜色分量混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void
            BlendPix(ValueType* pColor,
                     ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
        {
            BlendPix(pColor,
                     ColorType::MultCover(cr, cover),
                     ColorType::MultCover(cg, cover),
                     ColorType::MultCover(cb, cover),
                     ColorType::MultCover(alpha, cover));
        }
        /**
         * @brief 用颜色分量混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                     ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
        {
            pColor[Order::RED] = ColorType::Prelerp(pColor[Order::RED], cr, alpha);
            pColor[Order::GREEN] = ColorType::Prelerp(pColor[Order::GREEN], cg, alpha);
            pColor[Order::BLUE] = ColorType::Prelerp(pColor[Order::BLUE], cb, alpha);
            pColor[Order::ALPHA] = ColorType::Prelerp(pColor[Order::ALPHA], alpha, alpha);
        }
    };

    template <class ColorT, class Order>
    struct BlenderRgbaPlain : ConvRgbaPlain<ColorT, Order> {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;

        /**
         * @brief 用颜色分量及覆盖率混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
        {
            BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
        }
        /**
         * @brief 用颜色分量及覆盖率混合像素.
         *
         * @since 1.0
         * @version 1.0
         */
        static GRAPHIC_GEOMETRY_INLINE void BlendPix(
            ValueType* pColor, ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
        {
            if (alpha > ColorType::EmptyValue()) {
                CalcType a = pColor[Order::ALPHA];
                CalcType r = ColorType::Multiply(pColor[Order::RED], a);
                CalcType g = ColorType::Multiply(pColor[Order::GREEN], a);
                CalcType b = ColorType::Multiply(pColor[Order::BLUE], a);
                pColor[Order::ALPHA] = ColorType::Prelerp(a, alpha, alpha);
                pColor[Order::RED] = ColorType::Lerp(r, cr, alpha);
                pColor[Order::GREEN] = ColorType::Lerp(g, cg, alpha);
                pColor[Order::BLUE] = ColorType::Lerp(b, cb, alpha);
                MultiplierRgba<ColorT, Order>::Demultiply(pColor);
            }
        }
    };

    template <class Blender, class RenBuf>
    class PixfmtAlphaBlendRgba : public HeapBase {
    public:
        using PixfmtCategory = PixfmtRgbaTag;
        using RbufType = RenBuf;
        using RowData = typename RbufType::rowData;
        using BlenderType = Blender;
        using ColorType = typename BlenderType::ColorType;
        using OrderType = typename BlenderType::OrderType;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;

        enum
        {
            NUM_COMPONENTS = 4,
            PIX_STEP = 4,
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP
        };

        struct PixelType {
            ValueType colors[NUM_COMPONENTS];

            void Set(ValueType r, ValueType g, ValueType b, ValueType a)
            {
                colors[OrderType::RED] = r;
                colors[OrderType::GREEN] = g;
                colors[OrderType::BLUE] = b;
                colors[OrderType::ALPHA] = a;
            }

            void Set(const ColorType& color)
            {
                Set(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
            }

            void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
            {
                r = colors[OrderType::RED];
                g = colors[OrderType::GREEN];
                b = colors[OrderType::BLUE];
                a = colors[OrderType::ALPHA];
            }

            ColorType Get() const
            {
                return ColorType(colors[OrderType::RED], colors[OrderType::GREEN], colors[OrderType::BLUE], colors[OrderType::ALPHA]);
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

    private:
        /**
         * @brief 用颜色及覆盖率混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover)
        {
            blender_.BlendPix(p->colors, c.redValue, c.greenValue, c.blueValue, c.alphaValue, cover);
        }

        /**
         * @brief 用颜色混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c)
        {
            blender_.BlendPix(p->colors, c.redValue, c.greenValue, c.blueValue, c.alphaValue);
        }
        /**
         * @brief 用颜色及覆盖率设置或混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c, unsigned cover)
        {
            if (!c.IsTransparent()) {
                if (c.IsOpaque() && cover == COVER_MASK) {
                    p->Set(c.redValue, c.greenValue, c.blueValue, c.alphaValue);
                } else {
                    blender_.BlendPix(p->colors, c.redValue, c.greenValue, c.blueValue, c.alphaValue, cover);
                }
            }
        }
        /**
         * @brief 用颜色设置或混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c)
        {
            if (!c.IsTransparent()) {
                if (c.IsOpaque()) {
                    p->Set(c.redValue, c.greenValue, c.blueValue, c.alphaValue);
                } else {
                    blender_.BlendPix(p->colors, c.redValue, c.greenValue, c.blueValue, c.alphaValue);
                }
            }
        }

    public:
        PixfmtAlphaBlendRgba() :
            rbuf_(0)
        {}
        explicit PixfmtAlphaBlendRgba(RbufType& rb) :
            rbuf_(&rb)
        {}
        void Attach(RbufType& rb)
        {
            rbuf_ = &rb;
        }
        /**
         * @brief 把像素缓冲区附加到混合器.
         *
         * @since 1.0
         * @version 1.0
         */
        template <class PixFmt>
        bool Attach(PixFmt& pixf, int x1, int y1, int x2, int y2)
        {
            RectI r(x1, y1, x2, y2);
            if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) {
                int stride = pixf.Stride();
                rbuf_->attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
                return true;
            }
            return false;
        }

        /**
         * @brief 返回窗口的宽.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE unsigned Width() const
        {
            return rbuf_->GetWidth();
        }

        /**
          * @brief 返回窗口的高.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE unsigned Height() const
        {
            return rbuf_->GetHeight();
        }

        /**
          * @brief 返回窗口一行的字节数.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE int Stride() const
        {
            return rbuf_->GetStride();
        }

        /**
          * @brief 返回窗口一行的行地址.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
        {
            return rbuf_->row_ptr(y);
        }

        /**
          * @brief 返回窗口一行的行地址.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
        {
            return rbuf_->row_ptr(y);
        }

        /**
          * @brief 返回窗口一行的行数据.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE RowData Row(int y) const
        {
            return rbuf_->row(y);
        }

        /**
          * @brief 指针转为像素类型指针.
          *
          * @since 1.0
          * @version 1.0
          */
        GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
        {
            return rbuf_->row_ptr(y) + sizeof(ValueType) * (x * PIX_STEP);
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
        {
            return rbuf_->row_ptr(y) + sizeof(ValueType) * (x * PIX_STEP);
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
        {
            return (PixelType*)(rbuf_->row_ptr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
        }

        /**
         * @brief 获取指定左边的像素地址.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
        {
            int8u* p = rbuf_->row_ptr(y);
            return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
        {
            return (PixelType*)p;
        }

        /**
         * @brief 指针转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
        {
            return (const PixelType*)p;
        }

        /**
         * @brief 把纯色写入一个地址.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static void WritePlainColor(void* p, ColorType c)
        {
            BlenderType::SetPlainColor(PixValuePtr(p)->c, c);
        }

        /**
         * @brief 获取一个地址的纯色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static ColorType ReadPlainColor(const void* p)
        {
            return BlenderType::GetPlainColor(PixValuePtr(p)->c);
        }

        /**
         * @brief 给一个地址设置颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u* p, const ColorType& c)
        {
            ((PixelType*)p)->Set(c);
        }

        /**
         * @brief 获取一个像素的颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
        {
            if (const PixelType* p = PixValuePtr(x, y)) {
                return p->Get();
            }
            return ColorType::NoColor();
        }

        /**
         * @brief 拷贝颜色到像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
        {
            PixValuePtr(x, y, 1)->Set(c);
        }

        /**
        * @brief 用颜色c以cover（覆盖率 = 透明度）的透明度混合像素(x, y).
        *
        * @since 1.0
        * @version 1.0
        */
        GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
        {
            CopyOrBlendPix(PixValuePtr(x, y, 1), c, cover);
        }
        /**
         * @brief 从(x, y)开始打横顺序设置len长度的像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                               unsigned len,
                                               const ColorType& c)
        {
            PixelType v;
            v.Set(c);
            PixelType* p = PixValuePtr(x, y, len);
            do {
                *p = v;
                p = p->Next();
            } while (--len);
        }

        /**
         * @brief 从(x, y)开始打竖顺序设置len长度的像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyVline(int x, int y,
                                               unsigned len,
                                               const ColorType& c)
        {
            PixelType v;
            v.Set(c);
            do {
                *PixValuePtr(x, y++, 1) = v;
            } while (--len);
        }

        /**
         * @brief 从(x, y)开始打横顺序混合len长度的像素.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendHline(int x, int y,
                        unsigned len,
                        const ColorType& c,
                        int8u cover)
        {
            if (!c.IsTransparent()) {
                PixelType* p = PixValuePtr(x, y, len);
                if (c.IsOpaque() && cover == COVER_MASK) {
                    PixelType v;
                    v.Set(c);
                    do {
                        *p = v;
                        p = p->Next();
                    } while (--len);
                } else {
                    if (cover == COVER_MASK) {
                        do {
                            BlendPix(p, c);
                            p = p->Next();
                        } while (--len);
                    } else {
                        do {
                            BlendPix(p, c, cover);
                            p = p->Next();
                        } while (--len);
                    }
                }
            }
        }

        /**
         * @brief 从(x, y)开始打竖顺序混合len长度的像素.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendVline(int x, int y,
                        unsigned len,
                        const ColorType& c,
                        int8u cover)
        {
            if (!c.IsTransparent()) {
                if (c.IsOpaque() && cover == COVER_MASK) {
                    PixelType v;
                    v.Set(c);
                    do {
                        *PixValuePtr(x, y++, 1) = v;
                    } while (--len);
                } else {
                    if (cover == COVER_MASK) {
                        do {
                            BlendPix(PixValuePtr(x, y++, 1), c, c.alphaValue);
                        } while (--len);
                    } else {
                        do {
                            BlendPix(PixValuePtr(x, y++, 1), c, cover);
                        } while (--len);
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
        void BlendSolidHspan(int x, int y,
                             unsigned len,
                             const ColorType& c,
                             const int8u* covers)
        {
            if (!c.IsTransparent()) {
                PixelType* p = PixValuePtr(x, y, len);
                do {
                    if (c.IsOpaque() && *covers == cover_mask) {
                        p->Set(c);
                    } else {
                        BlendPix(p, c, *covers);
                    }
                    p = p->Next();
                    ++covers;
                } while (--len);
            }
        }
        /**
         * @brief 从(x, y)开始打竖顺序混合len长度的一系列颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendSolidVspan(int x, int y,
                             unsigned len,
                             const ColorType& c,
                             const int8u* covers)
        {
            if (!c.IsTransparent()) {
                do {
                    PixelType* p = PixValuePtr(x, y++, 1);
                    if (c.IsOpaque() && *covers == COVER_MASK) {
                        p->Set(c);
                    } else {
                        BlendPix(p, c, *covers);
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
        void CopyColorHspan(int x, int y,
                            unsigned len,
                            const ColorType* colors)
        {
            PixelType* p = PixValuePtr(x, y, len);
            do {
                p->Set(*colors++);
                p = p->Next();
            } while (--len);
        }

        /**
         * @brief 从(x, y)开始打竖顺序设置len长度的颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        void CopyColorVspan(int x, int y,
                            unsigned len,
                            const ColorType* colors)
        {
            do {
                PixValuePtr(x, y++, 1)->Set(*colors++);
            } while (--len);
        }

        /**
         * @brief 从(x, y)开始打横顺序混合len长度的一系列颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendColorHspan(int x, int y,
                             unsigned len,
                             const ColorType* colors,
                             const int8u* covers,
                             int8u cover)
        {
            PixelType* p = PixValuePtr(x, y, len);
            if (covers) {
                do {
                    CopyOrBlendPix(p, *colors++, *covers++);
                    p = p->Next();
                } while (--len);
            } else {
                if (cover == COVER_MASK) {
                    do {
                        CopyOrBlendPix(p, *colors++);
                        p = p->Next();
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(p, *colors++, cover);
                        p = p->Next();
                    } while (--len);
                }
            }
        }

        /**
         * @brief 从(x, y)开始纵向混合len长度的一系列颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendColorVspan(int x, int y,
                             unsigned len,
                             const ColorType* colors,
                             const int8u* covers,
                             int8u cover)
        {
            if (covers) {
                do {
                    CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++, *covers++);
                } while (--len);
            } else {
                if (cover == COVER_MASK) {
                    do {
                        CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++);
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(PixValuePtr(x, y++, 1), *colors++, cover);
                    } while (--len);
                }
            }
        }

        /**
         * @brief 每一像素执行一遍Function函数.
         *
         * @since 1.0
         * @version 1.0
         */
        template <class Function>
        void ForEachPixel(Function f)
        {
            for (unsigned y = 0; y < Height(); ++y) {
                RowData r = rbuf_->row(y);
                if (r.ptr) {
                    unsigned len = r.x2 - r.x1 + 1;
                    PixelType* p = PixValuePtr(r.x1, y, len);
                    do {
                        f(p->colors);
                        p = p->Next();
                    } while (--len);
                }
            }
        }
        /**
         * @brief 对像素上的颜色分量预乘.
         *
         * @since 1.0
         * @version 1.0
         */
        void Premultiply()
        {
            ForEachPixel(MultiplierRgba<ColorType, OrderType>::Premultiply);
        }
        /**
         * @brief 对像素上的颜色分量解复用.
         *
         * @since 1.0
         * @version 1.0
         */
        void Demultiply()
        {
            ForEachPixel(MultiplierRgba<ColorType, OrderType>::Demultiply);
        }

        template <class GammaLut>
        void ApplyGammaDir(const GammaLut& g)
        {
            ForEachPixel(ApplyGammaDirRgba<ColorType, OrderType, GammaLut>(g));
        }

        template <class GammaLut>
        void ApplyGammaInv(const GammaLut& g)
        {
            ForEachPixel(ApplyGammaInvRgba<ColorType, OrderType, GammaLut>(g));
        }
        /**
         * @brief 把源像素拷贝到rbuf_.
         * @param from 源像素缓存区,xdst,ydst 目的缓冲区起始位置,xsrc,ysrc 源缓冲区起始位置,len 要拷贝的长度
         * @since 1.0
         * @version 1.0
         */
        template <class RenBuf2>
        void CopyFrom(const RenBuf2& from,
                      int xdst, int ydst,
                      int xsrc, int ysrc,
                      unsigned len)
        {
            if (const int8u* p = from.row_ptr(ysrc)) {
                memmove_s(rbuf_->row_ptr(xdst, ydst, len) + xdst * PIX_WIDTH,
                          len * PIX_WIDTH, p + xsrc * PIX_WIDTH, len * PIX_WIDTH);
            }
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
                       int8u cover)
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

                if (cover == COVER_MASK) {
                    do {
                        CopyOrBlendPix(pdst, psrc->Get());
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(pdst, psrc->Get(), cover);
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    } while (--len);
                }
            }
        }

        template <class SrcPixelFormatRenderer>
        void BlendFromColor(const SrcPixelFormatRenderer& from,
                            const ColorType& color,
                            int xdst, int ydst,
                            int xsrc, int ysrc,
                            unsigned len,
                            int8u cover)
        {
            using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;
            using SrcColorType = typename SrcPixelFormatRenderer::ColorType;

            if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
                PixelType* pdst = PixValuePtr(xdst, ydst, len);

                do {
                    CopyOrBlendPix(pdst, color, SrcColorType::ScaleCover(cover, psrc->c[0]));
                    psrc = psrc->Next();
                    pdst = pdst->Next();
                } while (--len);
            }
        }

        template <class SrcPixelFormatRenderer>
        void BlendFromLut(const SrcPixelFormatRenderer& from,
                          const ColorType* colorLut,
                          int xdst, int ydst,
                          int xsrc, int ysrc,
                          unsigned len,
                          int8u cover)
        {
            using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

            if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
                PixelType* pdst = PixValuePtr(xdst, ydst, len);

                if (cover == COVER_MASK) {
                    do {
                        CopyOrBlendPix(pdst, colorLut[psrc->c[0]]);
                        psrc = psrc->Next();
                        pdst = pdst->Next();
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(pdst, colorLut[psrc->c[0]], cover);
                        psrc = psrc->Next();
                        pdst = pdst->Next();
                    } while (--len);
                }
            }
        }

    private:
        RbufType* rbuf_;
        Blender blender_;
    };

    template <class Blender, class RenBuf>
    class PixfmtCustomBlendRgba : public HeapBase {
    public:
        using PixfmtCategory = PixfmtRgbaTag;
        using RbufType = RenBuf;
        using RowData = typename RbufType::rowData;
        using BlenderType = Blender;
        using ColorType = typename BlenderType::ColorType;
        using OrderType = typename BlenderType::OrderType;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        enum
        {
            NUM_COMPONENTS = 4,
            PIX_STEP = 4,
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP,
        };
        struct PixelType {
            ValueType c[NUM_COMPONENTS];

            void Set(ValueType r, ValueType g, ValueType b, ValueType a)
            {
                c[OrderType::RED] = r;
                c[OrderType::G] = g;
                c[OrderType::B] = b;
                c[OrderType::A] = a;
            }

            void Set(const ColorType& color)
            {
                Set(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
            }

            void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
            {
                r = c[OrderType::RED];
                g = c[OrderType::G];
                b = c[OrderType::B];
                a = c[OrderType::A];
            }

            ColorType Get() const
            {
                return ColorType(
                    c[OrderType::RED],
                    c[OrderType::G],
                    c[OrderType::B],
                    c[OrderType::A]);
            }

            PixelType* Next()
            {
                return (PixelType*)(c + PIX_STEP);
            }

            const PixelType* Next() const
            {
                return (const PixelType*)(c + PIX_STEP);
            }

            PixelType* Advance(int n)
            {
                return (PixelType*)(c + n * PIX_STEP);
            }

            const PixelType* Advance(int n) const
            {
                return (const PixelType*)(c + n * PIX_STEP);
            }
        };

    private:
        GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover = COVER_FULL)
        {
            blender_.BlendPix(compOp_, p->c, c.redValue, c.greenValue, c.blueValue, c.alphaValue, cover);
        }

        GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c, unsigned cover = COVER_FULL)
        {
            if (!c.IsTransparent()) {
                if (c.IsOpaque() && cover == COVER_MASK) {
                    p->Set(c.redValue, c.greenValue, c.blueValue, c.alphaValue);
                } else {
                    BlendPix(p, c, cover);
                }
            }
        }

    public:
        PixfmtCustomBlendRgba() :
            rbuf_(0), compOp_(3)
        {}
        explicit PixfmtCustomBlendRgba(RbufType& rb, unsigned compOp = 3) :
            rbuf_(&rb), compOp_(compOp)
        {}
        void Attach(RbufType& rb)
        {
            rbuf_ = &rb;
        }
        /**
         * @brief 把像素附加到绘制区.
         *
         * @since 1.0
         * @version 1.0
         */
        template <class PixFmt>
        bool Attach(PixFmt& pixf, int x1, int y1, int x2, int y2)
        {
            RectI r(x1, y1, x2, y2);
            if (r.Clip(RectI(0, 0, pixf.Width() - 1, pixf.Height() - 1))) {
                int stride = pixf.Stride();
                rbuf_->Attach(pixf.PixPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
                return true;
            }
            return false;
        }

        void CompOp(unsigned op)
        {
            compOp_ = op;
        }

        unsigned CompOp() const
        {
            return compOp_;
        }
        /**
         * @brief 获取每屏幕（绘制缓冲区）宽度.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE unsigned Width() const
        {
            return rbuf_->GetWidth();
        }
        /**
         * @brief 获取每屏幕（绘制缓冲区）高度.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE unsigned Height() const
        {
            return rbuf_->GetHeight();
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
        GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
        {
            return rbuf_->row_ptr(y);
        }
        /**
         * @brief 通过纵坐标获取行地址.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
        {
            return rbuf_->row_ptr(y);
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
        /**
         * @brief 像素坐标转为像素位指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
        {
            return rbuf_->row_ptr(y) + sizeof(ValueType) * (x * PIX_STEP);
        }
        /**
         * @brief 像素坐标转为像素位指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
        {
            return rbuf_->row_ptr(y) + sizeof(ValueType) * (x * PIX_STEP);
        }
        /**
         * @brief 像素坐标转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
        {
            return (PixelType*)(rbuf_->row_ptr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
        }
        /**
         * @brief 像素坐标转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
        {
            int8u* p = rbuf_->row_ptr(y);
            return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
        }
        /**
         * @brief 像素地址转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
        {
            return (PixelType*)p;
        }
        /**
         * @brief 像素地址转为像素类型指针.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
        {
            return (const PixelType*)p;
        }
        /**
         * @brief 设置像素地址的颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u* p, const ColorType& c)
        {
            ((PixelType*)p)->Set(c);
        }
        /**
         * @brief 获取像素的颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
        {
            if (const PixelType* p = PixValuePtr(x, y)) {
                return p->Get();
            }
            return ColorType::NoColor();
        }
        /**
         * @brief 把指定的颜色拷贝到像素.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
        {
            MakePix(PixValuePtr(x, y, 1), c);
        }
        /**
         * @brief 在(x, y)坐标的像素混合颜色颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
        {
            BlendPix(PixValuePtr(x, y, 1), c, cover);
        }
        /**
         * @brief 从(x, y)横向向开始拷贝len长度的线性颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                               unsigned len,
                                               const ColorType& c)
        {
            PixelType v;
            v.Set(c);
            PixelType* p = PixValuePtr(x, y, len);
            do {
                *p = v;
                p = p->Next();
            } while (--len);
        }
        /**
         * @brief 从(x, y)纵向开始拷贝len长度的线性颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyVline(int x, int y,
                                               unsigned len,
                                               const ColorType& c)
        {
            PixelType v;
            v.Set(c);
            do {
                *PixValuePtr(x, y++, 1) = v;
            } while (--len);
        }
        /**
         * @brief 从(x, y)横向开始混合len长度的线性颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendHline(int x, int y, unsigned len,
                        const ColorType& c, int8u cover)
        {
            PixelType* p = PixValuePtr(x, y, len);
            do {
                BlendPix(p, c, cover);
                p = p->Next();
            } while (--len);
        }
        /**
         * @brief 从(x, y)纵向开始混合len长度的线性颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendVline(int x, int y, unsigned len,
                        const ColorType& c, int8u cover)
        {
            do {
                BlendPix(PixValuePtr(x, y++, 1), c, cover);
            } while (--len);
        }
        /**
         * @brief 从(x, y)横向开始混合len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendSolidHspan(int x, int y, unsigned len,
                             const ColorType& c, const int8u* covers)
        {
            PixelType* p = PixValuePtr(x, y, len);

            do {
                BlendPix(p, c, *covers++);
                p = p->Next();
            } while (--len);
        }
        /**
         * @brief 从(x, y)纵向开始混合len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendSolidVspan(int x, int y, unsigned len,
                             const ColorType& c, const int8u* covers)
        {
            do {
                BlendPix(PixValuePtr(x, y++, 1), c, *covers++);
            } while (--len);
        }
        /**
         * @brief 从(x, y)横向开始拷贝len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void CopyColorHspan(int x, int y,
                            unsigned len,
                            const ColorType* colors)
        {
            PixelType* p = PixValuePtr(x, y, len);

            do {
                p->Set(*colors++);
                p = p->Next();
            } while (--len);
        }
        /**
         * @brief 从(x, y)纵向开始拷贝len长度的一系列颜色.
         *
         * @since 1.0
         * @version 1.0
         */
        void CopyColorVspan(int x, int y,
                            unsigned len,
                            const ColorType* colors)
        {
            do {
                PixValuePtr(x, y++, 1)->Set(*colors++);
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
                             const int8u* covers,
                             int8u cover)
        {
            PixelType* p = PixValuePtr(x, y, len);

            do {
                BlendPix(p, *colors++, covers ? *covers++ : cover);
                p = p->Next();
            } while (--len);
        }
        /**
         * @brief 从(x, y)开始纵向混合len长度的一系列颜色及覆盖率.
         *
         * @since 1.0
         * @version 1.0
         */
        void BlendColorVspan(int x, int y, unsigned len,
                             const ColorType* colors,
                             const int8u* covers,
                             int8u cover)
        {
            do {
                BlendPix(PixValuePtr(x, y++, 1), *colors++, covers ? *covers++ : cover);
            } while (--len);
        }

        /**
         * @brief 每一像素执行一遍Function函数.
         *
         * @since 1.0
         * @version 1.0
         */
        template <class Function>
        void ForEachPixel(Function f)
        {
            unsigned y;
            for (y = 0; y < Height(); ++y) {
                RowData r = rbuf_->row(y);
                if (r.ptr) {
                    unsigned len = r.x2 - r.x1 + 1;
                    PixelType* p = PixValuePtr(r.x1, y, len);
                    do {
                        f(p->c);
                        p = p->Next();
                    } while (--len);
                }
            }
        }

        void Premultiply()
        {
            ForEachPixel(MultiplierRgba<ColorType, OrderType>::Premultiply);
        }

        void Demultiply()
        {
            ForEachPixel(MultiplierRgba<ColorType, OrderType>::Demultiply);
        }

        template <class GammaLut>
        void ApplyGammaDir(const GammaLut& g)
        {
            ForEachPixel(ApplyGammaDirRgba<ColorType, OrderType, GammaLut>(g));
        }

        template <class GammaLut>
        void ApplyGammaInv(const GammaLut& g)
        {
            ForEachPixel(ApplyGammaInvRgba<ColorType, OrderType, GammaLut>(g));
        }

        template <class RenBuf2>
        void CopyFrom(const RenBuf2& from,
                      int xdst, int ydst,
                      int xsrc, int ysrc,
                      unsigned len)
        {
            if (const int8u* p = from.RowPtr(ysrc)) {
                memmove_s(rbuf_->row_ptr(xdst, ydst, len) + xdst * PIX_WIDTH, len * PIX_WIDTH,
                          p + xsrc * PIX_WIDTH, len * PIX_WIDTH);
            }
        }

        template <class SrcPixelFormatRenderer>
        void BlendFrom(const SrcPixelFormatRenderer& from,
                       int xdst, int ydst,
                       int xsrc, int ysrc,
                       unsigned len,
                       int8u cover)
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
                    BlendPix(pdst, psrc->Get(), cover);
                    psrc = psrc->Advance(srcinc);
                    pdst = pdst->Advance(dstinc);
                } while (--len);
            }
        }

        template <class SrcPixelFormatRenderer>
        void BlendFromColor(const SrcPixelFormatRenderer& from,
                            const ColorType& color,
                            int xdst, int ydst,
                            int xsrc, int ysrc,
                            unsigned len,
                            int8u cover)
        {
            using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;
            using SrcColorType = typename SrcPixelFormatRenderer::ColorType;

            if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
                PixelType* pdst = PixValuePtr(xdst, ydst, len);

                do {
                    BlendPix(pdst, color, SrcColorType::ScaleCover(cover, psrc->c[0]));
                    psrc = psrc->Next();
                    pdst = pdst->Next();
                } while (--len);
            }
        }

        template <class SrcPixelFormatRenderer>
        void BlendFromLut(const SrcPixelFormatRenderer& from,
                          const ColorType* colorLut,
                          int xdst, int ydst,
                          int xsrc, int ysrc,
                          unsigned len,
                          int8u cover)
        {
            using SrcPixelType = typename SrcPixelFormatRenderer::PixelType;

            if (const SrcPixelType* psrc = from.PixValuePtr(xsrc, ysrc)) {
                PixelType* pdst = PixValuePtr(xdst, ydst, len);

                do {
                    BlendPix(pdst, colorLut[psrc->c[0]], cover);
                    psrc = psrc->Next();
                    pdst = pdst->Next();
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
