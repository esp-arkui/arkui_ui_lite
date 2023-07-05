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
#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
SpannableString::~SpannableString()
{
    if (text_ != nullptr) {
        UIFree(text_);
        text_ = nullptr;
    }
    if(isSizeSpan_!= nullptr) {
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
    if (backgroundColor_.Size() > 0) {
        backgroundColor_.Clear();
    }
    if (linebackgroundColor_.Size() > 0) {
        linebackgroundColor_.Clear();
    }
    if (foregroundColor_.Size() > 0) {
        foregroundColor_.Clear();
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
    InitIsSizeSpan( UIMalloc(textLen * sizeof(bool)) )
}
void SpannableString::SetTextStyle(TextStyle textStyle, uint16_t startIndex, uint16_t endIndex)
{
    StyleSpan* span = new StyleSpan(textStyle, startIndex, endIndex);
    spanList_.PushBack(span);
}

void SpannableString::InitIsSizeSpan(int characterSize)
{
    isSizeSpan_ = static_cast<bool*>(UIMalloc(characterSize * sizeof(bool)));
    if (isSizeSpan_ == nullptr) {
        GRAPHIC_LOGE("Text::SetAbsoluteSizeSpan invalid parameter");
        return;
    }
    for (uint32_t i = 0 ; i < characterSize; i++) {
        isSizeSpan_[i] = false;
    }
}

void SpannableString::SetSize( uint8_t inputSize, uint16_t startIndex, uint16_t endIndex)
{
    bool needAddNode = true;
    if(sizeList_.IsEmpty())
    {
        FontSizeSpan inputSpan = { startIndex, endIndex, inputSize};
        sizeList_.PushFront( inputSpan );
        for( int i=startIndex;i< endIndex;i++ )
        {
            isSizeSpan_[i]=true;
        }
        return ;
    }
    ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
    for(; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_)
    {
        needAddNode = true;
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        uint8_t tempSize = tempSpan->data_.size;
        // merge same value
        if( inputSize == tempSize )
        {
            if( startIndex <= tempStart )
            {
                if( endIndex <= tempEnd )
                {
                    endIndex = tempEnd;
                    tempSpan = sizeList_.Remove(tempSpan)->prev_;
                }
                else
                {
                    tempSpan = sizeList_.Remove(tempSpan)->prev_;
                }
            }
            else
            {
                if( endIndex <= tempEnd )
                {
                    needAddNode = false;
                    break;
                }
                else
                {
                    startIndex = tempStart;
                    tempSpan = sizeList_.Remove(tempSpan)->prev_;
                }
            }
        }
        else
        {
            if( startIndex > tempEnd )
            {
                // case A, no change
            }
            else if( startIndex > tempStart && endIndex >= tempEnd )
            {
                // case B
                tempSpan->data_.end = startIndex;
            }
            else if ( startIndex <= tempStart && endIndex >= tempEnd)
            {
                // case C
                tempSpan = sizeList_.Remove(tempSpan)->prev_;
            }
            else if( startIndex <= tempStart && endIndex < tempEnd )
            {
                // case D
                tempSpan->data_.start = endIndex + 1;
            }
            else if( endIndex < tempStart )
            {
                // case E, no change
            }
            else if( startIndex > tempStart && endIndex < tempEnd)
            {
                // case F
                // 由于插入是插入在节点之前，因此先插入再删除
                
                FontSizeSpan tempLeft = {tempStart, (uint16_t)(startIndex), tempSize} ;
                FontSizeSpan tempRight = {(uint16_t)(endIndex), tempEnd, tempSize};
                sizeList_.Insert(tempSpan, tempLeft);
                sizeList_.Insert(tempSpan,tempRight);
                tempSpan = sizeList_.Remove(tempSpan)->prev_;
            }
            else
            {
                // 应该不会走到这里，走到这里说明前面代码写错了，没有全覆盖。
            }
        }
        if( needAddNode )
        {
            FontSizeSpan inputSpan = { startIndex, endIndex, inputSize};
            sizeList_.PushBack( inputSpan );
            for( int i=startIndex;i< endIndex;i++ )
            {
                isSizeSpan_[i]=true;
            }
            // 放在末尾会与自身重复校验一次，可以放到最前面
        }
    }
}

bool SpannableString::GetSize(uint16_t index, uint8_t& outputSize)
{
    bool hasFind = false;
    ListNode<FontSizeSpan>* tempSpan = sizeList_.Begin();
    for(; tempSpan != sizeList_.End(); tempSpan = tempSpan->next_)
    {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if( tempStart <= index  && index < tempEnd )
        {
            outputSize = tempSpan->data_.size;
            hasFind =  true;
            break;
        }
    }
    return hasFind;
}

void SpannableString::SetBackgroundColorSpan(ColorType inputColor, int16_t startIndex, int16_t endIndex)
{
    BackgroundColor bgcolor;
    bgcolor.start = startIndex;
    bgcolor.end = endIndex;
    bgcolor.backgroundColor= inputColor;

    if(backgroundColor_.IsEmpty())
    {
        BackgroundColor inputSpan;
        inputSpan.start = startIndex;
        inputSpan.end = endIndex;
        inputSpan.backgroundColor = inputColor;
        backgroundColor_.PushFront( inputSpan );
    }
    else
    {
        // 标准模板
        bool needAddNode = true;
        ListNode<BackgroundColor>* tempSpan = backgroundColor_.Begin();
        for(; tempSpan != backgroundColor_.End(); tempSpan = tempSpan->next_)
        {
            needAddNode = true;
            uint16_t tempStart = tempSpan->data_.start;
            uint16_t tempEnd = tempSpan->data_.end;
            ColorType tempAttribute = tempSpan->data_.backgroundColor;
            // merge same value
            if( inputColor.full == tempAttribute.full )
            {
                if( startIndex <= tempStart )
                {
                    if( endIndex <= tempEnd )
                    {
                        endIndex = tempEnd;
                        tempSpan = backgroundColor_.Remove(tempSpan)->prev_;
                    }
                    else
                    {
                        tempSpan = backgroundColor_.Remove(tempSpan)->prev_;
                    }
                }
                else
                {
                    if( endIndex <= tempEnd )
                    {
                        needAddNode = false;
                        break;
                    }
                    else
                    {
                        startIndex = tempStart;
                        tempSpan = backgroundColor_.Remove(tempSpan)->prev_;
                    }
                }
            }
            else
            {
                if( startIndex > tempEnd )
                {
                    // case A, no change
                }
                else if( startIndex > tempStart && endIndex >= tempEnd )
                {
                    // case B
                    tempSpan->data_.end = startIndex;
                }
                else if ( startIndex <= tempStart && endIndex >= tempEnd)
                {
                    // case C
                    tempSpan = backgroundColor_.Remove(tempSpan)->prev_;
                }
                else if( startIndex <= tempStart && endIndex < tempEnd )
                {
                    // case D
                    tempSpan->data_.start = endIndex + 1;
                }
                else if( endIndex < tempStart )
                {
                    // case E, no change
                }
                else if( startIndex > tempStart && endIndex < tempEnd)
                {
                    // case F, delete then insert
                    BackgroundColor tempLeft ;
                    tempLeft.start = tempStart;
                    tempLeft.end = (uint16_t)(startIndex);
                    tempLeft.backgroundColor = tempAttribute;
                    BackgroundColor tempRight;
                    tempRight.start = (uint16_t)(endIndex);
                    tempRight.end = tempEnd;
                    tempRight.backgroundColor =  tempAttribute;
                    backgroundColor_.Insert(tempSpan, tempLeft);
                    backgroundColor_.Insert(tempSpan,tempRight);
                    tempSpan = backgroundColor_.Remove(tempSpan)->prev_;
                }
                else
                {
                    // error
                }
            }
            if( needAddNode )
            {
                BackgroundColor inputSpan;
                inputSpan.start = startIndex;
                inputSpan.end = endIndex;
                inputSpan.backgroundColor = inputColor;
                backgroundColor_.PushFront( inputSpan );
                // 放在末尾会与自身重复校验一次，可以放到最前面
            }
        }
    }
}

List<BackgroundColor> SpannableString::GetBackgroundColorSpan()
{
    return backgroundColor_;
}

#endif
}
