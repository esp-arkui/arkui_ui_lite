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

#include "ui_auto_test_slider.h"
#include "ui_test_slider.h"

namespace OHOS {
void UiAutoTestSlider::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_SLIDER_ID);
}

void UiAutoTestSlider::RunTestList()
{
    UIKitSliderTestSetRange001();
    UIKitSliderTestSetValue001();
    UIKitSliderTestSetImage001();
    UIKitSliderTestSetStyle001();
    UIKitSliderTestSetKnobWidth001();
    UIKitSliderTestSetStep001();
    UIKitSliderTestSetColor001();
    UIKitSliderTestSetDirection001();
    UIKitSliderTestSetValidSize001();
    UIKitSliderTestSetRadius001();
}

void UiAutoTestSlider::UIKitSliderTestSetRange001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETRANGE_INCMIN_BTN_ID_01);
    const char* fileName = "ui_test_slider_range_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_DECMIN_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_range_002.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_INCMIN_BTN_ID_01);
    const char* fileName3 = "ui_test_slider_range_003.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_DECMIN_BTN_ID_02);
    const char* fileName4 = "ui_test_slide_range_004.bin";
    CompareByBinary(fileName4);
}

void UiAutoTestSlider::UIKitSliderTestSetValue001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETVALUE_BTN_ID_01);
    const char* fileName = "ui_test_slider_value_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETVALUE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_value_002.bin";
    CompareByBinary(fileName2);
}

void UiAutoTestSlider::UIKitSliderTestSetImage001() const
{
    ClickViewById(UI_TEST_SLIDER_SETIMAGE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_image_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_UNSETIMAGE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_unset_image_002.bin";
    CompareByBinary(fileName2);
}

void UiAutoTestSlider::UIKitSliderTestSetStyle001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_style_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_style_002.bin";
    CompareByBinary(fileName2);
}

void UiAutoTestSlider::UIKitSliderTestSetKnobWidth001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETKNOBWIDTH_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_knob_width_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETKNOBWIDTH_BTN_ID_02);
    const char* fileName2 = "ui_test_slider_set_knob_width_002.bin";
    CompareByBinary(fileName2);
}

void UiAutoTestSlider::UIKitSliderTestSetStep001() const
{
    ClickViewById(UI_TEST_SLIDER_SETSTEP_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_step_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestSlider::UIKitSliderTestSetColor001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETCOLOR_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_color_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestSlider::UIKitSliderTestSetDirection001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_direction_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_direction_002.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_03);
    const char* fileName3 = "ui_test_slider_set_direction_003.bin";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_04);
    const char* fileName4 = "ui_test_slide_set_direction_004.bin";
    CompareByBinary(fileName4);
}

void UiAutoTestSlider::UIKitSliderTestSetValidSize001() const
{
    ClickViewById(UI_TEST_SLIDER_SETVALIDSIZE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_valid_size_001.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETVALIDSIZE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_valid_size_002.bin";
    CompareByBinary(fileName2);
}

void UiAutoTestSlider::UIKitSliderTestSetRadius001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETRADIUS_BTN_ID_01);
    const char* fileName = "ui_test_slide_set_radius_001.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS