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

#include "ui_auto_test_focus_manager.h"
#include "ui_test_focus_manager.h"
#if ENABLE_FOCUS_MANAGER
namespace OHOS {
void UIAutoTestFocusManager::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("FocusManager");
}

void UIAutoTestFocusManager::RunTestList()
{
    Reset();
    UIKitFocusManagerTest001();
    UIKitFocusManagerTest002();
    UIKitFocusManagerTest003();
    UIKitFocusManagerTest004();
    UIKitFocusManagerTest005();
    UIKitFocusManagerTest006();
    UIKitFocusManagerTest007();
    UIKitFocusManagerTest008();
    UIKitFocusManagerTest009();
}

void UIAutoTestFocusManager::UIKitFocusManagerTest001() const
{
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    const char* fileName2 = "ui_test_focus_manager_right_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    const char* fileName3 = "ui_test_focus_manager_right_02.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest002() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);

    ClickViewById(UI_TEST_FOCUS_MANAGER_LEFT_BTN_ID_01);
    const char* fileName2 = "ui_test_focus_manager_left_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_FOCUS_MANAGER_LEFT_BTN_ID_01);
    const char* fileName3 = "ui_test_focus_manager_left_02.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest003() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_DOWN_BTN_ID_01);
    const char* fileName2 = "ui_test_focus_manager_down_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_FOCUS_MANAGER_DOWN_BTN_ID_01);
    const char* fileName3 = "ui_test_focus_manager_down_02.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest004() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_DOWN_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_DOWN_BTN_ID_01);

    ClickViewById(UI_TEST_FOCUS_MANAGER_UP_BTN_ID_01);
    const char* fileName2 = "ui_test_focus_manager_up_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_FOCUS_MANAGER_UP_BTN_ID_01);
    const char* fileName3 = "ui_test_focus_manager_up_02.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest005() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_FOCUS_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);

    const char* fileName2 = "ui_test_focus_manager_focus.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest006() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_UNFOCUSED_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);

    const char* fileName2 = "ui_test_focus_manager_unfocused.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest007() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_INTERCEPT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);

    const char* fileName2 = "ui_test_focus_manager_intercept.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest008() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_UNBLOCK_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);
    ClickViewById(UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01);

    const char* fileName2 = "ui_test_focus_manager_unlock.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestFocusManager::UIKitFocusManagerTest009() const
{
    ClickViewById(UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01);
    const char* fileName1 = "ui_test_focus_manager_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_FOCUS_MANAGER_CLEAR_BTN_ID_01);
    const char* fileName2 = "ui_test_focus_manager_clear.bmp";
    CompareByBinary(fileName2);
}
} // namespace OHOS
#endif
