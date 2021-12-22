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

#include "gfx_utils/heap_base.h"
#include "graphic_render_pixfmt_rgba_blend.h"
#include "graphic_render_pixfmt_rgba_comp.h"
#include "graphic_render_pixfmt_rgba_gamma.h"
#include "graphic_render_pixfmt_rgba_multi.h"
#include "render/graphic_render_buffer.h"
#include "render/graphic_render_pixfmt_base.h"

namespace OHOS {
    enum CompOpEnum {
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

    using BlenderRgba32Pre = BlenderRgbaPre<Rgba8, OrderRgba>;
    using BlenderArgb32Pre = BlenderRgbaPre<Rgba8, OrderArgb>;
    using BlenderAbgr32Pre = BlenderRgbaPre<Rgba8, OrderAbgr>;
    using BlenderBgra32Pre = BlenderRgbaPre<Rgba8, OrderBgra>;

    using BlenderRgba32Plain = BlenderRgbaPlain<Rgba8, OrderRgba>;
    using BlenderArgb32Plain = BlenderRgbaPlain<Rgba8, OrderArgb>;
    using BlenderAbgr32Plain = BlenderRgbaPlain<Rgba8, OrderAbgr>;
    using BlenderBgra32Plain = BlenderRgbaPlain<Rgba8, OrderBgra>;

    using PixfmtRgba32 = PixfmtAlphaBlendRgba<BlenderRgba32, RenderingBuffer>;
    using PixfmtArgb32 = PixfmtAlphaBlendRgba<BlenderArgb32, RenderingBuffer>;
    using PixfmtAbgr32 = PixfmtAlphaBlendRgba<BlenderAbgr32, RenderingBuffer>;
    using PixfmtBgra32 = PixfmtAlphaBlendRgba<BlenderBgra32, RenderingBuffer>;

    using PixfmtRgba32Pre = PixfmtAlphaBlendRgba<BlenderRgba32Pre, RenderingBuffer>;
    using PixfmtArgb32Pre = PixfmtAlphaBlendRgba<BlenderArgb32Pre, RenderingBuffer>;
    using PixfmtAbgr32Pre = PixfmtAlphaBlendRgba<BlenderAbgr32Pre, RenderingBuffer>;
    using PixfmtBgra32Pre = PixfmtAlphaBlendRgba<BlenderBgra32Pre, RenderingBuffer>;

    using PixfmtRgba32Plain = PixfmtAlphaBlendRgba<BlenderRgba32Plain, RenderingBuffer>;
    using PixfmtArgb32Plain = PixfmtAlphaBlendRgba<BlenderArgb32Plain, RenderingBuffer>;
    using PixfmtAbgr32Plain = PixfmtAlphaBlendRgba<BlenderAbgr32Plain, RenderingBuffer>;
    using PixfmtBgra32Plain = PixfmtAlphaBlendRgba<BlenderBgra32Plain, RenderingBuffer>;

} // namespace OHOS

#endif
