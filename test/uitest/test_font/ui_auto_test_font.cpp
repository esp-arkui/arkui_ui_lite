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
    EnterSubMenu(UI_TEST_FONT_ID);
}

void UIAutoTestFont::RunTestList()
{
    Reset();
    FontFontEngineFontConvertTestFontTestPsram001();
    FontFontEngineFontConvertTestFontTestFontPath001();
    FontFontEngineFontConvertTestFontTestSetFontId001();
    FontFontEngineFontConvertTestFontTestSetFont001();
    FontFontEngineFontConvertTestFontTestGetFontHeight001();
    FontFontEngineFontConvertTestFontTestGetFontVersion001();
    FontFontEngineFontConvertTestFontTestGetFontId001();
    FontFontEngineFontConvertTestFontTestGetFontHeader001();
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestPsram001() const
{
    DragViewToHead(UI_TEST_FONT_PSRAM_LABEL_ID_01);
    const char* fileName = "ui_test_font_psram_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestFontPath001() const
{
    DragViewToHead(UI_TEST_FONT_FONTPATH_LABEL_ID_01);
    const char* fileName = "ui_test_font_font_path_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestSetFontId001() const
{
    DragViewToHead(UI_TEST_FONT_SETFONTID_LABEL_ID_01);
    const char* fileName = "ui_test_font_set_font_id_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestSetFont001() const
{
    DragViewToHead(UI_TEST_FONT_SETFONT_LABEL_ID_01);
    const char* fileName = "ui_test_font_set_font_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestGetFontHeight001() const
{
    DragViewToHead(UI_TEST_FONT_GETFONTHEIGHT_LABEL_ID_01);
    const char* fileName = "ui_test_font_get_font_height_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestGetFontVersion001() const
{
    DragViewToHead(UI_TEST_FONT_GETFONTVERSION_LABEL_ID_01);
    const char* fileName = "ui_test_font_get_font_version_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestGetFontId001() const
{
    DragViewToHead(UI_TEST_FONT_GETFONTID_LABEL_ID_01);
    const char* fileName = "ui_test_font_get_font_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestFont::FontFontEngineFontConvertTestFontTestGetFontHeader001() const
{
    DragViewToHead(UI_TEST_FONT_GETFONTHEADER_LABEL_ID_01);
    const char* fileName = "ui_test_font_get_font_header_01.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS