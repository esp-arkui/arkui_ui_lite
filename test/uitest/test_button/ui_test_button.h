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

#ifndef UI_TEST_BUTTON_H
#define UI_TEST_BUTTON_H

#include "components/ui_scroll_view.h"
#include "ui_test.h"

namespace OHOS {
namespace {
    constexpr char* UI_TEST_BUTTON_ID = "Button";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_LABEL_ID = "test_button_checkbox_label";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_01 = "test_button_checkbox_01";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_02 = "test_button_checkbox_02";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_03 = "test_button_checkbox_03";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_IMAGE_LABEL_ID = "test_button_checkbox_image_label";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01 = "test_button_checkbox_image_01";
    constexpr char* UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02 = "test_button_checkbox_image_02";
    constexpr char* UI_TEST_RADIO_BUTTON_LABEL_ID = "test_radio_button_label";
    constexpr char* UI_TEST_RADIO_BUTTON_ID_01 = "test_radio_button_01";
    constexpr char* UI_TEST_RADIO_BUTTON_ID_02 = "test_radio_button_02";
    constexpr char* UI_TEST_RADIO_BUTTON_ID_03 = "test_radio_button_03";
    constexpr char* UI_TEST_RADIO_BUTTON_IMAGE_LABEL_ID = "test_radio_button_image_label";
    constexpr char* UI_TEST_RADIO_BUTTON_IMAGE_ID_01 = "test_radio_button_image_01";
    constexpr char* UI_TEST_RADIO_BUTTON_IMAGE_ID_02 = "test_radio_button_image_02";
    constexpr char* UI_TEST_TOGGLE_BUTTON_LABEL_ID = "test_radio_toggle_button_label";
    constexpr char* UI_TEST_TOGGLE_BUTTON_ID_01 = "test_toggle_button_01";
    constexpr char* UI_TEST_TOGGLE_BUTTON_ID_02 = "test_toggle_button_02";
    constexpr char* UI_TEST_TOGGLE_BUTTON_ID_03 = "test_toggle_button_03";
    constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_LABEL_ID = "test_toggle_button_image_label";
    constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01 = "test_toggle_button_image_01";
    constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02 = "test_toggle_button_image_02";
}
class UITestBUTTON : public UITest {
public:
    UITestBUTTON() {}
    ~UITestBUTTON() {}
    void SetUp() override;
    void TearDown() override;
    UIView* GetTestView() override;

    /**
     * @brief Test Checkbox Function
     */
    void UIKit_Check_Box_Test_001() const;

    /**
     * @brief Test Checkbox's SetImage Function
     */
    void UIKit_Check_Box_Test_002() const;

    /**
     * @brief Test Radiobutton Function
     */
    void UIKit_Radio_Button_Test_001() const;

    /**
     * @brief Test Radiobutton's SetImage Function
     */
    void UIKit_Radio_Button_Test_002() const;

    /**
     * @brief Test Togglebutton Function
     */
    void UIKit_Toggle_Button_Test_001() const;

    /**
     * @brief Test Togglebutton's SetImage Function
     */
    void UIKit_Toggle_Button_Test_002() const;

    /**
     * @brief Test button Function
     */
    void UIKit_Button_Test_001() const;

private:
    static constexpr int16_t CHANGE_SIZE = 10;
    UIScrollView* container_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_BUTTON_H
