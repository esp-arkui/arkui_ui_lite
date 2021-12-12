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

#ifndef GRAPHIC_GEOMETRY_PIXFMT_RGBA_INCLUDED
#define GRAPHIC_GEOMETRY_PIXFMT_RGBA_INCLUDED

#include <cmath>
#include <cstring>

#include "agg_pixfmt_rgba_blend.h"
#include "agg_pixfmt_rgba_comp.h"
#include "agg_pixfmt_rgba_conv.h"
#include "agg_pixfmt_rgba_gamma.h"
#include "agg_pixfmt_rgba_multi.h"
#include "gfx_utils/heap_base.h"
#include "render/agg_pixfmt_base.h"
#include "render/rendering_buffer.h"

namespace OHOS {

    enum CompOpEnum
    {
        COMP_OP_CLEAR,
        COMP_OP_SRC,
        COMP_OP_DST,
        COMP_OP_SRC_OVER,
        COMP_OP_DST_OVER,
        COMP_OP_SRC_IN,
        COMP_OP_DST_IN,
        COMP_OP_SRC_OUT,
        COMP_OP_DST_OUT,
        COMP_OP_SRC_ATOP,
        COMP_OP_DST_ATOP,
        COMP_OP_XOR,
        COMP_OP_PLUS,
        COMP_OP_MULTIPLY,
        COMP_OP_SCREEN,
        COMP_OP_OVERLAY,
        COMP_OP_DARKEN,
        COMP_OP_LIGHTEN,
        COMP_OP_COLOR_DODGE,
        COMP_OP_COLOR_BURN,
        COMP_OP_HARD_LIGHT,
        COMP_OP_SOFT_LIGHT,
        COMP_OP_DIFFERENCE,
        COMP_OP_EXCLUSION,
        END_OF_COMP_OP_E
    };

    using BlenderRgba32 = BlenderRgba<Rgba8, OrderRgba>;
    using BlenderArgb32 = BlenderRgba<Rgba8, OrderArgb>;
    using BlenderAbgr32 = BlenderRgba<Rgba8, OrderAbgr>;
    using BlenderBgra32 = BlenderRgba<Rgba8, OrderBgra>;

    using BlenderSrgba32 = BlenderRgba<Srgba8, OrderRgba>;
    using BlenderSargb32 = BlenderRgba<Srgba8, OrderArgb>;
    using BlenderSabgr32 = BlenderRgba<Srgba8, OrderAbgr>;
    using BlenderSbgra32 = BlenderRgba<Srgba8, OrderBgra>;

    using BlenderRgba32Pre = BlenderRgbaPre<Rgba8, OrderRgba>;
    using BlenderArgb32Pre = BlenderRgbaPre<Rgba8, OrderArgb>;
    using BlenderAbgr32Pre = BlenderRgbaPre<Rgba8, OrderAbgr>;
    using BlenderBgra32Pre = BlenderRgbaPre<Rgba8, OrderBgra>;

    using BlenderSrgba32Pre = BlenderRgbaPre<Srgba8, OrderRgba>;
    using BlenderSargb32Pre = BlenderRgbaPre<Srgba8, OrderArgb>;
    using BlenderSabgr32Pre = BlenderRgbaPre<Srgba8, OrderAbgr>;
    using BlenderSbgra32Pre = BlenderRgbaPre<Srgba8, OrderBgra>;

    using BlenderRgba32Plain = BlenderRgbaPlain<Rgba8, OrderRgba>;
    using BlenderArgb32Plain = BlenderRgbaPlain<Rgba8, OrderArgb>;
    using BlenderAbgr32Plain = BlenderRgbaPlain<Rgba8, OrderAbgr>;
    using BlenderBgra32Plain = BlenderRgbaPlain<Rgba8, OrderBgra>;

    using BlenderSrgba32Plain = BlenderRgbaPlain<Srgba8, OrderRgba>;
    using BlenderSargb32Plain = BlenderRgbaPlain<Srgba8, OrderArgb>;
    using BlenderSabgr32Plain = BlenderRgbaPlain<Srgba8, OrderAbgr>;
    using BlenderSbgra32Plain = BlenderRgbaPlain<Srgba8, OrderBgra>;

    using BlenderRgba128 = BlenderRgba<Rgba32, OrderRgba>;
    using BlenderArgb128 = BlenderRgba<Rgba32, OrderArgb>;
    using BlenderAbgr128 = BlenderRgba<Rgba32, OrderAbgr>;
    using BlenderBgra128 = BlenderRgba<Rgba32, OrderBgra>;

    using BlenderRgba128Pre = BlenderRgbaPre<Rgba32, OrderRgba>;
    using BlenderArgb128Pre = BlenderRgbaPre<Rgba32, OrderArgb>;
    using BlenderAbgr128Pre = BlenderRgbaPre<Rgba32, OrderAbgr>;
    using BlenderBgra128Pre = BlenderRgbaPre<Rgba32, OrderBgra>;

