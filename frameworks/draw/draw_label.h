/*
 * Copyright (c) 2020-2022 Huawei Device Co., Ltd.
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

#ifndef GRAPHIC_LITE_DRAW_LABEL_H
#define GRAPHIC_LITE_DRAW_LABEL_H

#include "common/typed_text.h"
#include "draw/draw_utils.h"
#include "gfx_utils/geometry2d.h"
#include "gfx_utils/heap_base.h"
#include "gfx_utils/style.h"
#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
#include "common/spannable_string.h"
#endif

// ---- cjf ----
#include "font/fulltext.h"
// ---- cjf end ----

namespace OHOS {
class DrawLabel : public HeapBase {
public:
    static uint16_t DrawTextOneLine(BufferInfo& gfxDstBuffer, const LabelLineInfo& labelLine,
                                    uint16_t& letterIndex);

    static void DrawArcText(BufferInfo& gfxDstBuffer, const Rect& mask, const char* text, const Point& arcCenter,
                            uint16_t fontId, uint8_t fontSize, const ArcTextInfo arcTextInfo,
                            TextOrientation orientation, const Style& style, uint8_t opaScale, bool compatibilityMode);

    static bool CalculateAngle(uint16_t letterWidth,
                               uint16_t letterHeight,
                               int16_t letterSpace,
                               const ArcTextInfo arcTextInfo,
                               bool xorFlag,
                               uint32_t index,
                               TextOrientation orientation,
                               float& posX,
                               float& posY,
                               float& rotateAngle,
                               float& angle,
                               const Point& arcCenter,
                               bool compatibilityMode);

    static void DrawLetterWithRotate(BufferInfo& gfxDstBuffer,
                                     const Rect& mask,
                                     uint16_t fontId,
                                     uint8_t fontSize,
                                     uint32_t letter,
                                     const Point& pos,
                                     int16_t rotateAngle,
                                     const ColorType& color,
                                     OpacityType opaScale,
                                     bool compatibilityMode);
// ---- cjf ----
#if defined(SR_1) && SR_1
    static uint8_t GetLineMaxLetterSize(const char* text, uint16_t lineLength, uint16_t fontId, uint8_t fontSize,
                                        uint16_t letterIndex, SpannableString* spannableString);
#else
    static uint8_t GetLineMaxLetterSize(const char* text, uint16_t lineLength, uint16_t fontId, uint8_t fontSize,
                                        uint16_t letterIndex, SizeSpan* sizeSpans);
#endif
// ---- cjf end ----
    
    static void GetLineBackgroundColor(uint16_t letterIndex, List<LineBackgroundColor>* linebackgroundColor,
                                       bool& havelinebackground, ColorType& linebgColor);
    static void GetBackgroundColor(uint16_t letterIndex, List<BackgroundColor>* backgroundColor,
                                   bool& havebackground, ColorType& bgColor);
    static void GetForegroundColor(uint16_t letterIndex, List<ForegroundColor>* foregroundColor, ColorType& fgColor);
    static void DrawLineBackgroundColor(BufferInfo& gfxDstBuffer, uint16_t letterIndex,
                                        const LabelLineInfo& labelLine);
};
} // namespace OHOS
#endif // GRAPHIC_LITE_DRAW_LABEL_H
