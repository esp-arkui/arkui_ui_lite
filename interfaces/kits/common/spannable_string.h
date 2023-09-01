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
#include "font/ui_font_header.h"
#include "gfx_utils/color.h"
#include "gfx_utils/list.h"
namespace OHOS {
struct FontSizeSpan {
    uint16_t start;
    uint16_t end;
    uint8_t fontSize;
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
    SpannableString();
    ~SpannableString();

    bool GetSpannable(uint16_t index);
    void SetTextStyle(TextStyle textStyle, uint16_t startIndex, uint16_t endIndex);
    bool GetTextStyle(uint16_t index, TextStyle& textStyle);
    void SetFontSize(uint8_t inputFontSize, uint16_t startIndex, uint16_t endIndex);
    bool GetFontSize(uint16_t index, uint8_t& outputSize);
    void SetFontId(uint16_t inputFontId, uint16_t startIndex, uint16_t endIndex);
    bool GetFontId(uint16_t index, uint16_t& outputFontId);
    void SetHeight(int16_t inputHeight, uint16_t startIndex, uint16_t endIndex);
    bool GetHeight(uint16_t index, int16_t& outputHeight);
    bool InitIsSpannable(uint32_t characterSize);
    bool ScanSpannable(uint16_t inputLen);
    bool SetSpannableString(const SpannableString* spannableString);
    void ResetSpannableString();

protected:
    uint16_t isSpannableLen_;

    /* Record each letter having full text setting or not. */
    bool* isSpannable_;
    List<StyleSpan*> spanList_;
    List<FontSizeSpan> sizeList_;
    List<FontIdSpan> fontIdList_;
    List<LetterHeightSpan> heightList_;
    bool SetSpannable(bool value, uint16_t startIndex, uint16_t endIndex);
    
    /* Reset isSpannableLen_, Scan all fulltext setting, write to isSpannable_. */
    void FillWithSpan();
    void FillWithSize();
    void FillWithFontId();
    void FillWithHeight();

    /* merge same value. */
    template <typename SpanType>
    bool EqualInsert(uint16_t& startIndex,
                     uint16_t& endIndex,
                     uint16_t tempStart,
                     uint16_t tempEnd,
                     ListNode<SpanType>* tempSpan,
                     List<SpanType>& tempList)
    {
        bool needAddNode = true;
        if (startIndex <= tempStart) {
            if (endIndex <= tempEnd) {
                endIndex = tempEnd;
                tempSpan = tempList.Remove(tempSpan)->prev_;
            } else {
                tempSpan = tempList.Remove(tempSpan)->prev_;
            }
        } else {
            if (endIndex <= tempEnd) {
                needAddNode = false;
            } else {
                startIndex = tempStart;
                tempSpan = tempList.Remove(tempSpan)->prev_;
            }
        }
        return needAddNode;
    }

    /* merge different value. */
    template <typename SpanType>
    bool UnequalInsert(uint16_t& startIndex,
                       uint16_t& endIndex,
                       uint16_t tempStart,
                       uint16_t tempEnd,
                       ListNode<SpanType>* tempSpan,
                       List<SpanType>& tempList,
                       SpanType tempLeft,
                       SpanType tempRight)
    {
        bool needAddNode = true;
        if (startIndex > tempEnd) {
            // case A, no change
        } else if (startIndex > tempStart && endIndex >= tempEnd) {
            // case B
            tempSpan->data_.end = startIndex;
        } else if (startIndex <= tempStart && endIndex >= tempEnd) {
            // case C
            tempSpan = tempList.Remove(tempSpan)->prev_;
        } else if (startIndex <= tempStart && endIndex < tempEnd) {
            // case D
            tempSpan->data_.start = endIndex;
        } else if (endIndex < tempStart) {
            // case E, no change
        } else if (startIndex > tempStart && endIndex < tempEnd) {
            // case F
            tempList.Insert(tempSpan, tempLeft);
            tempList.Insert(tempSpan, tempRight);
            tempSpan = tempList.Remove(tempSpan)->prev_;
        } else {
            /* it shall not go here */
        }
        return needAddNode;
    }
};
} // namespace OHOS
#endif
