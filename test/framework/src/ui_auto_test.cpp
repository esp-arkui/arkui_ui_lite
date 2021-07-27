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

#include "ui_auto_test.h"

#include "compare_tools.h"
#include "components/root_view.h"
#include "components/ui_list.h"
#include "components/ui_view_group.h"
#include "dfx/event_injector.h"
#include "test_anti_aliasing/ui_auto_test_anti_aliasing.h"
#include "test_arc_label/ui_auto_test_arc_label.h"
#include "test_border_margin_padding/ui_auto_test_border_margin_padding.h"
#include "test_button/ui_auto_test_button.h"
#include "test_canvas/ui_auto_test_canvas.h"
#include "test_chart/ui_auto_test_chart_pillar.h"
#include "test_chart/ui_auto_test_chart_polyline.h"
#include "test_clip/ui_auto_test_clip.h"
#include "test_digital_clock/ui_auto_test_digital_clock.h"
#include "test_draw_line/ui_auto_test_draw_line.h"
#include "test_draw_rect/ui_auto_test_draw_rect.h"
#include "test_event_injector/ui_auto_test_event_injector.h"
#include "test_focus_manager/ui_auto_test_focus_manager.h"
#include "test_font/ui_auto_test_font.h"
#include "test_image/ui_auto_test_image.h"
#include "test_image_animator/ui_auto_test_image_animator.h"
#include "test_label/ui_auto_test_label.h"
#include "test_layout/ui_auto_test_basic_layout.h"
#include "test_opacity/ui_auto_test_opacity.h"
#include "test_picker/ui_auto_test_ui_picker.h"
#include "test_progress_bar/ui_auto_test_box_progress.h"
#include "test_progress_bar/ui_auto_test_circle_progress.h"
#include "test_qrcode/ui_auto_test_qrcode.h"
#include "test_render/ui_auto_test_render.h"
#include "test_scroll_bar/ui_auto_test_scroll_bar.h"
#include "test_slider/ui_auto_test_slider.h"
#include "test_texture_mapper/ui_auto_test_texture_mapper.h"
#include "test_transform/ui_auto_test_transform.h"
#include "test_ui_analog_clock/ui_auto_test_analog_clock.h"
#include "test_ui_list/ui_auto_test_ui_list.h"
#include "test_ui_list_view/ui_auto_test_list_layout.h"
#include "test_ui_scroll_view/ui_auto_test_ui_scroll_view.h"
#include "test_ui_swipe_view/ui_auto_test_ui_swipe_view.h"
#if ENABLE_VECTOR_FONT
#include "test_vector_font/ui_auto_test_vector_font.h"
#endif
#include "test_view_bitmap/ui_auto_test_view_bitmap.h"
#include "test_view_bounds/ui_auto_test_view_bounds.h"
#include "test_view_group/ui_auto_test_view_group.h"
#include "test_view_percent/ui_auto_test_view_percent.h"
#include "test_view_scale_rotate/ui_auto_test_view_scale_rotate.h"
#include "ui_test_app.h"
#include "ui_test_group.h"

