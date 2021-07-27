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

#ifndef UI_TEST_UI_PICKER_H
#define UI_TEST_UI_PICKER_H

#include "components/text_adapter.h"
#include "components/ui_label.h"
#include "components/ui_label_button.h"
#include "components/ui_picker.h"
#include "components/ui_scroll_view.h"
#include "components/ui_time_picker.h"
#include "ui_test.h"

namespace OHOS {
namespace {
constexpr char* UI_TEST_PICKER_UI_PICKER_ID_01 = "test_picker_ui_picker_01";
constexpr char* UI_TEST_PICKER_UI_PICKER_ID_02 = "test_picker_ui_picker_02";
constexpr char* UI_TEST_PICKER_UI_PICKER_ID_03 = "test_picker_ui_picker_03";
constexpr char* UI_TEST_PICKER_POSITIONING_BTN_ID_01 = "test_picker_positioning_01";
constexpr char* UI_TEST_PICKER_LEFT_TO_RIGHT_BTN_ID_01 = "test_picker_left_to_right_01";
constexpr char* UI_TEST_PICKER_RIGHT_TO_LEFT_BTN_ID_01 = "test_picker_right_to_left_01";
constexpr char* UI_TEST_PICKER_LOOP_ON_BTN_ID_01 = "test_picker_loop_on_01";
constexpr char* UI_TEST_PICKER_LOOP_OFF_BTN_ID_01 = "test_picker_loop_off_01";
}
class UITestUIPicker : public UITest, UIView::OnClickListener, UIPicker::SelectedListener,
    UITimePicker::SelectedListener {
public:
    UITestUIPicker() {}
    ~UITestUIPicker() {}
    void SetUp() override;
    void TearDown() override;
    bool OnClick(UIView& view, const ClickEvent& event) override;
    const UIView* GetTestView() override;
    void OnPickerStoped(UIPicker& picker) override;
    void OnTimePickerStoped(UITimePicker& picker) override;
    void CreatButtons();
    void UIKitPickerTestBase001();
    void UIKitPickerTestBase002();
    void UIKitPickerTestTimePicker001();

private:
    void SetLastPos(UIView* view);
    void SetUpButton(UILabelButton* btn, const char* title, const char* id = nullptr);
    UILabelButton* setLoopBtn_ = nullptr;
    UILabelButton* setLoopOffBtn_ = nullptr;
    UILabelButton* setSelectBtn_ = nullptr;
    UILabelButton* setLeftToRightBtn_ = nullptr;
    UILabelButton* setRightToLeftBtn_ = nullptr;
    UILabel* selectIndex_ = nullptr;
    UILabel* selectTime_ = nullptr;
    UIScrollView* container_ = nullptr;
    UIPicker* picker1_ = nullptr;
    UIPicker* picker3_ = nullptr;
    UITimePicker* picker2_ = nullptr;
    int16_t lastX_ = 0;
    int16_t lastY_ = 0;
};
} // namespace OHOS
#endif // UI_TEST_UI_PICKER_H
