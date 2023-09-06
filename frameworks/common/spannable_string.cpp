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
SpannableString::SpannableString() : isSpannableLen_(0), isSpannable_(nullptr) {}

SpannableString::~SpannableString()
{
    if (isSpannable_ != nullptr) {
        UIFree(isSpannable_);
        isSpannable_ = nullptr;
    }
    if (spanList_.Size() > 0) {
        spanList_.Clear();
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

void SpannableString::SetTextStyle(TextStyle inputTextStyle, uint16_t startIndex, uint16_t endIndex)
{
    StyleSpan* style = new StyleSpan(inputTextStyle, startIndex, endIndex);
    spanList_.PushBack(style);
    SetSpannable(true, startIndex, endIndex);
}
bool SpannableString::GetTextStyle(uint16_t index, TextStyle& textStyle)
{
    bool hasFind = false;
    ListNode<StyleSpan*>* tempSpan = spanList_.Begin();
    for (; tempSpan != spanList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_->start_;
        uint16_t tempEnd = tempSpan->data_->end_;
        if ((tempStart <= index) && (index < tempEnd)) {
            textStyle = tempSpan->data_->textStyle_;
            hasFind = true;
            break;
        }
    }
    return hasFind;
}

void SpannableString::ResetSpannableString()
{
    if (isSpannable_ != nullptr) {
        UIFree(isSpannable_);
    }
    isSpannable_ = nullptr;
    isSpannableLen_ = 0;
    if (spanList_.Size() > 0) {
        spanList_.Clear();
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

bool SpannableString::SetSpannableString(const SpannableString* input)
{
    bool hasSet = true;
    ResetSpannableString();
    bool hasInit = InitIsSpannable(input->isSpannableLen_);
    hasSet = hasSet & hasInit;
    if (hasInit) {
        for (uint32_t i = 0; i < isSpannableLen_; i++) {
            isSpannable_[i] = input->isSpannable_[i];
        }
        ListNode<FontSizeSpan>* node = input->sizeList_.Begin();
        while (node != input->sizeList_.End()) {
            SetFontSize(node->data_.fontSize, node->data_.start, node->data_.end);
            node = node->next_;
        }
        ListNode<FontIdSpan>* node_id = input->fontIdList_.Begin();
        while (node_id != input->fontIdList_.End()) {
            SetFontId(node_id->data_.fontId, node_id->data_.start, node_id->data_.end);
            node_id = node_id->next_;
        }
        ListNode<LetterHeightSpan>* node_height = input->heightList_.Begin();
        while (node_height != input->heightList_.End()) {
            SetHeight(node_height->data_.height, node_height->data_.start, node_height->data_.end);
            node_height = node_height->next_;
        }
        ListNode<StyleSpan*>* node_span = input->spanList_.Begin();
        while (node_span != input->spanList_.End()) {
            SetTextStyle(node_span->data_->textStyle_, node_span->data_->start_, node_span->data_->end_);
            node_span = node_span->next_;
        }
    }
    return hasSet;
}

void SpannableString::FillWithSpan()
{
    ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
    for (; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart < isSpannableLen_) {
            tempEnd = (tempEnd < isSpannableLen_) ? tempEnd : isSpannableLen_;
            for (uint16_t i = tempStart; i < tempEnd; i++) {
                isSpannable_[i] = true;
            }
        }
    }
}

void SpannableString::FillWithSize()
{
    ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
    for (; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart < isSpannableLen_) {
            tempEnd = (tempEnd < isSpannableLen_) ? tempEnd : isSpannableLen_;
            for (uint16_t i = tempStart; i < tempEnd; i++) {
                isSpannable_[i] = true;
            }
        }
    }
}

void SpannableString::FillWithFontId()
{
    ListNode<FontIdSpan>* tempSpan = fontIdList_.Begin();
    for (; tempSpan != fontIdList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart < isSpannableLen_) {
            tempEnd = (tempEnd < isSpannableLen_) ? tempEnd : isSpannableLen_;
            for (uint16_t i = tempStart; i < tempEnd; i++) {
                isSpannable_[i] = true;
            }
        }
    }
}

void SpannableString::FillWithHeight()
{
    ListNode<LetterHeightSpan>* tempSpan = heightList_.Begin();
    for (; tempSpan != heightList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if (tempStart < isSpannableLen_) {
            tempEnd = (tempEnd < isSpannableLen_) ? tempEnd : isSpannableLen_;
            for (uint16_t i = tempStart; i < tempEnd; i++) {
                isSpannable_[i] = true;
            }
        }
    }
}

bool SpannableString::InitIsSpannable(uint32_t characterSize)
{
    if (isSpannable_ != nullptr) {
        UIFree(isSpannable_);
        isSpannable_ = nullptr;
    }
    isSpannable_ = nullptr;
    isSpannable_ = static_cast<bool*>(UIMalloc(characterSize * sizeof(bool)));
    if (isSpannable_ == nullptr) {
        GRAPHIC_LOGE("SpannableString::InitSpannable() isSpannable_ == nullptr");
        return false;
    }
    isSpannableLen_ = characterSize;
    for (uint32_t i = 0; i < characterSize; i++) {
        isSpannable_[i] = false;
    }
    return true;
}

bool SpannableString::SetSpannable(bool value, uint16_t startIndex, uint16_t endIndex)
{
    bool isSuccess = false;
    if (isSpannable_ != nullptr) {
        for (int16_t i = startIndex; ((i >= 0) && (i < endIndex) && (i < isSpannableLen_)); i++) {
            isSpannable_[i] = value;
        }
        isSuccess = true;
    }
    return isSuccess;
}

bool SpannableString::GetSpannable(uint16_t index)
{
    bool result = false;
    if ((isSpannable_ != nullptr) && (index >= 0) && (index < isSpannableLen_)) {
        result = isSpannable_[index];
    }
    return result;
}

/* Reset isSpannableLen_, Scan all fulltext setting, write to isSpannable_. */
bool SpannableString::ScanSpannable(uint16_t inputLen)
{
    bool result = true;
    bool hasInit = InitIsSpannable(inputLen);
    FillWithSpan();
    FillWithSize();
    FillWithFontId();
    FillWithHeight();
    result = result & hasInit;
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
        SetSpannable(true, startIndex, endIndex);
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
                SetSpannable(true, startIndex, endIndex);
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
        if ((tempStart <= index) && (index < tempEnd)) {
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
    if (fontIdList_.IsEmpty()) {
        FontIdSpan inputSpan = {startIndex, endIndex, inputFontId};
        fontIdList_.PushFront(inputSpan);
        SetSpannable(true, startIndex, endIndex);
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
            SetSpannable(true, startIndex, endIndex);
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
        if ((tempStart <= index) && (index < tempEnd)) {
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
            SetSpannable(true, startIndex, endIndex);
        }
    }
}

bool SpannableString::GetHeight(uint16_t index, int16_t& outputHeight)
{
    bool hasFind = false;
    ListNode<LetterHeightSpan>* tempSpan = heightList_.Begin();
    for (; tempSpan != heightList_.End(); tempSpan = tempSpan->next_) {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if ((tempStart <= index) && (index < tempEnd)) {
            hasFind = true;
            outputHeight = tempSpan->data_.height;
            break;
        }
    }
    return hasFind;
}
} // namespace OHOS
