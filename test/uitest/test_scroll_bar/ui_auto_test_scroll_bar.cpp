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

#include "ui_auto_test_scroll_bar.h"
#include "ui_test_scroll_bar.h"

namespace OHOS {
void UIAutoTestScrollBar::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("ScrollBar");
}

void UIAutoTestScrollBar::RunTestList()
{
    Reset();
    UIKitScrollBarTest001();
    UIKitScrollBarTest002();
}

void UIAutoTestScrollBar::UIKitScrollBarTest001() const
{
    DragViewById("test_scroll_bar_scroll_view_id", DragDirection::BOTTOM_TO_TOP);
    const char* fileName = "ui_test_scroll_bar_scroll_view_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestScrollBar::UIKitScrollBarTest002() const
{
    Reset();
    DragViewById("test_scroll_bar_list_id", DragDirection::BOTTOM_TO_TOP);
    const char* fileName = "ui_test_scroll_bar_list_01.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS