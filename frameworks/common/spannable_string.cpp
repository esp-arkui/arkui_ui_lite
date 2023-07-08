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
    InitIsSizeSpan( textLen );
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

void SpannableString::SetFontId(uint16_t inputFontId, uint16_t startIndex, uint16_t endIndex)
{
    bool needAddNode = true;
    if(sizeList_.IsEmpty())
    {
        FontIdSpan inputSpan = { startIndex, endIndex, inputFontId};
        fontIdList_.PushFront( inputSpan );
        for( int i=startIndex;i< endIndex;i++ )
        {
            isSizeSpan_[i]=true;
        }
        return ;
    }

    ListNode<FontIdSpan>* tempSpan = fontIdList_.Begin();
    for(; tempSpan != fontIdList_.End(); tempSpan = tempSpan->next_)
    {
        needAddNode = true;
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        uint16_t tempSize = tempSpan->data_.fontId;
        // merge same value
        if( inputFontId == tempSize )
        {
            if( startIndex <= tempStart )
            {
                if( endIndex <= tempEnd )
                {
                    endIndex = tempEnd;
                    tempSpan = fontIdList_.Remove(tempSpan)->prev_;
                }
                else
                {
                    tempSpan = fontIdList_.Remove(tempSpan)->prev_;
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
                    tempSpan = fontIdList_.Remove(tempSpan)->prev_;
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
                tempSpan = fontIdList_.Remove(tempSpan)->prev_;
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
                
                FontIdSpan tempLeft = { tempStart, (uint16_t)(startIndex), tempSize};
                FontIdSpan tempRight = {(uint16_t)(endIndex), tempEnd, tempSize};
                fontIdList_.Insert(tempSpan, tempLeft);
                fontIdList_.Insert(tempSpan,tempRight);
                tempSpan = fontIdList_.Remove(tempSpan)->prev_;
            }
            else
            {
                // 应该不会走到这里，走到这里说明前面代码写错了，没有全覆盖。
            }
        }
        if( needAddNode )
        {
            FontIdSpan inputSpan = { startIndex, endIndex, inputFontId};
            fontIdList_.PushFront( inputSpan );
            for( int i=startIndex;i< endIndex;i++ )
            {
                isSizeSpan_[i]=true;
            }
            // 放在末尾会与自身重复校验一次，可以放到最前面
        }
    }
}

bool SpannableString::GetFontId(uint16_t index,uint16_t& outputFontId)
{
    bool hasFind = false;
    ListNode<FontIdSpan>* tempSpan = fontIdList_.Begin();
    for(; tempSpan != fontIdList_.End(); tempSpan = tempSpan->next_)
    {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if( tempStart <= index  && index < tempEnd )
        {
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
    for(; tempSpan != heightList_.End(); tempSpan = tempSpan->next_)
    {
        needAddNode = true;
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        int16_t tempHeight = tempSpan->data_.height;
        // merge same value
        if( inputHeight == tempHeight )
        {
            if( startIndex <= tempStart )
            {
                if( endIndex <= tempEnd )
                {
                    endIndex = tempEnd;
                    tempSpan = heightList_.Remove(tempSpan)->prev_;
                }
                else
                {
                    tempSpan = heightList_.Remove(tempSpan)->prev_;
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
                    tempSpan = heightList_.Remove(tempSpan)->prev_;
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
                tempSpan = heightList_.Remove(tempSpan)->prev_;
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
                
                LetterHeightSpan tempLeft = { tempStart, (uint16_t)(startIndex), tempHeight};
                LetterHeightSpan tempRight = { (uint16_t)(endIndex), tempEnd, tempHeight};
                heightList_.Insert(tempSpan, tempLeft);
                heightList_.Insert(tempSpan,tempRight);
                tempSpan = heightList_.Remove(tempSpan)->prev_;
            }
            else
            {
                // 应该不会走到这里，走到这里说明前面代码写错了，没有全覆盖。
            }
        }
        if( needAddNode )
        {
            LetterHeightSpan inputSpan = { startIndex, endIndex, inputHeight};
            heightList_.PushFront( inputSpan );
            // 放在末尾会与自身重复校验一次，可以放到最前面
        }
    }
}

uint16_t SpannableString::GetHeight(uint16_t index)
{
    uint16_t spannableHeight = 0;
    ListNode<LetterHeightSpan>* tempSpan = heightList_.Begin();
    for(; tempSpan != heightList_.End(); tempSpan = tempSpan->next_)
    {
        uint16_t tempStart = tempSpan->data_.start;
        uint16_t tempEnd = tempSpan->data_.end;
        if( tempStart <= index  && index < tempEnd )
        {
            spannableHeight = tempSpan->data_.height;
            break;
        }
    }
    return spannableHeight;
}


#endif
}
