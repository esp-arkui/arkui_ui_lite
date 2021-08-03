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

#include "ui_test_draw_rect.h"
#include "common/screen.h"
#include "components/ui_button.h"
#include "components/ui_label.h"
#include "components/ui_view_group.h"

namespace OHOS {
void UITestDrawRect::SetUp()
{
    if (container_ == nullptr) {
        container_ = new UIScrollView();
        container_->Resize(Screen::GetInstance().GetWidth(), Screen::GetInstance().GetHeight() - BACK_BUTTON_HEIGHT);
        container_->SetHorizontalScrollState(false);
    }
}

void UITestDrawRect::TearDown()
{
    DeleteChildren(container_);
    container_ = nullptr;
}

const UIView* UITestDrawRect::GetTestView()
{
    UIKitDrawRectTestRectNoBorder001();
    UIKitDrawRectTestRectNoBorder002();
    UIKitDrawRectTestRectNoBorder003();
    UIKitDrawRectTestRectNoBorder004();
    UIKitDrawRectTestRectNoBorder005();
    UIKitDrawRectTestRectNoBorder006();
    UIKitDrawRectTestRectNoBorder007();
    UIKitDrawRectTestRectNoBorder008();
    UIKitDrawRectTestRectNoBorder009();
    UIKitDrawRectTestRectNoBorder0010();
    UIKitDrawRectTestRectBorderNoRadius001();
    UIKitDrawRectTestRectBorderNoRadius002();
    UIKitDrawRectTestRectBorderRadius001();
    UIKitDrawRectTestRectBorderRadius002();
    UIKitDrawRectTestRectBorderRadius003();
    UIKitDrawRectTestRectBorderRadius004();
    UIKitDrawRectTestRectBorderRadius005();
    UIKitDrawRectTestRectBorderRadius006();
    UIKitDrawRectTestRectBorderRadius007();
    UIKitDrawRectTestRectBorderRadius008();
    UIKitDrawRectTestRectBorderRadius009();
    UIKitDrawRectTestRectBorderRadius010();
    UIKitDrawRectTestRectBorderRadius011();

    UIKitDrawRectTestRect001();
    UIKitDrawRectTestRectColor001();
    UIKitDrawRectTestRectRadius001();
    UIKitDrawRectTestRectRadius002();
    UIKitDrawRectTestRectRadius003();
    UIKitDrawRectTestRectRadius004();
    UIKitDrawRectTestRectOpacity001();
    UIKitDrawRectTestRectOpacity002();
    UIKitDrawRectTestRectOpacity003();
    UIKitDrawRectTestRectBorderColor001();
    UIKitDrawRectTestRectBorderWidth001();
    UIKitDrawRectTestRectBorderWidth002();
    UIKitDrawRectTestRectBorderOpacity001();
    UIKitDrawRectTestRectBorderOpacity002();
    UIKitDrawRectTestRectBorderOpacity003();
    UIKitDrawRectTestRectPadding001();
    UIKitDrawRectTestRectPadding002();
    UIKitDrawRectTestRectMargin001();
    UIKitDrawRectTestRectMargin002();
    UIKitDrawRectTestRectPaddingMargin001();
    UIKitDrawRectTestRectPaddingMargin002();
    UIKitDrawRectTestRectWidthAndHeight001();
    UIKitDrawRectTestRectWidthAndHeight002();
    UIKitDrawRectTestRectWidthAndHeight003();
    return container_;
}

UIViewGroup* UITestDrawRect::CreateTestCaseGroup() const
{
    UIViewGroup* group = new UIViewGroup();
    group->Resize(Screen::GetInstance().GetWidth(), 180); // 180 : value
    return group;
}

UILabel* UITestDrawRect::CreateTitleLabel() const
{
    UILabel* label = new UILabel();
    label->SetPosition(TEXT_DISTANCE_TO_LEFT_SIDE, TEXT_DISTANCE_TO_TOP_SIDE,
        Screen::GetInstance().GetWidth() - VIEW_DISTANCE_TO_LEFT_SIDE, TITLE_LABEL_DEFAULT_HEIGHT);
    label->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    return label;
}

UIView* UITestDrawRect::CreateUIView() const
{
    UIView* view = new UIView();
    view->SetStyle(STYLE_BACKGROUND_COLOR, Color::Green().full);
    view->SetStyle(STYLE_BACKGROUND_OPA, 127); // 127 : background opacity
    view->SetStyle(STYLE_BORDER_RADIUS, 5); // 5 : radius value
    view->SetStyle(STYLE_BORDER_COLOR, Color::Gray().full);
    view->SetStyle(STYLE_BORDER_OPA, 127); // 127 : border opacity
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : border width
    view->SetStyle(STYLE_PADDING_LEFT, 0);
    view->SetStyle(STYLE_PADDING_RIGHT, 0);
    view->SetStyle(STYLE_PADDING_TOP, 0);
    view->SetStyle(STYLE_PADDING_BOTTOM, 0);
    view->SetStyle(STYLE_MARGIN_LEFT, 0);
    view->SetStyle(STYLE_MARGIN_RIGHT, 0);
    view->SetStyle(STYLE_MARGIN_TOP, 0);
    view->SetStyle(STYLE_MARGIN_BOTTOM, 0);
    return view;
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder001");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius=0");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, 0);
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder002");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width > height, radius > height/2");
    group->Add(label);
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_02);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 + 10); // 2 : half height 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder003");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width > height, radius = height/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width size
    int16_t height = 50; // 50 : height size
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2); // 2 : half height
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder004()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder004");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width > height, radius < height/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_04);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width size
    int16_t height = 50; // 50 : height size
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 - 10); // 2 : half height 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder003", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder005()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder005");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width = height, radius > height/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_05);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = width;
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 + 10); // 2 : half height 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder004", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder006()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder006");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width = height, radius = height/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_06);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = width;
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2); // 2 : half
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder005", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder007()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder007");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width = height, radius < height/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_07);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = width;
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 - 10); // 2 : half height 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder006", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder008()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder008");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width < height, radius > width/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_08);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 120; // 120 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + 10); // 2 : half width 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder007", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder009()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder009");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width < height, radius = width/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_09);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 51; // 51 : width
    int16_t height = 120; // 120 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2); // 2 : half width
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder008", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectNoBorder0010()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectNoBorder0010");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形无border, radius>0, width < height, radius < width/2 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_10);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 120; // 120 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 - 10); // 2 : half width 10 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder009", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderNoRadius001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderNoRadius001");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形有border, radius=0");
    label->SetViewId(UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 10); // 10 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 0);
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectNoBorder0010", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderNoRadius002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderNoRadius002");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形有border = 1, radius=0");
    label->SetViewId(UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 1); // 1 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 0);
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderNoRadius001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius001");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形有border, radius<border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, borderWidth - 5); // 5 : math_num
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderNoRadius001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius002");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度 >高度，radius > 高度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 + borderWidth + 5); // 2 : half height 5 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius003");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度>高度，radius = 高度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 + borderWidth); // 2 : half height
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius004()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius004");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度>高度，radius < 高度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_04);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, height / 2 + borderWidth - 5); // 2 : half height 5 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius003", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius005()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius005");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度=高度，radius > 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_05);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth + 5); // 2 : half width 5 ： offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius004", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius006()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius006");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度=高度，radius = 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_06);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth); // 2 : half width
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius005", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius007()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius007");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度=高度，radius < 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_07);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth - 5); // 2 : half width 5 : offset
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius006", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius008()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius008");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度<高度，radius > 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_08);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 100; // 100 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth + 5); // 2 : half width 5 : math_num
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius007", 10); // 10 : value
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius009()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius009");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度<高度，radius = 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_09);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 100; // 100 : height
    int16_t borderWidth = 10; // 10 : value
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth); // 2 : math_num
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius008", 10); // 10 : value
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius010()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius010");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("border场景，radius>border.width，宽度<高度，radius < 宽度/2 + border.width");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_10);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 50; // 50 : width
    int16_t height = 100; // 100 : height
    int16_t borderWidth = 10;   // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, borderWidth);
    view->SetStyle(STYLE_BORDER_RADIUS, width / 2 + borderWidth - 5); // 2 : math_num 5 : math_num
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius009", 10); // 10 : value
}

