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

#include "ui_auto_test_ui_scroll_view.h"
#include "ui_test_ui_scroll_view.h"

namespace OHOS {
void UIAutoTestScrollView::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UIScrollView");
}

void UIAutoTestScrollView::RunTestList()
{
    UIKitUIScrollViewTestbar001();
    UIKitUIScrollViewTestbar002();
    UIKitUIScrollViewTestbar003();
    UIKitUIScrollViewTestbar004();
    UIKitUIScrollViewTestScrollableSet001();
    UIKitUIScrollViewTestScrollableSet002();
    UIKitUIScrollViewTestScrollBlankSet001();
}

void UIAutoTestScrollView::UIKitUIScrollViewTestbar001() const
{
    Reset();
    const char* fileName = "ui_test_ui_scroll_view_bar_base_01.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_bar_scroll_id_01", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_bar_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestbar002() const
{
    DragViewToHead("test_ui_scroll_view_bar_label_id_02");
    const char* fileName = "ui_test_ui_scroll_view_bar_base_02.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_bar_scroll_id_02", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_bar_02.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestbar003() const
{
    DragViewToHead("test_ui_scroll_view_bar_label_id_03");
    const char* fileName = "ui_test_ui_scroll_view_bar_base_03.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_bar_scroll_id_03", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_bar_03.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestbar004() const
{
    DragViewToHead("test_ui_scroll_view_bar_label_id_04");
    const char* fileName = "ui_test_ui_scroll_view_bar_base_04.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_bar_scroll_id_04", DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_bar_04.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestScrollableSet001() const
{
    Reset();
    DragViewToHead("test_ui_scroll_view_scrollable_label_id_01");
    const char* fileName = "ui_test_ui_scroll_view_scrollable_base_01.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_scrollable_scroll_id_01",
        DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_scrollable_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestScrollableSet002() const
{
    DragViewToHead("test_ui_scroll_view_scrollable_label_id_02");
    const char* fileName = "ui_test_ui_scroll_view_scrollable_base_02.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_scrollable_scroll_id_02",
         DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_scrollable_02.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestScrollView::UIKitUIScrollViewTestScrollBlankSet001() const
{
    DragViewToHead("test_ui_scroll_view_scroll_blank_set_label_id_01");
    const char* fileName = "ui_test_ui_scroll_view_scroll_blank_set_base_01.bmp";
    CompareByBinary(fileName);
    DragViewById("test_ui_scroll_view_scroll_blank_set_scroll_id_01",
        DragDirection::BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER);
    const char* fileName1 = "ui_test_ui_scroll_view_scroll_blank_set_01.bmp";
    CompareByBinary(fileName1);
}
} // namespace OHOS