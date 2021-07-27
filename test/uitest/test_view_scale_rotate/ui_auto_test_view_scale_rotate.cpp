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

#include "ui_auto_test_view_scale_rotate.h"
#include "auto_test_counter.h"
#include "ui_test_view_scale_rotate.h"

namespace OHOS {
// Take the 5 data of 1, 10, 20, 30, 40 frames as the comparison point.
uint16_t g_frameArray[5] = {1, 10, 20, 30, 40};
void UIAutoTestViewScaleRotate::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("View_Scale_Rotate");
}

void UIAutoTestViewScaleRotate::RunTestList()
{
    Reset();
    UIKitViewScaleRotateTestLabel001();
    UIKitViewScaleRotateTestLabelButton002();
    UIKitViewScaleRotateTestCanvas003();
    UIKitViewScaleRotateTestUIBoxProgress004();
    UIKitViewScaleRotateTestUICircleProgress005();
    UIKitViewScaleRotateTestUIDigitalClock006();
    UIKitViewScaleRotateTestGroup007();
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestLabel001() const
{
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_label",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestLabelButton002() const
{
    DragViewToHead("test_view_scale_rotate_label_button_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_label_button",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestCanvas003() const
{
    DragViewToHead("test_view_scale_rotate_ui_canvas_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_ui_canvas",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestUIBoxProgress004() const
{
    DragViewToHead("test_view_scale_rotate_ui_box_progress_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_ui_box_progress",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestUICircleProgress005() const
{
    DragViewToHead("test_view_scale_rotate_ui_circle_progress_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_ui_circle_progress",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestUIDigitalClock006() const
{
    DragViewToHead("test_view_scale_rotate_ui_digital_clock_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_ui_digital_clock",
        g_frameArray, sizeof(g_frameArray));
}

void UIAutoTestViewScaleRotate::UIKitViewScaleRotateTestGroup007() const
{
    DragViewToHead("test_view_scale_rotate_group_label_id");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_view_scale_rotate_group",
        g_frameArray, sizeof(g_frameArray));
}
} // namespace OHOS