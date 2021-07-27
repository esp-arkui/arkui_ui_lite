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
void UIAutoTestSlider::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Slider");
}

void UIAutoTestSlider::RunTestList()
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

void UIAutoTestSlider::UIKitSliderTestSetRange001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETRANGE_INCMIN_BTN_ID_01);
    const char* fileName = "ui_test_slider_range_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_DECMIN_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_range_002.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_INCMIN_BTN_ID_01);
    const char* fileName3 = "ui_test_slider_range_003.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_SLIDER_SETRANGE_DECMIN_BTN_ID_02);
    const char* fileName4 = "ui_test_slide_range_004.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestSlider::UIKitSliderTestSetValue001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETVALUE_BTN_ID_01);
    const char* fileName = "ui_test_slider_value_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETVALUE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_value_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSlider::UIKitSliderTestSetImage001() const
{
    ClickViewById(UI_TEST_SLIDER_SETIMAGE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_image_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_UNSETIMAGE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_unset_image_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSlider::UIKitSliderTestSetStyle001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_style_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_style_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSlider::UIKitSliderTestSetKnobWidth001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETKNOBWIDTH_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_knob_width_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETKNOBWIDTH_BTN_ID_02);
    const char* fileName2 = "ui_test_slider_set_knob_width_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSlider::UIKitSliderTestSetStep001() const
{
    ClickViewById(UI_TEST_SLIDER_SETSTEP_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_step_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestSlider::UIKitSliderTestSetColor001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETCOLOR_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_color_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestSlider::UIKitSliderTestSetDirection001() const
{
    Reset();
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_direction_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_direction_002.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_03);
    const char* fileName3 = "ui_test_slider_set_direction_003.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_SLIDER_SETDIRECTION_BTN_ID_04);
    const char* fileName4 = "ui_test_slide_set_direction_004.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestSlider::UIKitSliderTestSetValidSize001() const
{
    ClickViewById(UI_TEST_SLIDER_SETVALIDSIZE_BTN_ID_01);
    const char* fileName = "ui_test_slider_set_valid_size_001.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_SLIDER_SETVALIDSIZE_BTN_ID_02);
    const char* fileName2 = "ui_test_slide_set_valid_size_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSlider::UIKitSliderTestSetRadius001() const
{
    Reset();
    DragViewToHead(UI_TEST_SLIDER_SETIMAGE_LABEL_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETSTYLE_BTN_ID_01);
    ClickViewById(UI_TEST_SLIDER_SETRADIUS_BTN_ID_01);
    const char* fileName = "ui_test_slide_set_radius_001.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS