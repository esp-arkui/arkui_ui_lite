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

#ifndef SPANNABLE_STRING_H
#define SPANNABLE_STRING_H
#include "gfx_utils/list.h"
#include "font/ui_font_header.h"
#include "gfx_utils/color.h"
namespace OHOS {
//#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
struct FontSizeSpan {
    uint16_t start;
    uint16_t end;
    uint8_t size;
};

/**
 * @brief Stores the attribute information about letter height for full text.
 */
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
    char* text_;
    List<StyleSpan*> spanList_;
    void SetTextStyle(TextStyle textStyle, uint16_t startIndex, uint16_t endIndex);
    
    /** Record each letter having full text setting or not. */
    bool* isSizeSpan_;
    void InitIsSizeSpan(int characterSize);

    List<FontSizeSpan> sizeList_;
    void SetSize(uint8_t inputSize, uint16_t startIndex, uint16_t endIndex);
    bool GetSize(uint16_t index, uint8_t& outputSize);

    List<FontIdSpan> fontIdList_;
    void SetFontId(uint16_t inputFontId, uint16_t startIndex, uint16_t endIndex);
    bool GetFontId(uint16_t index,uint16_t& outputFontId);
    
    List<LetterHeightSpan> heightList_;
    void SetHeight(int16_t inputHeight, uint16_t startIndex, uint16_t endIndex);
    uint16_t GetHeight(uint16_t index);
};
//#endif
}
#endif
