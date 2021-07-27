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

#include "ui_auto_test_font.h"
#include "ui_test_font.h"

namespace OHOS {
void UIAutoTestFont::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Font");
}

void UIAutoTestFont::RunTestList()
{
    Reset();
    UIKitFontTestDispaly001();
    UIKitFontTestDispaly002();
    UIKitFontTestDispaly003();
    UIKitFontTestDispaly004();
    UIKitFontTestDispaly005();
    UIKitFontTestDispaly006();
    UIKitFontTestDispaly007();
    UIKitFontTestDispaly008();
    UIKitFontTestBaseline001();
    UIKitFontTestBaseline002();
    UIKitFontTestLineHeight001();
    UIKitFontTestLineHeight002();
}

void UIAutoTestFont::UIKitFontTestDispaly001() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_01);
    const char* fileName = "ui_test_font_display_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly002() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_02);
    const char* fileName = "ui_test_font_display_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly003() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_03);
    const char* fileName = "ui_test_font_display_03.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly004() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_04);
    const char* fileName = "ui_test_font_display_04.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly005() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_05);
    const char* fileName = "ui_test_font_display_05.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly006() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_06);
    const char* fileName = "ui_test_font_display_06.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly007() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_07);
    const char* fileName = "ui_test_font_display_07.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestDispaly008() const
{
    DragViewToHead(UI_TEST_FONT_DISPLAY_LABEL_08);
    const char* fileName = "ui_test_font_display_08.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestBaseline001() const
{
    DragViewToHead(UI_TEST_FONT_BASELINE_LABEL_01);
    const char* fileName = "ui_test_font_base_line_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestBaseline002() const
{
    DragViewToHead(UI_TEST_FONT_BASELINE_LABEL_02);
    const char* fileName = "ui_test_font_base_line_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestLineHeight001() const
{
    DragViewToHead(UI_TEST_FONT_LINE_HEIGHT_LABEL_01);
    const char* fileName = "ui_test_font_line_height_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestFont::UIKitFontTestLineHeight002() const
{
    DragViewToHead(UI_TEST_FONT_LINE_HEIGHT_LABEL_02);
    const char* fileName = "ui_test_font_line_height_02.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS
