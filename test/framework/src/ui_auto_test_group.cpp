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

#include "ui_auto_test_group.h"
#include "graphic_config.h"
#include "test_anti_aliasing/ui_auto_test_anti_aliasing.h"
#include "test_arc_label/ui_auto_test_arc_label.h"
#include "test_button/ui_auto_test_button.h"
#include "test_canvas/ui_auto_test_canvas.h"
#include "test_chart/ui_auto_test_chart_pillar.h"
#include "test_chart/ui_auto_test_chart_polyline.h"
#include "test_digital_clock/ui_auto_test_digital_clock.h"
#include "test_draw_line/ui_auto_test_draw_line.h"
#include "test_draw_rect/ui_auto_test_draw_rect.h"
#include "test_focus_manager/ui_auto_test_focus_manager.h"
#include "test_font/ui_auto_test_font.h"
#include "test_image/ui_auto_test_image.h"
#include "test_label/ui_auto_test_label.h"
#include "test_layout/ui_auto_test_basic_layout.h"
#include "test_opacity/ui_auto_test_opacity.h"
#include "test_picker/ui_auto_test_ui_picker.h"
#include "test_qrcode/ui_auto_test_qrcode.h"
#include "test_render/ui_auto_test_render.h"
#include "test_slider/ui_auto_test_slider.h"
#include "test_transform/ui_auto_test_transform.h"

namespace OHOS {
List<UIAutoTest*> UIAutoTestGroup::testCaseList_;

void UIAutoTestGroup::SetUpTestCase()
{
    testCaseList_.PushBack(new UiAutoTestAntiAliasing());
    testCaseList_.PushBack(new UiAutoTestArcLabel());
    testCaseList_.PushBack(new UiAutoTestButton());
    testCaseList_.PushBack(new UiAutoTestCanvas());
    testCaseList_.PushBack(new UiAutoTestChartPillar());
    testCaseList_.PushBack(new UiAutoTestChartPolyline());
    testCaseList_.PushBack(new UiAutoTestDigitalClock());
    testCaseList_.PushBack(new UiAutoTestDrawLine());
    testCaseList_.PushBack(new UiAutoTestDrawRect());
    testCaseList_.PushBack(new UiAutoTestFont());
    testCaseList_.PushBack(new UiAutoTestImage());
    testCaseList_.PushBack(new UiAutoTestLabel());
    testCaseList_.PushBack(new UiAutoTestBasicLayout());
    testCaseList_.PushBack(new UiAutoTestOpacity());
    testCaseList_.PushBack(new UiAutoTestUiPicker());
    testCaseList_.PushBack(new UiAutoTestQrcode());
    testCaseList_.PushBack(new UIAutoTestRender());
    testCaseList_.PushBack(new UiAutoTestTransform());
    testCaseList_.PushBack(new UiAutoTestSlider());
#if ENABLE_FOCUS_MANAGER
    testCaseList_.PushBack(new UiAutoTestFocusManager());
#endif
}

List<UIAutoTest*>& UIAutoTestGroup::GetTestCase()
{
    return testCaseList_;
}

void UIAutoTestGroup::TearDownTestCase()
{
    ListNode<UIAutoTest*>* node = testCaseList_.Begin();
    while (node != testCaseList_.End()) {
        delete node->data_;
        node = node->next_;
    }
    testCaseList_.Clear();
}
} // namespace OHOS
