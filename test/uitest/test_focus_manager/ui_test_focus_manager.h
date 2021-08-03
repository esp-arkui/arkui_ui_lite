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

#ifndef UI_TEST_FOCUS_MANAGER_H
#define UI_TEST_FOCUS_MANAGER_H

#include "graphic_config.h"
#if ENABLE_FOCUS_MANAGER
#include "components/ui_label_button.h"
#include "components/ui_scroll_view.h"
#include "dock/focus_manager.h"
#include "ui_test.h"

namespace OHOS {
namespace {
constexpr char* UI_TEST_FOCUS_MANAGER_LEFT_BTN_ID_01 = "test_focus_manager_left_01";
constexpr char* UI_TEST_FOCUS_MANAGER_RIGHT_BTN_ID_01 = "test_focus_manager_right_01";
constexpr char* UI_TEST_FOCUS_MANAGER_UP_BTN_ID_01 = "test_focus_manager_up_01";
constexpr char* UI_TEST_FOCUS_MANAGER_DOWN_BTN_ID_01 = "test_focus_manager_down_01";
constexpr char* UI_TEST_FOCUS_MANAGER_FOCUS_BTN_ID_01 = "test_focus_manager_focus_01";
constexpr char* UI_TEST_FOCUS_MANAGER_UNFOCUSED_BTN_ID_01 = "test_focus_manager_unfocused_01";
constexpr char* UI_TEST_FOCUS_MANAGER_INTERCEPT_BTN_ID_01 = "test_focus_manager_intercept_01";
constexpr char* UI_TEST_FOCUS_MANAGER_UNBLOCK_BTN_ID_01 = "test_focus_manager_unblock_01";
constexpr char* UI_TEST_FOCUS_MANAGER_RESET_BTN_ID_01 = "test_focus_manager_reset_01";
constexpr char* UI_TEST_FOCUS_MANAGER_CLEAR_BTN_ID_01 = "test_focus_manager_clear_01";
}
class UITestFocusManager : public UITest {
public:
    UITestFocusManager() {}
    ~UITestFocusManager() {}
    void SetUp() override;
    void TearDown() override;
    const UIView* GetTestView() override;

    void UIKitFocusManagerTest001();

private:
    UIView* CreateTestUILabel(UIViewGroup* parent, int16_t x, int16_t y,
        const char* text, bool focusable);
    UIViewGroup* CreateTestUIViewGroup(UIViewGroup* parent, bool focusable, bool interceptFocus);
    UILabelButton* SetUpButton(const char* title, int16_t x, int16_t y, UIViewGroup* viewGroup,
        UIView::OnClickListener* listener, const char* id = nullptr);
    UIScrollView* container_ = nullptr;
    UIView::OnFocusListener* testOnFocusListener_ = nullptr;
    UIView::OnClickListener* requestFocusByDirectionLeftListener_ = nullptr;
    UIView::OnClickListener* requestFocusByDirectionRightListener_ = nullptr;
    UIView::OnClickListener* requestFocusByDirectionUpListener_ = nullptr;
    UIView::OnClickListener* requestFocusByDirectionDownListener_ = nullptr;
    UIView::OnClickListener* setFocusableViewListener_ = nullptr;
    UIView::OnClickListener* setFocusableViewListener1_ = nullptr;
    UIView::OnClickListener* setGroupInterceptListener_ = nullptr;
    UIView::OnClickListener* setGroupInterceptListener1_ = nullptr;
    UIView::OnClickListener* resetFocusListener_ = nullptr;
    UIView::OnClickListener* clearFocusListener_ = nullptr;
};
} // namespace OHOS
#endif
#endif // UI_TEST_FOCUS_MANAGER_H