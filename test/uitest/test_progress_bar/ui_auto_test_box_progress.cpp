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

#include "ui_auto_test_box_progress.h"
#include "ui_test_box_progress.h"

namespace OHOS {
void UIAutoTestBoxProgress::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Box_Progress");
}

void UIAutoTestBoxProgress::RunTestList()
{
    UIKitBoxProgressTestUIBoxProgress001();
    UIKitBoxProgressTestSetRange002();
    UIKitBoxProgressTestSetValue003();
    UIKitBoxProgressTestSetImage004();
    UIKitBoxProgressTestSetStyle005();
    UIKitBoxProgressTestGetStyle006();
    UIKitBoxProgressTestSetCapType007();
    UIKitBoxProgressTestSetDirection008();
    UIKitBoxProgressTestEnableBackground009();
    UIKitBoxProgressTestSetStep010();
    UIKitBoxProgressTestSetValidSize011();
    UIKitBoxProgressTestSetBorderAndPadding012();
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestUIBoxProgress001() const
{
    Reset();
    ClickViewById("test_box_progress_reset_button");
    const char* fileName1 = "ui_test_box_progress_reset_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetRange002() const
{
    ClickViewById("test_box_progress_inc_min_progress_button");
    const char* fileName1 = "ui_test_box_progress_inc_min_progress_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_dec_min_progress_button");
    const char* fileName2 = "ui_test_box_progress_dec_min_progress_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_box_progress_inc_max_progress_button");
    const char* fileName3 = "ui_test_box_progress_inc_max_progress_01.bmp";
    CompareByBinary(fileName3);
    ClickViewById("test_box_progress_dec_max_progress_button");
    const char* fileName4 = "ui_test_box_progress_dec_max_progress_01.bmp";
    CompareByBinary(fileName4);
    ClickViewById("test_box_progress_reset_button");
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetValue003() const
{
    ClickViewById("test_box_progress_inc_progress_button");
    const char* fileName1 = "ui_test_box_progress_inc_progress_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_dec_progress_button");
    const char* fileName2 = "ui_test_box_progress_dec_progress_01.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_box_progress_reset_button");
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetImage004() const
{
    DragViewToHead("test_box_progress_image_label");
    ClickViewById("test_box_progress_set_image_button");
    const char* fileName1 = "ui_test_box_progress_set_image_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_no_image_button");
    const char* fileName2 = "ui_test_box_progress_no_image_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetStyle005() const
{
    Reset();
    ClickViewById("test_box_progress_reset_button");
    DragViewToHead("test_box_progress_image_label");
    ClickViewById("test_box_progress_set_style_button");
    const char* fileName1 = "ui_test_box_progress_set_style_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestGetStyle006() const
{
    ClickViewById("test_box_progress_get_style_button");
    const char* fileName1 = "ui_test_box_progress_get_style_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetCapType007() const
{
    Reset();
    ClickViewById("test_box_progress_reset_button");
    ClickViewById("test_box_progress_round_cap_button");
    const char* fileName1 = "ui_test_box_progress_cap_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_none_cap_button");
    const char* fileName2 = "ui_test_box_progress_cap_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetDirection008() const
{
    ClickViewById("test_box_progress_b2t_button");
    const char* fileName1 = "ui_test_box_progress_direction_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_t2b_button");
    const char* fileName2 = "ui_test_box_progress_direction_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_box_progress_r2l_button");
    const char* fileName3 = "ui_test_box_progress_direction_03.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_l2r_button");
    const char* fileName4 = "ui_test_box_progress_direction_04.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestEnableBackground009() const
{
    ClickViewById("test_box_progress_disable_background_button");
    const char* fileName1 = "ui_test_box_progress_background_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_enable_background_button");
    const char* fileName2 = "ui_test_box_progress_background_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetStep010() const
{
    DragViewToHead("test_box_progress_image_label");
    ClickViewById("test_box_progress_set_step_button");
    const char* fileName1 = "ui_test_box_progress_set_step_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetValidSize011() const
{
    ClickViewById("test_box_progress_dec_height_button");
    const char* fileName1 = "ui_test_box_progress_set_valid_size_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_dec_width_button");
    const char* fileName2 = "ui_test_box_progress_set_valid_size_02.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestBoxProgress::UIKitBoxProgressTestSetBorderAndPadding012() const
{
    ClickViewById("test_box_progress_border_inc_button");
    const char* fileName1 = "ui_test_box_progress_set_border_and_padding_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_box_progress_padding_left_inc_button");
    const char* fileName2 = "ui_test_box_progress_set_border_and_padding_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_box_progress_padding_right_inc_button");
    const char* fileName3 = "ui_test_box_progress_set_border_and_padding_03.bmp";
    CompareByBinary(fileName3);
    ClickViewById("test_box_progress_padding_top_inc_button");
    const char* fileName4 = "ui_test_box_progress_set_border_and_padding_04.bmp";
    CompareByBinary(fileName4);
    ClickViewById("test_box_progress_padding_bottom_inc_button");
    const char* fileName5 = "ui_test_box_progress_set_border_and_padding_05.bmp";
    CompareByBinary(fileName5);
    ClickViewById("test_box_progress_border_dec_button");
    const char* fileName6 = "ui_test_box_progress_set_border_and_padding_06.bmp";
    CompareByBinary(fileName6);
}
} // namespace OHOS
