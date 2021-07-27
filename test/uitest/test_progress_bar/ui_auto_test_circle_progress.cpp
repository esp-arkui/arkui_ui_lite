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

#include "ui_auto_test_circle_progress.h"
#include "ui_test_circle_progress.h"

namespace OHOS {
void UIAutoTestCircleProgress::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Circle_Progress");
}

void UIAutoTestCircleProgress::RunTestList()
{
    UIKitCircleProgressTestSetValue002();
    UIKitCircleProgressTestSetImage003();
    UIKitCircleProgressTestSetStyle004();
    UIKitCircleProgressTestGetStyle005();
    UIKitCircleProgressTestSetCapType006();
    UIKitCircleProgressTestSetStep007();
    UIKitCircleProgressTestSetRange008();
    UIKitCircleProgressTestEnableBackground009();
    UIKitCircleProgressTestSetCenter010();
    UIKitCircleProgressTestSetRadius011();
    UIKitCircleProgressTestSetLineWidth012();
    UIKitCircleProgressTestSetImagePosition013();
    UIKitCircleProgressTestSetLineColor014();
    UIKitCircleProgressTestSetAngle015();
    UIKitCircleProgressTestUICircleProgress001();
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetValue002() const
{
    Reset();
    ClickViewById("test_circle_progress_inc_value_btn");
    const char* fileName1 = "ui_test_circle_progress_set_value_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_dec_value_btn");
    const char* fileName2 = "ui_test_circle_progress_set_value_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetImage003() const
{
    ClickViewById("test_circle_progress_no_image_btn");
    const char* fileName1 = "ui_test_circle_progress_set_image_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_set_image_btn");
    const char* fileName2 = "ui_test_circle_progress_set_image_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetStyle004() const
{
    ClickViewById("test_circle_progress_set_style_btn");
    const char* fileName1 = "ui_test_circle_progress_set_style_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestGetStyle005() const
{
    Reset();
    DragViewToHead("test_circle_progress_get_style_label");
    ClickViewById("test_circle_progress_get_style_btn");
    const char* fileName1 = "ui_test_circle_progress_get_style_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetCapType006() const
{
    Reset();
    DragViewToHead("test_circle_progress_get_style_label");
    ClickViewById("test_circle_progress_round_cap_type_btn");
    const char* fileName1 = "ui_test_circle_progress_set_cap_type_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_none_cap_type_btn");
    const char* fileName2 = "ui_test_circle_progress_set_cap_type_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetStep007() const
{
    ClickViewById("test_circle_progress_set_step_btn");
    const char* fileName1 = "ui_test_circle_progress_set_step_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetRange008() const
{
    ClickViewById("test_circle_progress_inc_min_range_btn");
    const char* fileName1 = "ui_test_circle_progress_set_range_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_dec_min_range_btn");
    const char* fileName2 = "ui_test_circle_progress_set_range_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_circle_progress_inc_max_range_btn");
    const char* fileName3 = "ui_test_circle_progress_set_range_03.bmp";
    CompareByBinary(fileName3);
    ClickViewById("test_circle_progress_dec_max_range_btn");
    const char* fileName4 = "ui_test_circle_progress_set_range_04.bmp";
    CompareByBinary(fileName4);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestEnableBackground009() const
{
    Reset();
    ClickViewById("test_circle_progress_disable_background_btn");
    const char* fileName1 = "ui_test_circle_progress_enabel_background_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_enable_background_btn");
    const char* fileName2 = "ui_test_circle_progress_enabel_background_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetCenter010() const
{
    ClickViewById("test_circle_progress_center_x_btn");
    const char* fileName1 = "ui_test_circle_progress_set_center_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_center_y_btn");
    const char* fileName2 = "ui_test_circle_progress_set_center_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetRadius011() const
{
    ClickViewById("test_circle_progress_radius_inc_btn");
    const char* fileName1 = "ui_test_circle_progress_set_radius_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_radius_dec_btn");
    const char* fileName2 = "ui_test_circle_progress_set_radius_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetLineWidth012() const
{
    ClickViewById("test_circle_progress_width_inc_btn");
    const char* fileName1 = "ui_test_circle_progress_set_width_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_width_dec_btn");
    const char* fileName2 = "ui_test_circle_progress_set_width_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetImagePosition013() const
{
    Reset();
    ClickViewById("test_circle_progress_set_image_btn");
    const char* fileName1 = "ui_test_circle_progress_set_image_01.bmp";
    CompareByBinary(fileName1);
    DragViewToHead("test_circle_progress_get_style_label");
    ClickViewById("test_circle_progress_image_pos_x_btn");
    const char* fileName2 = "ui_test_circle_progress_set_image_pos_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_circle_progress_image_pos_y_btn");
    const char* fileName3 = "ui_test_circle_progress_set_image_pos_03.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetLineColor014() const
{
    Reset();
    DragViewToHead("test_circle_progress_get_style_label");
    ClickViewById("test_circle_progress_line_color_btn");
    const char* fileName1 = "ui_test_circle_progress_set_line_color_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestSetAngle015() const
{
    ClickViewById("test_circle_progress_inc_start_angle_btn");
    const char* fileName1 = "ui_test_circle_progress_set_angle_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_circle_progress_dec_start_angle_btn");
    const char* fileName2 = "ui_test_circle_progress_set_angle_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_circle_progress_inc_end_angle_btn");
    const char* fileName3 = "ui_test_circle_progress_set_angle_03.bmp";
    CompareByBinary(fileName3);
    ClickViewById("test_circle_progress_dec_end_angle_btn");
    const char* fileName4 = "ui_test_circle_progress_set_angle_04.bmp";
    CompareByBinary(fileName4);
    ClickViewById("test_circle_progress_swap_angle_btn");
    const char* fileName5 = "ui_test_circle_progress_set_angle_05.bmp";
    CompareByBinary(fileName5);
}

void UIAutoTestCircleProgress::UIKitCircleProgressTestUICircleProgress001() const
{
    Reset();
    ClickViewById("test_circle_progress_reset_btn");
    const char* fileName1 = "ui_test_circle_progress_creat_01.bmp";
    CompareByBinary(fileName1);
}
} // namespace OHOS
