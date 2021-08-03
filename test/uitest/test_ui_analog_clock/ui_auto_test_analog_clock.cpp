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

#include "ui_auto_test_analog_clock.h"
#include "auto_test_counter.h"
#include "ui_test_analog_clock.h"

namespace OHOS {
void UIAutoTestAnalogClock::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Analog_Clock");
}

void UIAutoTestAnalogClock::RunTestList()
{
    UIKitAnalogClockTestStart001();
    UIKitAnalogClockTestMode002();
    UIKitAnalogClockTestLeftMove003();
    UIKitAnalogClockTestRightMove004();
    UIKitAnalogClockTestUpMove005();
    UIKitAnalogClockTestDownMove006();
}
void  UIAutoTestAnalogClock::UIKitAnalogClockTestStart001() const
{
    Reset();
    // Take the 5 data of 1, 20, 40, 60, 80 frames as the comparison point.
    uint16_t frameArray[5] = {1, 20, 40, 60, 80};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_analog_clock_star",
        frameArray, sizeof(frameArray));
}

void  UIAutoTestAnalogClock::UIKitAnalogClockTestMode002() const
{
    Reset();
    ClickViewById("test_analog_clock_set_mode_btn");
    const char* fileName = "ui_test_analog_clock_mode_001.bmp";
    CompareByBinary(fileName);
}

void  UIAutoTestAnalogClock::UIKitAnalogClockTestLeftMove003() const
{
    ClickViewById("test_analog_clock_left_move_btn");
    const char* fileName = "ui_test_analog_clock_left_move_001.bmp";
    CompareByBinary(fileName);
}

void  UIAutoTestAnalogClock::UIKitAnalogClockTestRightMove004() const
{
    ClickViewById("test_analog_clock_right_move_btn");
    const char* fileName = "ui_test_analog_clock_right_move_001.bmp";
    CompareByBinary(fileName);
}

void  UIAutoTestAnalogClock::UIKitAnalogClockTestUpMove005() const
{
    ClickViewById("test_analog_clock_up_move_btn");
    const char* fileName = "ui_test_analog_clock_up_move_001.bmp";
    CompareByBinary(fileName);
}

void  UIAutoTestAnalogClock::UIKitAnalogClockTestDownMove006() const
{
    ClickViewById("test_analog_clock_down_move_btn");
    const char* fileName = "ui_test_analog_clock_down_move_001.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS