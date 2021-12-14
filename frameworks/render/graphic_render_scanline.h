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
* @file renderer_scanline.h
* @brief Defines 扫描线渲染器
* @since 1.0
* @version 1.0
*/

#ifndef RENDERER_SCANLINE_INCLUDED
#define RENDERER_SCANLINE_INCLUDED

#include <cstdlib>
#include <cstring>
#include <limits>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "render/graphic_render_base.h"

namespace OHOS {

    /**
     * @brief 渲染实线的反走样扫描线
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过renBase调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
     * @param raster 光栅
     * @param scanline 扫描线
     * @param renBase 渲染器
     * @param color 颜色
     */
    template <class Rasterizer, class Scanline,
              class BaseRenderer, class ColorT>
    void RenderScanlinesAntiAliasSolid(Rasterizer& raster, Scanline& scanline,
                                       BaseRenderer& renBase, const ColorT& color)
    {
        if (raster.RewindScanlines()) {
            typename BaseRenderer::color_type ren_color = color;

            scanline.Reset(raster.MinX(), raster.MaxX());
            while (raster.SweepScanline(scanline)) {
                int y = scanline.GetYLevel();
                unsigned num_spans = scanline.NumSpans();
                typename Scanline::ConstIterator span = scanline.Begin();

                while (true) {
                    int x = span->x;
                    if (span->spanLength > 0) {
                        renBase.BlendSolidHspan(x, y, (unsigned)span->spanLength,
                                                ren_color,
                                                span->covers);
                    } else {
                        renBase.BlendHline(x, y, (unsigned)(x - span->spanLength - 1),
                                           ren_color,
                                           *(span->covers));
                    }
                    if (--num_spans == 0) {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    /**
     * @brief Render 开始渲染
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过renBase_调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
     * @param scanline 扫描线
     */
    template <class BaseRenderer>
    class RendererScanlineAntiAliasSolid {
    public:
        using BaseRenType = BaseRenderer;
        using color_type = typename BaseRenType::color_type;

        RendererScanlineAntiAliasSolid() :
            renBase_(0)
        {
        }
        //构造函数传入BaseRenderer
        explicit RendererScanlineAntiAliasSolid(BaseRenType& renBase) :
            renBase_(&renBase)
        {
        }

        /**
         * @brief color 设置颜色
         */
        void SetColor(const color_type& c)
        {
            color_ = c;
        }

        /**
         * @brief color 获取当前颜色
         * @return
         */
        const color_type& GetColor() const
        {
            return color_;
        }

        void Prepare()
        {}

        /**
         * @brief Render 开始渲染
         * 通过scanline.begin获取第一个span,++span获取下一个span
         * 通过renBase_调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
         * @param scanline 扫描线
         */
        template <class Scanline>
        void Render(const Scanline& scanline)
        {
            int y = scanline.GetYLevel();
            unsigned num_spans = scanline.NumSpans();
            typename Scanline::ConstIterator span = scanline.Begin();

            for (;;) {
                int x = span->x;
                if (span->spanLength > 0) {
                    renBase_->BlendSolidHspan(x, y, (unsigned)span->spanLength,
                                              color_,
                                              span->covers);
                } else {
                    renBase_->BlendHline(x, y, (unsigned)(x - span->spanLength - 1),
                                         color_,
                                         *(span->covers));
                }
                if (--num_spans == 0) {
                    break;
                }
                ++span;
            }
        }

    private:
        BaseRenType* renBase_;
        color_type color_;
    };

    /**
     * @brief 渲染抗锯齿的扫描线
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过allocat_->Resize(spanLen)分配一个和span一样长度的颜色数组（color_type）
     * 通过线段生成器spanGenerat_->Generate(colors, x, y, len);将颜色数组填满得到和扫描线span对应的有数值的颜色数组
     * 最后通过renBase_调用对应的函数实现将颜色数组绘制到对应span所在的画布位置
     * @param raster 光栅
     * @param scanline 扫描线
     * @param renBase 渲染器
     * @param alloc 分配器
     * @param spanGen 线段生成器
     */
    template <class Rasterizer, class Scanline, class BaseRenderer,
              class SpanAllocator, class SpanGenerator>
    void RenderScanlinesAntiAlias(Rasterizer& raster, Scanline& scanline, BaseRenderer& renBase,
                                  SpanAllocator& alloc, SpanGenerator& spanGen)
    {
        if (raster.RewindScanlines()) {
            scanline.Reset(raster.MinX(), raster.MaxX());
            spanGen.Prepare(); //线段生成器预备
            while (raster.SweepScanline(scanline)) {
                int y = scanline.GetYLevel();

                unsigned num_spans = scanline.NumSpans();
                typename Scanline::ConstIterator span = scanline.Begin();
                while (true) {
                    int x = span->x;
                    int len = span->spanLength;
                    const typename Scanline::cover_type* covers = span->covers;

                    if (len < 0) {
                        len = -len;
                    }
                    typename BaseRenderer::color_type* colors = alloc.Resize(len);
                    spanGen.Generate(colors, x, y, len);
                    renBase.BlendColorHspan(x, y, len, colors,
                                            (span->spanLength < 0) ? 0 : covers, *covers);

                    if (--num_spans == 0) {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    /**
     * @brief 渲染抗锯齿的扫描线
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过allocat_->Resize(spanLen)分配一个和span一样长度的颜色数组（color_type）
     * 通过线段生成器spanGenerat_->Generate(colors, x, y, len);将颜色数组填满得到和扫描线span对应的有数值的颜色数组
     * 最后通过renBase_调用对应的函数实现将颜色数组绘制到对应span所在的画布位置
     * @param BaseRenderer 渲染器
     * @param SpanAllocator 分配器
     * @param SpanGenerator 线段生成器
     */
    template <class BaseRenderer, class SpanAllocator, class SpanGenerator>
    class RendererScanlineAntiAlias {
    public:
        using BaseRenType = BaseRenderer;
        using AllocType = SpanAllocator;
        using SpanGenType = SpanGenerator;
        RendererScanlineAntiAlias() :
            renBase_(0), allocat_(0), spanGenerat_(0)
        {
        }
        RendererScanlineAntiAlias(BaseRenType& renBase,
                                  AllocType& alloc,
                                  SpanGenType& span_gen) :
            renBase_(&renBase),
            allocat_(&alloc), spanGenerat_(&span_gen)
        {
        }

        void Prepare()
        {
            spanGenerat_->Prepare(); //线段生成器预备
        }
        /**
         * @brief Render 开始渲染
         * 通过scanline.begin获取第一个span,++span获取下一个span
         * 通过allocat_->Resize(spanLen)分配一个和span一样长度的颜色数组（color_type）
         * 通过线段生成器spanGenerat_->Generate(colors, x, y, len);将颜色数组填满得到和扫描线span对应的有数值的颜色数组
         * 最后通过renBase_调用对应的函数实现将颜色数组绘制到对应span所在的画布位置
         * @param scanline 扫描线
         */
        template <class Scanline>
        void Render(const Scanline& scanline)
        {
            int y = scanline.GetYLevel();

            unsigned num_spans = scanline.NumSpans();
            typename Scanline::ConstIterator span = scanline.Begin();
            while (true) {
                int x = span->x;
                int len = span->spanLength;
                const typename Scanline::cover_type* covers = span->covers;

                if (len < 0) {
                    len = -len;
                }
                typename BaseRenderer::color_type* colors = allocat_->Resize(len);
                spanGenerat_->Generate(colors, x, y, len);
                renBase_->BlendColorHspan(x, y, len, colors,
                                          (span->spanLength < 0) ? 0 : covers, *covers);

                if (--num_spans == 0) {
                    break;
                }
                ++span;
            }
        }

    private:
        BaseRenType* renBase_;
        AllocType* allocat_;
        SpanGenType* spanGenerat_;
    };

    /**
     * @brief 开始渲染
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过renBase_调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
     * @param scanline 扫描线
     * @param renBase 渲染器
     * @param color 颜色
     */
    template <class Scanline, class BaseRenderer, class ColorT>
    void RenderScanlineBinSolid(const Scanline& scanline,
                                BaseRenderer& renBase,
                                const ColorT& color)
    {
        unsigned num_spans = scanline.num_spans();
        typename Scanline::ConstIterator span = scanline.begin();
        for (;;) {
            renBase.blend_hline(span->x,
                                scanline.y(),
                                span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                color,
                                COVER_FULL);
            if (--num_spans == 0)
                break;
            ++span;
        }
    }

    /**
     * @brief 渲染不带抗锯齿扫描线（单色）
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过renBase_调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
     * @param raster 光栅
     * @param scanline 扫描线
     * @param renBase 渲染
     * @param color 颜色
     */
    template <class Rasterizer, class Scanline,
              class BaseRenderer, class ColorT>
    void RenderScanlinesBinSolid(Rasterizer& raster, Scanline& scanline,
                                 BaseRenderer& renBase, const ColorT& color)
    {
        if (raster.rewind_scanlines()) {
            typename BaseRenderer::color_type ren_color(color);

            scanline.reset(raster.min_x(), raster.max_x());
            while (raster.sweep_scanline(scanline)) {
                unsigned num_spans = scanline.num_spans();
                typename Scanline::ConstIterator span = scanline.begin();
                for (;;) {
                    renBase.BlendHline(span->x,
                                       scanline.y(),
                                       span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                       ren_color,
                                       COVER_FULL);
                    if (--num_spans == 0)
                        break;
                    ++span;
                }
            }
        }
    }

    template <class BaseRenderer>
    class RendererScanlineBinSolid {
    public:
        using BaseRenType = BaseRenderer;
        using color_type = typename BaseRenType::color_type;
        RendererScanlineBinSolid() :
            renBase_(0)
        {
        }
        explicit RendererScanlineBinSolid(BaseRenType& renBase) :
            renBase_(&renBase)
        {
        }
        void SetColor(const color_type& c)
        {
            color_ = c;
        }
        const color_type& GetColor() const
        {
            return color_;
        }
        void Prepare()
        {}

        /**
         * @brief Render 开始渲染
         * 通过scanline.begin获取第一个span,++span获取下一个span
         * 通过renBase_调用对应的函数实现将颜色（color）绘制到对应span所在的画布位置
         * @param scanline 扫描线
         */
        template <class Scanline>
        void Render(const Scanline& scanline)
        {
            unsigned num_spans = scanline.num_spans();
            typename Scanline::ConstIterator span = scanline.begin();
            for (;;) {
                renBase_->BlendHline(span->x,
                                     scanline.y(),
                                     span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                     color_,
                                     COVER_FULL);
                if (--num_spans == 0)
                    break;
                ++span;
            }
        }

    private:
        BaseRenType* renBase_;
        color_type color_;
    };

    /**
     * @brief 渲染不防走样的扫描线
     * 通过scanline.begin获取第一个span,++span获取下一个span
     * 通过allocat_->Resize(spanLen)分配一个和span一样长度的颜色数组（color_type）
     * 通过线段生成器spanGenerat_->Generate(colors, x, y, len);将颜色数组填满得到和扫描线span对应的有数值的颜色数组
     * 最后通过renBase_调用对应的函数实现将颜色数组绘制到对应span所在的画布位置
     * @param raster 光栅
     * @param scanline 扫描线
     * @param renBase 渲染器
     * @param alloc 分配器
     * @param spanGen 线段生成器
     */
    template <class Rasterizer, class Scanline, class BaseRenderer,
              class SpanAllocator, class SpanGenerator>
    void RenderScanlinesBin(Rasterizer& raster, Scanline& scanline, BaseRenderer& renBase,
                            SpanAllocator& alloc, SpanGenerator& spanGen)
    {
        if (raster.rewind_scanlines()) {
            scanline.reset(raster.min_x(), raster.max_x());
            spanGen.Prepare(); //线段生成器预备
            while (raster.sweep_scanline(scanline)) {
                int y = scanline.y();
                unsigned num_spans = scanline.num_spans();
                typename Scanline::ConstIterator span = scanline.begin();
                for (;;) {
                    int x = span->x;
                    int len = span->len;
                    if (len < 0) {
                        len = -len;
                    }
                    typename BaseRenderer::color_type* colors = alloc.Resize(len);
                    spanGen.Generate(colors, x, y, len);
                    renBase.BlendColorHspan(x, y, len, colors, 0, COVER_FULL);
                    if (--num_spans == 0)
                        break;
                    ++span;
                }
            }
        }
    }

    /**
     * @brief 渲染不防走样的扫描线
     * @param BaseRenderer 渲染器
     * @param SpanAllocator 分配器
     * @param SpanGenerator 线段生成器
     */
    template <class BaseRenderer, class SpanAllocator, class SpanGenerator>
    class RendererScanlineBin {
    public:
        using BaseRenType = BaseRenderer;
        using AllocType = SpanAllocator;
        using SpanGenType = SpanGenerator;
        RendererScanlineBin() :
            renBase_(0), allocat_(0), spanGenerat_(0)
        {
        }
        RendererScanlineBin(BaseRenType& renBase,
                            AllocType& alloc,
                            SpanGenType& span_gen) :
            renBase_(&renBase),
            allocat_(&alloc), spanGenerat_(&span_gen)
        {
        }

        /**
         * 准备
         */
        void Prepare()
        {
            spanGenerat_->Prepare(); //线段生成器预备
        }

        /**
         * @brief Render 开始渲染
         * 通过scanline.begin获取第一个span,++span获取下一个span
         * 通过allocat_->Resize(spanLen)分配一个和span一样长度的颜色数组（color_type）
         * 通过线段生成器spanGenerat_->Generate(colors, x, y, len);将颜色数组填满得到和扫描线span对应的有数值的颜色数组
         * 最后通过renBase_调用对应的函数实现将颜色数组绘制到对应span所在的画布位置
         * @param scanline 扫描线
         */
        template <class Scanline>
        void Render(const Scanline& scanline)
        {
            int y = scanline.y();
            unsigned num_spans = scanline.num_spans();
            typename Scanline::ConstIterator span = scanline.begin();
            for (;;) {
                int x = span->x;
                int len = span->len;
                if (len < 0)
                    len = -len;
                typename BaseRenderer::color_type* colors = allocat_->Resize(len);
                spanGenerat_->Generate(colors, x, y, len);
                renBase_->BlendColorHspan(x, y, len, colors, 0, COVER_FULL);
                if (--num_spans == 0)
                    break;
                ++span;
            }
        }

    private:
        BaseRenType* renBase_;
        AllocType* allocat_;
        SpanGenType* spanGenerat_;
    };

    /**
     * @brief 渲染扫描线
     * @param raster 光栅
     * @param scanline 扫描线
     * @param renBase 渲染器
     */
    template <class Rasterizer, class Scanline, class Renderer>
    void RenderScanlines(Rasterizer& raster, Scanline& scanline, Renderer& renBase)
    {
        if (raster.RewindScanlines()) {
            scanline.Reset(raster.MinX(), raster.MaxX());
            renBase.Prepare(); //线段生成器预备
            while (raster.SweepScanline(scanline)) {
                renBase.Render(scanline);
            }
        }
    }

} // namespace OHOS

#endif
