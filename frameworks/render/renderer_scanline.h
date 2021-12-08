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

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "render/renderer_base.h"

namespace OHOS
{
    /**
     *渲染实线的反走样扫描线
     *
     */
    template <class Rasterizer, class Scanline,
              class BaseRenderer, class ColorT>
    void render_scanlines_aa_solid(Rasterizer& raster, Scanline& scanline,
                                   BaseRenderer& renBase, const ColorT& color)
    {
        if (raster.rewind_scanlines())
        {
            typename BaseRenderer::color_type ren_color = color;

            scanline.reset(raster.min_x(), raster.max_x());
            while (raster.sweep_scanline(scanline))
            {
                int y = scanline.y();
                unsigned num_spans = scanline.num_spans();
                typename Scanline::const_iterator span = scanline.begin();

                while (true)
                {
                    int x = span->x;
                    if (span->len > 0)
                    {
                        renBase.blend_solid_hspan(x, y, (unsigned)span->len,
                                                  ren_color,
                                                  span->covers);
                    }
                    else
                    {
                        renBase.blend_hline(x, y, (unsigned)(x - span->len - 1),
                                            ren_color,
                                            *(span->covers));
                    }
                    if (--num_spans == 0)
                    {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    /**
     *渲染防走样的实线扫描线
     */
    template <class BaseRenderer>
    class renderer_scanline_aa_solid
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;

        renderer_scanline_aa_solid() :
            renBase_(0)
        {
        }
        //构造函数传入BaseRenderer
        explicit renderer_scanline_aa_solid(base_ren_type& renBase) :
            renBase_(&renBase)
        {
        }
        //函数传入BaseRenderer
        void attach(base_ren_type& renBase)
        {
            renBase_ = &renBase;
        }

        /**
         * @brief color 传入颜色
         */
        void color(const color_type& c)
        {
            color_ = c;
        }

        /**
         * @brief color 获取当前颜色
         * @return
         */
        const color_type& color() const
        {
            return color_;
        }

        void prepare()
        {
        }

        /**
         *开始渲染
         */
        template <class Scanline>
        void render(const Scanline& scanline)
        {
            int y = scanline.y();
            unsigned num_spans = scanline.num_spans();
            typename Scanline::const_iterator span = scanline.begin();

            for (;;)
            {
                int x = span->x;
                if (span->len > 0)
                {
                    renBase_->blend_solid_hspan(x, y, (unsigned)span->len,
                                                color_,
                                                span->covers);
                }
                else
                {
                    renBase_->blend_hline(x, y, (unsigned)(x - span->len - 1),
                                          color_,
                                          *(span->covers));
                }
                if (--num_spans == 0)
                {
                    break;
                }
                ++span;
            }
        }

    private:
        base_ren_type* renBase_;
        color_type color_;
    };

    template <class Rasterizer, class Scanline, class BaseRenderer,
              class SpanAllocator, class SpanGenerator>
    void render_scanlines_aa(Rasterizer& raster, Scanline& scanline, BaseRenderer& renBase,
                             SpanAllocator& alloc, SpanGenerator& span_gen)
    {
        if (raster.rewind_scanlines())
        {
            scanline.reset(raster.min_x(), raster.max_x());
            span_gen.prepare();
            while (raster.sweep_scanline(scanline))
            {
                int y = scanline.y();

                unsigned num_spans = scanline.num_spans();
                typename Scanline::const_iterator span = scanline.begin();
                while (true)
                {
                    int x = span->x;
                    int len = span->len;
                    const typename Scanline::cover_type* covers = span->covers;

                    if (len < 0)
                    {
                        len = -len;
                    }
                    typename BaseRenderer::color_type* colors = alloc.allocate(len);
                    span_gen.generate(colors, x, y, len);
                    renBase.blend_color_hspan(x, y, len, colors,
                                              (span->len < 0) ? 0 : covers, *covers);

                    if (--num_spans == 0)
                    {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    template <class BaseRenderer, class SpanAllocator, class SpanGenerator>
    class renderer_scanline_aa
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef SpanAllocator alloc_type;
        typedef SpanGenerator span_gen_type;
        renderer_scanline_aa() :
            renBase_(0), allocat_(0), spanGenerat_(0)
        {
        }
        renderer_scanline_aa(base_ren_type& renBase,
                             alloc_type& alloc,
                             span_gen_type& span_gen) :
            renBase_(&renBase),
            allocat_(&alloc),
            spanGenerat_(&span_gen)
        {
        }
        void attach(base_ren_type& renBase,
                    alloc_type& alloc,
                    span_gen_type& span_gen)
        {
            renBase_ = &renBase;
            allocat_ = &alloc;
            spanGenerat_ = &span_gen;
        }
        void prepare()
        {
            spanGenerat_->prepare();
        }
        template <class Scanline>
        void render(const Scanline& scanline)
        {
            int y = scanline.y();

            unsigned num_spans = scanline.num_spans();
            typename Scanline::const_iterator span = scanline.begin();
            while (true)
            {
                int x = span->x;
                int len = span->len;
                const typename Scanline::cover_type* covers = span->covers;

                if (len < 0)
                {
                    len = -len;
                }
                typename BaseRenderer::color_type* colors = allocat_->allocate(len);
                spanGenerat_->generate(colors, x, y, len);
                renBase_->blend_color_hspan(x, y, len, colors,
                                            (span->len < 0) ? 0 : covers, *covers);

                if (--num_spans == 0)
                {
                    break;
                }
                ++span;
            }
        }

    private:
        base_ren_type* renBase_;
        alloc_type* allocat_;
        span_gen_type* spanGenerat_;
    };

    template <class Scanline, class BaseRenderer, class ColorT>
    void render_scanline_bin_solid(const Scanline& scanline,
                                   BaseRenderer& renBase,
                                   const ColorT& color)
    {
        unsigned num_spans = scanline.num_spans();
        typename Scanline::const_iterator span = scanline.begin();
        for (;;)
        {
            renBase.blend_hline(span->x,
                                scanline.y(),
                                span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                color,
                                cover_full);
            if (--num_spans == 0)
                break;
            ++span;
        }
    }

    template <class Rasterizer, class Scanline,
              class BaseRenderer, class ColorT>
    void render_scanlines_bin_solid(Rasterizer& raster, Scanline& scanline,
                                    BaseRenderer& renBase, const ColorT& color)
    {
        if (raster.rewind_scanlines())
        {
            typename BaseRenderer::color_type ren_color(color);

            scanline.reset(raster.min_x(), raster.max_x());
            while (raster.sweep_scanline(scanline))
            {
                unsigned num_spans = scanline.num_spans();
                typename Scanline::const_iterator span = scanline.begin();
                for (;;)
                {
                    renBase.blend_hline(span->x,
                                        scanline.y(),
                                        span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                        ren_color,
                                        cover_full);
                    if (--num_spans == 0)
                        break;
                    ++span;
                }
            }
        }
    }

    template <class BaseRenderer>
    class renderer_scanline_bin_solid
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;
        renderer_scanline_bin_solid() :
            renBase_(0)
        {
        }
        explicit renderer_scanline_bin_solid(base_ren_type& renBase) :
            renBase_(&renBase)
        {
        }
        void attach(base_ren_type& renBase)
        {
            renBase_ = &renBase;
        }
        void color(const color_type& c)
        {
            color_ = c;
        }
        const color_type& color() const
        {
            return color_;
        }
        void prepare()
        {
        }
        template <class Scanline>

        /**
         *开始渲染
         */
        void render(const Scanline& scanline)
        {
            unsigned num_spans = scanline.num_spans();
            typename Scanline::const_iterator span = scanline.begin();
            for (;;)
            {
                renBase_->blend_hline(span->x,
                                      scanline.y(),
                                      span->x - 1 + ((span->len < 0) ? -span->len : span->len),
                                      color_,
                                      cover_full);
                if (--num_spans == 0)
                    break;
                ++span;
            }
        }

    private:
        base_ren_type* renBase_;
        color_type color_;
    };

    /**
     *渲染扫描线不带防走样
     */
    template <class Rasterizer, class Scanline, class BaseRenderer,
              class SpanAllocator, class SpanGenerator>
    void render_scanlines_bin(Rasterizer& raster, Scanline& scanline, BaseRenderer& renBase,
                              SpanAllocator& alloc, SpanGenerator& span_gen)
    {
        if (raster.rewind_scanlines())
        {
            scanline.reset(raster.min_x(), raster.max_x());
            span_gen.prepare();
            while (raster.sweep_scanline(scanline))
            {
                int y = scanline.y();
                unsigned num_spans = scanline.num_spans();
                typename Scanline::const_iterator span = scanline.begin();
                for (;;)
                {
                    int x = span->x;
                    int len = span->len;
                    if (len < 0)
                        len = -len;
                    typename BaseRenderer::color_type* colors = alloc.allocate(len);
                    span_gen.generate(colors, x, y, len);
                    renBase.blend_color_hspan(x, y, len, colors, 0, cover_full);
                    if (--num_spans == 0)
                        break;
                    ++span;
                }
            }
        }
    }

    /**
     *渲染扫描线（不防走样）
     */
    template <class BaseRenderer, class SpanAllocator, class SpanGenerator>
    class renderer_scanline_bin
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef SpanAllocator alloc_type;
        typedef SpanGenerator span_gen_type;
        renderer_scanline_bin() :
            renBase_(0), allocat_(0), spanGenerat_(0)
        {
        }
        renderer_scanline_bin(base_ren_type& renBase,
                              alloc_type& alloc,
                              span_gen_type& span_gen) :
            renBase_(&renBase),
            allocat_(&alloc),
            spanGenerat_(&span_gen)
        {
        }

        /**
         *传入参数
         */
        void attach(base_ren_type& renBase,
                    alloc_type& alloc,
                    span_gen_type& span_gen)
        {
            renBase_ = &renBase;
            allocat_ = &alloc;
            spanGenerat_ = &span_gen;
        }

        /**
         * 准备
         */
        void prepare()
        {
            spanGenerat_->prepare();
        }

        /**
         *开始渲染
         */
        template <class Scanline>
        void render(const Scanline& scanline)
        {
            int y = scanline.y();
            unsigned num_spans = scanline.num_spans();
            typename Scanline::const_iterator span = scanline.begin();
            for (;;)
            {
                int x = span->x;
                int len = span->len;
                if (len < 0)
                    len = -len;
                typename BaseRenderer::color_type* colors = allocat_->allocate(len);
                spanGenerat_->generate(colors, x, y, len);
                renBase_->blend_color_hspan(x, y, len, colors, 0, cover_full);
                if (--num_spans == 0)
                    break;
                ++span;
            }
        }

    private:
        base_ren_type* renBase_;
        alloc_type* allocat_;
        span_gen_type* spanGenerat_;
    };

    template <class Rasterizer, class Scanline, class Renderer>
    void render_scanlines(Rasterizer& raster, Scanline& scanline, Renderer& renBase)
    {
        if (raster.rewind_scanlines())
        {
            scanline.reset(raster.min_x(), raster.max_x());
            renBase.prepare();
            while (raster.sweep_scanline(scanline))
            {
                renBase.render(scanline);
            }
        }
    }

    template <class Rasterizer, class Scanline, class Renderer,
              class VertexSource, class ColorStorage, class PathId>
    void render_all_paths(Rasterizer& raster,
                          Scanline& scanline,
                          Renderer& renBase,
                          VertexSource& vs,
                          const ColorStorage& as,
                          const PathId& path_id,
                          unsigned num_paths)
    {
        for (unsigned i = 0; i < num_paths; i++)
        {
            raster.reset();
            raster.add_path(vs, path_id[i]);
            renBase.color(as[i]);
            render_scanlines(raster, scanline, renBase);
        }
    }
} // namespace OHOS

#endif
