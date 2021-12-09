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
* @brief Defines ��ɫ����gamma������.
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
template <class ColorT, class Order>
struct BlenderRgba : ConvRgbaPre<ColorT, Order> {
    using ColorType = ColorT;
    using OrderType = Order;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType;
    using LongType = typename ColorType::LongType;

    /**
     * @brief ����ɫ�����������.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
    {
        BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
    }
    /**
     * @brief ����ɫ�����������.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        pColor[Order::R] = ColorType::Lerp(pColor[Order::R], cr, alpha);
        pColor[Order::G] = ColorType::Lerp(pColor[Order::G], cg, alpha);
        pColor[Order::B] = ColorType::Lerp(pColor[Order::B], cb, alpha);
        pColor[Order::A] = ColorType::Prelerp(pColor[Order::A], alpha, alpha);
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
     * @brief ����ɫ�����������.
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
     * @brief ����ɫ�����������.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        pColor[Order::R] = ColorType::Prelerp(pColor[Order::R], cr, alpha);
        pColor[Order::G] = ColorType::Prelerp(pColor[Order::G], cg, alpha);
        pColor[Order::B] = ColorType::Prelerp(pColor[Order::B], cb, alpha);
        pColor[Order::A] = ColorType::Prelerp(pColor[Order::A], alpha, alpha);
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
     * @brief ����ɫ�����������ʻ������.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha, CoverType cover)
    {
        BlendPix(pColor, cr, cg, cb, ColorType::MultCover(alpha, cover));
    }
    /**
     * @brief ����ɫ�����������ʻ������.
     *
     * @since 1.0
     * @version 1.0
     */
    static GRAPHIC_GEOMETRY_INLINE void BlendPix(ValueType* pColor,
                                                 ValueType cr, ValueType cg, ValueType cb, ValueType alpha)
    {
        if (alpha > ColorType::EmptyValue()) {
            CalcType a = pColor[Order::A];
            CalcType r = ColorType::Multiply(pColor[Order::R], a);
            CalcType g = ColorType::Multiply(pColor[Order::G], a);
            CalcType b = ColorType::Multiply(pColor[Order::B], a);
            pColor[Order::A] = ColorType::Prelerp(a, alpha, alpha);
            pColor[Order::R] = ColorType::Lerp(r, cr, alpha);
            pColor[Order::G] = ColorType::Lerp(g, cg, alpha);
            pColor[Order::B] = ColorType::Lerp(b, cb, alpha);
            MultiplierRgba<ColorT, Order>::Demultiply(pColor);
        }
    }
};

template <class Blender, class RenBuf>
class PixfmtAlphaBlendRgba : public HeapBase {
public:
    using PixfmtCategory = PixfmtRgbaTag;
    using RbufType = RenBuf;
    using RowData = typename RbufType::RowData;
    using BlenderType = Blender;
    using ColorType = typename BlenderType::ColorType;
    using OrderType = typename BlenderType::OrderType;
    using ValueType = typename ColorType::ValueType;
    using CalcType = typename ColorType::CalcType

        enum {
            NUM_COMPONENTS = 4,
            PIX_STEP = 4,
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP,
        };

    struct PixelType {
        ValueType colors[NUM_COMPONENTS];

        void Set(ValueType r, ValueType g, ValueType b, ValueType a)
        {
            colors[OrderType::R] = r;
            colors[OrderType::G] = g;
            colors[OrderType::B] = b;
            colors[OrderType::A] = a;
        }

        void Set(const ColorType& color)
        {
            Set(color.r, color.g, color.b, color.a);
        }

        void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
        {
            r = colors[OrderType::R];
            g = colors[OrderType::G];
            b = colors[OrderType::B];
            a = colors[OrderType::A];
        }