    using BlenderRgba128Plain = BlenderRgbaPlain<Rgba32, OrderRgba>;
    using BlenderArgb128Plain = BlenderRgbaPlain<Rgba32, OrderArgb>;
    using BlenderAbgr128Plain = BlenderRgbaPlain<Rgba32, OrderAbgr>;
    using BlenderBgra128Plain = BlenderRgbaPlain<Rgba32, OrderBgra>;

    using PixfmtRgba32 = PixfmtAlphaBlendRgba<BlenderRgba32, RenderingBuffer>;
    using PixfmtArgb32 = PixfmtAlphaBlendRgba<BlenderArgb32, RenderingBuffer>;
    using PixfmtAbgr32 = PixfmtAlphaBlendRgba<BlenderAbgr32, RenderingBuffer>;
    using PixfmtBgra32 = PixfmtAlphaBlendRgba<BlenderBgra32, RenderingBuffer>;

    using PixfmtSrgba32 = PixfmtAlphaBlendRgba<BlenderSrgba32, RenderingBuffer>;
    using PixfmtSargb32 = PixfmtAlphaBlendRgba<BlenderSargb32, RenderingBuffer>;
    using PixfmtSabgr32 = PixfmtAlphaBlendRgba<BlenderSabgr32, RenderingBuffer>;
    using PixfmtSbgra32 = PixfmtAlphaBlendRgba<BlenderSbgra32, RenderingBuffer>;

    using PixfmtRgba32Pre = PixfmtAlphaBlendRgba<BlenderRgba32Pre, RenderingBuffer>;
    using PixfmtArgb32Pre = PixfmtAlphaBlendRgba<BlenderArgb32Pre, RenderingBuffer>;
    using PixfmtAbgr32Pre = PixfmtAlphaBlendRgba<BlenderAbgr32Pre, RenderingBuffer>;
    using PixfmtBgra32Pre = PixfmtAlphaBlendRgba<BlenderBgra32Pre, RenderingBuffer>;

    using PixfmtSrgba32Pre = PixfmtAlphaBlendRgba<BlenderSrgba32Pre, RenderingBuffer>;
    using PixfmtSargb32Pre = PixfmtAlphaBlendRgba<BlenderSargb32Pre, RenderingBuffer>;
    using PixfmtSabgr32Pre = PixfmtAlphaBlendRgba<BlenderSabgr32Pre, RenderingBuffer>;
    using PixfmtSbgra32Pre = PixfmtAlphaBlendRgba<BlenderSbgra32Pre, RenderingBuffer>;

    using PixfmtRgba32Plain = PixfmtAlphaBlendRgba<BlenderRgba32Plain, RenderingBuffer>;
    using PixfmtArgb32Plain = PixfmtAlphaBlendRgba<BlenderArgb32Plain, RenderingBuffer>;
    using PixfmtAbgr32Plain = PixfmtAlphaBlendRgba<BlenderAbgr32Plain, RenderingBuffer>;
    using PixfmtBgra32Plain = PixfmtAlphaBlendRgba<BlenderBgra32Plain, RenderingBuffer>;

    using PixfmtSrgba32Plain = PixfmtAlphaBlendRgba<BlenderSrgba32Plain, RenderingBuffer>;
    using PixfmtSargb32Plain = PixfmtAlphaBlendRgba<BlenderSargb32Plain, RenderingBuffer>;
    using PixfmtSabgr32Plain = PixfmtAlphaBlendRgba<BlenderSabgr32Plain, RenderingBuffer>;
    using PixfmtSbgra32Plain = PixfmtAlphaBlendRgba<BlenderSbgra32Plain, RenderingBuffer>;

    using PixfmtRgba128 = PixfmtAlphaBlendRgba<BlenderRgba128, RenderingBuffer>;
    using PixfmtArgb128 = PixfmtAlphaBlendRgba<BlenderArgb128, RenderingBuffer>;
    using PixfmtAbgr128 = PixfmtAlphaBlendRgba<BlenderAbgr128, RenderingBuffer>;
    using PixfmtBgra128 = PixfmtAlphaBlendRgba<BlenderBgra128, RenderingBuffer>;

    using PixfmtRgba128Pre = PixfmtAlphaBlendRgba<BlenderRgba128Pre, RenderingBuffer>;
    using PixfmtArgb128Pre = PixfmtAlphaBlendRgba<BlenderArgb128Pre, RenderingBuffer>;
    using PixfmtAbgr128Pre = PixfmtAlphaBlendRgba<BlenderAbgr128Pre, RenderingBuffer>;
    using PixfmtBgra128Pre = PixfmtAlphaBlendRgba<BlenderBgra128Pre, RenderingBuffer>;

    using PixfmtRgba128Plain = PixfmtAlphaBlendRgba<BlenderRgba128Plain, RenderingBuffer>;
    using PixfmtArgb128Plain = PixfmtAlphaBlendRgba<BlenderArgb128Plain, RenderingBuffer>;
    using PixfmtAbgr128Plain = PixfmtAlphaBlendRgba<BlenderAbgr128Plain, RenderingBuffer>;
    using PixfmtBgra128Plain = PixfmtAlphaBlendRgba<BlenderBgra128Plain, RenderingBuffer>;

} // namespace OHOS

#endif
