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

#ifndef UI_TEST_DIGITAL_CLOCK_H
#define UI_TEST_DIGITAL_CLOCK_H

#include "components/ui_scroll_view.h"
#include "font/ui_font.h"
#include "ui_test.h"

namespace OHOS {
namespace {
    constexpr char* UI_TEST_DIGITAL_CLOCK_ID = "Digital_Clock";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_LABEL_ID_01 = "test_digital_clock_display_01";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_LABEL_ID_02 = "test_digital_clock_display_02";
    constexpr char* UI_TEST_DIGITAL_CLOCK_SET_COLOR_LABEL_ID_01 = "test_digital_clock_set_color_01";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_01 = "test_digital_clock_display_mode_01";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_02 = "test_digital_clock_display_mode_02";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_03 = "test_digital_clock_display_mode_03";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_MODE_LABEL_ID_04 = "test_digital_clock_display_mode_04";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_LEADING_ZERO_LABEL_ID_01 =
        "test_digital_clock_display_leading_zero_01";
    constexpr char* UI_TEST_DIGITAL_CLOCK_DISPLAY_SET_OPACITY_LABEL_ID_01 =
        "test_digital_clock_set_opacity_01";
}
class UITestDigitalClock : public UITest {
public:
    UITestDigitalClock() {}
    ~UITestDigitalClock() {}
    void SetUp() override;
    void TearDown() override;
    UIView* GetTestView() override;

    void UIKit_UIDigitalClock_Test_Display_001();
    void UIKit_UIDigitalClock_Test_Display_002();
    void UIKit_UIDigitalClock_Test_SetColor();
    void UIKit_UIDigitalClock_Test_DisplayMode_001();
    void UIKit_UIDigitalClock_Test_DisplayMode_002();
    void UIKit_UIDigitalClock_Test_DisplayMode_003();
    void UIKit_UIDigitalClock_Test_DisplayMode_004();
    void UIKit_UIDigitalClock_Test_DisplayLeadingZero();
    void UIKit_UIDigitalClock_Test_SetOpacity();

private:
    UIScrollView* container_ = nullptr;
    void InnerTestTitle(const char* title, const char* id = nullptr);
};
} // namespace OHOS
#endif // UI_TEST_DIGITAL_CLOCK_H
