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

#include "common/spannable_string.h"
#include "gfx_utils/graphic_log.h"
#include "securec.h"
namespace OHOS {
SpannableString::~SpannableString()
{
    if (text_ != nullptr) {
        UIFree(text_);
        text_ = nullptr;
    }
    if (isSizeSpan_ != nullptr) {
        UIFree(isSizeSpan_);
        isSizeSpan_ = nullptr;
    }
    if (sizeList_.Size() > 0) {
        sizeList_.Clear();
    }
    if (fontIdList_.Size() > 0) {
        fontIdList_.Clear();
    }
    if (heightList_.Size() > 0) {
        heightList_.Clear();
    }
}
SpannableString::SpannableString(const char* text)
{
    if (text == nullptr) {
        return;
    }
    uint32_t textLen = static_cast<uint32_t>(strlen(text));
    if (textLen > MAX_TEXT_LENGTH) {
        return;
    }
    text_ = static_cast<char*>(UIMalloc(++textLen));
    if (text_ == nullptr) {
        return;
    }
    if (memcpy_s(text_, textLen, text, textLen) != EOK) {
        UIFree(text_);
        text_ = nullptr;
        return;
    }
    InitIsSizeSpan((int16_t)textLen);
}
void SpannableString::SetTextStyle(TextStyle textStyle, uint16_t startIndex, uint16_t endIndex)
{
    StyleSpan* span = new StyleSpan(textStyle, startIndex, endIndex);
    spanList_.PushBack(span);
}

void SpannableString::InitSpannableString()
{
    text_ = nullptr;
    isSizeSpan_ = nullptr;
    isSizeSpanLen_ = 0;
}

void SpannableString::SetText(const char* inputText)
{
    if (inputText == nullptr) {
        return;
    }
    uint32_t textLen = static_cast<uint32_t>(strlen(inputText));
    if (textLen > MAX_TEXT_LENGTH) {
        textLen = MAX_TEXT_LENGTH;
    }
    if (text_ != nullptr) {
        if (strcmp(inputText, text_) == 0) {
            return;
        }
        UIFree(text_);
        text_ = nullptr;
    }
    text_ = static_cast<char*>(UIMalloc(textLen + 1));
    if (text_ == nullptr) {
        return;
    }
    if (strncpy_s(text_, textLen + 1, inputText, textLen) != EOK) {
        UIFree(text_);
        text_ = nullptr;
        return;
    }
}

void SpannableString::InitIsSizeSpan(int16_t characterSize)
{
    isSizeSpan_ = static_cast<bool*>(UIMalloc(characterSize * sizeof(bool)));
    if (isSizeSpan_ == nullptr) {
        GRAPHIC_LOGE("SpannableString::InitIsSizeSpan() isSizeSpans_ == nullptr");
        return;
    }
    isSizeSpanLen_ = characterSize;
    for (uint32_t i = 0; i < characterSize; i++) {
        isSizeSpan_[i] = false;
    }
}

bool SpannableString::SetIsSizeSpan(bool value, uint16_t startIndex, uint16_t endIndex)
{
    bool isSuccess = false;
    if (isSizeSpan_ != nullptr) {
        for (int16_t i = startIndex; i < endIndex; i++) {
            isSizeSpan_[i] = value;
        }
        isSuccess = true;
    }
    return isSuccess;
}

bool SpannableString::GetIsSizeSpan(uint16_t index)
{
    bool result = false;
    if (isSizeSpan_ != nullptr && index >= 0 && index < isSizeSpanLen_) {
        result = isSizeSpan_[index];
    }
    return result;
}

/*
 * this function merge samge value when add node
 */
void SpannableString::SetFontSize(uint8_t inputFontSize, uint16_t startIndex, uint16_t endIndex)
{
    bool needAddNode = true;
    if (sizeList_.IsEmpty()) {
        FontSizeSpan inputSpan = {startIndex, endIndex, inputFontSize};
        sizeList_.PushFront(inputSpan);
        SetIsSizeSpan(true, startIndex, endIndex);
        return;
    } else {
        ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
        for (; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_) {
            needAddNode = true;
            uint16_t tempStart = tempSpan->data_.start;
            uint16_t tempEnd = tempSpan->data_.end;
            uint8_t tempSize = tempSpan->data_.fontSize;
            if (inputFontSize == tempSize) {
                needAddNode = EqualInsert<FontSizeSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan, sizeList_);
            } else {
                FontSizeSpan tempLeft = {tempStart, startIndex, tempSize};
                FontSizeSpan tempRight = {endIndex, tempEnd, tempSize};
                needAddNode = UnequalInsert<FontSizeSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan, sizeList_,
                                                          tempLeft, tempRight);
            }
            if (needAddNode) {
                FontSizeSpan inputSpan = {startIndex, endIndex, inputFontSize};
                sizeList_.PushBack(inputSpan);
                SetIsSizeSpan(true, startIndex, endIndex);
            }
        }
    }
}

