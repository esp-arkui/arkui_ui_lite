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

#include "draw/draw_label.h"
#include "common/typed_text.h"
#include "draw/draw_utils.h"
#include "engines/gfx/gfx_engine_manager.h"
#include "font/ui_font.h"
#include "font/ui_font_header.h"
#include "gfx_utils/graphic_log.h"

namespace OHOS {
uint16_t DrawLabel::DrawTextOneLine(BufferInfo& gfxDstBuffer, const LabelLineInfo& labelLine, uint16_t& letterIndex)
{
    if (labelLine.text == nullptr) {
        return 0;
    }
    UIFont* fontEngine = UIFont::GetInstance();
    if (labelLine.direct == TEXT_DIRECT_RTL) {
        labelLine.pos.x -= labelLine.offset.x;
    } else {
        labelLine.pos.x += labelLine.offset.x;
    }
    uint32_t i = 0;
    uint16_t retOffsetY = 0; // ret value elipse offsetY
    uint16_t offsetPosY = 0;
    uint8_t maxLetterSize = GetLineMaxLetterSize(labelLine.text, labelLine.lineLength, labelLine.fontId,
                                                 labelLine.fontSize, letterIndex, labelLine.spannableString);
    DrawLineBackgroundColor(gfxDstBuffer, letterIndex, labelLine);
    GlyphNode glyphNode;
    while (i < labelLine.lineLength) {
        uint32_t letter = TypedText::GetUTF8Next(labelLine.text, i, i);
        uint16_t fontId = labelLine.fontId;
        uint8_t fontSize = labelLine.fontSize;
        if (labelLine.spannableString != nullptr && labelLine.spannableString->GetIsSizeSpan(letterIndex)) {
            labelLine.spannableString->GetFontId(letterIndex, fontId);
            labelLine.spannableString->GetFontSize(letterIndex, fontSize);
        }
        bool havebackgroundColor = false;
        ColorType backgroundColor;
        GetBackgroundColor(letterIndex, labelLine.backgroundColor, havebackgroundColor, backgroundColor);
        ColorType foregroundColor = labelLine.style.textColor_;
        GetForegroundColor(letterIndex, labelLine.foregroundColor, foregroundColor);
        TextStyle textStyle = TEXT_STYLE_NORMAL;
        if (labelLine.textStyles) {
            textStyle = labelLine.textStyles[letterIndex];
        }
        LabelLetterInfo letterInfo{labelLine.pos,
                                   labelLine.mask,
                                   foregroundColor,
                                   labelLine.opaScale,
                                   0,
                                   0,
                                   letter,
                                   labelLine.direct,
                                   fontId,
                                   0,
                                   fontSize,
                                   textStyle,
                                   labelLine.baseLine,
                                   labelLine.style.letterSpace_,
                                   labelLine.style.lineSpace_,
                                   havebackgroundColor,
                                   backgroundColor};
        glyphNode.textStyle = letterInfo.textStyle;
        glyphNode.advance = 0;
        uint8_t* fontMap = fontEngine->GetBitmap(letterInfo.letter, glyphNode, letterInfo.fontId, letterInfo.fontSize,
                                                 letterInfo.shapingId);
        if (fontMap != nullptr) {
            uint8_t weight = fontEngine->GetFontWeight(glyphNode.fontId);
            // 16: rgb565->16 rgba8888->32 font with rgba
            if (weight >= 16) {
                DrawUtils::GetInstance()->DrawColorLetter(gfxDstBuffer, letterInfo, fontMap, glyphNode, maxLetterSize);
            } else {
                letterInfo.offsetY = labelLine.ellipsisOssetY == 0 ? offsetPosY : labelLine.ellipsisOssetY;
                retOffsetY = offsetPosY;
                DrawUtils::GetInstance()->DrawNormalLetter(gfxDstBuffer, letterInfo, fontMap, glyphNode, maxLetterSize);
            }
        }
        if (labelLine.direct == TEXT_DIRECT_RTL) {
            labelLine.pos.x -= (glyphNode.advance + labelLine.style.letterSpace_);
        } else {
            labelLine.pos.x += (glyphNode.advance + labelLine.style.letterSpace_);
        }
        letterIndex++;
    }
    return retOffsetY;
}

uint8_t DrawLabel::GetLineMaxLetterSize(const char* text, uint16_t lineLength, uint16_t fontId, uint8_t fontSize,
                                        uint16_t letterIndex, SpannableString* spannableString)
{
    if (spannableString == nullptr) {
        return fontSize;
    }
    uint32_t i = 0;
    uint8_t maxLetterSize = fontSize;
    while (i < lineLength) {
        uint32_t unicode = TypedText::GetUTF8Next(text, i, i);
        if (TypedText::IsColourWord(unicode, fontId, fontSize)) {
            letterIndex++;
            continue;
        }
        if (spannableString != nullptr && spannableString->GetIsSizeSpan(letterIndex)) {
            uint8_t tempSize = fontSize;
            spannableString->GetFontSize(letterIndex, tempSize);
            if (tempSize > maxLetterSize) {
                maxLetterSize = tempSize;
            }
        }
        letterIndex++;
    }
    return maxLetterSize;
}

void DrawLabel::DrawArcText(BufferInfo& gfxDstBuffer,
                            const Rect& mask,
                            const char* text,
                            const Point& arcCenter,
                            uint16_t fontId,
                            uint8_t fontSize,
                            const ArcTextInfo arcTextInfo,
                            TextOrientation orientation,
                            const Style& style,
                            OpacityType opaScale,
                            bool compatibilityMode)
{
    if ((text == nullptr) || (arcTextInfo.lineStart == arcTextInfo.lineEnd) || (arcTextInfo.radius == 0)) {
        GRAPHIC_LOGE("DrawLabel::DrawArcText invalid parameter\n");
        return;
    }
    OpacityType opa = DrawUtils::GetMixOpacity(opaScale, style.textOpa_);
    if (opa == OPA_TRANSPARENT) {
        return;
    }
    uint16_t letterWidth;
    UIFont* fontEngine = UIFont::GetInstance();

    uint16_t letterHeight = fontEngine->GetHeight(fontId, fontSize);
    uint32_t i = arcTextInfo.lineStart;
    float angle = arcTextInfo.startAngle;
    float posX;
    float posY;
    float rotateAngle;

    bool orientationFlag = (orientation == TextOrientation::INSIDE);
    bool directFlag = (arcTextInfo.direct == TEXT_DIRECT_LTR);
    bool xorFlag = !((orientationFlag && directFlag) || (!orientationFlag && !directFlag));

    while (i < arcTextInfo.lineEnd) {
        uint32_t tmp = i;
        uint32_t letter = TypedText::GetUTF8Next(text, tmp, i);
        if (letter == 0) {
            continue;
        }
        if ((letter == '\r') || (letter == '\n')) {
            break;
        }
        letterWidth = fontEngine->GetWidth(letter, fontId, fontSize, 0);
        if (!DrawLabel::CalculateAngle(letterWidth, letterHeight, style.letterSpace_,
                                       arcTextInfo, xorFlag, tmp, orientation,
                                       posX, posY, rotateAngle, angle,
                                       arcCenter, compatibilityMode)) {
            continue;
        }

        DrawLetterWithRotate(gfxDstBuffer, mask, fontId, fontSize, letter, Point { MATH_ROUND(posX), MATH_ROUND(posY) },
                             static_cast<int16_t>(rotateAngle), style.textColor_, opaScale, compatibilityMode);
    }
}

bool DrawLabel::CalculateAngle(uint16_t letterWidth,
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
                               bool compatibilityMode)
{
    const int DIVIDER_BY_TWO = 2;
    if (compatibilityMode) {
        if ((index == arcTextInfo.lineStart) && xorFlag) {
            angle += TypedText::GetAngleForArcLen(static_cast<float>(letterWidth), letterHeight, arcTextInfo.radius,
                                                  arcTextInfo.direct, orientation);
        }
        uint16_t arcLen = letterWidth + letterSpace;
        if (arcLen == 0) {
            return false;
        }
        float incrementAngle = TypedText::GetAngleForArcLen(static_cast<float>(arcLen), letterHeight,
                                                            arcTextInfo.radius, arcTextInfo.direct, orientation);

        rotateAngle = (orientation == TextOrientation::INSIDE) ? angle : (angle - SEMICIRCLE_IN_DEGREE);

        // 2: half
        float fineTuningAngle = incrementAngle * (static_cast<float>(letterWidth) / (DIVIDER_BY_TWO * arcLen));
        rotateAngle += (xorFlag ? -fineTuningAngle : fineTuningAngle);
        TypedText::GetArcLetterPos(arcCenter, arcTextInfo.radius, angle, posX, posY);
        angle += incrementAngle;
    } else {
        float incrementAngle = TypedText::GetAngleForArcLen(letterWidth, letterSpace, arcTextInfo.radius);
        if (incrementAngle >= -EPSINON && incrementAngle <= EPSINON) {
            return false;
        }

        float fineTuningAngle = incrementAngle / DIVIDER_BY_TWO;
        rotateAngle = xorFlag ?  (angle - SEMICIRCLE_IN_DEGREE - fineTuningAngle) : (angle + fineTuningAngle);
        TypedText::GetArcLetterPos(arcCenter, arcTextInfo.radius, angle, posX, posY);
        angle = xorFlag ? (angle - incrementAngle) : (angle + incrementAngle);
    }

    return true;
}

void DrawLabel::DrawLetterWithRotate(BufferInfo& gfxDstBuffer,
                                     const Rect& mask,
                                     uint16_t fontId,
                                     uint8_t fontSize,
                                     uint32_t letter,
                                     const Point& pos,
                                     int16_t rotateAngle,
                                     const ColorType& color,
                                     OpacityType opaScale,
                                     bool compatibilityMode)
{
    UIFont* fontEngine = UIFont::GetInstance();
    FontHeader head;
    GlyphNode node;
    node.textStyle = TEXT_STYLE_NORMAL;
    if (fontEngine->GetFontHeader(head, fontId, fontSize) != 0) {
        return;
    }
    const uint8_t* fontMap = fontEngine->GetBitmap(letter, node, fontId, fontSize, 0);
    if (fontMap == nullptr) {
        return;
    }
    uint8_t fontWeight = fontEngine->GetFontWeight(fontId);
    ColorMode colorMode = fontEngine->GetColorType(fontId);
    int16_t offset = compatibilityMode ? head.ascender : 0;
    Rect rectLetter;
    rectLetter.SetPosition(pos.x + node.left, pos.y + offset - node.top);
    rectLetter.Resize(node.cols, node.rows);
    TransformMap transMap(rectLetter);
    transMap.Rotate(rotateAngle, Vector2<float>(-node.left, node.top - offset));
    TransformDataInfo letterTranDataInfo = {ImageHeader{colorMode, 0, 0, 0, node.cols, node.rows}, fontMap, fontWeight,
                                            BlurLevel::LEVEL0, TransformAlgorithm::BILINEAR};
    BaseGfxEngine::GetInstance()->DrawTransform(gfxDstBuffer, mask, Point { 0, 0 }, color, opaScale, transMap,
                                                letterTranDataInfo);
}

void DrawLabel::GetLineBackgroundColor(uint16_t letterIndex, List<LineBackgroundColor>* linebackgroundColor,
                                       bool& havelinebackground, ColorType& linebgColor)
{
    if (linebackgroundColor->Size() > 0) {
        ListNode<LineBackgroundColor>* lbColor = linebackgroundColor->Begin();
        for (; lbColor != linebackgroundColor->End(); lbColor = lbColor->next_) {
            uint32_t start = lbColor->data_.start;
            uint32_t end = lbColor->data_.end;
            if (letterIndex >= start && letterIndex <= end) {
                havelinebackground = true;
                linebgColor = lbColor->data_.linebackgroundColor ;
            }
        }
    }
};

void DrawLabel::GetBackgroundColor(uint16_t letterIndex, List<BackgroundColor>* backgroundColor,
                                   bool& havebackground, ColorType& bgColor)
{
    if (backgroundColor->Size() > 0) {
        ListNode<BackgroundColor>* bColor = backgroundColor->Begin();
        for (; bColor != backgroundColor->End(); bColor = bColor->next_) {
            uint16_t start = bColor->data_.start;
            uint16_t end = bColor->data_.end;
            if (letterIndex >= start && letterIndex <= end) {
                havebackground = true;
                bgColor = bColor->data_.backgroundColor ;
            }
        }
    }
};

void DrawLabel::GetForegroundColor(uint16_t letterIndex, List<ForegroundColor>* foregroundColor, ColorType& fgColor)
{
    if (foregroundColor->Size() > 0) {
        ListNode<ForegroundColor>* fColor = foregroundColor->Begin();
        for (; fColor != foregroundColor->End(); fColor = fColor->next_) {
            uint32_t start = fColor->data_.start;
            uint32_t end = fColor->data_.end;
            if (letterIndex >= start && letterIndex <= end) {
                fgColor = fColor->data_.fontColor;
            }
        }
    }
};

void DrawLabel::DrawLineBackgroundColor(BufferInfo& gfxDstBuffer, uint16_t letterIndex, const LabelLineInfo& labelLine)
{
    uint32_t i = 0;
    while (i < labelLine.lineLength) {
        TypedText::GetUTF8Next(labelLine.text, i, i);
        bool havelinebackground = false;
        ColorType linebackgroundColor;
        GetLineBackgroundColor(letterIndex, labelLine.linebackgroundColor, havelinebackground, linebackgroundColor);
        if (havelinebackground) {
                Style style;
                style.bgColor_ = linebackgroundColor;
                Rect linebackground(labelLine.mask.GetLeft(), labelLine.pos.y,
                                    labelLine.mask.GetRight(), labelLine.pos.y + labelLine.lineHeight);
                BaseGfxEngine::GetInstance()->DrawRect(gfxDstBuffer, labelLine.mask,
                                                       linebackground, style, linebackgroundColor.alpha);
        }
        letterIndex++;
    }
};
} // namespace OHOS