namespace OHOS {
void UIAutoTest::ResetMainMenu() const
{
    while (RootView::GetInstance()->GetChildById(UI_TEST_MAIN_LIST_ID) == nullptr) {
        ClickViewById(UI_TEST_BACK_BUTTON_ID);
    }
}

void UIAutoTest::EnterSubMenu(const char* id) const
{
    if (id == nullptr) {
        return;
    }
    UIView* view = RootView::GetInstance()->GetChildById(id);
    if (view == nullptr) {
        UIView* view = RootView::GetInstance()->GetChildById(UI_TEST_MAIN_LIST_ID);
        if (view == nullptr) {
            return;
        }
        ListNode<TestCaseInfo>* node = UITestGroup::GetTestCase().Begin();
        while (node != UITestGroup::GetTestCase().End()) {
            if ((node->data_.sliceId != nullptr) && (strcmp(id, node->data_.sliceId) == 0)) {
                UITestGroup::GetTestCase().PushFront(node->data_);
                UITestGroup::GetTestCase().Remove(node);
                break;
            }
            node = node->next_;
        }
        reinterpret_cast<UIList*>(view)->RefreshList();
        CompareTools::WaitSuspend();
    }
    ClickViewById(id);
}

void UIAutoTest::ClickViewById(const char* id) const
{
    if (id == nullptr) {
        return;
    }
    UIView* view = RootView::GetInstance()->GetChildById(id);
    if (view == nullptr) {
        return;
    }
    Point point;
    point.x = view->GetOrigRect().GetX();
    point.y = view->GetOrigRect().GetY();
    EventInjector::GetInstance()->SetClickEvent(point);
    CompareTools::WaitSuspend();
}

void UIAutoTest::DragViewToHead(const char* id) const
{
    if (id == nullptr) {
        return;
    }
    UIView* view = RootView::GetInstance()->GetChildById(id);
    if (view == nullptr) {
        return;
    }
    Point startPoint;
    startPoint.x = view->GetOrigRect().GetX();
    startPoint.y = view->GetOrigRect().GetY();

    Point endPoint;
    endPoint.x = startPoint.x;
    endPoint.y = 80; // 80 :end point y position;
    EventInjector::GetInstance()->SetDragEvent(startPoint, endPoint, 300); // 300: drag time
    CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
}

void UIAutoTest::DragViewById(const char* id, DragDirection direction) const
{
    if (id == nullptr) {
        return;
    }
    UIView* view = RootView::GetInstance()->GetChildById(id);
    if (view == nullptr) {
        return;
    }
    Point startPoint;
    Point endPoint;
    switch (direction) {
        case BOTTOM_TO_TOP:
            startPoint.x = view->GetOrigRect().GetX();
            startPoint.y = view->GetOrigRect().GetBottom();
            endPoint.x = startPoint.x;
            endPoint.y = view->GetOrigRect().GetTop();
            break;
        case TOP_TO_BOTTOM:
            startPoint.x = view->GetOrigRect().GetX();
            startPoint.y = view->GetOrigRect().GetTop();
            endPoint.x = startPoint.x;
            endPoint.y = view->GetOrigRect().GetBottom();
            break;
        case LEFT_TO_RIGHT:
            startPoint.x = view->GetOrigRect().GetLeft();
            startPoint.y = view->GetOrigRect().GetY();
            endPoint.x = view->GetOrigRect().GetRight();
            endPoint.y = startPoint.y;
            break;
        case RIGHT_TO_LEFT:
            startPoint.x = view->GetOrigRect().GetRight();
            startPoint.y = view->GetOrigRect().GetY();
            endPoint.x = view->GetOrigRect().GetLeft();
            endPoint.y = startPoint.y;
            break;
        case UPPER_LEFT_CORNER_TO_BOTTOM_RIGHT_CORNER:
            startPoint.x = view->GetOrigRect().GetLeft();
            startPoint.y = view->GetOrigRect().GetTop();
            endPoint.x = view->GetOrigRect().GetRight();
            endPoint.y = view->GetOrigRect().GetBottom();
            break;
        case BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER:
            startPoint.x = view->GetOrigRect().GetRight();
            startPoint.y = view->GetOrigRect().GetBottom();
            endPoint.x = view->GetOrigRect().GetLeft();
            endPoint.y = view->GetOrigRect().GetTop();
            break;
        case UPPER_RIGHT_CORNER_TO_BOTTOM_LEFT_CORNER:
            startPoint.x = view->GetOrigRect().GetRight();
            startPoint.y = view->GetOrigRect().GetTop();
            endPoint.x = view->GetOrigRect().GetLeft();
            endPoint.y = view->GetOrigRect().GetBottom();
            break;
        case BOTTOM_LEFT_CORNER_TO_UPPER_RIGHT_CORNER:
            startPoint.x = view->GetOrigRect().GetLeft();
            startPoint.y = view->GetOrigRect().GetBottom();
            endPoint.x = view->GetOrigRect().GetRight();
            endPoint.y = view->GetOrigRect().GetTop();
            break;
        default:
            return;
    }

    EventInjector::GetInstance()->SetDragEvent(startPoint, endPoint, 300); // 300: drag time
    CompareTools::WaitSuspend(2000); // 2000 :wait time after set drag event
}

void UIAutoTest::CompareByBinary(const char* fileName) const
{
    if (fileName == nullptr) {
        return;
    }
    char filePath[DEFAULT_FILE_NAME_MAX_LENGTH] = {0};
    CompareTools::StrnCatPath(filePath, DEFAULT_FILE_NAME_MAX_LENGTH, fileName, strlen(fileName));
    if (CompareTools::CheckFileExist(filePath, sizeof(filePath))) {
        CompareTools::CompareFile(filePath, sizeof(filePath));
    } else {
        CompareTools::SaveFile(filePath, sizeof(filePath));
    }
}

void UIAutoTest::SetUpTestCase()
{
    AutoTestCaseGroup::AddTestCase(new UIAutoTestAntiAliasing());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestArcLabel());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestBorderMarginPadding());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestButton());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestCanvas());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestChartPillar());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestChartPolyline());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestClip());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestDigitalClock());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestDrawLine());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestDrawRect());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestEventInjector());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestFocusManager());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestFont());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestImage());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestImageAnimator());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestLabel());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestBasicLayout());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestOpacity());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestUiPicker());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestBoxProgress());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestCircleProgress());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestQrcode());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestRender());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestScrollBar());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestSlider());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestTextureMapper());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestTransform());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestAnalogClock());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestUIList());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestUIListLayout());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestScrollView());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestSwipeView());
#if ENABLE_VECTOR_FONT
    AutoTestCaseGroup::AddTestCase(new UIAutoTestVectorFont());
#endif
    AutoTestCaseGroup::AddTestCase(new UIAutoTestViewBitmap());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestViewBounds());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestViewGroup());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestViewPercent());
    AutoTestCaseGroup::AddTestCase(new UIAutoTestViewScaleRotate());
}
} // namespace OHOS
