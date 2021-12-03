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
* @file agg_renderer_scanline.h
*
* @brief Defines 扫描线渲染器
*
* @since 1.0
* @version 1.0
*/


#ifndef OHOS_RENDERER_SCANLINE_INCLUDED
#define OHOS_RENDERER_SCANLINE_INCLUDED

#include <limits>
#include <cstdlib>
#include <cstring>
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "render/agg_renderer_base.h"

namespace OHOS
{

    /**
     *渲染实线的反走样扫描线
     *
     */
    template<class Rasterizer, class Scanline, 
             class BaseRenderer, class ColorT>
    void render_scanlines_aa_solid(Rasterizer& ras, Scanline& sl, 
                                   BaseRenderer& ren, const ColorT& color)
    {
        if(ras.rewind_scanlines())
        {
            typename BaseRenderer::color_type ren_color = color;

            sl.reset(ras.min_x(), ras.max_x());
            while(ras.sweep_scanline(sl))
            {
                int y = sl.y();
                unsigned num_spans = sl.num_spans();
                typename Scanline::const_iterator span = sl.begin();

                while (true)
                {
                    int x = span->x;
                    if(span->len > 0)
                    {
                        ren.blend_solid_hspan(x, y, (unsigned)span->len, 
                                              ren_color, 
                                              span->covers);
                    }
                    else
                    {
                        ren.blend_hline(x, y, (unsigned)(x - span->len - 1), 
                                        ren_color, 
                                        *(span->covers));
                    }
                    if(--num_spans == 0)
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
    template<class BaseRenderer> class renderer_scanline_aa_solid
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;

        renderer_scanline_aa_solid() : m_ren(0) {}
        //构造函数传入BaseRenderer
        explicit renderer_scanline_aa_solid(base_ren_type& ren) : m_ren(&ren) {}
        //函数传入BaseRenderer
        void attach(base_ren_type& ren)
        {
            m_ren = &ren;
        }

        /**
         * @brief color 传入颜色
         */
        void color(const color_type& c) { m_color = c; }

        /**
         * @brief color 获取当前颜色
         * @return
         */
        const color_type& color() const { return m_color; }

        void prepare() {}

        /**
         *开始渲染
         */
        template<class Scanline> void render(const Scanline& sl)
        {
            int y = sl.y();
            unsigned num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();

            for(;;)
            {
                int x = span->x;
                if(span->len > 0)
                {
                    m_ren->blend_solid_hspan(x, y, (unsigned)span->len,
                                          m_color,
                                          span->covers);
                }
                else
                {
                    m_ren->blend_hline(x, y, (unsigned)(x - span->len - 1),
                                    m_color,
                                    *(span->covers));
                }
                if(--num_spans == 0) break;
                ++span;
            }
        }
        
    private:
        base_ren_type* m_ren;
        color_type m_color;
    };

    template<class Rasterizer, class Scanline, class BaseRenderer, 
             class SpanAllocator, class SpanGenerator>
    void render_scanlines_aa(Rasterizer& ras, Scanline& sl, BaseRenderer& ren, 
                             SpanAllocator& alloc, SpanGenerator& span_gen)
    {
        if(ras.rewind_scanlines())
        {
            sl.reset(ras.min_x(), ras.max_x());
            span_gen.prepare();
            while(ras.sweep_scanline(sl))
            {
                int y = sl.y();

                unsigned num_spans = sl.num_spans();
                typename Scanline::const_iterator span = sl.begin();
                while (true)
                {
                    int x = span->x;
                    int len = span->len;
                    const typename Scanline::cover_type* covers = span->covers;

                    if(len < 0) len = -len;
                    typename BaseRenderer::color_type* colors = alloc.allocate(len);
                    span_gen.generate(colors, x, y, len);
                    ren.blend_color_hspan(x, y, len, colors,
                                          (span->len < 0) ? 0 : covers, *covers);

                    if(--num_spans == 0)
                    {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    template<class BaseRenderer, class SpanAllocator, class SpanGenerator> 
    class renderer_scanline_aa
    {
    public:
        typedef BaseRenderer  base_ren_type;
        typedef SpanAllocator alloc_type;
        typedef SpanGenerator span_gen_type;
        renderer_scanline_aa() : m_ren(0), m_alloc(0), m_span_gen(0) {}
        renderer_scanline_aa(base_ren_type& ren, 
                             alloc_type& alloc, 
                             span_gen_type& span_gen) :
            m_ren(&ren),
            m_alloc(&alloc),
            m_span_gen(&span_gen)
        {}
        void attach(base_ren_type& ren, 
                    alloc_type& alloc, 
                    span_gen_type& span_gen)
        {
            m_ren = &ren;
            m_alloc = &alloc;
            m_span_gen = &span_gen;
        }
        void prepare() { m_span_gen->prepare(); }
        template<class Scanline> void render(const Scanline& sl)
        {
            int y = sl.y();

            unsigned num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();
            while (true)
            {
                int x = span->x;
                int len = span->len;
                const typename Scanline::cover_type* covers = span->covers;

                if(len < 0) len = -len;
                typename BaseRenderer::color_type* colors = m_alloc->allocate(len);
                m_span_gen->generate(colors, x, y, len);
                m_ren->blend_color_hspan(x, y, len, colors,
                                      (span->len < 0) ? 0 : covers, *covers);

                if(--num_spans == 0)
                {
                    break;
                }
                ++span;
            }
        }

    private:
        base_ren_type* m_ren;
        alloc_type*    m_alloc;
        span_gen_type* m_span_gen;
    };

    template<class Scanline, class BaseRenderer, class ColorT> 
    void render_scanline_bin_solid(const Scanline& sl, 
                                   BaseRenderer& ren, 
                                   const ColorT& color)
    {
        unsigned num_spans = sl.num_spans();
        typename Scanline::const_iterator span = sl.begin();
        for(;;)
        {
            ren.blend_hline(span->x, 
                            sl.y(), 
                            span->x - 1 + ((span->len < 0) ? 
                                              -span->len : 
                                               span->len), 
                               color, 
                               cover_full);
            if(--num_spans == 0) break;
            ++span;
        }
    }

    template<class Rasterizer, class Scanline, 
             class BaseRenderer, class ColorT>
    void render_scanlines_bin_solid(Rasterizer& ras, Scanline& sl, 
                                    BaseRenderer& ren, const ColorT& color)
    {
        if(ras.rewind_scanlines())
        {
            typename BaseRenderer::color_type ren_color(color);

            sl.reset(ras.min_x(), ras.max_x());
            while(ras.sweep_scanline(sl))
            {
                unsigned num_spans = sl.num_spans();
                typename Scanline::const_iterator span = sl.begin();
                for(;;)
                {
                    ren.blend_hline(span->x, 
                                    sl.y(), 
                                    span->x - 1 + ((span->len < 0) ? 
                                                      -span->len : 
                                                       span->len), 
                                       ren_color, 
                                       cover_full);
                    if(--num_spans == 0) break;
                    ++span;
                }
            }
        }
    }


    template<class BaseRenderer> class renderer_scanline_bin_solid
    {
    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;
        renderer_scanline_bin_solid() : m_ren(0) {}
        explicit renderer_scanline_bin_solid(base_ren_type& ren) : m_ren(&ren) {}
        void attach(base_ren_type& ren)
        {
            m_ren = &ren;
        }
        void color(const color_type& c) { m_color = c; }
        const color_type& color() const { return m_color; }
        void prepare() {}
        template<class Scanline>
        void render(const Scanline& sl)
        {
            unsigned num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();
            for(;;)
            {
                m_ren->blend_hline(span->x,
                                sl.y(),
                                span->x - 1 + ((span->len < 0) ?
                                                  -span->len :
                                                   span->len),
                                   m_color,
                                   cover_full);
                if(--num_spans == 0) break;
                ++span;
            }
        }
        
    private:
        base_ren_type* m_ren;
        color_type m_color;
    };

    /**
     *渲染扫描线不带防走样
     */
    template<class Rasterizer, class Scanline, class BaseRenderer,
             class SpanAllocator, class SpanGenerator>
    void render_scanlines_bin(Rasterizer& ras, Scanline& sl, BaseRenderer& ren, 
                              SpanAllocator& alloc, SpanGenerator& span_gen)
    {
        if(ras.rewind_scanlines())
        {
            sl.reset(ras.min_x(), ras.max_x());
            span_gen.prepare();
            while(ras.sweep_scanline(sl))
            {
                int y = sl.y();
                unsigned num_spans = sl.num_spans();
                typename Scanline::const_iterator span = sl.begin();
                for(;;)
                {
                    int x = span->x;
                    int len = span->len;
                    if(len < 0) len = -len;
                    typename BaseRenderer::color_type* colors = alloc.allocate(len);
                    span_gen.generate(colors, x, y, len);
                    ren.blend_color_hspan(x, y, len, colors, 0, cover_full);
                    if(--num_spans == 0) break;
                    ++span;
                }
            }
        }
    }

    template<class BaseRenderer, class SpanAllocator, class SpanGenerator> 
    class renderer_scanline_bin
    {
    public:
        typedef BaseRenderer  base_ren_type;
        typedef SpanAllocator alloc_type;
        typedef SpanGenerator span_gen_type;
        renderer_scanline_bin() : m_ren(0), m_alloc(0), m_span_gen(0) {}
        renderer_scanline_bin(base_ren_type& ren, 
                              alloc_type& alloc, 
                              span_gen_type& span_gen) :
            m_ren(&ren),
            m_alloc(&alloc),
            m_span_gen(&span_gen)
        {}
        void attach(base_ren_type& ren, 
                    alloc_type& alloc, 
                    span_gen_type& span_gen)
        {
            m_ren = &ren;
            m_alloc = &alloc;
            m_span_gen = &span_gen;
        }
        void prepare() { m_span_gen->prepare(); }
        template<class Scanline> void render(const Scanline& sl)
        {
            int y = sl.y();
            unsigned num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();
            for(;;)
            {
                int x = span->x;
                int len = span->len;
                if(len < 0) len = -len;
                typename BaseRenderer::color_type* colors = m_alloc->allocate(len);
                m_span_gen->generate(colors, x, y, len);
                m_ren->blend_color_hspan(x, y, len, colors, 0, cover_full);
                if(--num_spans == 0) break;
                ++span;
            }
        }

    private:
        base_ren_type* m_ren;
        alloc_type*    m_alloc;
        span_gen_type* m_span_gen;
    };

    template<class Rasterizer, class Scanline, class Renderer>
    void render_scanlines(Rasterizer& ras, Scanline& sl, Renderer& ren)
    {
        if(ras.rewind_scanlines())
        {
            sl.reset(ras.min_x(), ras.max_x());
            ren.prepare();
            while(ras.sweep_scanline(sl))
            {
                ren.render(sl);
            }
        }
    }

    template<class Rasterizer, class Scanline, class Renderer, 
             class VertexSource, class ColorStorage, class PathId>
    void render_all_paths(Rasterizer& ras, 
                          Scanline& sl,
                          Renderer& r, 
                          VertexSource& vs, 
                          const ColorStorage& as, 
                          const PathId& path_id,
                          unsigned num_paths)
    {
        for(unsigned i = 0; i < num_paths; i++)
        {
            ras.reset();
            ras.add_path(vs, path_id[i]);
            r.color(as[i]);
            render_scanlines(ras, sl, r);
        }
    }
}

#endif
