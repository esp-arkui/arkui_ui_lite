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

#include "components/ui_button.h"
#include "components/ui_checkbox.h"
#include "components/ui_scroll_view.h"
#include "ui_test.h"

namespace OHOS {
namespace {
constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_01 = "test_button_checkbox_01";
constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_02 = "test_button_checkbox_02";
constexpr char* UI_TEST_BUTTON_CHECKBOX_ID_03 = "test_button_checkbox_03";
constexpr char* UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_01 = "test_button_checkbox_image_01";
constexpr char* UI_TEST_BUTTON_CHECKBOX_IMAGE_ID_02 = "test_button_checkbox_image_02";
constexpr char* UI_TEST_RADIO_BUTTON_ID_01 = "test_radio_button_01";
constexpr char* UI_TEST_RADIO_BUTTON_ID_02 = "test_radio_button_02";
constexpr char* UI_TEST_RADIO_BUTTON_ID_03 = "test_radio_button_03";
constexpr char* UI_TEST_RADIO_BUTTON_IMAGE_ID_01 = "test_radio_button_image_01";
constexpr char* UI_TEST_RADIO_BUTTON_IMAGE_ID_02 = "test_radio_button_image_02";
constexpr char* UI_TEST_TOGGLE_BUTTON_ID_01 = "test_toggle_button_01";
constexpr char* UI_TEST_TOGGLE_BUTTON_ID_02 = "test_toggle_button_02";
constexpr char* UI_TEST_TOGGLE_BUTTON_ID_03 = "test_toggle_button_03";
constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_ID_01 = "test_toggle_button_image_01";
constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_ID_02 = "test_toggle_button_image_02";
constexpr char* UI_TEST_BUTTON_RESET_ID = "test_button_reset_01";
constexpr char* UI_TEST_BUTTON_BIG_ID_01 = "test_button_big_01";
constexpr char* UI_TEST_BUTTON_SMALL_ID_01 = "test_button_small_01";
constexpr char* UI_TEST_BUTTON_LIFT_ID_01 = "test_button_lift_01";
constexpr char* UI_TEST_BUTTON_RIGHT_ID_01 = "test_button_right_01";
constexpr char* UI_TEST_BUTTON_UP_ID_01 = "test_button_up_01";
constexpr char* UI_TEST_BUTTON_DOWN_ID_01 = "test_button_down_01";
constexpr char* UI_TEST_BUTTON_RED_ID_01 = "test_button_red_01";
constexpr char* UI_TEST_BUTTON_WHITE_ID_01 = "test_button_white_01";
constexpr char* UI_TEST_BUTTON_HIDE_ID_01 = "test_button_hide_01";
constexpr char* UI_TEST_BUTTON_DISPLAY_ID_01 = "test_button_display_01";
constexpr char* UI_TEST_CHECKBOX_IMAGE_LABEL_ID = "test_checkbox_image_label_01";
constexpr char* UI_TEST_TOGGLE_BUTTON_IMAGE_LABEL_ID = "test_toggle_button_image_label_01";
}
class UITestBUTTON : public UITest {
public:
    UITestBUTTON() {}
    ~UITestBUTTON() {}
    void SetUp() override;
    void TearDown() override;
    const UIView* GetTestView() override;

    /**
     * @brief Test Checkbox Function
     */
    void UIKitCheckBoxTest001();

    /**
     * @brief Test Checkbox's SetImage Function
     */
    void UIKitCheckBoxTest002() const;

    /**
     * @brief Test Radiobutton Function
     */
    void UIKitRadioButtonTest001();

    /**
     * @brief Test Radiobutton's SetImage Function
     */
    void UIKitRadioButtonTest002() const;

    /**
     * @brief Test Togglebutton Function
     */
    void UIKitToggleButtonTest001();

    /**
     * @brief Test Togglebutton's SetImage Function
     */
    void UIKitToggleButtonTest002();

    /**
     * @brief Test button Function
     */
    void UIKitButtonTest001();

private:
    void UIKitButtonTest002(UIScrollView* container, UIButton* button);
    UIViewGroup* CreateButtonGroup(int16_t posX, int16_t posY, int16_t width, int16_t height,
                                    UICheckBox::OnChangeListener** listener,
                                    UIViewType type = UI_CHECK_BOX,
                                    const char* name = "aa",
                                    const char* id = nullptr);
    static constexpr int16_t CHANGE_SIZE = 10;
    UIScrollView* container_ = nullptr;

    UICheckBox::OnChangeListener* checkBoxChangeListener_ = nullptr;
    UICheckBox::OnChangeListener* checkBoxChangeListener1_ = nullptr;
    UICheckBox::OnChangeListener* checkBoxChangeListener2_ = nullptr;
    UICheckBox::OnChangeListener* radioChangeListener_ = nullptr;
    UICheckBox::OnChangeListener* radioChangeListener1_ = nullptr;
    UICheckBox::OnChangeListener* radioChangeListener2_ = nullptr;
    UICheckBox::OnChangeListener* toggleChangeListener_ = nullptr;
    UICheckBox::OnChangeListener* toggleChangeListener1_ = nullptr;
    UICheckBox::OnChangeListener* toggleChangeListener2_ = nullptr;

    UIView::OnClickListener* clickEnableVisiableListener_ = nullptr;
    UIView::OnClickListener* clickDisableVisiableListener_ = nullptr;
    UIView::OnClickListener* clickEnableTouchableListener_ = nullptr;
    UIView::OnClickListener* clickDisableTouchableListener_ = nullptr;
    UIView::OnClickListener* clickColorToWhiteListener_ = nullptr;
    UIView::OnClickListener* clickColorToRedListener_ = nullptr;
    UIView::OnClickListener* clickRevetColorListener_ = nullptr;
    UIView::OnClickListener* clickRevetToOriginListener_ = nullptr;
    UIView::OnClickListener* clickBigListener_ = nullptr;
    UIView::OnClickListener* clickLeftListener_ = nullptr;
    UIView::OnClickListener* clickRightListener_ = nullptr;
    UIView::OnClickListener* clickUpListener_ = nullptr;
    UIView::OnClickListener* clickDownListener_ = nullptr;
    UIView::OnClickListener* clickSmallListener_ = nullptr;
    UIView::OnClickListener* enableAnimationListener_ = nullptr;
    UIView::OnClickListener* disableAnimationListener_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_BUTTON_H
