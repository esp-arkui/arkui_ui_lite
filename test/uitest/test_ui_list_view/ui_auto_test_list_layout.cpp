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

#include "ui_auto_test_list_layout.h"
#include "ui_test_list_layout.h"

namespace OHOS {
void UIAutoTestUIListLayout::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("ListLayout");
}

void UIAutoTestUIListLayout::RunTestList()
{
    Reset();
    UIKitListLayoutTestAddInsert001();
    UIKitListLayoutTestRemove001();
    UIKitListLayoutTestAddInsert002();
    UIKitListLayoutTestRemove002();
}

void UIAutoTestUIListLayout::UIKitListLayoutTestAddInsert001() const
{
    DragViewToHead(UI_TEST_LIST_LAYOUT_ADD_INSERT_LABEL_01);
    const char* fileName = "ui_test_list_layout_add_insert_label_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestUIListLayout::UIKitListLayoutTestRemove001() const
{
    DragViewToHead(UI_TEST_LIST_LAYOUT_REMOVE_LABEL_01);
    const char* fileName = "ui_test_list_layout_remove_label_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestUIListLayout::UIKitListLayoutTestAddInsert002() const
{
    DragViewToHead(UI_TEST_LIST_LAYOUT_ADD_INSERT_LABEL_02);
    const char* fileName = "ui_test_list_layout_add_insert_label_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestUIListLayout::UIKitListLayoutTestRemove002() const
{
    DragViewToHead(UI_TEST_LIST_LAYOUT_REMOVE_LABEL_02);
    const char* fileName = "ui_test_list_layout_remove_label_02.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS
