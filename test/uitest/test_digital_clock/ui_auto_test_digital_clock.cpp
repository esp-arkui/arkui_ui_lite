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

#include "ui_auto_test_digital_clock.h"
#include "ui_test_digital_clock.h"

namespace OHOS {
void UIAutoTestDigitalClock::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Digital_Clock");
}

void UIAutoTestDigitalClock::RunTestList()
{
    Reset();
    UIKitUIDigitalClockTestDisplay001();
    UIKitUIDigitalClockTestDisplay002();
    UIKitUIDigitalClockTestSetColor001();
    UIKitUIDigitalClockTestDisplayMode001();
    UIKitUIDigitalClockTestDisplayMode002();
    UIKitUIDigitalClockTestDisplayMode003();
    UIKitUIDigitalClockTestDisplayMode004();
    UIKitUIDigitalClockTestDisplayLeadingZero001();
    UIKitUIDigitalClockTestSetOpacity001();
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplay001() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_LABEL_ID_01);
    const char* fileName = "ui_test_digital_clock_display_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplay002() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_LABEL_ID_02);
    const char* fileName = "ui_test_digital_clock_display_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestSetColor001() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_SET_COLOR_LABEL_ID_01);
    const char* fileName = "ui_test_digital_clock_set_color_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplayMode001() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_01);
    const char* fileName = "ui_test_digital_clock_display_mode_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplayMode002() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_02);
    const char* fileName = "ui_test_digital_clock_display_mode_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplayMode003() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_03);
    const char* fileName = "ui_test_digital_clock_display_mode_03.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplayMode004() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_04);
    const char* fileName = "ui_test_digital_clock_display_mode_04.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestDisplayLeadingZero001() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_LEADING_ZERO_LABEL_ID_01);
    const char* fileName = "ui_test_digital_clock_display_leading_zero_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestDigitalClock::UIKitUIDigitalClockTestSetOpacity001() const
{
    DragViewToHead(UI_TEST_DIGITAL_CLOCK_DISPLAY_SET_OPACITY_LABEL_ID_01);
    const char* fileName = "ui_test_digital_clock_set_opacity_01.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS