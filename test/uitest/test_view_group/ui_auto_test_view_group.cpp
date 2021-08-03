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

#include "ui_auto_test_view_group.h"
#include "ui_test_view_group.h"

namespace OHOS {
void UIAutoTestViewGroup::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UIViewGroup");
}

void UIAutoTestViewGroup::RunTestList()
{
    UIKitViewGroupAddTest001();
    UIKitViewGroupRemoveTest002();
    UIKitViewGroupRemoveAddTest003();
}

void UIAutoTestViewGroup::UIKitViewGroupAddTest001() const
{
    Reset();
    ClickViewById("addBtn");
    const char* fileName1 = "ui_test_view_group_add.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestViewGroup::UIKitViewGroupRemoveTest002() const
{
    Reset();
    ClickViewById("addBtn");
    const char* fileName1 = "ui_test_view_group_add.bmp";
    CompareByBinary(fileName1);
    ClickViewById("removeBtn");
    const char* fileName2 = "ui_test_view_group_remove.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestViewGroup::UIKitViewGroupRemoveAddTest003() const
{
    Reset();
    ClickViewById("addBtn");
    const char* fileName1 = "ui_test_view_group_add.bmp";
    CompareByBinary(fileName1);
    ClickViewById("removeAddBtn");
    const char* fileName2 = "ui_test_view_group_remove_add.bmp";
    CompareByBinary(fileName2);
}
} // namespace OHOS
