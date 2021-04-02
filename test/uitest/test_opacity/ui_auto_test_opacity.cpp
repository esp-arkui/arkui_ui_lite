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

#include "ui_auto_test_opacity.h"
#include "ui_test_opacity.h"

namespace OHOS {
void UiAutoTestOpacity::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_OPACITY_ID);
}

void UiAutoTestOpacity::RunTestList()
{
    Reset();
    UIKitOpacityTestUILabel001();
    UIKitOpacityTestUILabel002();
    UIKitOpacityTestUIButton001();
    UIKitOpacityTestUIButton002();
    UIKitOpacityTestUILabelButton001();
    UIKitOpacityTestUIArcLabel001();
    UIKitOpacityTestUIImageView001();
    UIKitOpacityTestUIView001();
    UIKitOpacityTestUIViewGroup001();
    UIKitOpacityTestUIList001();
    UIKitOpacityTestUIScrollView001();
    UIKitOpacityTestUISwipeView001();
}

void UiAutoTestOpacity::UIKitOpacityTestUILabel001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_label_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUILabel002() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_LABEL_ID_02);
    const char* fileName = "ui_test_opacity_label_002.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIButton001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_BUTTON_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_button_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIButton002() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_BUTTON_LABEL_ID_02);
    const char* fileName = "ui_test_opacity_ui_button_002.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUILabelButton001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_LABELBUTTON_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_label_button_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIArcLabel001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_ARCLABEL_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_arc_label_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIImageView001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_IMAGE_VIEW_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_image_view_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIView001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_VIEW_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_view_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIViewGroup001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_VIEWGROUP_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_view_group_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIList001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_LIST_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_list_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUIScrollView001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_SCROLLVIEW_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_scroll_view_001.bin";
    CompareByBinary(fileName);
}

void UiAutoTestOpacity::UIKitOpacityTestUISwipeView001() const
{
    DragViewToHead(UI_TEST_OPACITY_UI_SWIPEVIEW_LABEL_ID_01);
    const char* fileName = "ui_test_opacity_ui_swipe_view_001.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS