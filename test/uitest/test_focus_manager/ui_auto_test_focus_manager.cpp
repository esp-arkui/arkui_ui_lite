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
    EnterSubMenu(UI_TEST_FOCUS_MANAGER_ID);
}

void UIAutoTestFocusManager::RunTestList()
{
    Reset();
    UIKitFocusManagerTest001();
}

void UIAutoTestFocusManager::UIKitFocusManagerTest001() const
{
    const char* fileName = "ui_test_focus_manager_01.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS
#endif