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
#include "auto_test_counter.h"
#include "ui_test_opacity.h"

namespace OHOS {
void UIAutoTestOpacity::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Opacity");
}

void UIAutoTestOpacity::RunTestList()
{
    Reset();
    UIKitOpacityTestUILabel001();
    UIKitOpacityTestUILabel002();
    UIKitOpacityTestUIButton001();
    UIKitOpacityTestUIButton002();
    UIKitOpacityTestUILabelButton001();
    UIKitOpacityTestUIArcLabel001();
    UIKitOpacityTestUIImageView001();
    UIKitOpacityTestUIImageAnimator001();
    UIKitOpacityTestUIView001();
    UIKitOpacityTestUIViewGroup001();
    UIKitOpacityTestUIList001();
    UIKitOpacityTestUIScrollView001();
    UIKitOpacityTestUISwipeView001();
}

void UIAutoTestOpacity::UIKitOpacityTestUILabel001() const
{
    DragViewToHead("UIKitOpacityTestUILabel001");
    const char* fileName = "ui_test_opacity_ui_label_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUILabel002() const
{
    DragViewToHead("UIKitOpacityTestUILabel002");
    const char* fileName = "ui_test_opacity_ui_label_02.bmp";
    CompareByBinary(fileName);
}
void UIAutoTestOpacity::UIKitOpacityTestUIButton001() const
{
    DragViewToHead("UIKitOpacityTestUIButton001");
    const char* fileName = "ui_test_opacity_ui_button_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIButton002() const
{
    DragViewToHead("UIKitOpacityTestUIButton002");
    const char* fileName = "ui_test_opacity_ui_button_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUILabelButton001() const
{
    DragViewToHead("UIKitOpacityTestUILabelButton001");
    const char* fileName = "ui_test_opacity_ui_label_button_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIArcLabel001() const
{
    DragViewToHead("UIKitOpacityTestUIArcLabel001");
    const char* fileName = "ui_test_opacity_ui_arc_label_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIImageView001() const
{
    DragViewToHead("UIKitOpacityTestUIImageView001");
    const char* fileName = "ui_test_opacity_ui_image_view_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIImageAnimator001() const
{
    DragViewToHead("UIKitOpacityTestUIImageAnimator001");
    ClickViewById("test_opacity_toggle_btn");
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_star", frameArray, 5);
    ClickViewById("test_opacity_toggle_btn");
}

void UIAutoTestOpacity::UIKitOpacityTestUIView001() const
{
    DragViewToHead("UIKitOpacityTestUIView001");
    const char* fileName = "ui_test_opacity_ui_view_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIViewGroup001() const
{
    DragViewToHead("UIKitOpacityTestUIViewGroup001");
    const char* fileName = "ui_test_opacity_ui_view_group_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIList001() const
{
    DragViewToHead("UIKitOpacityTestUIList001");
    const char* fileName = "ui_test_opacity_ui_list_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUIScrollView001() const
{
    DragViewToHead("UIKitOpacityTestUIScrollView001");
    const char* fileName = "ui_test_opacity_ui_scroll_view_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestOpacity::UIKitOpacityTestUISwipeView001() const
{
    DragViewToHead("UIKitOpacityTestUISwipeView001");
    const char* fileName = "ui_test_opacity_ui_swipe_view_01.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS