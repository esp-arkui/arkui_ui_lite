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

#include "ui_auto_test_event_injector.h"
#include "compare_tools.h"
#include "ui_test_event_injector.h"

namespace OHOS {
void UIAutoTestEventInjector::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Event_Injector");
}

void UIAutoTestEventInjector::RunTestList()
{
    UIKitUIEventInjectorAutoTestClick001();
    UIKitUIEventInjectorAutoTestDrag002();
    UIKitUIEventInjectorAutoTestLongPress003();
    UIKitUIEventInjectorAutoTestKeyEvent004();
    UIKitUIEventInjectorAutoTestUpToDown005();
    UIKitUIEventInjectorAutoTestDownToUp006();
    UIKitUIEventInjectorAutoTestLeftToRight007();
    UIKitUIEventInjectorAutoTestRightToLeft008();
    UIKitUIEventInjectorAutoTestULeftToLRight009();
    UIKitUIEventInjectorAutoTestLRightToULeft010();
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestClick001() const
{
    Reset();
    const char* fileName1 = "ui_test_event_injector_click_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_CLICK_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_click_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestDrag002() const
{
    Reset();
    const char* fileName1 = "ui_test_event_injector_drag_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_DRAG_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_drag_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestLongPress003() const
{
    Reset();
    const char* fileName1 = "ui_test_event_injector_long_press_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_LONG_PRESS_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_long_press_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestKeyEvent004() const
{
    Reset();
    const char* fileName1 = "ui_test_event_injector_key_event_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_KEY_EVENT_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_key_event_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestUpToDown005() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_DOWN_TO_UP_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_down_to_up_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_EVENT_INJECTOR_UP_TO_DOWN_BTN_ID_01);
    CompareByBinary(fileName1);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestDownToUp006() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_DOWN_TO_UP_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_down_to_up_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestLeftToRight007() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_RIGHT_TO_LEFT_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_right_to_left_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_EVENT_INJECTOR_LEFT_TO_RIGHT_ID_01);
    CompareByBinary(fileName1);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestRightToLeft008() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_RIGHT_TO_LEFT_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_right_to_left_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestULeftToLRight009() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_LRIGHT_TO_ULEFT_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_lright_to_uleft_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_EVENT_INJECTOR_ULEFT_TO_LRIGHT_BTN_ID_01);
    CompareByBinary(fileName1);
}

void UIAutoTestEventInjector::UIKitUIEventInjectorAutoTestLRightToULeft010() const
{
    Reset();
    DragViewToHead(UI_TEST_EVENT_INJECTOR_DRAG_LABEL_ID_01);
    const char* fileName1 = "ui_test_event_injector_drag_start.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_EVENT_INJECTOR_LRIGHT_TO_ULEFT_BTN_ID_01);
    const char* fileName2 = "ui_test_event_injector_lright_to_uleft_01.bmp";
    CompareByBinary(fileName2);
}
} // namespace OHOS