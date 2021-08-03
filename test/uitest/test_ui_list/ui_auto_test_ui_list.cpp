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

#include "ui_auto_test_ui_list.h"
#include "ui_test_ui_list.h"

namespace OHOS {
void UIAutoTestUIList::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UIList");
}

void UIAutoTestUIList::RunTestList()
{
    UIKitListInitTestFullScreen001();
    UIKitListInitTestHalScreen001();
    UIKitListScrollTestBlankSet001();
}

void UIAutoTestUIList::UIKitListInitTestFullScreen001() const
{
    Reset();
    const char* fileName = "ui_test_list_full_screen_base.bmp";
    CompareByBinary(fileName);
    DragViewById("test_list_full_screen_list_id", DragDirection::TOP_TO_BOTTOM);
    DragViewById("test_list_full_screen_list_id", DragDirection::BOTTOM_TO_TOP);
    const char* fileName1 = "ui_test_list_full_screen_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestUIList::UIKitListInitTestHalScreen001() const
{
    Reset();
    const char* fileName = "ui_test_list_hal_screen_base.bmp";
    CompareByBinary(fileName);
    DragViewById("test_list_hal_screen_list_id", DragDirection::TOP_TO_BOTTOM);
    const char* fileName1 = "ui_test_list_hal_screen_base.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestUIList::UIKitListScrollTestBlankSet001() const
{
    Reset();
    DragViewToHead("test_ui_list_blank_set_label_id");
    const char* fileName = "ui_test_list_blank_set_base.bmp";
    CompareByBinary(fileName);
    ClickViewById("test_ui_list_close_blank_button_id");
    DragViewById("test_ui_list_blank_set_list_id", DragDirection::TOP_TO_BOTTOM);
    const char* fileName1 = "ui_test_list_blank_set_off.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_list_open_blank_button_id");
    DragViewById("test_ui_list_blank_set_list_id", DragDirection::TOP_TO_BOTTOM);
    const char* fileName2 = "ui_test_list_blank_set_on.bmp";
    CompareByBinary(fileName2);
}
} // namespace OHOS
