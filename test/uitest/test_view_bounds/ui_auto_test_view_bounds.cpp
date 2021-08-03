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

#include "ui_auto_test_view_bounds.h"
#include "ui_test_view_bounds.h"

namespace OHOS {
void UIAutoTestViewBounds::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("View Bounds");
}

void UIAutoTestViewBounds::RunTestList()
{
    Reset();
    UIKitViewBoundsSetState001();
    UIKitViewBoundsMargin001();
    UIKitViewBoundsPadding001();
    UIKitViewBoundsBorder001();
    UIKitViewBounds001();
    UIKitViewBoundsSmallView001();
    UIKitViewBoundsInvisible001();
    UIKitViewBoundsPostDraw001();
    Reset();
    ClickViewById("test_view_bounds_toggle_button_id");
    UIKitViewBoundsSetState002();
    UIKitViewBoundsMargin002();
    UIKitViewBoundsPadding002();
    UIKitViewBoundsBorder002();
    UIKitViewBounds002();
    UIKitViewBoundsSmallView002();
    UIKitViewBoundsInvisible002();
    UIKitViewBoundsPostDraw002();
    Reset();
    ClickViewById("test_view_bounds_toggle_button_id");
}

void UIAutoTestViewBounds::UIKitViewBoundsSetState001() const
{
    DragViewToHead("UIKitViewBoundsSetState001");
    const char* fileName = "ui_test_view_bounds_set_state_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsMargin001() const
{
    DragViewToHead("UIKitViewBoundsMargin001");
    const char* fileName = "ui_test_view_bounds_margin_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsPadding001() const
{
    DragViewToHead("UIKitViewBoundsPadding001");
    const char* fileName = "ui_test_view_bounds_padding_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsBorder001() const
{
    DragViewToHead("UIKitViewBoundsBorder001");
    const char* fileName = "ui_test_view_bounds_padding_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBounds001() const
{
    DragViewToHead("UIKitViewBounds001");
    const char* fileName = "ui_test_view_bounds_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsSmallView001() const
{
    DragViewToHead("UIKitViewBoundsSmallView001");
    const char* fileName = "ui_test_view_bounds_small_view_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsInvisible001() const
{
    DragViewToHead("UIKitViewBoundsInvisible001");
    const char* fileName = "ui_test_view_bounds_invisible_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsPostDraw001() const
{
    DragViewToHead("UIKitViewBoundsPostDraw001");
    const char* fileName = "ui_test_view_bounds_post_draw_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsSetState002() const
{
    DragViewToHead("UIKitViewBoundsSetState001");
    const char* fileName = "ui_test_view_bounds_set_state_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsMargin002() const
{
    DragViewToHead("UIKitViewBoundsMargin001");
    const char* fileName = "ui_test_view_bounds_margin_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsPadding002() const
{
    DragViewToHead("UIKitViewBoundsPadding001");
    const char* fileName = "ui_test_view_bounds_padding_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsBorder002() const
{
    DragViewToHead("UIKitViewBoundsBorder001");
    const char* fileName = "ui_test_view_bounds_padding_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBounds002() const
{
    DragViewToHead("UIKitViewBounds001");
    const char* fileName = "ui_test_view_bounds_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsSmallView002() const
{
    DragViewToHead("UIKitViewBoundsSmallView001");
    const char* fileName = "ui_test_view_bounds_small_view_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsInvisible002() const
{
    DragViewToHead("UIKitViewBoundsInvisible001");
    const char* fileName = "ui_test_view_bounds_invisible_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestViewBounds::UIKitViewBoundsPostDraw002() const
{
    DragViewToHead("UIKitViewBoundsPostDraw001");
    const char* fileName = "ui_test_view_bounds_post_draw_02.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS
