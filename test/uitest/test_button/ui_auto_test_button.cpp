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
    EnterSubMenu(UI_TEST_BUTTON_ID);
}

void UIAutoTestButton::RunTestList()
{
    UIKitCheckBoxTest001();
    UIKitRadioButtonTest001();
    UIKitToggleButtonTest001();
    UIKitCheckBoxTest002();
    UIKitRadioButtonTest002();
    UIKitToggleButtonTest002();
}

void UIAutoTestButton::UIKitCheckBoxTest001() const
{
    Reset();
    DragViewToHead(UI_TEST_BUTTON_CHECKBOX_LABEL_ID);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_01);
    const char* fileName = "ui_test_button_checkbox_01_on.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_01);
    const char* fileName2 = "ui_test_button_checkbox_01_off.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_02);
    const char* fileName3 = "ui_test_button_checkbox_02_on.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_02);
    const char* fileName4 = "ui_test_button_checkbox_02_off.bin";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_03);
    const char* fileName5 = "ui_test_button_checkbox_03_on.bin";
    CompareByBinary(fileName5);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_ID_03);
    const char* fileName6 = "ui_test_button_checkbox_03_off.bin";
    CompareByBinary(fileName6);
}

void UIAutoTestButton::UIKitRadioButtonTest001() const
{
    Reset();
    DragViewToHead(UI_TEST_RADIO_BUTTON_LABEL_ID);
    const char* fileName = "ui_test_radio_button_base.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_01);
    const char* fileName2 = "ui_test_radio_button_01.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_02);
    const char* fileName3 = "ui_test_radio_button_02.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_RADIO_BUTTON_ID_03);
    const char* fileName4 = "ui_test_radio_button_03.bin";
    CompareByBinary(fileName4);
}

void UIAutoTestButton::UIKitToggleButtonTest001() const
{
    Reset();
    DragViewToHead(UI_TEST_TOGGLE_BUTTON_LABEL_ID);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_01);
    const char* fileName = "ui_test_toggle_button_01_on.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_01);
    const char* fileName2 = "ui_test_toggle_button_01_off.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_02);
    const char* fileName3 = "ui_test_toggle_button_02_on.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_02);
    const char* fileName4 = "ui_test_toggle_button_02_off.bin";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_03);
    const char* fileName5 = "ui_test_toggle_button_03_on.bin";
    CompareByBinary(fileName5);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_ID_03);
    const char* fileName6 = "ui_test_toggle_button_03_off.bin";
    CompareByBinary(fileName6);
}

void UIAutoTestButton::UIKitCheckBoxTest002() const
{
    Reset();
    DragViewToHead(UI_TEST_BUTTON_CHECKBOX_IMAGE_LABEL_ID);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01);
    const char* fileName = "ui_test_button_checkbox_image_01_on.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01);
    const char* fileName2 = "ui_test_button_checkbox_image_01_off.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02);
    const char* fileName3 = "ui_test_button_checkbox_image_02_on.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02);
    const char* fileName4 = "ui_test_button_checkbox_image_02_off.bin";
    CompareByBinary(fileName4);
}

void UIAutoTestButton::UIKitRadioButtonTest002() const
{
    Reset();
    DragViewToHead(UI_TEST_BUTTON_CHECKBOX_IMAGE_LABEL_ID);
    DragViewToHead(UI_TEST_RADIO_BUTTON_IMAGE_LABEL_ID);
    const char* fileName = "ui_test_radio_button_image_base.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_RADIO_BUTTON_IMAGE_ID_01);
    const char* fileName2 = "ui_test_radio_button_image_01.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_RADIO_BUTTON_IMAGE_ID_02);
    const char* fileName3 = "ui_test_radio_button_image_02.bin";
    CompareByBinary(fileName3);
}

void UIAutoTestButton::UIKitToggleButtonTest002() const
{
    Reset();
    DragViewToHead(UI_TEST_BUTTON_CHECKBOX_IMAGE_LABEL_ID);
    DragViewToHead(UI_TEST_RADIO_BUTTON_IMAGE_LABEL_ID);
    DragViewToHead(UI_TEST_TOGGLE_BUTTON_IMAGE_LABEL_ID);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01);
    const char* fileName = "ui_test_toggle_button_image_01_on.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01);
    const char* fileName2 = "ui_test_toggle_button_image_01_off.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02);
    const char* fileName3 = "ui_test_toggle_button_image_02_on.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02);
    const char* fileName4 = "ui_test_toggle_button_image_02_off.bin";
    CompareByBinary(fileName4);
}
} // namespace OHOS