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

#include "ui_test_font.h"
#include "common/screen.h"
#if ENABLE_VECTOR_FONT
#else
#include "common/ui_text_language.h"
#endif
#include "components/ui_label.h"
#include "font/ui_font.h"
#if ENABLE_MULTI_FONT
#include "font/ui_multi_font_manager.h"
#endif

namespace OHOS {
namespace {
const int16_t GAP = 5;
const int16_t TITLE_HEIGHT = 20;
const uint16_t LABEL_WIDTH = 400;
const uint16_t LABEL_HEIGHT = 50;
const uint16_t FONT_SIZE = 30;
const char* SOURCE_HAN_SANS_SC_REGULAR = "SourceHanSansSC-Regular.otf";
const char* ROBOTO_CONDENSED_REGULAR = "RobotoCondensed-Regular.ttf";
} // namespace

void UITestFont::SetUp()
{
    if (container_ == nullptr) {
        container_ = new UIScrollView();
        container_->SetThrowDrag(true);
        container_->SetHorizontalScrollState(false);
        container_->Resize(Screen::GetInstance().GetWidth(), Screen::GetInstance().GetHeight() - LABEL_HEIGHT);
        positionX_ = 50; // 50: init position x
        positionY_ = 5;  // 5: init position y
    }
}

void UITestFont::TearDown()
{
    DeleteChildren(container_);
    container_ = nullptr;
}

void UITestFont::InnerTestTitle(const char* title)
{
    UILabel* titleLabel = new UILabel();
    titleLabel->SetPosition(TEXT_DISTANCE_TO_LEFT_SIDE, positionY_, Screen::GetInstance().GetWidth(),
                            TITLE_LABEL_DEFAULT_HEIGHT);
#if ENABLE_VECTOR_FONT
    titleLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, DEFAULT_VECTOR_FONT_SIZE);
#else
    titleLabel->SetFontId(F_SOURCEHANSANSSC_REGULAR_18_4);
#endif
    titleLabel->SetText(title);
    container_->Add(titleLabel);
    positionY_ += TITLE_HEIGHT + GAP;
}

const UIView* UITestFont::GetTestView()
{
    UIKitFontTestDispaly001();
    UIKitFontTestDispaly002();
    UIKitFontTestDispaly003();
    UIKitFontTestDispaly004();
    UIKitFontTestDispaly005();
    UIKitFontTestDispaly006();
    UIKitFontTestDispaly007();
    UIKitFontTestDispaly008();
    UIKitFontTestDispaly009();
    UIKitFontTestDispaly010();
    UIKitFontTestBaseline001();
    UIKitFontTestBaseline002();
    UIKitFontTestLineHeight001();
    UIKitFontTestLineHeight002();
#if ENABLE_MULTI_FONT
    UIKitFontMultiLanguage001();
    UIKitFontMultiLanguage002();
#endif
    return container_;
}
void UITestFont::UIKitFontTestDispaly001()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display single line Chinese");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("轻量图形子系统");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontTestDispaly002()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline Chinese");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2 : double
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText(" 你好\n轻量图形子系统");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 2 + GAP;  // 2 : double
}


void UITestFont::UIKitFontTestDispaly003()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display single line English");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("uikit test");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT + GAP;
}


void UITestFont::UIKitFontTestDispaly004()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline English");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2 : double
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText(" Hello\n uikit");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}


void UITestFont::UIKitFontTestDispaly005()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display single line Chinese and English");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("你好，轻量级图形 uikit");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontTestDispaly006()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline Chinese and English");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2 : double
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("你好\n轻量级图形 uikit");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}


void UITestFont::UIKitFontTestDispaly007()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display single line text color");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetStyle(STYLE_TEXT_COLOR, Color::Red().full);
    label->SetText("你好，轻量级图形");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontTestDispaly008()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline text color");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2 : double
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetStyle(STYLE_TEXT_COLOR, Color::Yellow().full);
    label->SetText("你好\n 轻量级图形 uikit");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}


void UITestFont::UIKitFontTestDispaly009()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline text color");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_ + 100, positionY_);  // 100 : offset x
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
    label->SetLineBreakMode(UILabel::LineBreakMode::LINE_BREAK_ADAPT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->Rotate(90, Vector2<float>{0, 0}); // 90 : angle
    label->SetText("轻量级图形");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 4 + GAP; // 4 : coefficient
}


void UITestFont::UIKitFontTestDispaly010()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Display multiline text color");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_ + 100, positionY_); // 100 : offset x
    label->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT); // 2 : half
    label->SetLineBreakMode(UILabel::LineBreakMode::LINE_BREAK_MARQUEE);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->Rotate(90, Vector2<float>{0, 0}); // 90 : angle
    label->SetText("轻量级图形,轻量级图形,轻量级图形");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 4 + GAP; // 4 : coefficient
}


