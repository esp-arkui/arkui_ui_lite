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

#ifndef UI_AUTO_TEST_FONT_H
#define UI_AUTO_TEST_FONT_H

#include "ui_auto_test.h"

namespace OHOS {
class UiAutoTestFont : public UIAutoTest {
public:
    UiAutoTestFont() {}
    ~UiAutoTestFont() {}

    void Reset() const;
    void RunTestList();

    /**
     * @brief Auto test psram
     */
    void FontFontEngineFontConvertTestFontTestPsram001() const;

    /**
     * @brief Auto test Font path
     */
    void FontFontEngineFontConvertTestFontTestFontPath001() const;

    /**
     * @brief Auto test Set Font Id
     */
    void FontFontEngineFontConvertTestFontTestSetFontId001() const;

    /**
     * @brief Auto test Set Font
     */
    void FontFontEngineFontConvertTestFontTestSetFont001() const;

    /**
     * @brief Auto test Get Font Height
     */
    void FontFontEngineFontConvertTestFontTestGetFontHeight001() const;

    /**
     * @brief Auto test get font version
     */
    void FontFontEngineFontConvertTestFontTestGetFontVersion001() const;

    /**
     * @brief Auto test get font Id
     */
    void FontFontEngineFontConvertTestFontTestGetFontId001() const;

    /**
     * @brief Auto test get font header
     */
    void FontFontEngineFontConvertTestFontTestGetFontHeader001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_FONT_H