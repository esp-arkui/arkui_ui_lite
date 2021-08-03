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

#include "ui_auto_test_button.h"
#include "ui_test_button.h"

namespace OHOS {
void UIAutoTestButton::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Button");
}

void UIAutoTestButton::RunTestList()
{
    UIKitCheckBoxAutoTest001();
    UIKitRadioButtonAutoTest001();
    UIKitToggleButtonAutoTest001();
    UIKitCheckBoxAutoTest002();
    UIKitRadioButtonAutoTest002();
    UIKitToggleButtonAutoTest002();
    UIKitButtonAutoTest001();
}

void UIAutoTestButton::UIKitCheckBoxAutoTest001() const
{
    Reset();
    const char* fileName1 = "ui_test_button_checkbox_all_off.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_01);
    const char* fileName2 = "ui_test_button_checkbox_01_on.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_02);
    const char* fileName3 = "ui_test_button_checkbox_02_on.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_02);
    const char* fileName4 = "ui_test_button_checkbox_02_off.bmp";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_03);
    const char* fileName5 = "ui_test_button_checkbox_03_on.bmp";
    CompareByBinary(fileName5);
}

void UIAutoTestButton::UIKitRadioButtonAutoTest001() const
{
    Reset();
    const char* fileName1 = "ui_test_radio_button_all_off.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_01);
    const char* fileName2 = "ui_test_radio_button_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_02);
    const char* fileName3 = "ui_test_radio_button_02.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_03);
    const char* fileName4 = "ui_test_radio_button_03.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestButton::UIKitToggleButtonAutoTest001() const
{
    Reset();
    const char* fileName = "ui_test_toggle_button_01_all_off.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_01);
    const char* fileName2 = "ui_test_toggle_button_01_on.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_02);
    const char* fileName3 = "ui_test_toggle_button_02_on.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_03);
    const char* fileName5 = "ui_test_toggle_button_03_on.bmp";
    CompareByBinary(fileName5);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_03);
    const char* fileName6 = "ui_test_toggle_button_03_off.bmp";
    CompareByBinary(fileName6);
}

void UIAutoTestButton::UIKitCheckBoxAutoTest002() const
{
    Reset();
    DragViewToHead(UI_TEST_CHECKBOX_IMAGE_LABEL_ID);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01);
    const char* fileName = "ui_test_button_checkbox_image_01_on.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01);
    const char* fileName2 = "ui_test_button_checkbox_image_01_off.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02);
    const char* fileName3 = "ui_test_button_checkbox_image_02_on.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02);
    const char* fileName4 = "ui_test_button_checkbox_image_02_off.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestButton::UIKitRadioButtonAutoTest002() const
{
    const char* fileName = "ui_test_radio_button_image_base.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_RADIO_BUTTON_IMAGE_ID_01);
    const char* fileName2 = "ui_test_radio_button_image_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_RADIO_BUTTON_IMAGE_ID_02);
    const char* fileName3 = "ui_test_radio_button_image_02.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestButton::UIKitToggleButtonAutoTest002() const
{
    DragViewToHead(UI_TEST_TOGGLE_BUTTON_IMAGE_LABEL_ID);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01);
    const char* fileName = "ui_test_toggle_button_image_01_on.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01);
    const char* fileName2 = "ui_test_toggle_button_image_01_off.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02);
    const char* fileName3 = "ui_test_toggle_button_image_02_on.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02);
    const char* fileName4 = "ui_test_toggle_button_image_02_off.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestButton::UIKitButtonAutoTest001() const
{
    ClickViewById(UI_TEST_BUTTON_BIG_ID_01);
    const char* fileName1 = "ui_test_button_big.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_SMALL_ID_01);
    const char* fileName2 = "ui_test_button_small.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_LIFT_ID_01);
    const char* fileName3 = "ui_test_button_lift.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_RIGHT_ID_01);
    const char* fileName4 = "ui_test_button_right.bmp";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_UP_ID_01);
    const char* fileName5 = "ui_test_button_up.bmp";
    CompareByBinary(fileName5);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_DOWN_ID_01);
    const char* fileName6 = "ui_test_button_down.bmp";
    CompareByBinary(fileName6);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_RED_ID_01);
    const char* fileName7 = "ui_test_button_red.bmp";
    CompareByBinary(fileName7);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_WHITE_ID_01);
    const char* fileName8 = "ui_test_button_white.bmp";
    CompareByBinary(fileName8);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_HIDE_ID_01);
    const char* fileName = "ui_test_button_hide.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);

    ClickViewById(UI_TEST_BUTTON_DISPLAY_ID_01);
    const char* fileName9 = "ui_test_button_display.bmp";
    CompareByBinary(fileName9);
    ClickViewById(UI_TEST_BUTTON_RESET_ID);
}
} // namespace OHOS
