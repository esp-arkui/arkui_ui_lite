/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#include "gfx_utils/list.h"
#include "font/ui_font_header.h"
#include "gfx_utils/color.h"

#include <climits>
#include <gtest/gtest.h>
#define ENABLE_SPANNABLE_STRING 1

using namespace testing::ext;
namespace OHOS {

class SpannableStringTest : public testing::Test {
public:
    SpannableStringTest() : spannableString_(nullptr) {}
    virtual ~SpannableStringTest() {}
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp();
    void TearDown();
    SpannableString* spannableString_;
};

void SpannableStringTest::SetUp()
{
    if (spannableString_ == nullptr) {
        spannableString_ = new SpannableString();
    }
}

void SpannableStringTest::TearDown()
{
    if (spannableString_ != nullptr) {
        delete spannableString_;
        spannableString_ = nullptr;
    }
}

/**
 * @tc.name: SpannableStringIsSizeSpan_001
 * @tc.desc: Verify InitIsSizeSpan function, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringIsSizeSpan_001, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    int characterSize = 10;
    spannableString_->InitIsSizeSpan(characterSize);
    if(sizeof(spannableString_->isSizeSpan_)!=10)
    {
        EXPECT_EQ(characterSize, sizeof(spannableString_->isSizeSpan_));
        return ;
    }
    bool defaultValue = false;
    for(int i=0;i<characterSize;i++)
    {
        EXPECT_EQ(defaultValue, spannableString_->isSizeSpan_[i]); 
    }
}

/**
 * @tc.name: SpannableStringGetSize_001
 * @tc.desc: Verify GetSize function, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringGetSize_001, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t startIndex = 0;
    uint16_t endIndex = 0;
    bool wishHasFind = false;
    spannableString_->SetSize(inputSize,startIndex,endIndex);
    uint8_t outputSize = 0;
    bool wishHasFind = true;
    bool currentHasFind = spannableString_->GetSize(i,outputSize);
    EXPECT_EQ(wishHasFind, currentHasFind);
    EXPECT_EQ(outputSize, inputSize);
}

/**
 * @tc.name: SpannableStringSetSize_001
 * @tc.desc: Verify SetSize function, ,case start < end,equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_001, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t startIndex = 0;
    uint16_t endIndex = 2;
    spannableString_->SetSize(inputSize,startIndex,endIndex);
    for(int i=startIndex;i<endIndex;i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i,outputSize);
        EXPECT_EQ(outputSize, inputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
    }
}

/**
 * @tc.name: SpannableStringSetSize_002
 * @tc.desc: Verify SetSize function,case start == end, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_002, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t startIndex = 0;
    uint16_t endIndex = 0;
    spannableString_->SetSize(inputSize,startIndex,endIndex);
    for(int i=startIndex;i<endIndex;i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i,outputSize);
        EXPECT_EQ(outputSize, inputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
    }
}

/**
 * @tc.name: SpannableStringSetSize_003
 * @tc.desc: Verify SetSize function,case start > end, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_003, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t startIndex = 2;
    uint16_t endIndex = 0;
    bool wishHasFind = false;
    spannableString_->SetSize(inputSize,startIndex,endIndex);
    for(int i=endIndex;i<startIndex;i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = false;
        bool currentHasFind = spannableString_->GetSize(i,outputSize);
        EXPECT_NE(outputSize, inputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
    }
}

/**
 * @tc.name: SpannableStringSetSize_equalSize_001
 * @tc.desc: Verify SetSize function, when exist's size equals input's size, case 01, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_equalSize_001, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 2;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(inputSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, inputSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, inputSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_equalSize_002
 * @tc.desc: Verify SetSize function, when exist's size equals input's size, case 02, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_equalSize_002, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t existStartIndex = 2;
    uint16_t existEndIndex = 4;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint16_t insertStartIndex = 0;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(inputSize, insertStartIndex, insertEndIndex);

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, inputSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_equalSize_003
 * @tc.desc: Verify SetSize function, when exist's size equals input's size, case 03, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_equalSize_003, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 2;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint16_t insertStartIndex = 1;
    uint16_t insertEndIndex = 5;
    spannableString_->SetSize(inputSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, inputSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_equalSize_004
 * @tc.desc: Verify SetSize function, when exist's size equals input's size, case 04, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_equalSize_004, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t inputSize = 10;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(inputSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, inputSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_001
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 01, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_001, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 2;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 4;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_002
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 02, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_002, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 2;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_003
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 03, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_003, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 4;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<insertStartIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_004
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 04, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_004, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<insertStartIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_005
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 05, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_005, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 0;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<insertStartIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }

    for(int i=insertEndIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_006
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 06, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_006, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 2;
    uint16_t existEndIndex = 4;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_007
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 07, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_007, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 2;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_008
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 08, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_008, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 2;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertEndIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_009
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 09, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_009, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 2;
    uint16_t existEndIndex = 4;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 1;
    uint16_t insertEndIndex = 5;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_010
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 10, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_010, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 4;
    uint16_t existEndIndex = 6;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_011
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 11, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_011, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 4;
    uint16_t existEndIndex = 10;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 8;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=insertEndIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_012
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 12, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_012, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 6;
    uint16_t existEndIndex = 10;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 6;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

/**
 * @tc.name: SpannableStringSetSize_notEqualSize_013
 * @tc.desc: Verify SetSize function, when exist's size != input's size, case 13, equal.
 * @tc.type: FUNC
 */
HWTEST_F(SpannableStringTest, SpannableStringSetSize_notEqualSize_013, TestSize.Level1)
{
    if (spannableString_ == nullptr) {
        EXPECT_NE(ptr, nullptr)
        return;
    }
    uint8_t existSize = 8;
    uint16_t existStartIndex = 6;
    uint16_t existEndIndex = 10;
    spannableString_->SetSize(inputSize, existStartIndex, existEndIndex);

    uint8_t insertSize = 10;
    uint16_t insertStartIndex = 2;
    uint16_t insertEndIndex = 4;
    spannableString_->SetSize(existSize, insertStartIndex, insertEndIndex);

    for(int i=existStartIndex; i<existEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, existSize);
    }

    for(int i=insertStartIndex; i<insertEndIndex; i++)
    {
        uint8_t outputSize = 0;
        bool wishHasFind = true;
        bool currentHasFind = spannableString_->GetSize(i, outputSize);
        EXPECT_EQ(wishHasFind, currentHasFind);
        EXPECT_EQ(outputSize, insertSize);
    }
}

}

