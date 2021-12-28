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

} // namespace OHOS

#endif
