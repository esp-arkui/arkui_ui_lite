/*
 * Copyright (c) 2020-2022 Huawei Device Co., Ltd.
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

#include "common/ui_text_language.h"
#include "dfx/ui_view_bounds.h"

#include "ui_test_vector_font.h"
#if ENABLE_VECTOR_FONT
#include "common/screen.h"
#include "components/ui_label.h"
#if ENABLE_MULTI_FONT
#include "font/ui_multi_font_manager.h"
#endif
namespace {
const int16_t GAP = 5;
const int16_t TITLE_HEIGHT = 29;
const uint16_t LABEL_WIDTH = 200;
const uint16_t TWO = 2;
const uint16_t NINE = 9;
const uint16_t ELEVEN = 11;
const uint16_t THIRTEEN = 13;
const uint16_t SIXTEEN = 16;
const uint16_t LABEL_HEIGHT = 50;
const uint16_t BUF_SIZE = 200;
const uint16_t LANGUAGE_FILE_ID_MAX = 9;
const char* HYQIHEI_65S = "SourceHanSansSC-Regular.otf"; //"HYQiHei-65S.otf";
const char* NOTONASKHARABIC_REGULAR = "NotoNaskhArabic-Regular.ttf";
const char* ROBOTOCONDENSED_REGULAR = "RobotoCondensed-Regular.ttf";
const char* NOTOSANSCJKJP_REGULAR = "NotoSansCJKjp-Regular.otf";
const char* NOTOSANSTHAI_REGULAR = "NotoSansThai-Regular.ttf";
const char* NOTOSANSHEBREW_REGULAR = "NotoSansHebrew-Regular.ttf";
const char* NOTOSANSDEVANAGARI_REGULAR = "NotoSansDevanagari-Regular.otf";
const char* NOTOSANSMYANMAR_CONDENSED = "NotoSansMyanmar-Condensed.ttf";
const char* NOTOSANSBENGALI_REGULAR = "NotoSansBengali-Regular.ttf";
const char* test_cjf = "happyfont.ttf";
} // namespace
namespace OHOS {
void UITestVectorFont::SetUp()
{
    if (container_ == nullptr) {
        container_ = new UIScrollView();
        container_->SetThrowDrag(true);
        container_->SetHorizontalScrollState(false);
        container_->Resize(Screen::GetInstance().GetWidth(), Screen::GetInstance().GetHeight() - BACK_BUTTON_HEIGHT);
        positionX_ = 50; // 50: init position x
        positionY_ = 5;  // 5: init position y
    }
}

void UITestVectorFont::TearDown()
{
    DeleteChildren(container_);
    container_ = nullptr;
}

void UITestVectorFont::InnerTestTitle(const char* title)
{
    UILabel* titleLabel = new UILabel();
    UIFont::GetInstance()->RegisterFontInfo(DEFAULT_VECTOR_FONT_FILENAME);
    titleLabel->SetPosition(TEXT_DISTANCE_TO_LEFT_SIDE, positionY_, Screen::GetInstance().GetWidth(),
                            TITLE_LABEL_DEFAULT_HEIGHT);
    titleLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    titleLabel->SetText(title);
    container_->Add(titleLabel);
    positionY_ += TITLE_HEIGHT + GAP;
}

const UIView* UITestVectorFont::GetTestView()
{
//#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
//    TestDrawTextITALYBOLD();
//#endif
//    FontFontEngineVectorTestCHLang002();
    cjf_size();
    cjf_fontId();
    cjf_sizeFontId();
    cjf_BackgroundColor();
//    cjf_LineBackgroundColor();

//    FontFontEngineVectorTestCHLang001();
//    FontFontEngineVectorTestCHLang002();
//    FontFontEngineVectorTestJALang001();
//    FontFontEngineVectorTestJALang002();
//    FontFontEngineVectorTestKRLang001();
//    FontFontEngineVectorTestVILang001();

#if ENABLE_MULTI_FONT
    FontFontEngineMultiLanguageTestJALang001();
    FontFontEngineMultiLanguageTestJALang002();
    FontFontEngineMultiLanguageTestKRLang001();
    FontFontEngineMultiLanguageTestVILang001();
    FontFontEngineMultiLanguageTestCSLang001();
    FontFontEngineMultiLanguageTestELLang001();
    FontFontEngineMultiLanguageTestROLang001();
#endif
    return container_;
}


// ---- cjf ----
void UITestVectorFont::cjf_size()
{
    if (container_ != nullptr)
    {
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        label->SetVisible(true);

        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("测试字号");
        container_->Add(label);
        #if defined(SR_1) && SR_1
            label->SetSizeSpan(0,2,36);
        #else
            label->SetAbsoluteSizeSpan(0,2,36);
        #endif

        positionY_ += LABEL_HEIGHT + GAP;
    }
}
// ---- cjf end ----

// ---- cjf ----
void UITestVectorFont::cjf_fontId()
{
    if (container_ != nullptr) {
        UILabel* label_2 = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        uint8_t test_value = UIFont::GetInstance()->RegisterFontInfo(test_cjf);
        label_2->SetPosition(positionX_, positionY_);
        label_2->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label_2->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label_2->SetFont(HYQIHEI_65S, 24); // 24 : size
        label_2->SetText("快乐字体 普通字体");

        label_2->SetVisible(true);

        #if defined(SR_1) && SR_1
            label_2->SetFontNameSpan(0,4,test_cjf);
        #else
            label_2->SetFont(test_cjf, 24);
        #endif

//        label_2->SetFont(test_cjf,24);

        container_->Add(label_2);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}
// ---- cjf end ----


// ---- cjf --------
void UITestVectorFont::cjf_sizeFontId()
{
    if (container_ != nullptr)
    {
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        uint8_t test_value = UIFont::GetInstance()->RegisterFontInfo(test_cjf);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("快乐字体 普通字体");

        label->SetVisible(true);

        #if defined(SR_1) && SR_1
            label->SetFontNameSpan(0,4,test_cjf);
            label->SetSizeSpan(3,6,12);
        #else
            // 无事发生
        #endif

        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

// ---- cjf end ----

// ---- cjf  ----
void UITestVectorFont::cjf_BackgroundColor()
{
    if (container_ != nullptr) {
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("红色绿色黄色");
        label->SetBackgroundColorSpan(Color::Red(),0,2);
        label->SetBackgroundColorSpan(Color::Green(),2,4);
        label->SetBackgroundColorSpan(Color::Yellow(),4,6);
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;


        UILabel* label_2 = new UILabel();
        label_2->SetPosition(positionX_, positionY_);
        label_2->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label_2->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label_2->SetFont(HYQIHEI_65S, 24); // 24 : size
        label_2->SetText("红色绿色黄色");
        label_2->SetForegroundColorSpan(Color::Red(),0,2);
        label_2->SetForegroundColorSpan(Color::Green(),2,4);
        label_2->SetForegroundColorSpan(Color::Yellow(),4,6);
        container_->Add(label_2);
        positionY_ += LABEL_HEIGHT + GAP;

        UILabel* label_3 = new UILabel();
        label_3->SetPosition(positionX_, positionY_);
        label_3->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label_3->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label_3->SetFont(HYQIHEI_65S, 24); // 24 : size
        label_3->SetText("红色绿色黄色");
        label_3->SetLineBackgroundSpan(Color::Red(),0,2);
        label_3->SetLineBackgroundSpan(Color::Green(),2,4);
        label_3->SetLineBackgroundSpan(Color::Yellow(),4,6);
        container_->Add(label_3);
        positionY_ += LABEL_HEIGHT + GAP;

        UIViewBounds::GetInstance()->SetShowState(true);

    }
}
void UITestVectorFont::cjf_LineBackgroundColor()
{
    if (container_ != nullptr) {
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH*2, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("红色绿色黄色");

//        label->SetLineBackgroundSpan(Color::Green(),2,4);
//        label->SetLineBackgroundSpan(Color::Yellow(),4,6);
        label->SetLineBackgroundSpan(Color::Red(),0,2);

        UIViewBounds::GetInstance()->SetShowState(true);
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;



    }
}

// ---- cjf end ----
void UITestVectorFont::FontFontEngineVectorTestCHLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH Language");
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("测试中文");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineVectorTestCHLang002()
{
    if (container_ != nullptr) {
        InnerTestTitle("Multiline text");
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2：double
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("测试中英文混合：text和text text和text text和text");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT * 2 + GAP; // 2：double
    }
}

void UITestVectorFont::FontFontEngineVectorTestJALang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("JA Language");
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("ランニン");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineVectorTestJALang002()
{
    if (container_ != nullptr) {
        InnerTestTitle("JA Language");
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 30); // 30 : size
        label->SetText("ランニン");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineVectorTestKRLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("KR Language");
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("韩语리기");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineVectorTestVILang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("VI Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(ROBOTOCONDENSED_REGULAR, 24); // 24 : size
        label->SetText("Kết thúc tập luyện?");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

#if ENABLE_MULTI_FONT
void UITestVectorFont::FontFontEngineMultiLanguageTestJALang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH+JA Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                              UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(HYQIHEI_65S), findPath,
                                                             sizeof(findPath));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(HYQIHEI_65S, 24); // 24 : size
        label->SetText("日语ランニン");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestJALang002()
{
    if (container_ != nullptr) {
        InnerTestTitle("UnRegister JA Language");
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("韩语日语리기ランニン");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestKRLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH+KR Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                              UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(NOTOSANSCJKJP_REGULAR),
                                                             findPath, sizeof(findPath));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("韩语리기");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestVILang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH+VI Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath4[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                               UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(NOTOSANSCJKJP_REGULAR),
                                                             findPath4, sizeof(findPath4));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("越南语Kết thúc tập luyện?");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestCSLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH+CS Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath4[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                               UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(NOTOSANSCJKJP_REGULAR),
                                                             findPath4, sizeof(findPath4));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("捷克语sledování tepové frekvence");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestELLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("CH+EL Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath4[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                               UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(NOTOSANSCJKJP_REGULAR),
                                                             findPath4, sizeof(findPath4));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("西班牙España");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}

void UITestVectorFont::FontFontEngineMultiLanguageTestROLang001()
{
    if (container_ != nullptr) {
        InnerTestTitle("RO Language");
        UIFont::GetInstance()->RegisterFontInfo(NOTOSANSCJKJP_REGULAR);
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(ROBOTOCONDENSED_REGULAR);
        uint8_t findPath4[] = {UIFont::GetInstance()->GetFontId(HYQIHEI_65S),
                               UIFont::GetInstance()->GetFontId(ROBOTOCONDENSED_REGULAR)};
        UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(NOTOSANSCJKJP_REGULAR),
                                                             findPath4, sizeof(findPath4));
        UILabel* label = new UILabel();
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
        label->SetLineBreakMode(UILabel::LINE_BREAK_MARQUEE);
        label->SetFont(NOTOSANSCJKJP_REGULAR, 24); // 24 : size
        label->SetText("罗马尼亚Română");
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}
#endif
#if defined(ENABLE_SPANNABLE_STRING) && ENABLE_SPANNABLE_STRING
void UITestVectorFont::TestDrawTextITALYBOLD()
{
    if (container_ != nullptr) {
        InnerTestTitle("Display bold italic bold_italic");
        UILabel* label = new UILabel();
        UIFont::GetInstance()->RegisterFontInfo(HYQIHEI_65S);
        UIFont::GetInstance()->RegisterFontInfo(DEFAULT_VECTOR_FONT_FILENAME);
        label->SetPosition(positionX_, positionY_);
        label->Resize(LABEL_WIDTH * TWO, LABEL_HEIGHT);
        label->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
        label->SetText("图形子系统展示正常粗体斜体粗斜体");
        SpannableString spannableString(label->GetText());
        spannableString.SetTextStyle(TextStyle::TEXT_STYLE_ITALIC, ELEVEN, THIRTEEN);
        spannableString.SetTextStyle(TEXT_STYLE_BOLD, NINE, ELEVEN);
        spannableString.SetTextStyle(TEXT_STYLE_BOLD_ITALIC, THIRTEEN, SIXTEEN);
        label->SetText(&spannableString);
        container_->Add(label);
        positionY_ += LABEL_HEIGHT + GAP;
    }
}
#endif
} // namespace OHOS
#endif // ENABLE_VECTOR_FONT