bool SpannableString::GetFontSize(uint16_t index, uint8_t& outputSize)
{
    bool hasFind = false;
    ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
    for (; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart <= index && index < tempEnd) {
            outputSize = tempSpan->data_.fontSize;
            hasFind = true;
            break;
        }
    }
    return hasFind;
}

void SpannableString::SetFontId(uint16_t inputFontId, uint16_t startIndex, uint16_t endIndex)
{
    bool needAddNode = true;
    if (sizeList_.IsEmpty()) {
        FontIdSpan inputSpan = {startIndex, endIndex, inputFontId};
        fontIdList_.PushFront(inputSpan);
        SetIsSizeSpan(true, startIndex, endIndex);
        return;
    }
    ListNode<FontIdSpan>* tempSpan = fontIdList_.Begin();
    for (; tempSpan != fontIdList_.End(); tempSpan = tempSpan->next_) {
        needAddNode = true;
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        uint16_t tempId = tempSpan->data_.fontId;
        if (inputFontId == tempId) {
            needAddNode = EqualInsert<FontIdSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan, fontIdList_);
        } else {
            FontIdSpan tempLeft = {tempStart, startIndex, tempId};
            FontIdSpan tempRight = {endIndex, tempEnd, tempId};
            needAddNode = UnequalInsert<FontIdSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan, fontIdList_,
                                                    tempLeft, tempRight);
        }
        if (needAddNode) {
            FontIdSpan inputSpan = {startIndex, endIndex, inputFontId};
            fontIdList_.PushBack(inputSpan);
            SetIsSizeSpan(true, startIndex, endIndex);
        }
    }
}

bool SpannableString::GetFontId(uint16_t index, uint16_t& outputFontId)
{
    bool hasFind = false;
    ListNode<FontIdSpan>* tempSpan = fontIdList_.Begin();
    for (; tempSpan != fontIdList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart <= index && index < tempEnd) {
            outputFontId = tempSpan->data_.fontId;
            hasFind = true;
            break;
        }
    }
    return hasFind;
}

void SpannableString::SetHeight(int16_t inputHeight, uint16_t startIndex, uint16_t endIndex)
{
    bool needAddNode = true;
    ListNode<LetterHeightSpan>* tempSpan = heightList_.Begin();
    for (; tempSpan != heightList_.End(); tempSpan = tempSpan->next_) {
        needAddNode = true;
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        int16_t tempHeight = tempSpan->data_.height;
        if (inputHeight == tempHeight) {
            needAddNode =
                EqualInsert<LetterHeightSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan, heightList_);
        } else {
            LetterHeightSpan tempLeft = {tempStart, startIndex, tempHeight};
            LetterHeightSpan tempRight = {endIndex, tempEnd, tempHeight};
            needAddNode = UnequalInsert<LetterHeightSpan>(startIndex, endIndex, tempStart, tempEnd, tempSpan,
                                                          heightList_, tempLeft, tempRight);
        }
        if (needAddNode) {
            LetterHeightSpan inputSpan = {startIndex, endIndex, inputHeight};
            heightList_.PushBack(inputSpan);
            SetIsSizeSpan(true, startIndex, endIndex);
        }
    }
}

bool SpannableString::GetHeight(uint16_t index, uint16_t& outputHeight)
{
    bool hasFind = false;
    ListNode<LetterHeightSpan>* tempSpan = heightList_.Begin();
    for (; tempSpan != heightList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart <= index && index < tempEnd) {
            hasFind = true;
            outputHeight = tempSpan->data_.height;
            break;
        }
    }
    return hasFind;
}
} // namespace OHOS