void UITestFont::UIKitFontTestBaseline001()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle("Font baseline alignment");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("hello, uikit");

    UILabel* label2 = new UILabel();
    label2->SetPosition(positionX_ + (LABEL_WIDTH / 2), positionY_); // 2 : half
    label2->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    label2->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label2->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label2->SetText("hello, uikit");

    container_->Add(label);
    container_->Add(label2);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontTestBaseline002()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Font baseline alignment");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("hello, uikit");
    label->SetStyle(STYLE_LINE_HEIGHT, 30); // 30 : line height

    UILabel* label2 = new UILabel();
    label2->SetPosition(positionX_ + (LABEL_WIDTH / 2), positionY_); // 2 : half
    label2->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    label2->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label2->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label2->SetText("hello, uikit");
    label2->SetStyle(STYLE_LINE_HEIGHT, 30); // 30 : line height

    container_->Add(label);
    container_->Add(label2);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontTestLineHeight001()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Font lineheight alignment");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT * 2); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("hello,\n uikit");

    UILabel* label2 = new UILabel();
    label2->SetPosition(positionX_ + (LABEL_WIDTH / 2), positionY_); // 2 : half
    label2->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT * 2); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    label2->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label2->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label2->SetText("hello,\n uikit");

    container_->Add(label);
    container_->Add(label2);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}

void UITestFont::UIKitFontTestLineHeight002()
{
    if (container_ == nullptr) {
        return;
    }
    InnerTestTitle(" Font lineheight alignment");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT * 2); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    label->SetFont(SOURCE_HAN_SANS_SC_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_SOURCEHANSANSSC_REGULAR_30_4);
#endif
    label->SetText("hello,\n uikit");
    label->SetStyle(STYLE_LINE_HEIGHT, 40); // 40 : line height

    UILabel* label2 = new UILabel();
    label2->SetPosition(positionX_ + (LABEL_WIDTH / 2), positionY_); // 2 : half
    label2->Resize(LABEL_WIDTH / 2, LABEL_HEIGHT * 2); // 2 : half
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    label2->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label2->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label2->SetText("hello,\n uikit");
    label2->SetStyle(STYLE_LINE_HEIGHT, 40); // 40 : line height

    container_->Add(label);
    container_->Add(label2);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}

#if ENABLE_MULTI_FONT
void UITestFont::UIKitFontMultiLanguage001()
{
    if (container_ == nullptr) {
        return;
    }
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    uint8_t findPath[] = {UIFont::GetInstance()->GetFontId(SOURCE_HAN_SANS_SC_REGULAR)};
    UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(ROBOTO_CONDENSED_REGULAR),
                                                         findPath, sizeof(findPath));
#else
    uint8_t findPath[] = {F_SOURCEHANSANSSC_REGULAR_30_4};
    UIMultiFontManager::GetInstance()->SetSearchFontList(F_ROBOTOCONDENSED_REGULAR_30_4, findPath, sizeof(findPath));
#endif
    InnerTestTitle(" Display multilingual display");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT);
#if ENABLE_VECTOR_FONT
    label->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label->SetText("Hello, HarmonyOS Lite GUI");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT + GAP;
}

void UITestFont::UIKitFontMultiLanguage002()
{
    if (container_ == nullptr) {
        return;
    }
#if ENABLE_VECTOR_FONT
    UIFont::GetInstance()->RegisterFontInfo(SOURCE_HAN_SANS_SC_REGULAR);
    UIFont::GetInstance()->RegisterFontInfo(ROBOTO_CONDENSED_REGULAR);
    uint8_t findPath[] = {UIFont::GetInstance()->GetFontId(SOURCE_HAN_SANS_SC_REGULAR)};
    UIMultiFontManager::GetInstance()->SetSearchFontList(UIFont::GetInstance()->GetFontId(ROBOTO_CONDENSED_REGULAR),
                                                         findPath, sizeof(findPath));
#else
    uint8_t findPath[] = {F_SOURCEHANSANSSC_REGULAR_30_4};
    UIMultiFontManager::GetInstance()->SetSearchFontList(F_ROBOTOCONDENSED_REGULAR_30_4, findPath, sizeof(findPath));
#endif
    InnerTestTitle(" Display multilingual display");
    UILabel* label = new UILabel();
    label->SetPosition(positionX_, positionY_);
    label->Resize(LABEL_WIDTH, LABEL_HEIGHT * 2); // 2 : double
#if ENABLE_VECTOR_FONT
    label->SetFont(ROBOTO_CONDENSED_REGULAR, FONT_SIZE);
#else
    label->SetFontId(F_ROBOTOCONDENSED_REGULAR_30_4);
#endif
    label->SetText("Hello\n HarmonyOS Lite GUI");
    container_->Add(label);
    positionY_ += LABEL_HEIGHT * 2 + GAP; // 2 : double
}
#endif // ENABLE_MULTI_FONT
} // namespace OHOS
