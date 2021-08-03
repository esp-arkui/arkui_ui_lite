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

#include "ui_auto_test_ui_picker.h"
#include "compare_tools.h"
#include "components/root_view.h"
#include "dfx/event_injector.h"
#include "ui_test_ui_picker.h"

namespace OHOS {
void UIAutoTestUiPicker::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UIPicker");
}

void UIAutoTestUiPicker::RunTestList()
{
    UIKitPickerTest001();
    UIKitPickerTest002();
    UIKitPickerTest003();
    UIKitPickerTest004();
    UIKitTimerPickerTest001();
}

void UIAutoTestUiPicker::UIKitPickerTest001() const
{
    Reset();
    const char* fileName1 = "ui_test_ui_picker_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_PICKER_POSITIONING_BTN_ID_01);
    const char* fileName2 = "ui_test_ui_picker_positioning.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestUiPicker::UIKitPickerTest002() const
{
    Reset();
    const char* fileName1 = "ui_test_ui_picker_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_PICKER_LEFT_TO_RIGHT_BTN_ID_01);
    const char* fileName2 = "ui_test_ui_picker_left_to_right.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_PICKER_RIGHT_TO_LEFT_BTN_ID_01);
    CompareByBinary(fileName1);
}

void UIAutoTestUiPicker::UIKitPickerTest003() const
{
    Reset();
    const char* fileName1 = "ui_test_ui_picker_base.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_PICKER_LOOP_ON_BTN_ID_01);
    const char* fileName2 = "ui_test_ui_picker_loop_01.bmp";
    CompareByBinary(fileName2);
    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_01, DragDirection::TOP_TO_BOTTOM);
    const char* fileName3 = "ui_test_ui_picker_loop_02.bmp";
    CompareByBinary(fileName3);

    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_01, DragDirection::BOTTOM_TO_TOP);
    const char* fileName4 = "ui_test_ui_picker_loop_03.bmp";
    CompareByBinary(fileName4);

    ClickViewById(UI_TEST_PICKER_LOOP_OFF_BTN_ID_01);
    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_01, DragDirection::TOP_TO_BOTTOM);
    CompareByBinary(fileName1);
}

void UIAutoTestUiPicker::UIKitPickerTest004() const
{
    Reset();
    const char* fileName1 = "ui_test_ui_picker_base.bmp";
    CompareByBinary(fileName1);
    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_03, DragDirection::BOTTOM_TO_TOP);
    const char* fileName2 = "ui_test_ui_picker_interval_01.bmp";
    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_03, DragDirection::BOTTOM_TO_TOP);
    const char* fileName3 = "ui_test_ui_picker_interval_02.bmp";
    DragViewById(UI_TEST_PICKER_UI_PICKER_ID_03, DragDirection::BOTTOM_TO_TOP);
    const char* fileName4 = "ui_test_ui_picker_interval_03.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestUiPicker::UIKitTimerPickerTest001() const
{
    Reset();
    const char* fileName1 = "ui_test_ui_picker_base.bmp";
    CompareByBinary(fileName1);
    UIView* view = RootView::GetInstance()->GetChildById(UI_TEST_PICKER_UI_PICKER_ID_02);
    if (view == nullptr) {
        return;
    }
    Point basePoint = {view->GetOrigRect().GetX(), view->GetOrigRect().GetY()};
    Point startPoint = {basePoint.x, static_cast<int16_t>(basePoint.y + 200)}; // 200: x offer
    Point endPoint = {basePoint.x, static_cast<int16_t>(basePoint.y + 50)};     // 50: y offer
    EventInjector::GetInstance()->SetDragEvent(startPoint, endPoint, 300); // 300: dray time
    CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
    const char* fileName2 = "ui_test_ui_time_picker_01.bmp";
    CompareByBinary(fileName2);
    startPoint.x += 80; // 80: y Offset
    endPoint.x += 80; // 80: y Offset
    EventInjector::GetInstance()->SetDragEvent(startPoint, endPoint, 300); // 300: dray time
    CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
    const char* fileName3 = "ui_test_ui_time_picker_02.bmp";
    CompareByBinary(fileName3);
    startPoint.x += 80; // 80: y Offset
    endPoint.x += 80; // 80: y Offset
    EventInjector::GetInstance()->SetDragEvent(startPoint, endPoint, 300); // 300: dray time
    CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
    const char* fileName4 = "ui_test_ui_time_picker_03.bmp";
    CompareByBinary(fileName4);
}
} // namespace OHOS
