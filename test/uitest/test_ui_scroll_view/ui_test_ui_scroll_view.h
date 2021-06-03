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

#ifndef UI_TEST_UI_SCROLL_VIEW_H
#define UI_TEST_UI_SCROLL_VIEW_H

#include "components/ui_checkbox.h"
#include "components/ui_label.h"
#include "components/ui_scroll_view.h"
#include "layout/grid_layout.h"
#include "ui_test.h"

namespace OHOS {
class UITestUIScrollView : public UITest,
                           public UICheckBox::OnChangeListener,
                           public UIScrollView::OnScrollListener,
                           public UIView::OnClickListener {
public:
    UITestUIScrollView() {}
    virtual ~UITestUIScrollView() {}
    void SetUp() override;
    void TearDown() override;
    const UIView* GetTestView() override;
    bool OnChange(UIView* view, UICheckBox::UICheckBoxState state) override;
    void OnScrollStart() override;
    void OnScrollEnd() override;
    bool OnClick(UIView& view, const ClickEvent& event) override;
    void UIKit_UIScrollView_Test_001();

private:
    UIViewGroup* container_ = nullptr;
    GridLayout* layoutButton_ = nullptr;
    UIScrollView* scroll_ = nullptr;
    UILabel* scrollSateLabel_ = nullptr;
    UILabelButton* button_ = nullptr;
    int16_t buttonHeigh_ = 600;
    int16_t buttonWidth_ = 600;
};
} // namespace OHOS
#endif // UI_TEST_UI_SCROLL_VIEW_H
