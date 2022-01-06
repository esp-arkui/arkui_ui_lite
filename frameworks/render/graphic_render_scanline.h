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

#include <cstdlib>
#include <cstring>
#include <limits>

#include <gfx_utils/graphics/scanline/graphic_geometry_scanline.h>

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


    template <class Rasterizer, class Scanline,
              class BaseRenderer, class ColorT>
    void RenderClipAntiAliasSolid(Rasterizer& raster, Scanline& scanline,
                                  BaseRenderer& renBase, const ColorT& color)
    {
        if (raster.RewindScanlines()) {
            typename BaseRenderer::color_type ren_color = color;

            // 需要裁减的图形上方
            for (int y = 0; y < raster.MinY(); y++) {
                renBase.BlendHline(0, y, (unsigned)(renBase.Width()),
                                   ren_color,
                                   COVER_FULL);
            }

            // 需要裁减的图形下方
            for (int y = raster.MaxY(); y < renBase.Height(); y++) {
                renBase.BlendHline(0, y, (unsigned)(renBase.Width()),
                                   ren_color,
                                   COVER_FULL);
            }
            // 需要裁减的图形左方和右方
            for (int y = raster.MinY(); y < raster.MaxY(); y++) {
                for (int x = 0; x < renBase.Width(); x++) {
                    // 裁减图形的左侧
                    if (x < raster.MinX()) {
                        renBase.BlendHline(x, y, (unsigned)(raster.MinX() - x - 1),
                                           ren_color,
                                           COVER_FULL);
                        x = raster.MinX();
                    }
                    // 裁减图形的右侧
                    if (x > raster.MaxX()) {
                        renBase.BlendHline(raster.MaxX(), y, (unsigned)(renBase.Width() - raster.MaxX()),
                                           ren_color,
                                           COVER_FULL);
                        x = renBase.Width();
                    }
                }
            }

            scanline.Reset(raster.MinX(), raster.MaxX());
            while (raster.SweepScanline(scanline)) {
                int y = scanline.GetYLevel();
                unsigned num_spans = scanline.NumSpans();
                typename Scanline::ConstIterator span = scanline.Begin();
                int preX = 0;
                while (true) {
                    int x = span->x;
                    if (preX < x) {
                        if (span->spanLength > 0) {
                            renBase.BlendHline(preX, y, (unsigned)(x - preX),
                                               ren_color,
                                               COVER_FULL);
                        } else {
                            renBase.BlendHline(x, y, (unsigned)(x - span->spanLength - 1),
                                               ren_color,
                                               *(span->covers));
                        }
                        preX = x + (span->spanLength > 0 ? span->spanLength : -span->spanLength);
                    }
                    if (--num_spans == 0) {
                        preX = 0;
                        break;
                    }
                    ++span;
                    preX = 0;
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
            spanGen.Prepare(); // 线段生成器预备
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

    //===========================================================================================//
    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class ColorT>
    void sbool_combine_shapes_aa(GlobalCompositeOperation op,
                                 Rasterizer& raster1, Rasterizer& raster2,
                                 Scanline& sl1, Scanline& sl2, BaseRenderer& renBase,const ColorT& color1,const ColorT& color2,const ColorT& color3)
    {
        switch(op)
        {
        case SOURCE_ATOP       : BlendSourceAtop   (raster1, raster2, sl1, sl2, renBase,color1,color2); break;
        case SOURCE_IN         : BlendSourceIn     (raster1, raster2, sl1, sl2, renBase,color1); break;
        case SOURCE_OUT        : BlendSourceOver    (raster1, raster2, sl1, sl2, renBase,color1,color3); break;
        case DESTINATION_OVER  : BlendSourceOver   (raster1, raster2, sl1, sl2, renBase,color1,color2); break;
        case DESTINATION_ATOP  : BlendSourceAtop   (raster2, raster1, sl2, sl1, renBase,color2,color1); break;
        case DESTINATION_IN    : BlendSourceIn     (raster2, raster1, sl2, sl1, renBase,color2); break;
        case DESTINATION_OUT   : BlendSourceOver    (raster2, raster1, sl2, sl1, renBase,color2,color3); break;
        case LIGHTER           : BlendLIGHTER    (raster1, raster2, sl1, sl2, renBase,color1,color2,color3); break;
        case COPY              : RenderScanlinesAntiAliasSolid(raster1,sl1,renBase,color1); break;
        case XOR               : BlendXOR   (raster1, raster2, sl1, sl2, renBase,color1,color2,color3); break;
        }
    }

    template<class Span>
    void calcinterScanline(ScanlineUnPackedContainer& scanline3,int x1,int x2,Span& span1,Span& span2){
        scanline3.Reset(x1,x2+span2->spanLength);
        unsigned len3 = x2+span2->spanLength-x1;
        int8u* cover1 = span1->covers;
        int8u* cover2 = span2->covers+(x1-x2);
        int x3=x1;
        for(unsigned i=0; i < len3;i++,cover1++,cover2++){
            if(*(cover1)!=COVER_FULL){
                scanline3.AddCell(x3++,*cover1);
            }else{
                scanline3.AddCell(x3++,*cover2);
            }
        }
    }

    template<class Span>
    void calcOutScanline(ScanlineUnPackedContainer& scanline3,int x1,int x2,Span& span1,Span& span2){
        scanline3.Reset(x1,x1+span1->spanLength);
        unsigned len3 = span1->spanLength;
        int8u* cover1 = span1->covers;
        int8u* cover2 = span2->covers+(x1-x2);
        for(unsigned i=0; i < len3;i++,cover1++,cover2++){
            scanline3.AddCell(x1++,*cover1);
        }
    }


    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class ColorT>
    void BlendSourceAtop(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1,const ColorT& color2)
    {
        typename BaseRenderer::color_type ren_color1 = color1;
        typename BaseRenderer::color_type ren_color2 = color2;
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if(raster1.SweepScanline(scanline1)){
                y1 = scanline1.GetYLevel();
            }
            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();

                unsigned num_spans1;
                typename Scanline::ConstIterator span1;

                if(y1 == y2){
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }

                if(y2 > y1){
                    while(raster1.SweepScanline(scanline1)){
                        y1 = scanline1.GetYLevel();
                        if(y1 == y2){
                            num_spans1 = scanline1.NumSpans();
                            span1 = scanline1.Begin();
                            break;
                        }
                    }
                }

                while (true) {
                    int x2 = span2->x;
                    if(y1 == y2){
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if(span1->spanLength > 0){
                                renBase.BlendSolidHspan(x2, y2, (unsigned)span2->spanLength,
                                                        ren_color2,
                                                        span2->covers);
                                // span1在span2中
                                // spa2   ------------------------
                                // spa1     -------------------
                                if(x1>=x2 &&x1+span1->spanLength<=x2+span2->spanLength){
                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span1->spanLength,
                                                            ren_color1,
                                                            span1->covers);
                                }

                                // spa2   ------------------------
                                // spa1                 -------------------
                                if(x1>x2 &&
                                   x1<x2+span2->spanLength &&
                                   x1+span1->spanLength >= x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3,x1,x2,span1,span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
                                                            span3->covers);
                                }

                                // spa2              ------------------------
                                // spa1     -------------------
                                if(x1<x2&&
                                   x1+span1->spanLength > x2 &&
                                   x1+span1->spanLength < x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3,x2,x1,span2,span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x2, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
                                                            span3->covers);
                                }
                            }
                        }
                        ++span1;

                    }else{
                        if (span2->spanLength > 0) {
                            renBase.BlendSolidHspan(x2, y2, (unsigned)span2->spanLength,
                                                    ren_color2,
                                                    span2->covers);
                        } else {
                            renBase.BlendHline(x2, y2, (unsigned)(x2 - span2->spanLength - 1),
                                               ren_color2,
                                               *(span2->covers));
                        }
                    }
                    if (--num_spans2 == 0) {
                        break;
                    }
                    ++span2;
                }
                if(y1 == y2){
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
             class ColorT>
    void BlendSourceIn(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1)
    {
        typename BaseRenderer::color_type ren_color1 = color1;
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if(raster1.SweepScanline(scanline1)){
                y1 = scanline1.GetYLevel();
            }
            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();

                unsigned num_spans1;
                typename Scanline::ConstIterator span1;

                if(y1 == y2){
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }

                if(y2 > y1){
                    while(raster1.SweepScanline(scanline1)){
                        y1 = scanline1.GetYLevel();
                        if(y1 == y2){
                            num_spans1 = scanline1.NumSpans();
                            span1 = scanline1.Begin();
                            break;
                        }
                    }
                }

                while (true) {
                    int x2 = span2->x;
                    if(y1 == y2){
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if(span1->spanLength > 0){
                                // span1在span2中
                                // spa2   ------------------------
                                // spa1     -------------------
                                if(x1>=x2 &&x1+span1->spanLength<=x2+span2->spanLength){
                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span1->spanLength,
                                                            ren_color1,
                                                            span1->covers);
                                }

                                // spa2   ------------------------
                                // spa1                 -------------------
                                if(x1>x2 &&
                                   x1<x2+span2->spanLength &&
                                   x1+span1->spanLength >= x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3,x1,x2,span1,span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
                                                            span3->covers);
                                }

                                // spa2              ------------------------
                                // spa1     -------------------
                                if(x1<x2&&
                                   x1+span1->spanLength > x2 &&
                                   x1+span1->spanLength < x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcinterScanline(scanline3,x2,x1,span2,span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x2, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
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
                if(y1 == y2){
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
             class ColorT>
    void BlendSourceOut(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1,const ColorT& color2)
    {
        typename BaseRenderer::color_type ren_color1 = color1;
        typename BaseRenderer::color_type ren_color2 = color2;
        if (raster1.RewindScanlines()&&raster2.RewindScanlines()) {
            scanline1.Reset(raster1.MinX(), raster1.MaxX());
            scanline2.Reset(raster2.MinX(), raster2.MaxX());

            int y1 = 0;
            if(raster1.SweepScanline(scanline1)){
                y1 = scanline1.GetYLevel();
            }
            while (raster2.SweepScanline(scanline2)) {
                int y2 = scanline2.GetYLevel();
                unsigned num_spans2 = scanline2.NumSpans();
                typename Scanline::ConstIterator span2 = scanline2.Begin();

                unsigned num_spans1;
                typename Scanline::ConstIterator span1;

                if(y1 == y2){
                    num_spans1 = scanline1.NumSpans();
                    span1 = scanline1.Begin();
                }

                if(y2 > y1){
                    while(raster1.SweepScanline(scanline1)){
                        y1 = scanline1.GetYLevel();
                        if(y1 == y2){
                            num_spans1 = scanline1.NumSpans();
                            span1 = scanline1.Begin();
                            break;
                        }
                    }
                }

                while (true) {
                    int x2 = span2->x;
                    if(y1 == y2){
                        int x1 = span1->x;
                        if (span2->spanLength > 0) {
                            if(span1->spanLength > 0){
                                // span1在span2中
                                // spa2   ------------------------
                                // spa1     -------------------
                                if(x1>=x2 &&x1+span1->spanLength<=x2+span2->spanLength){
//                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span1->spanLength,
//                                                            ren_color1,
//                                                            span1->covers);
                                }

                                // spa2   ------------------------
                                // spa1                 -------------------
                                if(x1>x2 &&
                                   x1<x2+span2->spanLength &&
                                   x1+span1->spanLength >= x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanline(scanline3,x1,x2,span1,span2);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x1, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
                                                            span3->covers);
                                }

                                // spa2              ------------------------
                                // spa1     -------------------
                                if(x1<x2&&
                                   x1+span1->spanLength > x2 &&
                                   x1+span1->spanLength < x2+span2->spanLength){
                                    ScanlineUnPackedContainer scanline3;
                                    calcOutScanline(scanline3,x2,x1,span2,span1);
                                    typename Scanline::ConstIterator span3 = scanline3.Begin();
                                    renBase.BlendSolidHspan(x2, y2, (unsigned)span3->spanLength,
                                                            ren_color1,
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
                if(y1 == y2){
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
             class ColorT>
    void BlendSourceOver(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1,const ColorT& color2)
    {

        RenderScanlinesAntiAliasSolid(raster1,scanline1,renBase,color1);
        RenderScanlinesAntiAliasSolid(raster2,scanline2,renBase,color2);
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class ColorT>
    void BlendXOR(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1,const ColorT& color2,const ColorT& color3)
    {

        RenderScanlinesAntiAliasSolid(raster2,scanline2,renBase,color2);
        RenderScanlinesAntiAliasSolid(raster1,scanline1,renBase,color1);
        BlendSourceIn(raster1, raster2, scanline1, scanline2, renBase,color3);
    }

    template<class Rasterizer,
             class Scanline,
             class BaseRenderer,
             class ColorT>
    void BlendLIGHTER(Rasterizer& raster1, Rasterizer& raster2,
                               Scanline& scanline1, Scanline& scanline2,
                               BaseRenderer& renBase,const ColorT& color1,const ColorT& color2,const ColorT& color3)
    {

        RenderScanlinesAntiAliasSolid(raster2,scanline2,renBase,color2);
        RenderScanlinesAntiAliasSolid(raster1,scanline1,renBase,color1);
        BlendSourceIn(raster1, raster2, scanline1, scanline2, renBase,color3);
    }


} // namespace OHOS

#endif
