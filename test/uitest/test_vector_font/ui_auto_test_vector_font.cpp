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

#include "ui_auto_test_vector_font.h"
#if ENABLE_VECTOR_FONT
#include "ui_test_vector_font.h"

namespace OHOS {
void UIAutoTestVectorFont::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Vector_Font");
}

void UIAutoTestVectorFont::RunTestList()
{
    Reset();
    FontFontEngineVectorTestCHLang001();
    FontFontEngineVectorTestCHLang002();
    FontFontEngineVectorTestJALang001();
    FontFontEngineVectorTestJALang002();
    FontFontEngineVectorTestKRLang001();
    FontFontEngineVectorTestVILang001();
}

void UIAutoTestVectorFont::FontFontEngineVectorTestCHLang001() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_CH_LANG_01);
    const char* fileName = "ui_test_vector_font_ch_lang_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestVectorFont::FontFontEngineVectorTestCHLang002() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_CH_LANG_02);
    const char* fileName = "ui_test_vector_font_ch_lang_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestVectorFont::FontFontEngineVectorTestJALang001() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_JA_LANG_01);
    const char* fileName = "ui_test_vector_font_ja_lang_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestVectorFont::FontFontEngineVectorTestJALang002() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_JA_LANG_02);
    const char* fileName = "ui_test_vector_font_ja_lang_02.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestVectorFont::FontFontEngineVectorTestKRLang001() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_KR_LANG_01);
    const char* fileName = "ui_test_vector_font_kr_lang_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestVectorFont::FontFontEngineVectorTestVILang001() const
{
    DragViewToHead(UI_TEST_VECTOR_FONT_VI_LANG_01);
    const char* fileName = "ui_test_vector_font_vi_lang_01.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS
#endif