        ColorType Get() const
        {
            return ColorType(colors[OrderType::R], colors[OrderType::G], colors[OrderType::B], colors[OrderType::A]);
        }
        /**
        * @brief ��ȡ��һ�����ص���ɫ����.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Next()
        {
            return (PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief ��ȡ��һ�����ص���ɫ�����׵�ַ.
        *
        * @since 1.0
        * @version 1.0
        */
        const PixelType* Next() const
        {
            return (const PixelType*)(colors + PIX_STEP);
        }
        /**
        * @brief ��ȡ��n�����ص���ɫ�����׵�ַ.
        *
        * @since 1.0
        * @version 1.0
        */
        PixelType* Advance(int n)
        {
            return (PixelType*)(colors + n * PIX_STEP);
        }
        /**
        * @brief ��ȡ��n�����ص���ɫ�����׵�ַ.
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
     * @brief ����ɫ�������ʻ�ϵ�ָ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover)
    {
        blender_.BlendPix(p->c, c.r, c.g, c.b, c.a, cover);
    }

    /**
     * @brief ����ɫ��ϵ�ָ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c)
    {
        blender_.BlendPix(p->c, c.r, c.g, c.b, c.a);
    }
    /**
     * @brief ����ɫ�����������û��ϵ�ָ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c, unsigned cover)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque() && cover == COVER_MASK) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                blender_.BlendPix(p->c, c.r, c.g, c.b, c.a, cover);
            }
        }
    }
    /**
     * @brief ����ɫ���û��ϵ�ָ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* p, const ColorType& c)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque()) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                blender_.BlendPix(p->c, c.r, c.g, c.b, c.a);
            }
        }
    }

public:
    PixfmtAlphaBlendRgba()
        : rbuf_(0)
    {}
    explicit PixfmtAlphaBlendRgba(RbufType& rb)
        : rbuf_(&rb)
    {}
    void Attach(RbufType& rb)
    {
        rbuf_ = &rb;
    }
    /**
     * @brief �����ػ��������ӵ������.
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

    /**
     * @brief ���ش��ڵĿ�.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE unsigned Width() const
    {
        return rbuf_->Width();
    }

    /**
      * @brief ���ش��ڵĸ�.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE unsigned Height() const
    {
        return rbuf_->Height();
    }

    /**
      * @brief ���ش���һ�е��ֽ���.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int Stride() const
    {
        return rbuf_->Stride();
    }

    /**
      * @brief ���ش���һ�е��е�ַ.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
    {
        return rbuf_->RowPtr(y);
    }

    /**
      * @brief ���ش���һ�е��е�ַ.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
    {
        return rbuf_->RowPtr(y);
    }

    /**
      * @brief ���ش���һ�е�������.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE row_data Row(int y) const
    {
        return rbuf_->Row(y);
    }

    /**
      * @brief ָ��תΪ��������ָ��.
      *
      * @since 1.0
      * @version 1.0
      */
    GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    /**
     * @brief ָ��תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }

    /**
     * @brief ָ��תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
    {
        return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
    }

    /**
     * @brief ��ȡָ����ߵ����ص�ַ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
    {
        int8u* p = rbuf_->RowPtr(y);
        return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
    }

    /**
     * @brief ָ��תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
    {
        return (PixelType*)p;
    }

    /**
     * @brief ָ��תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
    {
        return (const PixelType*)p;
    }

    /**
     * @brief �Ѵ�ɫд��һ����ַ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static void WritePlainColor(void* p, ColorType c)
    {
        BlenderType::SetPlainColor(PixValuePtr(p)->c, c);
    }

    /**
     * @brief ��ȡһ����ַ�Ĵ�ɫ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static ColorType ReadPlainColor(const void* p)
    {
        return BlenderType::GetPlainColor(PixValuePtr(p)->c);
    }

    /**
     * @brief ��һ����ַ������ɫ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u* p, const ColorType& c)
    {
        ((PixelType*)p)->Set(c);
    }

    /**
     * @brief ��ȡһ�����ص���ɫ.
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
     * @brief ������ɫ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
    {
        PixValuePtr(x, y, 1)->Set(c);
    }

    /**
    * @brief ����ɫc��cover�������� = ͸���ȣ���͸���Ȼ������(x, y).
    *
    * @since 1.0
    * @version 1.0
    */
    GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
    {
        CopyOrBlendPix(PixValuePtr(x, y, 1), c, cover);
    }
    /**
     * @brief ��(x, y)��ʼ���˳������len���ȵ�����.
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
     * @brief ��(x, y)��ʼ����˳������len���ȵ�����.
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
     * @brief ��(x, y)��ʼ���˳����len���ȵ�����.
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
     * @brief ��(x, y)��ʼ����˳����len���ȵ�����.
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
                        BlendPix(PixValuePtr(x, y++, 1), c, c.a);
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
     * @brief ��(x, y)��ʼ���˳����len���ȵ�һϵ����ɫ.
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
     * @brief ��(x, y)��ʼ����˳����len���ȵ�һϵ����ɫ.
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
     * @brief ��(x, y)��ʼ���˳������len���ȵ���ɫ.
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
     * @brief ��(x, y)��ʼ����˳������len���ȵ���ɫ.
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
     * @brief ��(x, y)��ʼ���˳����len���ȵ�һϵ����ɫ.
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
     * @brief ��(x, y)��ʼ������len���ȵ�һϵ����ɫ.
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
     * @brief ÿһ����ִ��һ��Function����.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class Function>
    void ForEachPixel(Function f)
    {
        for (unsigned y = 0; y < height(); ++y) {
            RowData r = rbuf_->Row(y);
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
    /**
     * @brief �������ϵ���ɫ����Ԥ��.
     *
     * @since 1.0
     * @version 1.0
     */
    void Premultiply()
    {
        ForEachPixel(MultiplierRgba<ColorType, OrderType>::Premultiply);
    }
    /**
     * @brief �������ϵ���ɫ�����⸴��.
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
     * @brief ��Դ���ؿ�����rbuf_.
     * @param from Դ���ػ�����,xdst,ydst Ŀ�Ļ�������ʼλ��,xsrc,ysrc Դ��������ʼλ��,len Ҫ�����ĳ���
     * @since 1.0
     * @version 1.0
     */
    template <class RenBuf2>
    void CopyFrom(const RenBuf2& from,
                  int xdst, int ydst,
                  int xsrc, int ysrc,
                  unsigned len)
    {
        if (const int8u* p = from.RowPtr(ysrc)) {
            std::memmove_s(rbuf_->RowPtr(xdst, ydst, len) + xdst * PIX_WIDTH, len * PIX_WIDTH, p + xsrc * PIX_WIDTH, len * PIX_WIDTH);
        }
    }
    /**
     * @brief ��Դ���ؼ������ʻ�ϵ�rbuf_.
     * @param from Դ���ػ�����,xdst,ydst Ŀ�Ļ�������ʼλ��,xsrc,ysrc Դ��������ʼλ��,
     *        len Ҫ��ϵĳ��� cover ������
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
class PixfmtCustomBlendRgba public HeapBase
{
public:
    using PixfmtCategory = PixfmtRgbaTag;
    using RbufType = RenBuf;
    using RowData = typename RbufType::RowData;
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
            c[OrderType::R] = r;
            c[OrderType::G] = g;
            c[OrderType::B] = b;
            c[OrderType::A] = a;
        }

        void Set(const ColorType& color)
        {
            Set(color.r, color.g, color.b, color.a);
        }

        void Get(ValueType& r, ValueType& g, ValueType& b, ValueType& a) const
        {
            r = c[OrderType::R];
            g = c[OrderType::G];
            b = c[OrderType::B];
            a = c[OrderType::A];
        }

        ColorType Get() const
        {
            return ColorType(
                c[OrderType::R],
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
    GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType * p, const ColorType& c, unsigned cover = COVER_FULL)
    {
        blender_.BlendPix(compOp_, p->c, c.r, c.g, c.b, c.a, cover);
    }

    GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType * p, const ColorType& c, unsigned cover = COVER_FULL)
    {
        if (!c.IsTransparent()) {
            if (c.IsOpaque() && cover == COVER_MASK) {
                p->Set(c.r, c.g, c.b, c.a);
            } else {
                BlendPix(p, c, cover);
            }
        }
    }

public:
    PixfmtCustomBlendRgba()
        : rbuf_(0), compOp_(3)
    {}
    explicit PixfmtCustomBlendRgba(RbufType & rb, unsigned compOp = 3)
        : rbuf_(&rb), compOp_(compOp)
    {}
    void Attach(RbufType & rb)
    {
        rbuf_ = &rb;
    }
    /**
     * @brief �����ظ��ӵ�������.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class PixFmt>
    bool Attach(PixFmt & pixf, int x1, int y1, int x2, int y2)
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
     * @brief ��ȡÿ��Ļ�����ƻ����������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE unsigned Width() const
    {
        return rbuf_->Width();
    }
    /**
     * @brief ��ȡÿ��Ļ�����ƻ��������߶�.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE unsigned Height() const
    {
        return rbuf_->Height();
    }
    /**
     * @brief ��ȡÿһ�е�����ռ�õ��ڴ�.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE int Stride() const
    {
        return rbuf_->Stride();
    }
    /**
     * @brief ͨ���������ȡ�е�ַ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
    {
        return rbuf_->RowPtr(y);
    }
    /**
     * @brief ͨ���������ȡ�е�ַ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
    {
        return rbuf_->RowPtr(y);
    }
    /**
     * @brief ͨ���������ȡ������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE RowData Row(int y) const
    {
        return rbuf_->Row(y);
    }
    /**
     * @brief ��������תΪ����λָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y)
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }
    /**
     * @brief ��������תΪ����λָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const
    {
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP);
    }
    /**
     * @brief ��������תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
    {
        return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
    }
    /**
     * @brief ��������תΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
    {
        int8u* p = rbuf_->RowPtr(y);
        return p ? (PixelType*)(p + sizeof(ValueType) * (x * PIX_STEP)) : 0;
    }
    /**
     * @brief ���ص�ַתΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static PixelType* PixValuePtr(void* p)
    {
        return (PixelType*)p;
    }
    /**
     * @brief ���ص�ַתΪ��������ָ��.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static const PixelType* PixValuePtr(const void* p)
    {
        return (const PixelType*)p;
    }
    /**
     * @brief �������ص�ַ����ɫ.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE static void MakePix(int8u * p, const ColorType& c)
    {
        ((PixelType*)p)->Set(c);
    }
    /**
     * @brief ��ȡ���ص���ɫ.
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
     * @brief ��ָ������ɫ����������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
    {
        MakePix(PixValuePtr(x, y, 1), c);
    }
    /**
     * @brief ��(x, y)��������ػ����ɫ��ɫ��������.
     *
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& c, int8u cover)
    {
        BlendPix(PixValuePtr(x, y, 1), c, cover);
    }
    /**
     * @brief ��(x, y)������ʼ����len���ȵ�������ɫ.
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
     * @brief ��(x, y)����ʼ����len���ȵ�������ɫ.
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
     * @brief ��(x, y)����ʼ���len���ȵ�������ɫ��������.
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
     * @brief ��(x, y)����ʼ���len���ȵ�������ɫ��������.
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
     * @brief ��(x, y)����ʼ���len���ȵ�һϵ����ɫ��������.
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
     * @brief ��(x, y)����ʼ���len���ȵ�һϵ����ɫ��������.
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
     * @brief ��(x, y)����ʼ����len���ȵ�һϵ����ɫ��������.
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
     * @brief ��(x, y)����ʼ����len���ȵ�һϵ����ɫ.
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
     * @brief ��(x, y)��ʼ������len���ȵ�һϵ����ɫ��������.
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
     * @brief ��(x, y)��ʼ������len���ȵ�һϵ����ɫ��������.
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
     * @brief ÿһ����ִ��һ��Function����.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class Function>
    void ForEachPixel(Function f)
    {
        unsigned y;
        for (y = 0; y < Height(); ++y) {
            row_data r = rbuf_->row(y);
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
            std::memmove(rbuf_->RowPtr(xdst, ydst, len) + xdst * PIX_IDTH, p + xsrc * PIX_IDTH, len * PIX_IDTH);
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