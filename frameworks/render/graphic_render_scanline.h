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

/**
* @file renderer_scanline.h
* @brief Defines 扫描线渲染器
* @since 1.0
* @version 1.0
*/

#ifndef RENDERER_SCANLINE_INCLUDED
#define RENDERER_SCANLINE_INCLUDED

#include <gfx_utils/graphics/scanline/graphic_geometry_scanline.h>

#include <cstdlib>
#include <cstring>
#include <limits>
#include <gfx_utils/graphics/scanline/graphic_geometry_scanline.h>
#include "gfx_utils/color.h"
#include "gfx_utils/graphics/spancolorfill/graphic_spancolor_fill_base.h"
#include "render/graphic_render_base.h"

namespace OHOS {
    /**
     * @brief Anti aliasing scanline for rendering solid lines.
     * Via scanline Begin gets the first span, and + + span gets the next span.
     * The corresponding function is called by renbase to draw the color to the
     * canvas where the corresponding span is located.
     * @param raster grating
     * @param scanline Scan line
     * @param renBase Renderer
     * @param color colour
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
     * @brief Rendering anti aliased scanlines.
     * Via scanline Begin gets the first span, and + + span gets the next span.
     * Via allocat_ - > Resize (spanlen) allocates a color_type with the same length as span.
     * Through segment generator spangenerate_ - > Generate(colors, x, y, len); Fill the color
     * array to get the color array with values corresponding to the scan line span.
     * Finally, through renbase_ Call the corresponding function to draw the color array
     * to the canvas position of the corresponding span.
     * @param raster grating
     * @param scanline Scan line
     * @param renBase Renderer
     * @param alloc distributor
     * @param spanGen Segment generator
     */
    template <class Rasterizer, class Scanline, class BaseRenderer,
              class SpanAllocator, class SpanGenerator>
    void RenderScanlinesAntiAlias(Rasterizer& raster, Scanline& scanline, BaseRenderer& renBase,
                                  SpanAllocator& alloc, SpanGenerator& spanGen)
    {
        if (raster.RewindScanlines()) {
            scanline.Reset(raster.MinX(), raster.MaxX());
            spanGen.Prepare(); // Segment generator preparation
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
    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator1,
             class SpanGenerator2>
    void BlendScanLine(GlobalCompositeOperation op,
                                 Rasterizer& raster1, Rasterizer& raster2,
                                 Scanline& sl1, Scanline& sl2, BaseRenderer& renBase,
                                 SpanAllocator& alloc1, SpanGenerator1& spanGen1,
                                 SpanAllocator& alloc2, SpanGenerator2& spanGen2)
    {
        switch(op)
        {
        case SOURCE_ATOP       : BlendSourceAtop   (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1,alloc2,spanGen2); break;
        case SOURCE_IN         : BlendSourceIn     (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1); break;
        case SOURCE_OUT        : BlendSourceOut   (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1); break;
        case DESTINATION_OVER  : BlendSourceOver   (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1,alloc2,spanGen2); break;
        case DESTINATION_ATOP  : BlendSourceAtop   (raster2, raster1, sl2, sl1, renBase,alloc2,spanGen2,alloc1,spanGen1); break;
        case DESTINATION_IN    : BlendSourceIn     (raster2, raster1, sl2, sl1, renBase,alloc2,spanGen2); break;
        case DESTINATION_OUT   : BlendSourceOut    (raster2, raster1, sl2, sl1, renBase,alloc2,spanGen2); break;
        case LIGHTER           : BlendLIGHTER    (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1,alloc2,spanGen2); break;
        case COPY              : RenderScanlinesAntiAlias(raster1,sl1,renBase,alloc1,spanGen1); break;
        case XOR               : BlendXOR   (raster1, raster2, sl1, sl2, renBase,alloc1,spanGen1,alloc2,spanGen2); break;
        }
    }

    template <class Span>
    void calcinterScanline(ScanlineUnPackedContainer& scanline3, int x1, int x2, Span& span1, Span& span2)
    {
        scanline3.Reset(x1, x2 + span2->spanLength);
        unsigned len3 = x2 + span2->spanLength - x1;
        uint8_t* cover1 = span1->covers;
        uint8_t* cover2 = span2->covers + (x1 - x2);
        int x3 = x1;
        for (unsigned i = 0; i < len3; i++, cover1++, cover2++) {
            if (*(cover1) != COVER_FULL) {
                scanline3.AddCell(x3++, *cover1);
            } else {
                scanline3.AddCell(x3++, *cover2);
            }
        }
    }

    template <class Span>
    void calcOutScanlineRight(ScanlineUnPackedContainer& scanline, int x1, int x2, Span& span1, Span& span2)
    {
        uint8_t* cover1 = span1->covers;
        uint8_t* cover2 = span2->covers;
        scanline.Reset(x2, x2 + span2->spanLength);
        for (int i = 1; i <= span2->spanLength; i++, cover2++) {
            if (x1 == x2) {
                scanline.AddCell(x2++, COVER_FULL - (*cover1));
                x1++;
                cover1++;
            } else {
                scanline.AddCell(x2++, *cover2);
            }
        }
    }

    template <class Span>
    void calcOutScanlineLeft(ScanlineUnPackedContainer& scanline, int x1, int x2, Span& span1, Span& span2)
    {
        uint8_t* cover2 = span2->covers + (x1 - x2);
        scanline.Reset(x1, x1 + span1->spanLength);
        int len1 = span2->spanLength - (x1 - x2); // Length of intersection
        int len2 = span1->spanLength - len1;      // Length of intersection
        uint8_t* cover1 = span1->covers + len1;
        for (int i = 0; i < len1; i++, cover2++) {
            scanline.AddCell(x1++, COVER_FULL - (*cover2));
        }

        for (int i = 0; i < len2; i++, cover1++) {
            scanline.AddCell(x1++, *cover1);
        }
    }

    template <class Span>
    void calcOutScanlineAll(ScanlineUnPackedContainer& scanline, int x1, int x2, Span& span1, Span& span2)
    {
        uint8_t* cover1 = span1->covers;
        uint8_t* cover2 = span2->covers;
        int x3 = x1;
        scanline.Reset(x2, x2 + span2->spanLength);
        for (int i = 0; i < span2->spanLength; i++, cover2++) {
            if (x3 == x2 && x3 <= x1 + span1->spanLength - 1) {
                scanline.AddCell(x2++, COVER_FULL - (*cover1));
                x3++;
                cover1++;
            } else {
                scanline.AddCell(x2++, *cover2);
            }
        }
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator1,
             class SpanGenerator2>
    void BlendSourceAtop(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,
                         SpanAllocator& alloc1, SpanGenerator1& spanGen1,
                         SpanAllocator& alloc2, SpanGenerator2& spanGen2)
    {
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if (raster1.SweepScanline(scanline1)) {
                y1 = scanline1.GetYLevel();
                spanGen1.Prepare(); // Segment generator preparation
            }
            while (raster2.SweepScanline(scanline2)) {
                spanGen2.Prepare(); // Segment generator preparation
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();

                unsigned num_spans1;
                typename Scanline::ConstIterator span1;

                if (y1 == y2) {
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }

                if (y2 > y1) {
                    while (raster1.SweepScanline(scanline1)) {
                        y1 = scanline1.GetYLevel();
                        if (y1 == y2) {
                            num_spans1 = scanline1.NumSpans();
                            span1 = scanline1.Begin();
                            break;
                        }
                    }
                }

                while (true) {
                    int x2 = span2->x;
                    if (y1 == y2) {
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if(span1->spanLength > 0){
                                typename BaseRenderer::color_type* colors = alloc2.Resize(span2->spanLength);
                                spanGen2.Generate(colors, x2, y2, span2->spanLength);
                                renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                        colors,
                                                        span2->covers);


                                if(x1<=x2 && x2+span2->spanLength < x1+span1->spanLength){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span2->spanLength);
                                    spanGen1.Generate(colors, x2, y2, span2->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                            colors,
                                                            span2->covers);
                                }

                                // Span1 in Span2
                                // spa2   ------------------------
                                // spa1     -------------------
                                if(x1>=x2 &&x1+span1->spanLength<=x2+span2->spanLength){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span1->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span1->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span1->spanLength,
                                                            colors,
                                                            span1->covers);
                                }

                                // spa2   ------------------------
                                // spa1                 -------------------
                                if (x1 > x2 &&
                                    x1 < x2 + span2->spanLength &&
                                    x1 + span1->spanLength >= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }

                                // spa2              ------------------------
                                // spa1     -------------------
                                if(x1<x2&&
                                   x1+span1->spanLength > x2 &&
                                   x1+span1->spanLength <= x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x2, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                            }
                        }
                        ++span1;
                    } else {
                        if (span2->spanLength > 0) {
                            typename BaseRenderer::color_type* colors = alloc1.Resize(span2->spanLength);
                            spanGen2.Generate(colors, x2, y2, span2->spanLength);
                            renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                    colors,
                                                    span2->covers);
                        }
                    }
                    if (--num_spans2 == 0) {
                        break;
                    }
                    ++span2;
                }
                if (y1 == y2) {
                    raster1.SweepScanline(scanline1);
                    y1 = scanline1.GetYLevel();
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }
            }
        }
    }
    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator>
    void BlendSourceIn(Rasterizer& raster1, Rasterizer& raster2,
                       Scanline& scanline1, Scanline& scanline2,
                       BaseRenderer& renBase,
                       SpanAllocator& alloc1,
                       SpanGenerator& spanGen1)
    {
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if (raster1.SweepScanline(scanline1)) {
                y1 = scanline1.GetYLevel();
                spanGen1.Prepare();
            }
            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();

                unsigned num_spans1;
                typename Scanline::ConstIterator span1;

                if (y1 == y2) {
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }

                if (y2 > y1) {
                    while (raster1.SweepScanline(scanline1)) {
                        y1 = scanline1.GetYLevel();
                        if (y1 == y2) {
                            num_spans1 = scanline1.NumSpans();
                            span1 = scanline1.Begin();
                            break;
                        }
                    }
                }

                while (true) {
                    int x2 = span2->x;
                    if (y1 == y2) {
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if(span1->spanLength > 0){
                                // spa2       -----------------
                                // spa1     -----------------------
                                if(x1<=x2 && x2+span2->spanLength < x1+span1->spanLength){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span2->spanLength);
                                    spanGen1.Generate(colors, x2, y2, span2->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                            colors,
                                                            span2->covers);
                                }

                                // Span1 in Span2
                                // spa2   ------------------------
                                // spa1     -------------------
                                if(x1>=x2 &&x1+span1->spanLength<=x2+span2->spanLength){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span1->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span1->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span1->spanLength,
                                                            colors,
                                                            span1->covers);
                                }

                                // spa2   ------------------------
                                // spa1                 -------------------
                                if (x1 > x2 &&
                                    x1 < x2 + span2->spanLength &&
                                    x1 + span1->spanLength >= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }

                                // spa2              ------------------------
                                // spa1     -------------------
                                if(x1<x2&&
                                   x1+span1->spanLength > x2 &&
                                   x1+span1->spanLength <= x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x2, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                            }
                        }
                        ++span1;
                    }
                    if (--num_spans2 == 0) {
                        break;
                    }
                    ++span2;
                }
                if (y1 == y2) {
                    raster1.SweepScanline(scanline1);
                    y1 = scanline1.GetYLevel();
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }
            }
        }
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator>
    void BlendSourceOut(Rasterizer& raster1, Rasterizer& raster2,
                        Scanline& scanline1, Scanline& scanline2,
                        BaseRenderer& renBase,
                        SpanAllocator& alloc1,
                        SpanGenerator& spanGen1)
    {
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if (raster1.SweepScanline(scanline1)) {
                y1 = scanline1.GetYLevel();
                spanGen1.Prepare();
            }

            if (raster1.MinY() < raster2.MinY()) {
                do {
                    y1 = scanline1.GetYLevel();
                    if (y1 == raster2.MinY()) {
                        break;
                    }
                    unsigned num_spans = scanline1.NumSpans();
                    typename Scanline::ConstIterator span = scanline1.Begin();

                    while (true) {
                        int x = span->x;
                        if (span->spanLength > 0) {
                            typename BaseRenderer::color_type* colors = alloc1.Resize(span->spanLength);
                            spanGen1.Generate(colors, x, y1, span->spanLength);
                            renBase.BlendColorHspan(x, y1, (unsigned)span->spanLength,
                                                    colors,
                                                    span->covers);
                        }
                        if (--num_spans == 0) {
                            break;
                        }
                        ++span;
                    }
                } while (raster1.SweepScanline(scanline1));
            }

            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();
                unsigned num_spans1;
                typename Scanline::ConstIterator span1;
                if (y1 == y2) {
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }
                while (true) {
                    int x2 = span2->x;
                    if (y1 == y2) {
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if (span1->spanLength > 0) {
                                // spa2       -----------------
                                // spa1     -----------------------
                                if (x1 < x2 && x2 + span2->spanLength < x1 + span1->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineAll(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                                // spa2   ------------------------
                                // spa1                 -------------------
                                // Intersecting part
                                if (x1 >= x2 &&
                                    x1 < x2 + span2->spanLength &&
                                    x1 + span1->spanLength >= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline4;
                                    calcOutScanlineLeft(scanline4, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span4 = scanline4.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span4->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span4->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span4->spanLength,
                                                            colors,
                                                            span4->covers);
                                }
                                // spa2              ------------------------
                                // spa1     -------------------
                                if (x1 <= x2 &&
                                    x1 + span1->spanLength >= x2 &&
                                    x1 + span1->spanLength <= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineRight(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                                // spa2              ------------------------
                                // spa1     -------                            ----------
                                // No blending points are drawn separately
                                if(x1+span1->spanLength < x2||
                                   x2+span2->spanLength < x1){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span1->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span1->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span1->spanLength,
                                                            colors,
                                                            span1->covers);
                                }
                            }
                        }
                        ++span1;
                    }
                    if (--num_spans2 == 0) {
                        break;
                    }
                    ++span2;
                }
                if (y1 == y2 && y1 < raster2.MaxY() - 1) {
                    if (raster1.SweepScanline(scanline1)) {
                        y1 = scanline1.GetYLevel();
                        num_spans1 = scanline1.NumSpans();
                        span1 = scanline1.Begin();
                    }
                }
            }
            // Draw the rest of raster1
            while (raster1.SweepScanline(scanline1)) {
                int y = scanline1.GetYLevel();
                unsigned num_spans = scanline1.NumSpans();
                typename Scanline::ConstIterator span = scanline1.Begin();
                while (true) {
                    int x = span->x;
                    if (span->spanLength > 0) {
                        typename BaseRenderer::color_type* colors = alloc1.Resize(span->spanLength);
                        spanGen1.Generate(colors, x, y, span->spanLength);
                        renBase.BlendColorHspan(x, y, (unsigned)span->spanLength,
                                                colors,
                                                span->covers);
                    }
                    if (--num_spans == 0) {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator1,
             class SpanGenerator2>
    void BlendSourceOver(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,
                         SpanAllocator& alloc1,
                         SpanGenerator1& spanGen1,
                         SpanAllocator& alloc2,
                         SpanGenerator2& spanGen2)
    {   
        RenderScanlinesAntiAlias(raster1,scanline1,renBase,alloc1,spanGen1);
        RenderScanlinesAntiAlias(raster2,scanline2,renBase,alloc2,spanGen2);
    }


    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator1,
             class SpanGenerator2>
    void BlendXOR(Rasterizer& raster1, Rasterizer& raster2,
                  Scanline& scanline1, Scanline& scanline2,
                  BaseRenderer& renBase,
                  SpanAllocator& alloc1,
                  SpanGenerator1& spanGen1,
                  SpanAllocator& alloc2,
                  SpanGenerator2& spanGen2)
    {
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if (raster1.SweepScanline(scanline1)) {
                y1 = scanline1.GetYLevel();
                spanGen1.Prepare();
            }
            if (raster1.MinY() < raster2.MinY()) {
                do {
                    y1 = scanline1.GetYLevel();
                    if (y1 == raster2.MinY()) {
                        break;
                    }
                    unsigned num_spans = scanline1.NumSpans();
                    typename Scanline::ConstIterator span = scanline1.Begin();
                    while (true) {
                        int x = span->x;
                        if (span->spanLength > 0) {
                            typename BaseRenderer::color_type* colors = alloc1.Resize(span->spanLength);
                            spanGen1.Generate(colors, x, y1, span->spanLength);
                            renBase.BlendColorHspan(x, y1, (unsigned)span->spanLength,
                                                    colors,
                                                    span->covers);
                        }
                        if (--num_spans == 0) {
                            break;
                        }
                        ++span;
                    }
                } while (raster1.SweepScanline(scanline1));
            }

            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                spanGen2.Prepare();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();
                unsigned num_spans1;
                typename Scanline::ConstIterator span1;
                if (y1 == y2) {
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }
                while (true) {
                    int x2 = span2->x;
                    if (y1 == y2) {
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if (span1->spanLength > 0) {
                                // span1 in span2
                                // spa2   ------------------------
                                // spa1     -------------------
                                if (x1 >= x2 && x1 + span1->spanLength <= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineAll(scanline3, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc2.Resize(span3->spanLength);
                                    spanGen2.Generate(colors, x2, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                                // spa2       -----------------
                                // spa1     -----------------------
                                if (x1 < x2 && x2 + span2->spanLength < x1 + span1->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineAll(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);
                                }
                                // spa2   ------------------------
                                // spa1                 -------------------
                                // Intersecting part
                                if (x1 >= x2 &&
                                    x1 < x2 + span2->spanLength &&
                                    x1 + span1->spanLength >= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineRight(scanline3, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc2.Resize(span3->spanLength);
                                    spanGen2.Generate(colors, x2, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);

                                    ScanlineUnPackedContainer scanline4;
                                    calcOutScanlineLeft(scanline4, x1, x2, span1, span2);
                                    typename Scanline::ConstIterator span4 = scanline4.Begin();
                                    typename BaseRenderer::color_type* colors2 = alloc1.Resize(span4->spanLength);
                                    spanGen1.Generate(colors2, x1, y2, span4->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span4->spanLength,
                                                            colors2,
                                                            span4->covers);
                                }
                                // spa2              ------------------------
                                // spa1     -------------------
                                if (x1 <= x2 &&
                                    x1 + span1->spanLength >= x2 &&
                                    x1 + span1->spanLength <= x2 + span2->spanLength) {
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanlineRight(scanline3, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span3->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span3->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span3->spanLength,
                                                            colors,
                                                            span3->covers);

                                    ScanlineUnPackedContainer scanline4;
                                    calcOutScanlineLeft(scanline4, x2, x1, span2, span1);
                                    typename Scanline::ConstIterator span4 = scanline4.Begin();
                                    typename BaseRenderer::color_type* colors2 = alloc2.Resize(span4->spanLength);
                                    spanGen2.Generate(colors2, x2, y2, span4->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span4->spanLength,
                                                            colors2,
                                                            span4->covers);
                                }
                                // spa2              ------------------------
                                // spa1     -------                            ----------
                                // No blending points are drawn separately
                                if(x1+span1->spanLength < x2||
                                   x2+span2->spanLength < x1){
                                    typename BaseRenderer::color_type* colors = alloc1.Resize(span1->spanLength);
                                    spanGen1.Generate(colors, x1, y2, span1->spanLength);
                                    renBase.BlendColorHspan(x1, y2, (unsigned)span1->spanLength,
                                                            colors,
                                                            span1->covers);
                                    typename BaseRenderer::color_type* colors2 = alloc2.Resize(span2->spanLength);
                                    spanGen2.Generate(colors2, x2, y2, span2->spanLength);
                                    renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                            colors2,
                                                            span2->covers);
                                }
                            }
                        }
                        ++span1;
                    } else {
                        if (span2->spanLength > 0) {
                            typename BaseRenderer::color_type* colors2 = alloc2.Resize(span2->spanLength);
                            spanGen2.Generate(colors2, x2, y2, span2->spanLength);
                            renBase.BlendColorHspan(x2, y2, (unsigned)span2->spanLength,
                                                    colors2,
                                                    span2->covers);
                        }
                    }
                    if (--num_spans2 == 0) {
                        break;
                    }
                    ++span2;
                }
                if (y1 == y2 && y1 < raster2.MaxY() - 1) {
                    if (raster1.SweepScanline(scanline1)) {
                        y1 = scanline1.GetYLevel();
                        num_spans1 = scanline1.NumSpans();
                        span1 = scanline1.Begin();
                    }
                }
            }
            // Draw the rest of raster1
            while (raster1.SweepScanline(scanline1)) {
                int y = scanline1.GetYLevel();
                unsigned num_spans = scanline1.NumSpans();
                typename Scanline::ConstIterator span = scanline1.Begin();
                while (true) {
                    int x = span->x;
                    if (span->spanLength > 0) {
                        typename BaseRenderer::color_type* colors = alloc1.Resize(span->spanLength);
                        spanGen1.Generate(colors, x, y, span->spanLength);
                        renBase.BlendColorHspan(x, y, (unsigned)span->spanLength,
                                                colors,
                                                span->covers);
                    }
                    if (--num_spans == 0) {
                        break;
                    }
                    ++span;
                }
            }
        }
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class SpanAllocator,
             class SpanGenerator1,
             class SpanGenerator2>
    void BlendLIGHTER(Rasterizer& raster1, Rasterizer& raster2,
                      Scanline& scanline1, Scanline& scanline2,
                      BaseRenderer& renBase,
                      SpanAllocator& alloc1,
                      SpanGenerator1& spanGen1,
                      SpanAllocator& alloc2,
                      SpanGenerator2& spanGen2)
    {
        BlendXOR(raster1, raster2, scanline1, scanline2, renBase,alloc1,spanGen1,alloc2,spanGen2);
        typedef Rgba8 Rgba8Color;
        typedef SpanBlendColor<SpanGenerator1,SpanGenerator2,Rgba8Color> SpanBlendColor;
        SpanBlendColor spanBlendColor(spanGen1,spanGen2);
        BlendSourceIn(raster1, raster2, scanline1, scanline2, renBase,alloc1,spanBlendColor);
    }
} // namespace OHOS
#endif
