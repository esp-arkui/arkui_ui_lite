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

#include "ui_auto_test_canvas.h"
#include "ui_test_canvas.h"

namespace OHOS {
void UiAutoTestCanvas::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_CANVAS_ID);
}

void UiAutoTestCanvas::RunTestList()
{
    Reset();
    UIKitCanvasTestDrawLine001();
    UIKitCanvasTestDrawLine002();
    UIKitCanvasTestDrawCurve001();
    UIKitCanvasTestDrawCurve002();
    UIKitCanvasTestDrawRect001();
    UIKitCanvasTestDrawRect002();
    UIKitCanvasTestDrawRect003();
    UIKitCanvasTestDrawCircle001();
    UIKitCanvasTestDrawCircle002();
    UIKitCanvasTestDrawCircle003();
    UIKitCanvasTestDrawArc001();
    UIKitCanvasTestDrawImage001();
    UIKitCanvasTestDrawLabel001();
    UIKitCanvasTestDrawSector001();
    UIKitCanvasTestClear001();
    UIKitCanvasTestDrawPath001();
    UIKitCanvasTestDrawPath002();
    UIKitCanvasTestDrawPath003();
    UIKitCanvasTestDrawPath004();
    UIKitCanvasTestDrawPath005();
    UIKitCanvasTestDrawPath006();
    UIKitCanvasTestDrawPath007();
    UIKitCanvasTestDrawPath008();
    UIKitCanvasTestDrawPath009();
    UIKitCanvasTestDrawPath010();
    UIKitCanvasTestDrawPath011();
    UIKitCanvasTestDrawPath012();
    UIKitCanvasTestDrawPath013();
    UIKitCanvasTestDrawPath014();
    UIKitCanvasTestDrawPath015();
    UIKitCanvasTestDrawPath016();
    UIKitCanvasTestDrawPath017();
    UIKitCanvasTestDrawPath018();
    UIKitCanvasTestDrawPath019();
    UIKitCanvasTestDrawPath020();
    UIKitCanvasTestDrawPath021();
    UIKitCanvasTestDrawPath022();
    UIKitCanvasTestDrawPath023();
    UIKitCanvasTestDrawPath024();
    UIKitCanvasTestDrawPath025();
    UIKitCanvasTestDrawPath026();
    UIKitCanvasTestDrawPath027();
    UIKitCanvasTestDrawPath028();
    UIKitCanvasTestDrawPath029();
    UIKitCanvasTestDrawPath030();
    UIKitCanvasTestDrawPath031();
    UIKitCanvasTestDrawPath032();
    UIKitCanvasTestDrawPath033();
    UIKitCanvasTestDrawPath034();
}
void UiAutoTestCanvas::UIKitCanvasTestDrawLine001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_LINE_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_line_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawLine002() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_LINE_LABEL_ID_02);
    const char* fileName = "ui_test_canvas_draw_line_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawCurve001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_CURVE_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_curve_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawCurve002() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_CURVE_LABEL_ID_02);
    const char* fileName = "ui_test_canvas_draw_curve_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawRect001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_rect_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawRect002() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_02);
    const char* fileName = "ui_test_canvas_draw_rect_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawRect003() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_03);
    const char* fileName = "ui_test_canvas_draw_rect_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawCircle001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_circle_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawCircle002() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_02);
    const char* fileName = "ui_test_canvas_draw_circle_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawCircle003() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_03);
    const char* fileName = "ui_test_canvas_draw_circle_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawArc001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_ARC_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_arc_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawImage001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_IMAGE_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_image_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawLabel001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_LABEL_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_label_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawSector001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_SECTOR_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_sector_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestClear001() const
{
    DragViewToHead(UI_TEST_CANVAS_CLEAR_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_clear_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath001() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_01);
    const char* fileName = "ui_test_canvas_draw_path_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath002() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_02);
    const char* fileName = "ui_test_canvas_draw_path_02.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath003() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_03);
    const char* fileName = "ui_test_canvas_draw_path_03.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath004() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_04);
    const char* fileName = "ui_test_canvas_draw_path_04.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath005() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_05);
    const char* fileName = "ui_test_canvas_draw_path_05.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath006() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_06);
    const char* fileName = "ui_test_canvas_draw_path_06.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath007() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_07);
    const char* fileName = "ui_test_canvas_draw_path_07.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath008() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_08);
    const char* fileName = "ui_test_canvas_draw_path_08.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath009() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_09);
    const char* fileName = "ui_test_canvas_draw_path_09.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath010() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_10);
    const char* fileName = "ui_test_canvas_draw_path_10.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath011() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_11);
    const char* fileName = "ui_test_canvas_draw_path_11.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath012() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_12);
    const char* fileName = "ui_test_canvas_draw_path_12.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath013() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_13);
    const char* fileName = "ui_test_canvas_draw_path_13.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath014() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_14);
    const char* fileName = "ui_test_canvas_draw_path_14.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath015() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_15);
    const char* fileName = "ui_test_canvas_draw_path_15.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath016() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_16);
    const char* fileName = "ui_test_canvas_draw_path_16.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath017() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_17);
    const char* fileName = "ui_test_canvas_draw_path_17.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath018() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_18);
    const char* fileName = "ui_test_canvas_draw_path_18.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath019() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_19);
    const char* fileName = "ui_test_canvas_draw_path_19.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath020() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_20);
    const char* fileName = "ui_test_canvas_draw_path_20.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath021() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_21);
    const char* fileName = "ui_test_canvas_draw_path_21.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath022() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_22);
    const char* fileName = "ui_test_canvas_draw_path_22.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath023() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_23);
    const char* fileName = "ui_test_canvas_draw_path_23.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath024() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_24);
    const char* fileName = "ui_test_canvas_draw_path_24.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath025() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_25);
    const char* fileName = "ui_test_canvas_draw_path_25.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath026() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_26);
    const char* fileName = "ui_test_canvas_draw_path_26.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath027() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_27);
    const char* fileName = "ui_test_canvas_draw_path_27.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath028() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_28);
    const char* fileName = "ui_test_canvas_draw_path_28.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath029() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_29);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath030() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_30);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath031() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_31);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath032() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_32);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath033() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_33);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}

void UiAutoTestCanvas::UIKitCanvasTestDrawPath034() const
{
    DragViewToHead(UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_34);
    const char* fileName = "ui_test_canvas_draw_path_29.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS