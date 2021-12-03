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
* @file agg_renderer_base.h
* @brief Defines 基础渲染器
* @since 1.0
* @version 1.0
*/

#ifndef RENDERER_BASE_INCLUDED
#define RENDERER_BASE_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "render/agg_rendering_buffer.h"

namespace OHOS
{
    template<class PixelFormat>

    class RendererBase
    {
    public:
        typedef PixelFormat pixfmt_type;
        typedef typename pixfmt_type::color_type color_type;
        typedef typename pixfmt_type::row_data row_data;

        //--------------------------------------------------------------------
        RendererBase() : pixfmtType(0), clipBox(1, 1, 0, 0) {}
        explicit RendererBase(pixfmt_type& ren) :
            pixfmtType(&ren),
            clipBox(0, 0, ren.width() - 1, ren.height() - 1)
        {}
        void attach(pixfmt_type& ren)
        {
            pixfmtType = &ren;
            clipBox = rect_i(0, 0, ren.width() - 1, ren.height() - 1);
        }
        unsigned GetWidth()  const { return pixfmtType->width();  }
        unsigned GetHeight() const { return pixfmtType->height(); }

        //--------------------------------------------------------------------
        bool clip_box(int x1, int y1, int x2, int y2)
        {
            rect_i cb(x1, y1, x2, y2);
            cb.normalize();
            if(cb.clip(rect_i(0, 0, GetWidth() - 1, GetHeight() - 1)))
            {
                clipBox = cb;
                return true;
            }
            clip_box_naked(1,1,0,0);
            return false;
        }

        //--------------------------------------------------------------------
        void reset_clipping(bool visibility)
        {
            if(visibility)
            {
                clip_box_naked(0,0,GetWidth() - 1,GetHeight() - 1);
            } else {
                clip_box_naked(1,1,0,0);
            }
        }

        //--------------------------------------------------------------------
        void clip_box_naked(int x1, int y1, int x2, int y2)
        {
            clipBox.x1 = x1;
            clipBox.y1 = y1;
            clipBox.x2 = x2;
            clipBox.y2 = y2;
        }

        //--------------------------------------------------------------------
        bool inbox(int x, int y) const
        {
            return x >= clipBox.x1 && y >= clipBox.y1 &&
                   x <= clipBox.x2 && y <= clipBox.y2;
        }

        //--------------------------------------------------------------------
        const rect_i& GetClipBox() const { return clipBox;    }
        int           GetXmin()     const { return clipBox.x1; }
        int           GetYmin()     const { return clipBox.y1; }
        int           GetXmax()     const { return clipBox.x2; }
        int           GetYmax()     const { return clipBox.y2; }
        void clear(const color_type& c)
        {
            unsigned y;
            if(GetWidth())
            {
                for(y = 0; y < GetHeight(); y++)
                {
                    pixfmtType->copy_hline(0, y, GetWidth(), c);
                }
            }
        }


        //--------------------------------------------------------------------
        void fill(const color_type& c)
        {
            unsigned y;
            if(GetWidth())
            {
                for(y = 0; y < GetHeight(); y++)
                {
                    pixfmtType->blend_hline(0, y, GetWidth(), c, cover_mask);
                }
            }
        }

        //--------------------------------------------------------------------
        color_type pixel(int x, int y) const
        {
            return inbox(x, y) ?
                   pixfmtType->pixel(x, y) :
                   color_type::no_color();
        }

        //--------------------------------------------------------------------
        void blend_hline(int x1, int y, int x2,
                         const color_type& c, cover_type cover)
        {
            if(x1 > x2)
            {
                int t = x2;
                x2 = x1;
                x1 = t;
            }
            if(y  > GetYmax()||y  < GetYmin()||x1 > GetXmax()||x2 < GetXmin())
            {
                return;
            }
            if(x1 < GetXmin()) x1 = GetXmin();
            if(x2 > GetXmax()) x2 = GetXmax();

            pixfmtType->blend_hline(x1, y, x2 - x1 + 1, c, cover);
        }

        //--------------------------------------------------------------------
        void blend_solid_hspan(int x, int y, int len,
                               const color_type& c,
                               const cover_type* covers)
        {
            if(y > GetYmax()||y < GetYmin())
            {
                return;
            }
            if(x < GetXmin())
            {
                len -= GetXmin() - x;
                if(len <= 0) return;
                covers += GetXmin() - x;
                x = GetXmin();
            }
            if(x + len > GetXmax())
            {
                len = GetXmax() - x + 1;
                if(len <= 0) return;
            }
            pixfmtType->blend_solid_hspan(x, y, len, c, covers);
        }

        //--------------------------------------------------------------------
        void blend_solid_vspan(int x, int y, int len,
                               const color_type& c,
                               const cover_type* covers)
        {
            if(x > GetXmax()||x < GetXmin())
            {
                return;
            }

            if(y < GetYmin())
            {
                len -= GetYmin() - y;
                if(len <= 0) return;
                covers += GetYmin() - y;
                y = GetYmin();
            }
            if(y + len > GetYmax())
            {
                len = GetYmax() - y + 1;
                if(len <= 0) return;
            }
            pixfmtType->blend_solid_vspan(x, y, len, c, covers);
        }

        //--------------------------------------------------------------------
        void copy_color_hspan(int x, int y, int len, const color_type* colors)
        {
            if(y > GetYmax()||y < GetYmin())
            {
                return;
            }
            if(x < GetXmin())
            {
                int d = GetXmin() - x;
                len -= d;
                if(len <= 0) return;
                colors += d;
                x = GetXmin();
            }
            if(x + len > GetXmax())
            {
                len = GetXmax() - x + 1;
                if(len <= 0) return;
            }
            pixfmtType->copy_color_hspan(x, y, len, colors);
        }

        //--------------------------------------------------------------------
        void blend_color_hspan(int x, int y, int len,
                               const color_type* colors,
                               const cover_type* covers,
                               cover_type cover = OHOS::cover_full)
        {
            if(y > GetYmax()||y < GetYmin())
            {
                return;
            }
            if(x < GetXmin())
            {
                int d = GetXmin() - x;
                len -= d;
                if(len <= 0) return;
                if(covers) covers += d;
                colors += d;
                x = GetXmin();
            }
            if(x + len > GetXmax())
            {
                len = GetXmax() - x + 1;
                if(len <= 0) return;
            }
            pixfmtType->blend_color_hspan(x, y, len, colors, covers, cover);
        }

        //--------------------------------------------------------------------
        rect_i clip_rect_area(rect_i& dst, rect_i& src, int wsrc, int hsrc) const
        {
            rect_i rc(0,0,0,0);
            rect_i cb = GetClipBox();
            ++cb.x2;
            ++cb.y2;

            if(src.x1 < 0)
            {
                dst.x1 -= src.x1;
                src.x1 = 0;
            }
            if(src.y1 < 0)
            {
                dst.y1 -= src.y1;
                src.y1 = 0;
            }

            if(src.x2 > wsrc) src.x2 = wsrc;
            if(src.y2 > hsrc) src.y2 = hsrc;

            if(dst.x1 < cb.x1)
            {
                src.x1 += cb.x1 - dst.x1;
                dst.x1 = cb.x1;
            }
            if(dst.y1 < cb.y1)
            {
                src.y1 += cb.y1 - dst.y1;
                dst.y1 = cb.y1;
            }

            if(dst.x2 > cb.x2) dst.x2 = cb.x2;
            if(dst.y2 > cb.y2) dst.y2 = cb.y2;

            rc.x2 = dst.x2 - dst.x1;
            rc.y2 = dst.y2 - dst.y1;

            if(rc.x2 > src.x2 - src.x1) rc.x2 = src.x2 - src.x1;
            if(rc.y2 > src.y2 - src.y1) rc.y2 = src.y2 - src.y1;
            return rc;
        }

        //--------------------------------------------------------------------
        template<class RenBuf>
        void copy_from(const RenBuf& src,
                       const rect_i* rect_src_ptr = 0,
                       int dx = 0,
                       int dy = 0)
        {
            rect_i rsrc(0, 0, src.width(), src.height());
            if(rect_src_ptr)
            {
                rsrc.x1 = rect_src_ptr->x1;
                rsrc.y1 = rect_src_ptr->y1;
                rsrc.x2 = rect_src_ptr->x2 + 1;
                rsrc.y2 = rect_src_ptr->y2 + 1;
            }

            rect_i rdst(rsrc.x1 + dx, rsrc.y1 + dy, rsrc.x2 + dx, rsrc.y2 + dy);

            rect_i rc = clip_rect_area(rdst, rsrc, src.width(), src.height());

            if(rc.x2 > 0)
            {
                int incy = 1;
                if(rdst.y1 > rsrc.y1)
                {
                    rsrc.y1 += rc.y2 - 1;
                    rdst.y1 += rc.y2 - 1;
                    incy = -1;
                }
                while(rc.y2 > 0)
                {
                    pixfmtType->copy_from(src,
                                     rdst.x1, rdst.y1,
                                     rsrc.x1, rsrc.y1,
                                     rc.x2);
                    rdst.y1 += incy;
                    rsrc.y1 += incy;
                    --rc.y2;
                }
            }
        }

        template<class SrcPixelFormatRenderer>
        void blend_from(const SrcPixelFormatRenderer& src,
                        const rect_i* rect_src_ptr = 0,
                        int dx = 0,
                        int dy = 0,
                        cover_type cover = OHOS::cover_full)
        {
            rect_i rsrc(0, 0, src.width(), src.height());
            if(rect_src_ptr)
            {
                rsrc.x1 = rect_src_ptr->x1;
                rsrc.y1 = rect_src_ptr->y1;
                rsrc.x2 = rect_src_ptr->x2 + 1;
                rsrc.y2 = rect_src_ptr->y2 + 1;
            }

            rect_i rdst(rsrc.x1 + dx, rsrc.y1 + dy, rsrc.x2 + dx, rsrc.y2 + dy);
            rect_i rc = clip_rect_area(rdst, rsrc, src.width(), src.height());

            if(rc.x2 > 0)
            {
                int incy = 1;
                if(rdst.y1 > rsrc.y1)
                {
                    rsrc.y1 += rc.y2 - 1;
                    rdst.y1 += rc.y2 - 1;
                    incy = -1;
                }
                while(rc.y2 > 0)
                {
                    typename SrcPixelFormatRenderer::row_data rw = src.row(rsrc.y1);
                    if(rw.ptr)
                    {
                        int x1src = rsrc.x1;
                        int x1dst = rdst.x1;
                        int len   = rc.x2;
                        if(rw.x1 > x1src)
                        {
                            x1dst += rw.x1 - x1src;
                            len   -= rw.x1 - x1src;
                            x1src  = rw.x1;
                        }
                        if(len > 0)
                        {
                            if(x1src + len-1 > rw.x2)
                            {
                                len -= x1src + len - rw.x2 - 1;
                            }
                            if(len > 0)
                            {
                                pixfmtType->blend_from(src,
                                                  x1dst, rdst.y1,
                                                  x1src, rsrc.y1,
                                                  len,
                                                  cover);
                            }
                        }
                    }
                    rdst.y1 += incy;
                    rsrc.y1 += incy;
                    --rc.y2;
                }
            }
        }

    private:
        pixfmt_type* pixfmtType;
        rect_i       clipBox;
    };

}

#endif
