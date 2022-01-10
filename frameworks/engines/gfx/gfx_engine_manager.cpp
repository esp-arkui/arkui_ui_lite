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

#include "engines/gfx/gfx_engine_manager.h"
#include "draw/clip_utils.h"
#include "draw/draw_arc.h"
#include "draw/draw_curve.h"
#include "draw/draw_line.h"
#include "draw/draw_rect.h"

namespace OHOS {
    BaseGfxEngine* BaseGfxEngine::baseEngine_ = nullptr;
    void BaseGfxEngine::DrawArc(BufferInfo& dst,
                                ArcInfo& arcInfo,
                                const Rect& mask,
                                const Style& style,
                                OpacityType opacity,
                                uint8_t cap)
    {
        DrawArc::GetInstance()->Draw(dst, arcInfo, mask, style, opacity, cap);
    }

    void BaseGfxEngine::DrawLine(BufferInfo& dst,
                                 const Point& start,
                                 const Point& end,
                                 const Rect& mask,
                                 int16_t width,
                                 ColorType color,
                                 OpacityType opacity)
    {
        DrawLine::Draw(dst, start, end, mask, width, color, opacity);
    }

    void BaseGfxEngine::DrawLetter(BufferInfo& gfxDstBuffer,
                                   const uint8_t* fontMap,
                                   const Rect& fontRect,
                                   const Rect& subRect,
                                   const uint8_t fontWeight,
                                   const ColorType& color,
                                   const OpacityType opa)
    {
        DrawUtils::GetInstance()->DrawLetter(gfxDstBuffer, fontMap, fontRect, subRect, fontWeight, color, opa);
    }

    void BaseGfxEngine::DrawCubicBezier(BufferInfo& dst,
                                        const Point& start,
                                        const Point& control1,
                                        const Point& control2,
                                        const Point& end,
                                        const Rect& mask,
                                        int16_t width,
                                        ColorType color,
                                        OpacityType opacity)
    {
        DrawCurve::DrawCubicBezier(dst, start, control1, control2, end, mask, width, color, opacity);
    }

    void BaseGfxEngine::DrawRect(BufferInfo& dst,
                                 const Rect& rect,
                                 const Rect& dirtyRect,
                                 const Style& style,
                                 OpacityType opacity)
    {
        DrawRect::Draw(dst, rect, dirtyRect, style, opacity);
    }

    void BaseGfxEngine::DrawTransform(BufferInfo& dst,
                                      const Rect& mask,
                                      const Point& position,
                                      ColorType color,
                                      OpacityType opacity,
                                      const TransformMap& transMap,
                                      const TransformDataInfo& dataInfo)
    {
        DrawUtils::GetInstance()->DrawTransform(dst, mask, position, color,
                                                opacity, transMap, dataInfo);
    }

    void BaseGfxEngine::ClipCircle(const ImageInfo* info, float x, float y, float radius)
    {
        ClipPath path;
        path.Circle({x, y}, radius);
        ClipImageBlitter blitter(info);
        ClipUtils clip;
        clip.PerformScan(path, blitter);
    }

    void BaseGfxEngine::Blit(BufferInfo& dst,
                             const Point& dstPos,
                             const BufferInfo& src,
                             const Rect& subRect,
                             const BlendOption& blendOption)
    {
        DrawUtils::GetInstance()->BlendWithSoftWare(static_cast<uint8_t*>(src.virAddr), src.rect, src.stride,
                                                    src.rect.GetHeight(), src.mode, src.color, blendOption.opacity, static_cast<uint8_t*>(dst.virAddr),
                                                    dst.stride, dst.mode, subRect.GetX(), subRect.GetY());
    }

    void BaseGfxEngine::Fill(BufferInfo& dst, const Rect& fillArea, const ColorType color, const OpacityType opacity)
    {
        DrawUtils::GetInstance()->FillAreaWithSoftWare(dst, fillArea, color, opacity);
    }

    uint8_t* BaseGfxEngine::AllocBuffer(uint32_t size, uint32_t usage)
    {
        return static_cast<uint8_t*>(malloc(size));
    }

    void BaseGfxEngine::FreeBuffer(uint8_t* buffer)
    {
        free(buffer);
    }

#ifdef ARM_NEON_OPT
    void BaseGfxEngine::BlendLerpPix(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha, uint8_t cover)
    {
        DrawUtils::GetInstance()->BlendLerpPix(pColor, cr, cg, cb, alpha, cover);
    }

    void BaseGfxEngine::BlendLerpPix(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
    {
        DrawUtils::GetInstance()->BlendLerpPix(pColor, cr, cg, cb, alpha);
    }

    void BaseGfxEngine::BlendLerpPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t srcCover)
    {
        DrawUtils::GetInstance()->BlendLerpPix(dstColors, srcColors, srcCover);
    }
    void BaseGfxEngine::BlendLerpPix(uint8_t* dstColors, uint8_t* srcColors, uint8_t* srcCovers)
    {
        DrawUtils::GetInstance()->BlendLerpPix(dstColors, srcColors, srcCovers);
    }

    void BaseGfxEngine::BlendLerpPix(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb,
                                     uint8_t alpha, uint8_t* covers)
    {
        DrawUtils::GetInstance()->BlendLerpPix(pColor, cr, cg, cb, alpha, covers);
    }

    void BaseGfxEngine::BlendPreLerpPix(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb,
                                        uint8_t alpha, uint8_t cover)
    {
        DrawUtils::GetInstance()->BlendPreLerpPix(pColor, cr, cg, cb, alpha, cover);
    }

    void BaseGfxEngine::BlendPreLerpPix(uint8_t* pColor, uint8_t cr, uint8_t cg, uint8_t cb, uint8_t alpha)
    {
        DrawUtils::GetInstance()->BlendPreLerpPix(pColor, cr, cg, cb, alpha);
    }
    void BaseGfxEngine::BlendPreLerpPix(uint8_t *dstColors, uint8_t *srcColors, uint8_t srcCover)
    {
        DrawUtils::GetInstance()->BlendPreLerpPix(dstColors, srcColors, srcCover);
    }

    void BaseGfxEngine::BlendPreLerpPix(uint8_t *dstColors, uint8_t *srcColors, uint8_t *srcCovers)
    {
        DrawUtils::GetInstance()->BlendPreLerpPix(dstColors, srcColors, srcCovers);
    }
    void BaseGfxEngine::BlendPreLerpPix(uint8_t *pColor, uint8_t cr, uint8_t cg, uint8_t cb,
                                        uint8_t alpha, uint8_t *covers)
    {
        DrawUtils::GetInstance()->BlendPreLerpPix(pColor, cr, cg, cb, alpha, covers);
    }
#endif
} // namespace OHOS
