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

#ifndef UI_TEST_UI_LIST_VIEW_H
#define UI_TEST_UI_LIST_VIEW_H

#include "components/ui_label.h"
#include "components/ui_scroll_view.h"
#include "ui_test.h"

namespace OHOS {
namespace {
constexpr char* UI_TEST_LIST_LAYOUT_ADD_INSERT_LABEL_01 = "test_list_layout_add_insert_label_01";
constexpr char* UI_TEST_LIST_LAYOUT_REMOVE_LABEL_01 = "test_list_layout_remove_label_01";
constexpr char* UI_TEST_LIST_LAYOUT_ADD_INSERT_LABEL_02 = "test_list_layout_add_insert_label_02";
constexpr char* UI_TEST_LIST_LAYOUT_REMOVE_LABEL_02 = "test_list_layout_remove_label_02";
};
class UITestListLayout : public UITest {
public:
    UITestListLayout() {}
    ~UITestListLayout() {}
    void SetUp() override;
    void TearDown() override;
    const UIView* GetTestView() override;

    void UIKitListVerticalAddTestAutoSize001();
    void UIKitListVerticalRemoveTestAutoSize001();
    void UIKitListVerticalRemoveTestAutoSize002();
    void UIKitListHorAddTestAutoSize001();
    void UIKitListHorRemoveTestAutoSize001();
    void UIKitListHorRemoveTestAutoSize002();

private:
    void SetLastPos(UIView* view);
    UIScrollView* container_ = nullptr;
    int16_t lastX_ = 0;
    int16_t lastY_ = 0;
    UILabel* listDirect_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_INPUT_EVENT_H
