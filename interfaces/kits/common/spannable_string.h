/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

// ---- cjf ----
#include "font/fulltext.h"
// ---- cjf end ----

#ifndef SPANNABLE_STRING_H
#define SPANNABLE_STRING_H
#include "gfx_utils/list.h"
#include "font/ui_font_header.h"

// ---- cjf ----
#if defined(SR_1) && SR_1
#include "gfx_utils/color.h"
#endif
// ---- cjf end ----

namespace OHOS {
#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
// ---- cjf ----
#if defined(SR_1) && SR_1
struct FontSizeSpan {
    uint16_t start;
    uint16_t end;
    uint8_t size;
};
// 整形后的高度，不可由开发者设置
struct LetterHeightSpan {
    uint16_t start;
    uint16_t end;
    int16_t height;
};

struct FontIdSpan {
    uint16_t start;
    uint16_t end;
    uint16_t fontId;
};

struct BackgroundColor : public HeapBase {
    int16_t start;
    int16_t end;
    ColorType backgroundColor;
};

struct ForegroundColor : public HeapBase {
    int16_t start;
    int16_t end;
    ColorType fontColor;
};

struct LineBackgroundColor : public HeapBase {
    int16_t start;
    int16_t end;
    ColorType linebackgroundColor;
};

#endif
// ---- cjf end ----
class StyleSpan : public HeapBase {
public:
    StyleSpan() {};
    ~StyleSpan() {};
    TextStyle textStyle_;
    uint16_t start_;
    uint16_t end_;
    StyleSpan(TextStyle textStyle, uint16_t start, uint16_t end)
    {
        textStyle_ = textStyle;
        start_ = start;
        end_ = end;
    };
};
class SpannableString : public HeapBase {
public:
    SpannableString() {};
    ~SpannableString();
    SpannableString(const char* text);
    char* text_; // cjf end 这一属性原本就有，需要删除
    List<StyleSpan*> spanList_;
    void SetTextStyle(TextStyle textStyle, uint16_t startIndex, uint16_t endIndex);
// ---- cjf ----
#if defined(SR_1) && SR_1
    bool* isSizeSpan_; // 与 text_ = text+1 长度一样，true 富文本
    void InitIsSizeSpan(int characterSize);

    List<FontSizeSpan> sizeList_;
    void SetSize(uint8_t inputSize, uint16_t startIndex, uint16_t endIndex);
    bool GetSize(uint16_t index, uint8_t& outputSize);

    // 如果 fontId 和 size 分离的情况，也就是只有size没有id， 或者只有 id 没有 size，就用返回0，在其他的地方的代码进行处理
    // 按逻辑应该在这里处理，返回默认值
    List<FontIdSpan> fontIdList_;
    void SetFontId(uint16_t inputFontId, uint16_t startIndex, uint16_t endIndex);
    bool GetFontId(uint16_t index,uint16_t& outputFontId);

    List<LetterHeightSpan> heightList_;
    void SetHeight(int16_t inputHeight, uint16_t startIndex, uint16_t endIndex);
    uint16_t GetHeight(uint16_t index);

    //保留text中关于颜色的接口，先不映射到 spannable_string 中
    List<BackgroundColor> backgroundColor_;
    List<ForegroundColor> foregroundColor_;
    List<LineBackgroundColor> linebackgroundColor_;

    //---- 还没去重
    void SetBackgroundColorSpan(ColorType backgroundColor, int16_t start, int16_t end);
    List<BackgroundColor> GetBackgroundColorSpan();
    void SetForegroundColorSpan(ColorType fontColor, int16_t start, int16_t end);
    List<ForegroundColor> GetForegroundColorSpan();
    void SetLineBackgroundSpan(ColorType linebackgroundColor, int16_t start, int16_t end);
    List<LineBackgroundColor> GetLineBackgroundSpan();

#endif
// ---- cjf end ----
};
#endif
}
#endif