void UITestDrawRect::UIKitDrawRectTestRectBorderRadius011()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderRadius011");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形有border = 2, radius = 1");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_11);
    group->Add(label);

    UIView* view = CreateUIView();
    int16_t width = 120; // 120 : width
    int16_t height = 50; // 50 : height
    int16_t borderWidth = 10; // 10 : borderWidth
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 2); // 2: borderWidth
    view->SetStyle(STYLE_BORDER_RADIUS, 1); // 1 : math_num
    view->Resize(width, height);
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius010", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRect001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRect001");
    group->SetPosition(0, 0);

    UILabel* label = CreateTitleLabel();
    label->SetText("矩形绘制: ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECT_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 15); // 15 : value
    view->SetStyle(STYLE_BORDER_OPA, OPA_OPAQUE);
    view->SetStyle(STYLE_BORDER_RADIUS, 10); // 10 : offset size
    view->Resize(120, 50);   // 120 : width 50 : weight
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderRadius009", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectColor001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectColor001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同颜色的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTCOLOR_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 30); // 30 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start x 50 : start y
    view1->SetStyle(STYLE_BORDER_WIDTH, 3); // 3 : value
    view1->SetStyle(STYLE_BORDER_RADIUS, 3); // 3 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRect001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectRadius001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectRadius001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置圆角为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 10); // 10 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectColor001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectRadius002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectRadius002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置圆角等于宽度的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 50); // 50 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectRadius001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectRadius003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectRadius003");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置圆角等于宽度的正方形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 50); // 50 : value
    view->Resize(50, 50); // 50 : value 50 : value
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectRadius002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectRadius004()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectRadius004");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同圆角大小的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_04);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 20); // 20 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 10); // 10 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : offset 50 : offset
    view1->SetStyle(STYLE_BORDER_WIDTH, 3); // 3 : value
    view1->SetStyle(STYLE_BORDER_RADIUS, 5); // 5 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectRadius003", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectOpacity001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectOpacity001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不透明背景矩形绘制: ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BACKGROUND_OPA, OPA_OPAQUE);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectRadius004", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectOpacity002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectOpacity002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置全透明背景矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BACKGROUND_OPA, OPA_TRANSPARENT);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectOpacity001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectOpacity003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectOpacity003");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同透明度矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BACKGROUND_OPA, 102); // 102 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start x 50 : start y
    view1->SetStyle(STYLE_BACKGROUND_OPA, 204); // 204 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectOpacity002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderColor001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderColor001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同边框颜色的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDERCOLOR_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_COLOR, Color::Yellow().full);
    view->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value

    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start x 50 : start y
    view1->SetStyle(STYLE_BORDER_COLOR, Color::Red().full);
    view1->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectOpacity003", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderWidth001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderWidth001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置边框宽度为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderColor001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderWidth002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderWidth002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同宽度边框大小的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : offset 50 : offset
    view1->SetStyle(STYLE_BORDER_WIDTH, 10); // 10 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderWidth001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderOpacity001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderOpacity001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不透明边框矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view->SetStyle(STYLE_BORDER_OPA, OPA_OPAQUE);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderWidth002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderOpacity002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderOpacity002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置全透明边框矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view->SetStyle(STYLE_BORDER_OPA, OPA_TRANSPARENT);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderOpacity001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectBorderOpacity003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectBorderOpacity003");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置不同透明度边框矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view->SetStyle(STYLE_BORDER_OPA, 153); // 153 : value
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start x 50 : start y
    view1->SetStyle(STYLE_BORDER_WIDTH, 5); // 5 : value
    view1->SetStyle(STYLE_BORDER_OPA, 204); // 204 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderOpacity002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectPadding001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectPadding001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置padding为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_PADDING_LEFT, 0);
    view->SetStyle(STYLE_PADDING_RIGHT, 0);
    view->SetStyle(STYLE_PADDING_TOP, 0);
    view->SetStyle(STYLE_PADDING_BOTTOM, 0);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectBorderOpacity003", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectPadding002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectPadding002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置padding不为零的矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(0, 50); // 0 : start x 50 : start y
    view->Resize(100, 50); // 100 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(120, 50); // 120 : start x 50 : start y
    view1->SetStyle(STYLE_PADDING_LEFT, 10); // 10 : value
    view1->SetStyle(STYLE_PADDING_RIGHT, 10); // 10 : value
    view1->SetStyle(STYLE_PADDING_TOP, 10); // 10 : value
    view1->SetStyle(STYLE_PADDING_BOTTOM, 10); // 10 : value
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    UIView* view2 = CreateUIView();
    view2->SetPosition(280, 50); // 280 : start x 50 : start y
    view2->SetStyle(STYLE_PADDING_LEFT, 20); // 20 : value
    view2->SetStyle(STYLE_PADDING_RIGHT, 10); // 10 : value
    view2->SetStyle(STYLE_PADDING_TOP, 20); // 20 : value
    view2->SetStyle(STYLE_PADDING_BOTTOM, 10); // 10 : value
    view2->Resize(120, 50); // 120 : width 50 : height
    group->Add(view2);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectPadding001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectMargin001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectMargin001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置margin为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_MARGIN_LEFT, 0);
    view->SetStyle(STYLE_MARGIN_RIGHT, 0);
    view->SetStyle(STYLE_MARGIN_TOP, 0);
    view->SetStyle(STYLE_MARGIN_BOTTOM, 0);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectPadding002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectMargin002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectMargin002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置margin不为零的矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_MARGIN_LEFT, 10); // 10 : margin left
    view->SetStyle(STYLE_MARGIN_RIGHT, 10); // 10 : margin right
    view->SetStyle(STYLE_MARGIN_TOP, 10); // 10 : margin top
    view->SetStyle(STYLE_MARGIN_BOTTOM, 10); // 10 : margin bottom
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start x 50 : start y
    view1->SetStyle(STYLE_MARGIN_LEFT, 20); // 20 : margin left
    view1->SetStyle(STYLE_MARGIN_RIGHT, 10); // 10 : margin right
    view1->SetStyle(STYLE_MARGIN_TOP, 20); // 20 : margin top
    view1->SetStyle(STYLE_MARGIN_BOTTOM, 10); // 10 : margin bottom
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectMargin001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectPaddingMargin001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectPaddingMargin001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置 padding/margin 均为零的矩形绘制:");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTPADDINGMARGIN_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_MARGIN_LEFT, 0);
    view->SetStyle(STYLE_MARGIN_RIGHT, 0);
    view->SetStyle(STYLE_MARGIN_TOP, 0);
    view->SetStyle(STYLE_MARGIN_BOTTOM, 0);
    view->SetStyle(STYLE_PADDING_LEFT, 0);
    view->SetStyle(STYLE_PADDING_RIGHT, 0);
    view->SetStyle(STYLE_PADDING_TOP, 0);
    view->SetStyle(STYLE_PADDING_BOTTOM, 0);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectMargin002", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectPaddingMargin002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectPaddingMargin002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置padding/margin均不为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTPADDINGMARGIN_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_MARGIN_LEFT, 0);
    view->SetStyle(STYLE_MARGIN_RIGHT, 0);
    view->SetStyle(STYLE_MARGIN_TOP, 0);
    view->SetStyle(STYLE_MARGIN_BOTTOM, 0);
    view->SetStyle(STYLE_PADDING_LEFT, 0);
    view->SetStyle(STYLE_PADDING_RIGHT, 0);
    view->SetStyle(STYLE_PADDING_TOP, 0);
    view->SetStyle(STYLE_PADDING_BOTTOM, 0);
    view->Resize(120, 50); // 120 : width 50 : height
    group->Add(view);

    UIView* view1 = CreateUIView();
    view1->SetPosition(250, 50); // 250 : start 50 : start
    view1->SetStyle(STYLE_MARGIN_LEFT, 10); // 10 : margin left
    view1->SetStyle(STYLE_MARGIN_RIGHT, 10); // 10 : margin right
    view1->SetStyle(STYLE_MARGIN_TOP, 10); // 10 : margin top
    view1->SetStyle(STYLE_MARGIN_BOTTOM, 10); // 10 : margin bottom
    view1->SetStyle(STYLE_PADDING_LEFT, 10); // 10 : padding left
    view1->SetStyle(STYLE_PADDING_RIGHT, 10); // 10 : padding right
    view1->SetStyle(STYLE_PADDING_TOP, 10); // 10 : padding top
    view1->SetStyle(STYLE_PADDING_BOTTOM, 10); // 10 : padding bottom
    view1->Resize(120, 50); // 120 : width 50 : height
    group->Add(view1);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectPaddingMargin001", 10); // 10 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectWidthAndHeight001()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectWidthAndHeight001");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置宽度为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_01);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, 10); // 10 : border radius
    view->Resize(0, 50); // 0 : width 50 : height
    container_->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectPaddingMargin002", 20); // 20 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectWidthAndHeight002()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectWidthAndHeight002");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置高度为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_02);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0); // 10 : value
    view->SetStyle(STYLE_BORDER_RADIUS, 0); // 10 : value
    view->Resize(120, 0); // 120 : width 0 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectWidthAndHeight001", 20); // 20 : offset size
}

void UITestDrawRect::UIKitDrawRectTestRectWidthAndHeight003()
{
    if (container_ == nullptr) {
        return;
    }

    UIViewGroup* group = CreateTestCaseGroup();
    group->SetViewId("UIKitDrawRectTestRectWidthAndHeight003");

    UILabel* label = CreateTitleLabel();
    label->SetText("设置宽度和高度都为零的矩形绘制 ");
    label->SetViewId(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_03);
    group->Add(label);

    UIView* view = CreateUIView();
    view->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, VIEW_DISTANCE_TO_TOP_SIDE);
    view->SetStyle(STYLE_BORDER_WIDTH, 0);
    view->SetStyle(STYLE_BORDER_RADIUS, 10); // 10 : border radius
    view->Resize(0, 0);  // 0 : width 0 : height
    group->Add(view);

    container_->Add(group);
    group->LayoutBottomToSibling("UIKitDrawRectTestRectWidthAndHeight002", 20); // 20 : offset size
}
} // namespace OHOS
