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

#include "ui_auto_test_border_margin_padding.h"
#include "compare_tools.h"
#include "components/root_view.h"
#include "dfx/event_injector.h"
#include "securec.h"
#include "ui_test_border_margin_padding.h"

namespace OHOS {
void UIAutoTestBorderMarginPadding::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Border_Margin_Padding");
}

void UIAutoTestBorderMarginPadding::RunTestList()
{
    startPoint_.y = Screen::GetInstance().GetWidth();

    UIKitBorderMarginPaddingAllOff001();
    UIKitBorderMarginPaddingAllOn001();
    UIKitBorderMarginPaddingOnMargin001();
    UIKitBorderMarginPaddingOnBorder001();
    UIKitBorderMarginPaddingOnPadding001();
    UIKitBorderMarginPaddingOffMargin001();
    UIKitBorderMarginPaddingOffBorder001();
    UIKitBorderMarginPaddingOffPadding001();
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingCompare(const char* name) const
{
    if (name == nullptr) {
        return;
    }
    UIView* view = RootView::GetInstance()->GetChildById("ui_test_border_margin_padding_mark");
    if (view == nullptr && view->GetViewType() != UIViewType::UI_CHECK_BOX) {
        return;
    }
    char fileName[DEFAULT_FILE_NAME_MAX_LENGTH] = {0};
    uint16_t count = 0;
    static_cast<UICheckBox*>(view)->SetState(UICheckBox::UICheckBoxState::UNSELECTED);
    while (static_cast<UICheckBox*>(view)->GetState() != UICheckBox::UICheckBoxState::SELECTED) {
        EventInjector::GetInstance()->SetDragEvent(startPoint_, endPoint_, 300); // 300: dray time
        CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
        ClickViewById("ui_test_border_margin_padding_mark");
        if (sprintf_s(fileName, DEFAULT_FILE_NAME_MAX_LENGTH,
            "ui_test_border_margin_padding_%s_%d.bmp", name, count++) >= 0) {
            CompareByBinary(fileName);
        }
    }
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingAllOff001() const
{
    Reset();
    UIKitBorderMarginPaddingCompare("all_off");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingAllOn001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_MARGIN_BTN_ID_01);
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_BORDER_BTN_ID_01);
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_PADDING_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("all_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOnMargin001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_MARGIN_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("margin_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOnBorder001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_BORDER_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("border_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOnPadding001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_BORDER_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("padding_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOffMargin001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_BORDER_BTN_ID_01);
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_PADDING_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("margin_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOffBorder001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_MARGIN_BTN_ID_01);
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_PADDING_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("border_on");
}

void UIAutoTestBorderMarginPadding::UIKitBorderMarginPaddingOffPadding001() const
{
    Reset();
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_MARGIN_BTN_ID_01);
    ClickViewById(UI_TEST_BORDER_MARGIN_PADDING_BORDER_BTN_ID_01);
    UIKitBorderMarginPaddingCompare("padding_off");
}
} // namespace OHOS
