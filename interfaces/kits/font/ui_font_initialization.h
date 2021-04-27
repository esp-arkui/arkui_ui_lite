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
#ifndef GRAPHIC_LITE_UI_FONT_INITIALIZATION_H
#define GRAPHIC_LITE_UI_FONT_INITIALIZATION_H

#include "font/ui_font_header.h"

namespace OHOS {
class UIFontInitialization {
public:
    static UIFontInitialization* GetInstance();

    void SetTextLangFontsTable(const UITextLanguageFontParam* uiTextLangFontsTable, uint8_t totalFontId);

    void SetLangTextDefaultParamTable(const LangTextParam* langTextDefaultParamTable, uint8_t totalLangId);

    void SetMaxTextId(uint8_t totalTextId);

    /**
     * @brief Get the Text Lang Fonts Table
     * @param uint8_t langFontId [in] the font id
     * @return UITextLanguageFontParam* the font table, definition is in font tool
     */
    UITextLanguageFontParam* GetTextLangFontsTable(uint8_t langFontId);

    /**
     * @brief Get the Total Lang Id
     * @return uint8_t the total Lang id, definition is in font tool
     */
    uint8_t GetTotalLangId() const;

    /**
     * @brief Get the Total Font Id
     * @return uint8_t the total font id, definition is in font tool
     */
    uint8_t GetTotalFontId() const;

    uint8_t GetBitmapFontIdMax() const;

    /**
     * @brief Get the Total Text Id
     * @return uint8_t the total text id, definition is in font tool
     */
    uint16_t GetTotalTextId() const;

    /**
     * @brief Get the Lang Text default param table
     * @return LangTextParam* the param table, definition is in font tool
     */
    LangTextParam* GetLangTextDefaultParamTable();

    UITextLanguageFontParam* uiTextLangFontsTable_;
    LangTextParam* langTextDefaultParamTable_;
private:
    UIFontInitialization();
    ~UIFontInitialization() {}
    uint8_t totalLangId_;
    uint8_t totalFontId_;
    uint8_t totalTextId_;
};
}
#endif // GRAPHIC_LITE_UI_FONT_INITIALIZATION_H