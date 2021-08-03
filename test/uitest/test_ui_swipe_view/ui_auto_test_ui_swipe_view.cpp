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

#include "ui_auto_test_ui_swipe_view.h"
#include "ui_test_ui_swipe_view.h"

namespace OHOS {
void UIAutoTestSwipeView::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UISwipeView");
}

void UIAutoTestSwipeView::RunTestList()
{
    UIKitSwipeViewTestHorizontal001();
    UIKitSwipeViewTestHorizontal002();
    UIKitSwipeViewTestVer001();
    UIKitSwipeViewTestVer002();
    UIKitSwipeViewTestAdd001();
    UIKitSwipeViewTestAdd002();
    UIKitSwipeViewTestAdd003();
    UIKitSwipeViewTestRemove001();
    UIKitSwipeViewTestRemove002();
    UIKitSwipeViewTestRemove003();
    UIKitSwipeViewTestAlign001();
}

void UIAutoTestSwipeView::UIKitSwipeViewTestHorizontal001() const
{
    Reset();
    const char* fileName = "ui_test_ui_swipe_view_horizontal_base_01.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_horizontal_swipe_id_01", DragDirection::RIGHT_TO_LEFT);
    const char* fileName1 = "ui_test_ui_swipe_view_horizontal_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestHorizontal002() const
{
    DragViewToHead("test_ui_scroll_view_horizontal_label_id_02");
    const char* fileName = "ui_test_ui_swipe_view_horizontal_base_02.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_horizontal_swipe_id_02", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_swipe_view_horizontal_02_01.bmp";
    CompareByBinary(fileName1);
    DragViewById("test_ui_scroll_view_horizontal_swipe_id_02", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName2 = "ui_test_ui_swipe_view_horizontal_02_02.bmp";
    CompareByBinary(fileName2);
    DragViewById("test_ui_scroll_view_horizontal_swipe_id_02", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName3 = "ui_test_ui_swipe_view_horizontal_02_03.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestVer001() const
{
    Reset();
    const char* fileName = "ui_test_ui_swipe_view_ver_base_01.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_ver_swipe_id_01", DragDirection::BOTTOM_TO_TOP);
    const char* fileName1 = "ui_test_ui_swipe_view_ver_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestVer002() const
{
    DragViewToHead("test_ui_scroll_view_horizontal_label_id_02");
    DragViewToHead("test_ui_scroll_view_ver_label_id_02");
    const char* fileName = "ui_test_ui_swipe_view_ver_base_02.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_ver_swipe_id_02", DragDirection::BOTTOM_TO_TOP);
    const char* fileName1 = "ui_test_ui_swipe_view_ver_02_01.bmp";
    CompareByBinary(fileName1);
    DragViewById("test_ui_scroll_view_ver_swipe_id_02", DragDirection::BOTTOM_TO_TOP);
    const char* fileName2 = "ui_test_ui_swipe_view_ver_02_02.bmp";
    CompareByBinary(fileName2);
    DragViewById("test_ui_scroll_view_ver_swipe_id_02", DragDirection::BOTTOM_TO_TOP);
    const char* fileName3= "ui_test_ui_swipe_view_ver_02_03.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestAdd001() const
{
    Reset();
    DragViewToHead("test_ui_scroll_view_horizontal_label_id_02");
    DragViewToHead("test_ui_swipe_view_align_left_label_id");
    DragViewToHead("test_ui_swipe_view_align_center_label_id");
    DragViewToHead("test_ui_swipe_view_align_right_label_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_add_head_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    const char* fileName2 = "ui_test_ui_swipe_view_add_head_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName3 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestAdd002() const
{
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_tail_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_add_tail_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName2 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestAdd003() const
{
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_mid_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_add_mid_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName2 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestRemove001() const
{
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_remove_head_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_remove_head_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName2 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestRemove002() const
{
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_remove_mid_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_remove_mid_01.bmp";
    CompareByBinary(fileName1);
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName2 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestRemove003() const
{
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_add_head_btn_id");
    ClickViewById("test_ui_swipe_view_remove_all_btn_id");
    const char* fileName1 = "ui_test_ui_swipe_view_remove_all_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestSwipeView::UIKitSwipeViewTestAlign001() const
{
    Reset();
    DragViewToHead("test_ui_scroll_view_horizontal_label_id_02");
    DragViewToHead("test_ui_swipe_view_align_left_label_id");
    const char* fileName1 = "ui_test_ui_swipe_view_align_left_01.bmp";
    CompareByBinary(fileName1);
    DragViewToHead("test_ui_swipe_view_align_center_label_id");
    const char* fileName2 = "ui_test_ui_swipe_view_align_center_01.bmp";
    CompareByBinary(fileName2);
    DragViewToHead("test_ui_swipe_view_align_right_label_id");
    const char* fileName3 = "ui_test_ui_swipe_view_align_right_01.bmp";
    CompareByBinary(fileName3);
}
} // namespace OHOS