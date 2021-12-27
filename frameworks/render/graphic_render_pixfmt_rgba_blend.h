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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_BLEND_INCLUDED

#include <cmath>
#include <cstring>

#include "gfx_utils/heap_base.h"
#include "graphic_render_pixfmt_rgba_gamma.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"
namespace OHOS {

#define BASE_PIXFMT_BLEND_USING_DEF \
    using RbufType = RenBuf; \
    using RowData = typename RbufType::rowData; \
    using BlenderType = Blender; \
    using ColorType = typename BlenderType::ColorType; \
    using OrderType = typename BlenderType::OrderType; \
    using ValueType = typename ColorType::ValueType; \
    using CalcType = typename ColorType::CalcType; \
    using PixelType = PixelType<ValueType, OrderType, ColorType>;

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

    //获取每屏幕（绘制缓冲区）大小
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
    virtual GRAPHIC_GEOMETRY_INLINE int8u* PixPtr(int x, int y) \
    { \
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP); \
    } \
\
    virtual GRAPHIC_GEOMETRY_INLINE const int8u* PixPtr(int x, int y) const \
    { \
        return rbuf_->RowPtr(y) + sizeof(ValueType) * (x * PIX_STEP); \
    }

    enum {
        NUM_COMPONENTS = 4,
        PIX_STEP = 4
    };

    template <class ColorT, class Order>
    struct RgbaBlender {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;

        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
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
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
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
    struct RgbaPrelerpBlender {
        using ColorType = ColorT;
        using OrderType = Order;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;
        using LongType = typename ColorType::LongType;

        /**
         * @brief 用颜色分量混合像素.
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量，cover 覆盖率
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
         * @param pColor 颜色，cr，cg，cb，alpha 颜色分量
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

    template <class ValueType, class OrderType, class ColorType>
    struct PixelType {
        ValueType colors[NUM_COMPONENTS];
        /**
         * @brief 设置颜色.
         * @param r，g，b，a 颜色分量
         * @since 1.0
         * @version 1.0
         */
        void SetPixelColor(ValueType redValue, ValueType greenValue, ValueType blueValue, ValueType alphaValue)
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
        void GetPixelColor(ValueType& red, ValueType& green, ValueType& blue, ValueType& alpha) const
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
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP
        };

    protected:
        /**
         * @brief 用颜色及覆盖率混合到指定像素.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* pixelPtr, const ColorType& color, unsigned cover)
        {
            blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue, color.blueValue, color.alphaValue, cover);
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
        virtual GRAPHIC_GEOMETRY_INLINE void CopyOrBlendPix(PixelType* pixelPtr, const ColorType& color, unsigned cover)
        {
            if (!color.IsTransparent()) {
                if (color.IsOpaque() && cover == COVER_MASK) {
                    pixelPtr->SetPixelColor(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
                } else {
                    blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue, color.blueValue, color.alphaValue, cover);
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
                    pixelPtr->SetPixelColor(color.redValue, color.greenValue, color.blueValue, color.alphaValue);
                } else {
                    blender_.BlendPix(pixelPtr->colors, color.redValue, color.greenValue, color.blueValue, color.alphaValue);
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
        virtual GRAPHIC_GEOMETRY_INLINE int8u* RowPtr(int y)
        {
            return rbuf_->RowPtr(y);
        }

        /**
          * @brief 返回窗口一行的行地址.
          *
          * @since 1.0
          * @version 1.0
          */
        virtual GRAPHIC_GEOMETRY_INLINE const int8u* RowPtr(int y) const
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
            return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
        }

        /**
         * @brief 获取指定左边的像素地址.
         *
         * @since 1.0
         * @version 1.0
         */
        virtual GRAPHIC_GEOMETRY_INLINE const PixelType* PixValuePtr(int x, int y) const
        {
            int8u* pixelPtr = rbuf_->RowPtr(y);
            return pixelPtr ? (PixelType*)(pixelPtr + sizeof(ValueType) * (x * PIX_STEP)) : 0;
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
        GRAPHIC_GEOMETRY_INLINE static void SetPixelColor(int8u* pixelPtr, const ColorType& color)
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
        virtual GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y, const ColorType& color, int8u cover)
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
            do {
                *pixelPtr = vPixelValue;
                pixelPtr = pixelPtr->Next();
            } while (--len);
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
                                int8u cover)
        {
            if (!color.IsTransparent()) {
                PixelType* p = PixValuePtr(x, y, len);
                if (color.IsOpaque() && cover == COVER_MASK) {
                    PixelType pixelValue;
                    pixelValue.SetPixelColor(color);
                    do {
                        *p = pixelValue;
                        p = p->Next();
                    } while (--len);
                } else {
                    if (cover == COVER_MASK) {
                        do {
                            BlendPix(p, color);
                            p = p->Next();
                        } while (--len);
                    } else {
                        do {
                            BlendPix(p, color, cover);
                            p = p->Next();
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
        virtual void BlendSolidHspan(int x, int y,
                                     unsigned len,
                                     const ColorType& color,
                                     const int8u* covers)
        {
            if (!color.IsTransparent()) {
                PixelType* pixelPtr = PixValuePtr(x, y, len);
                do {
                    if (color.IsOpaque() && *covers == COVER_MASK) {
                        pixelPtr->SetPixelColor(color);
                    } else {
                        BlendPix(pixelPtr, color, *covers);
                    }
                    pixelPtr = pixelPtr->Next();
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
        virtual void BlendSolidVspan(int x, int y,
                                     unsigned len,
                                     const ColorType& color,
                                     const int8u* covers)
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
            do {
                pixelPtr->SetPixelColor(*colors++);
                pixelPtr = pixelPtr->Next();
            } while (--len);
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
                                     const int8u* covers,
                                     int8u cover)
        {
            PixelType* pixelPtr = PixValuePtr(x, y, len);
            if (covers) {
                do {
                    CopyOrBlendPix(pixelPtr, *colors++, *covers++);
                    pixelPtr = pixelPtr->Next();
                } while (--len);
            } else {
                if (cover == COVER_MASK) {
                    do {
                        CopyOrBlendPix(pixelPtr, *colors++);
                        pixelPtr = pixelPtr->Next();
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(pixelPtr, *colors++, cover);
                        pixelPtr = pixelPtr->Next();
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
        void ForEachPixel(Function emitFunc)
        {
            for (unsigned y = 0; y < Height(); ++y) {
                RowData r = rbuf_->Row(y);
                if (r.ptr) {
                    unsigned len = r.x2 - r.x1 + 1;
                    PixelType* pixelPtr = PixValuePtr(r.x1, y, len);
                    do {
                        emitFunc(pixelPtr->colors);
                        pixelPtr = pixelPtr->Next();
                    } while (--len);
                }
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
                        CopyOrBlendPix(pdst, psrc->GetPixelColor());
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    } while (--len);
                } else {
                    do {
                        CopyOrBlendPix(pdst, psrc->GetPixelColor(), cover);
                        psrc = psrc->Advance(srcinc);
                        pdst = pdst->Advance(dstinc);
                    } while (--len);
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
            PIX_WIDTH = sizeof(ValueType) * PIX_STEP,
            COMP_OP_VALUE = 3
        };

    private:
        GRAPHIC_GEOMETRY_INLINE void BlendPix(PixelType* p, const ColorType& c, unsigned cover = COVER_FULL)
        {
            PixfmtCustomBlendRgba<Blender, RenBuf>::blender_.BlendPix(compOp_, p->colors, c.redValue, c.greenValue, c.blueValue, c.alphaValue, cover);
        }

    public:
        PixfmtCustomBlendRgba() :
            rbuf_(0), compOp_(COMP_OP_VALUE)
        {}
        explicit PixfmtCustomBlendRgba(RbufType& rb, unsigned compOp = 3) :
            rbuf_(&rb), compOp_(compOp)
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
        GRAPHIC_GEOMETRY_INLINE PixelType* PixValuePtr(int x, int y, unsigned len)
        {
            return (PixelType*)(rbuf_->RowPtr(x, y, len) + sizeof(ValueType) * (x * PIX_STEP));
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

        GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                   unsigned len,
                                   const ColorType& covers)
        {
            PixelType v;
            v.SetPixelColor(covers);
            PixelType* p = PixValuePtr(x, y, len);
            do
            {
                *p = v;
                p = p->Next();
            }
            while (--len);
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
