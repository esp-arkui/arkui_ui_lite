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

#include "ui_auto_test_draw_line.h"
#include "ui_test_draw_line.h"

namespace OHOS {
void UiAutoTestDrawLine::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_DRAW_LINE_ID);
}

void UiAutoTestDrawLine::RunTestList()
{
    Reset();
    UIKitDrawLineTestLine001();
    UIKitDrawLineTestLine002();
    UIKitDrawLineTestLine003();
    UIKitDrawLineTestLine004();
    UIKitDrawLineTestLine005();
    UIKitDrawLineTestLine006();
    UIKitDrawLineTestLine007();
    UIKitDrawLineTestLine008();
    UIKitDrawLineTestLine009();
    UIKitDrawLineTestLine010();
    UIKitDrawLineTestLine011();
    UIKitDrawLineTestLine012();
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine001() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_01);
    const char* fileName = "ui_test_draw_line_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine002() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_02);
    const char* fileName = "ui_test_draw_line_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine003() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_03);
    const char* fileName = "ui_test_draw_line_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine004() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_04);
    const char* fileName = "ui_test_draw_line_04.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine005() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_05);
    const char* fileName = "ui_test_draw_line_05.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine006() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_06);
    const char* fileName = "ui_test_draw_line_06.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine007() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_07);
    const char* fileName = "ui_test_draw_line_07.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine008() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_08);
    const char* fileName = "ui_test_draw_line_08.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine009() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_09);
    const char* fileName = "ui_test_draw_line_09.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine010() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_10);
    const char* fileName = "ui_test_draw_line_10.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine011() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_11);
    const char* fileName = "ui_test_draw_line_11.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawLine::UIKitDrawLineTestLine012() const
{
    DragViewToHead(UI_TEST_DRAW_LINE_LABEL_ID_12);
    const char* fileName = "ui_test_draw_line_12.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS