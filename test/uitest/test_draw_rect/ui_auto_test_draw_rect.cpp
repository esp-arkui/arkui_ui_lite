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

#include "ui_auto_test_draw_rect.h"
#include "ui_test_draw_rect.h"

namespace OHOS {
void UiAutoTestDrawRect::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_DRAW_RECT_ID);
}

void UiAutoTestDrawRect::RunTestList()
{
    Reset();
    UIKitDrawRectTestRectNoBorder001();
    UIKitDrawRectTestRectNoBorder002();
    UIKitDrawRectTestRectNoBorder003();
    UIKitDrawRectTestRectNoBorder004();
    UIKitDrawRectTestRectNoBorder005();
    UIKitDrawRectTestRectNoBorder006();
    UIKitDrawRectTestRectNoBorder007();
    UIKitDrawRectTestRectNoBorder008();
    UIKitDrawRectTestRectNoBorder009();
    UIKitDrawRectTestRectNoBorder010();
    UIKitDrawRectTestRectBorderNoRadius001();
    UIKitDrawRectTestRectBorderNoRadius002();
    UIKitDrawRectTestRectBorderRadius001();
    UIKitDrawRectTestRectBorderRadius002();
    UIKitDrawRectTestRectBorderRadius003();
    UIKitDrawRectTestRectBorderRadius004();
    UIKitDrawRectTestRectBorderRadius005();
    UIKitDrawRectTestRectBorderRadius006();
    UIKitDrawRectTestRectBorderRadius007();
    UIKitDrawRectTestRectBorderRadius008();
    UIKitDrawRectTestRectBorderRadius009();
    UIKitDrawRectTestRectBorderRadius010();
    UIKitDrawRectTestRectBorderRadius011();
    UIKitDrawRectTestRect001();
    UIKitDrawRectTestRectColor001();
    UIKitDrawRectTestRectRadius001();
    UIKitDrawRectTestRectRadius002();
    UIKitDrawRectTestRectRadius003();
    UIKitDrawRectTestRectRadius004();
    UIKitDrawRectTestRectOpacity001();
    UIKitDrawRectTestRectOpacity002();
    UIKitDrawRectTestRectOpacity003();
    UIKitDrawRectTestRectBorderColor001();
    UIKitDrawRectTestRectBorderWidth001();
    UIKitDrawRectTestRectBorderWidth002();
    UIKitDrawRectTestRectBorderOpacity001();
    UIKitDrawRectTestRectBorderOpacity002();
    UIKitDrawRectTestRectBorderOpacity003();
    UIKitDrawRectTestRectPadding001();
    UIKitDrawRectTestRectPadding002();
    UIKitDrawRectTestRectMargin001();
    UIKitDrawRectTestRectMargin002();
    UIKitDrawRectTestRectPaddingMargin001();
    UIKitDrawRectTestRectPaddingMargin002();
    UIKitDrawRectTestRectWidthAndHeight001();
    UIKitDrawRectTestRectWidthAndHeight002();
    UIKitDrawRectTestRectWidthAndHeight003();
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_no_border_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_no_border_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_no_border_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder004() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_04);
    const char* fileName = "ui_test_draw_rect_no_border_04.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder005() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_05);
    const char* fileName = "ui_test_draw_rect_no_border_05.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder006() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_06);
    const char* fileName = "ui_test_draw_rect_no_border_06.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder007() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_07);
    const char* fileName = "ui_test_draw_rect_no_border_07.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder008() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_08);
    const char* fileName = "ui_test_draw_rect_no_border_08.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder009() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_09);
    const char* fileName = "ui_test_draw_rect_no_border_09.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectNoBorder010() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_10);
    const char* fileName = "ui_test_draw_rect_no_border_10.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderNoRadius001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_no_radius_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderNoRadius002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_no_radius_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_border_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_border_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_border_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius004() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_04);
    const char* fileName = "ui_test_draw_rect_border_04.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius005() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_05);
    const char* fileName = "ui_test_draw_rect_border_05.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius006() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_06);
    const char* fileName = "ui_test_draw_rect_border_06.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius007() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_07);
    const char* fileName = "ui_test_draw_rect_border_07.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius008() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_08);
    const char* fileName = "ui_test_draw_rect_border_08.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius009() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_09);
    const char* fileName = "ui_test_draw_rect_border_09.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius010() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_10);
    const char* fileName = "ui_test_draw_rect_border_10.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderRadius011() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_11);
    const char* fileName = "ui_test_draw_rect_border_11.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRect001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECT_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectColor001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTCOLOR_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_color_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectRadius001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_radius_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectRadius002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_radius_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectRadius003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_rect_radius_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectRadius004() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_04);
    const char* fileName = "ui_test_draw_rect_rect_radius_04.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectOpacity001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_opacity_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectOpacity002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_opacity_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectOpacity003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_rect_opacity_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderColor001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDERCOLOR_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_border_color_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderWidth001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_border_width_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderWidth002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_border_width_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderOpacity001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_border_opacity_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderOpacity002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_border_opacity_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectBorderOpacity003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_rect_border_opacity_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectPadding001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_padding_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectPadding002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_padding_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectMargin001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_margin_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectMargin002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_margin_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectPaddingMargin001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_margin_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectPaddingMargin002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_margin_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectWidthAndHeight001() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_01);
    const char* fileName = "ui_test_draw_rect_rect_width_and_height_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectWidthAndHeight002() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_02);
    const char* fileName = "ui_test_draw_rect_rect_width_and_height_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestDrawRect::UIKitDrawRectTestRectWidthAndHeight003() const
{
    DragViewToHead(UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_03);
    const char* fileName = "ui_test_draw_rect_rect_width_and_height_03.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS