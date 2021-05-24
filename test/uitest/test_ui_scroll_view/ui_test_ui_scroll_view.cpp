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

#include "common/screen.h"
#include "components/ui_label_button.h"
#include "components/ui_toggle_button.h"
#include "ui_test_ui_scroll_view.h"

namespace OHOS {
const char* SIZE_INC_BTN = "size inc btn";
const char* SIZE_DEC_BTN = "size dec btn";
const char* BLANK_SIZE_BTN = "blank size btn";
const char* REBOUND_SIZE_BTN = "rebound size btn";
const char* X_SCROLL_BTN = "x scroll btn";
const char* Y_SCROLL_BTN = "y scroll btn";
const char* SCROLL_THROW_BTN = "scroll throw";
const char* X_SLIDER_BTN = "x slider btn";
const char* Y_SLIDER_BTN = "y slider btn";
const char* SCROLL_BY_X_BTN = "scroll by x btn";
const char* SCROLL_BY_Y_BTN = "scroll by y btn";
} // namespace OHOS
namespace OHOS {
void UITestUIScrollView::SetUp()
{
    if (container_ == nullptr) {
        container_ = new UIViewGroup();
        container_->Resize(Screen::GetInstance().GetWidth(), Screen::GetInstance().GetHeight() - BACK_BUTTON_HEIGHT);
    }
    if (scroll_ == nullptr) {
        scroll_ = new UIScrollView();
        scroll_->Resize(500, 400);
        scroll_->SetIntercept(true);
        scroll_->SetStyle(STYLE_BACKGROUND_COLOR, Color::Red().full);
        scroll_->RegisterScrollListener(this);
    }
    if (layoutButton_ == nullptr) {
        layoutButton_ = new GridLayout();
        layoutButton_->Resize(400, 400);
        layoutButton_->SetRows(6); // 3: rows
        layoutButton_->SetCols(4); // 4: cols
    }
    if (scrollSateLabel_ == nullptr) {
        scrollSateLabel_ = new UILabel();
        scrollSateLabel_->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
        scrollSateLabel_->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
        scrollSateLabel_->SetText("stop");
        scrollSateLabel_->ReMeasure();
    }
    if (button_ == nullptr) {
        button_ = new UILabelButton();
        button_->SetText("Button");
        button_->Resize(buttonWidth_, buttonHeigh_);
        scroll_->Add(button_);
    }
    container_->Add(scroll_);
    container_->Add(layoutButton_);
    scroll_->LayoutLeftOfParent(20);
    layoutButton_->LayoutRightOfParent();
}

void UITestUIScrollView::TearDown()
{
    DeleteChildren(container_);
    container_ = nullptr;
    scroll_ = nullptr;
    layoutButton_ = nullptr;
    scrollSateLabel_ = nullptr;
    button_ = nullptr;
}

const UIView* UITestUIScrollView::GetTestView()
{
    UIKit_UIScrollView_Test_001();
    return container_;
}

void UITestUIScrollView::UIKit_UIScrollView_Test_001()
{
    UILabel* scrollState = new UILabel();
    scrollState->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    scrollState->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    scrollState->SetText("state: ");
    scrollState->ReMeasure();
    layoutButton_->Add(scrollState);
    layoutButton_->Add(scrollSateLabel_);

    UILabelButton* btnInc = new UILabelButton();
    btnInc->SetViewId(SIZE_INC_BTN);
    btnInc->Resize(100, 30);
    btnInc->SetText("inc size");
    btnInc->SetOnClickListener(this);
    layoutButton_->Add(btnInc);

    UILabelButton* btnDec = new UILabelButton();
    btnDec->SetViewId(SIZE_DEC_BTN);
    btnDec->Resize(100, 30);
    btnDec->SetText("dec size");
    btnDec->SetOnClickListener(this);
    layoutButton_->Add(btnDec);

    UILabel* blankLabel = new UILabel();
    blankLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    blankLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    blankLabel->SetText("blank ");
    blankLabel->ReMeasure();
    layoutButton_->Add(blankLabel);
    UIToggleButton* blankBtn = new UIToggleButton();
    blankBtn->SetViewId(BLANK_SIZE_BTN);
    blankBtn->SetOnChangeListener(this);
    layoutButton_->Add(blankBtn);

    UILabel* reboundLabel = new UILabel();
    reboundLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    reboundLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    reboundLabel->SetText("rebound ");
    reboundLabel->ReMeasure();
    layoutButton_->Add(reboundLabel);
    UIToggleButton* reboundBtn = new UIToggleButton();
    reboundBtn->SetViewId(REBOUND_SIZE_BTN);
    reboundBtn->SetOnChangeListener(this);
    layoutButton_->Add(reboundBtn);

    UILabel* xScrollLabel = new UILabel();
    xScrollLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    xScrollLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    xScrollLabel->SetText("xScroll ");
    xScrollLabel->ReMeasure();
    layoutButton_->Add(xScrollLabel);
    UIToggleButton* xScrollBtn = new UIToggleButton();
    xScrollBtn->SetViewId(X_SCROLL_BTN);
    xScrollBtn->SetOnChangeListener(this);
    xScrollBtn->SetState(true);
    layoutButton_->Add(xScrollBtn);

    UILabel* yScrollLabel = new UILabel();
    yScrollLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    yScrollLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    yScrollLabel->SetText("yScroll ");
    yScrollLabel->ReMeasure();
    layoutButton_->Add(yScrollLabel);
    UIToggleButton* yScrollBtn = new UIToggleButton();
    yScrollBtn->SetViewId(Y_SCROLL_BTN);
    yScrollBtn->SetOnChangeListener(this);
    yScrollBtn->SetState(true);
    layoutButton_->Add(yScrollBtn);

    UILabel* throwLabel = new UILabel();
    throwLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    throwLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    throwLabel->SetText("throw ");
    throwLabel->ReMeasure();
    layoutButton_->Add(throwLabel);
    UIToggleButton* throwBtn = new UIToggleButton();
    throwBtn->SetViewId(SCROLL_THROW_BTN);
    throwBtn->SetOnChangeListener(this);
    throwBtn->SetState(true);
    layoutButton_->Add(throwBtn);

    UILabel* xSliderLabel = new UILabel();
    xSliderLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    xSliderLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    xSliderLabel->SetText("xSlider ");
    xSliderLabel->ReMeasure();
    layoutButton_->Add(xSliderLabel);
    UIToggleButton* xSliderBtn = new UIToggleButton();
    xSliderBtn->SetViewId(X_SLIDER_BTN);
    xSliderBtn->SetOnChangeListener(this);
    xSliderBtn->SetState(false);
    layoutButton_->Add(xSliderBtn);

    UILabel* ySliderLabel = new UILabel();
    ySliderLabel->SetLineBreakMode(UILabel::LINE_BREAK_ADAPT);
    ySliderLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    ySliderLabel->SetText("ySlider ");
    ySliderLabel->ReMeasure();
    layoutButton_->Add(ySliderLabel);
    UIToggleButton* ySliderBtn = new UIToggleButton();
    ySliderBtn->SetViewId(Y_SLIDER_BTN);
    ySliderBtn->SetOnChangeListener(this);
    ySliderBtn->SetState(false);
    layoutButton_->Add(ySliderBtn);

    UILabelButton* scrollByX = new UILabelButton();
    scrollByX->SetViewId(SCROLL_BY_X_BTN);
    scrollByX->Resize(100, 30);
    scrollByX->SetText("scrollBy X");
    scrollByX->SetOnClickListener(this);
    layoutButton_->Add(scrollByX);

    UILabelButton* scrollByY = new UILabelButton();
    scrollByY->SetViewId(SCROLL_BY_Y_BTN);
    scrollByY->Resize(100, 30);
    scrollByY->SetText("scrollBy Y");
    scrollByY->SetOnClickListener(this);
    layoutButton_->Add(scrollByY);

    layoutButton_->LayoutChildren();
}

void UITestUIScrollView::OnScrollStart()
{
    if (scrollSateLabel_ != nullptr) {
        scrollSateLabel_->SetText("moving");
        scrollSateLabel_->Invalidate();
    }
}

void UITestUIScrollView::OnScrollEnd()
{
    if (scrollSateLabel_ != nullptr) {
        scrollSateLabel_->SetText("stop");
        scrollSateLabel_->Invalidate();
    }
}

bool UITestUIScrollView::OnChange(UIView* view, UICheckBox::UICheckBoxState state)
{
    if (view->GetViewId() == nullptr) {
        return false;
    }

    if (strcmp(view->GetViewId(), REBOUND_SIZE_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetReboundSize(50);
        } else {
            scroll_->SetReboundSize(0);
        }
    } else if (strcmp(view->GetViewId(), BLANK_SIZE_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetScrollBlankSize(50);
        } else {
            scroll_->SetScrollBlankSize(0);
        }
    } else if (strcmp(view->GetViewId(), X_SCROLL_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetHorizontalScrollState(true);
        } else {
            scroll_->SetHorizontalScrollState(false);
        }
    } else if (strcmp(view->GetViewId(), Y_SCROLL_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetVerticalScrollState(true);
        } else {
            scroll_->SetVerticalScrollState(false);
        }
    } else if (strcmp(view->GetViewId(), SCROLL_THROW_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetThrowDrag(true);
        } else {
            scroll_->SetThrowDrag(false);
        }
    } else if (strcmp(view->GetViewId(), X_SLIDER_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetXScrollBarVisible(true);
        } else {
            scroll_->SetXScrollBarVisible(false);
        }
    } else if (strcmp(view->GetViewId(), Y_SLIDER_BTN) == 0) {
        if (state == UICheckBox::SELECTED) {
            scroll_->SetYScrollBarVisible(true);
        } else {
            scroll_->SetYScrollBarVisible(false);
        }
    }
    container_->Invalidate();
    return false;
}

bool UITestUIScrollView::OnClick(UIView& view, const ClickEvent& event)
{
    if (view.GetViewId() == nullptr) {
        return false;
    }
    if (strcmp(view.GetViewId(), SIZE_INC_BTN) == 0) {
        buttonWidth_ += 20;
        buttonHeigh_ += 20;
        scroll_->Remove(button_);
        button_->Resize(buttonWidth_, buttonHeigh_);
        scroll_->Add(button_);
    } else if (strcmp(view.GetViewId(), SIZE_DEC_BTN) == 0) {
        buttonWidth_ -= 20;
        buttonHeigh_ -= 20;
        scroll_->Remove(button_);
        button_->Resize(buttonWidth_, buttonHeigh_);
        scroll_->Add(button_);
    } else if (strcmp(view.GetViewId(), SCROLL_BY_X_BTN) == 0) {
        scroll_->ScrollBy(20, 0);
    } else if (strcmp(view.GetViewId(), SCROLL_BY_Y_BTN) == 0) {
        scroll_->ScrollBy(0, 20);
    }
    container_->Invalidate();
    return false;
}

} // namespace OHOS
