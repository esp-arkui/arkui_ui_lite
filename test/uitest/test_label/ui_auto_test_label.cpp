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

#include "ui_auto_test_label.h"
#include "ui_test_label.h"

namespace OHOS {
void UIAutoTestLabel::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UILabel");
}

void UIAutoTestLabel::RunTestList()
{
    UIKitUILabelTestFontSize001();
    UIKitUILabelTestHorAlign001();
    UIKitUILabelTestColor001();
    UIKitUILabelTestBeyond001();
    UIKitUILabelTestLine001();
    UIKitUILabelTestVorAlign001();
    UIKitUILabelTestDirection001();
    UIKitUILabelTestDynamicHeight001();
    UIKitUILabelTestDynamicWidth001();
}

void UIAutoTestLabel::UIKitUILabelTestFontSize001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_FONTSIZE_BTN_ID_01);
    const char* fileName = "ui_test_label_fontsize_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_FONTSIZE_BTN_ID_02);
    const char* fileName2 = "ui_test_label_fontsize_002.bmp";
    CompareByBinary(fileName2);

    ClickViewById(UI_TEST_LABEL_FONTSIZE_BTN_ID_03);
    const char* fileName3 = "ui_test_label_fontsize_003.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestLabel::UIKitUILabelTestHorAlign001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_HORALIGN_BTN_ID_01);
    const char* fileName = "ui_test_label_hor_align_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_HORALIGN_BTN_ID_02);
    const char* fileName2 = "ui_test_label_hor_align_002.bmp";
    CompareByBinary(fileName2);

    ClickViewById(UI_TEST_LABEL_HORALIGN_BTN_ID_03);
    const char* fileName3 = "ui_test_label_hor_align_003.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestLabel::UIKitUILabelTestColor001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_COLOR_BTN_ID_01);
    const char* fileName = "ui_test_label_color_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_COLOR_BTN_ID_02);
    const char* fileName2 = "ui_test_label_color_002.bmp";
    CompareByBinary(fileName2);

    ClickViewById(UI_TEST_LABEL_COLOR_BTN_ID_03);
    const char* fileName3 = "ui_test_label_color_003.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestLabel::UIKitUILabelTestBeyond001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_BEYOND_BTN_ID_01);
    const char* fileName = "ui_test_label_beyond_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_BEYOND_BTN_ID_02);
    const char* fileName2 = "ui_test_label_beyond_002.bmp";
    CompareByBinary(fileName2);

    ClickViewById(UI_TEST_LABEL_BEYOND_BTN_ID_03);
    const char* fileName3 = "ui_test_label_beyond_003.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestLabel::UIKitUILabelTestLine001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_LINE_BTN_ID_01);
    const char* fileName = "ui_test_label_line_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_LINE_BTN_ID_02);
    const char* fileName2 = "ui_test_label_line_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestLabel::UIKitUILabelTestVorAlign001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_VERALIGN_BTN_ID_01);
    const char* fileName = "ui_test_label_ver_align_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_VERALIGN_BTN_ID_02);
    const char* fileName2 = "ui_test_label_ver_align_002.bmp";
    CompareByBinary(fileName2);

    ClickViewById(UI_TEST_LABEL_VERALIGN_BTN_ID_03);
    const char* fileName3 = "ui_test_label_ver_align_003.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestLabel::UIKitUILabelTestDirection001() const
{
    Reset();
    ClickViewById(UI_TEST_LABEL_DIRECTION_BTN_ID_01);
    const char* fileName = "ui_test_label_direction_001.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_LABEL_DIRECTION_BTN_ID_02);
    const char* fileName2 = "ui_test_label_direction_002.bmp";
    CompareByBinary(fileName2);
}

void UIAutoTestLabel::UIKitUILabelTestDynamicHeight001() const
{
    Reset();
    DragViewToHead(UI_TEST_LABEL_SIZE_LABEL_ID_01);
    ClickViewById(UI_TEST_LABEL_DYNAMIC_HEIGHT_BTN_ID_01);
    const char* fileName = "ui_test_label_dynamic_height_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestLabel::UIKitUILabelTestDynamicWidth001() const
{
    Reset();
    DragViewToHead(UI_TEST_LABEL_SIZE_LABEL_ID_01);
    ClickViewById(UI_TEST_LABEL_DYNAMIC_WIDTH_BTN_ID_01);
    const char* fileName = "ui_test_label_dynamic_width_001.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS