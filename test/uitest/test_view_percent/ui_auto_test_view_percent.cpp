/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
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

#include "ui_auto_test_view_percent.h"
#include "ui_test_view_percent.h"

namespace OHOS {
void UIAutoTestViewPercent::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("View_Percent");
}

void UIAutoTestViewPercent::RunTestList()
{
    Reset();
    UIKitViewPercentFlexLayout001();
    UIKitViewPercentGridLayout001();
    UIKitViewPercentBigger001();
    UIKitViewPercentSmaller001();
}

void UIAutoTestViewPercent::UIKitViewPercentFlexLayout001() const
{
    DragViewToHead("test_percent_flex_layout_id");
    const char* fileName = "ui_test_percent_flex_layout_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewPercent::UIKitViewPercentGridLayout001() const
{
    DragViewToHead("test_percent_grid_layout_id");
    const char* fileName = "ui_test_percent_grid_layout_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewPercent::UIKitViewPercentBigger001() const
{
    ClickViewById("test_percent_bigger_id");
    ClickViewById("test_percent_bigger_id");
    const char* fileName = "ui_test_percent_grid_layout_bigger_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewPercent::UIKitViewPercentSmaller001() const
{
    ClickViewById("test_percent_smaller_id");
    const char* fileName = "ui_test_percent_grid_layout_smaller_01.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS
