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

#include "ui_auto_test_arc_label.h"
#include "ui_test_arc_label.h"

namespace OHOS {
void UIAutoTestArcLabel::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Arc_Label");
}

void UIAutoTestArcLabel::RunTestList()
{
    Reset();
    UIKitUIArcLabelAutoTestDisplay001();
    UIKitUIArcLabelAutoTestDisplay002();
    UIKitUIArcLabelAutoTestDisplay003();
    UIKitUIArcLabelAutoTestDisplay004();
    Reset();
    UIKitUIArcLabelAutoTestAlign001();
    UIKitUIArcLabelAutoTestAlign002();
    UIKitUIArcLabelAutoTestAlign003();
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestDisplay001() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_01);
    const char* fileName = "ui_test_arc_label_display_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestDisplay002() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_02);
    const char* fileName = "ui_test_arc_label_display_002.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestDisplay003() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_03);
    const char* fileName = "ui_test_arc_label_display_003.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestDisplay004() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_04);
    const char* fileName = "ui_test_arc_label_display_004.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestAlign001() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_01);
    const char* fileName = "ui_test_arc_label_align_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestAlign002() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_02);
    const char* fileName = "ui_test_arc_label_align_002.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelAutoTestAlign003() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_03);
    const char* fileName = "ui_test_arc_label_align_003.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS