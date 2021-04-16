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
    EnterSubMenu(UI_TEST_ARC_LABEL_ID);
}

void UIAutoTestArcLabel::RunTestList()
{
    Reset();
    UIKitUIArcLabelTestDisplay001();
    UIKitUIArcLabelTestDisplay002();
    UIKitUIArcLabelTestDisplay003();
    UIKitUIArcLabelTestDisplay004();
    Reset();
    UIKitUIArcLabelTestAlign001();
    UIKitUIArcLabelTestAlign002();
    UIKitUIArcLabelTestAlign003();
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestDisplay001() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_01);
    const char* fileName = "ui_test_arc_label_display_001.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestDisplay002() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_02);
    const char* fileName = "ui_test_arc_label_display_002.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestDisplay003() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_03);
    const char* fileName = "ui_test_arc_label_display_003.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestDisplay004() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_DISPLAY_ID_04);
    const char* fileName = "ui_test_arc_label_display_004.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestAlign001() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_01);
    const char* fileName = "ui_test_arc_label_align_001.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestAlign002() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_02);
    const char* fileName = "ui_test_arc_label_align_002.bin";
    CompareByBinary(fileName);
}

void UIAutoTestArcLabel::UIKitUIArcLabelTestAlign003() const
{
    DragViewToHead(UI_TEST_ARC_LABEL_ALIGN_ID_03);
    const char* fileName = "ui_test_arc_label_align_003.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS