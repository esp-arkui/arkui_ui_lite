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

#ifndef UI_TEST_CANVAS_H
#define UI_TEST_CANVAS_H

#include "components/ui_canvas.h"
#include "components/ui_label_button.h"
#include "components/ui_scroll_view.h"
#include "layout/grid_layout.h"
#include "ui_test.h"

namespace OHOS {
namespace {
constexpr char* UI_TEST_CANVAS_DRAW_LINE_LABEL_ID_01 = "test_canvas_draw_line_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_LINE_LABEL_ID_02 = "test_canvas_draw_line_label_02";
constexpr char* UI_TEST_CANVAS_DRAW_CURVE_LABEL_ID_01 = "test_canvas_draw_curve_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_CURVE_LABEL_ID_02 = "test_canvas_draw_curve_label_02";
constexpr char* UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_01 = "test_canvas_draw_rect_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_02 = "test_canvas_draw_rect_label_02";
constexpr char* UI_TEST_CANVAS_DRAW_RECT_LABEL_ID_03 = "test_canvas_draw_rect_label_03";
constexpr char* UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_01 = "test_canvas_draw_circle_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_02 = "test_canvas_draw_circle_label_02";
constexpr char* UI_TEST_CANVAS_DRAW_CIRCLE_LABEL_ID_03 = "test_canvas_draw_circle_label_03";
constexpr char* UI_TEST_CANVAS_DRAW_ARC_LABEL_ID_01 = "test_canvas_draw_arc_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_IMAGE_LABEL_ID_01 = "test_canvas_draw_image_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_LABEL_LABEL_ID_01 = "test_canvas_draw_label_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_SECTOR_LABEL_ID_01 = "test_canvas_draw_sector_label_01";
constexpr char* UI_TEST_CANVAS_CLEAR_LABEL_ID_01 = "test_canvas_clear_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_01 = "test_canvas_draw_path_label_01";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_02 = "test_canvas_draw_path_label_02";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_03 = "test_canvas_draw_path_label_03";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_04 = "test_canvas_draw_path_label_04";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_05 = "test_canvas_draw_path_label_05";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_06 = "test_canvas_draw_path_label_06";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_07 = "test_canvas_draw_path_label_07";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_08 = "test_canvas_draw_path_label_08";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_09 = "test_canvas_draw_path_label_09";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_10 = "test_canvas_draw_path_label_10";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_11 = "test_canvas_draw_path_label_11";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_12 = "test_canvas_draw_path_label_12";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_13 = "test_canvas_draw_path_label_13";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_14 = "test_canvas_draw_path_label_14";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_15 = "test_canvas_draw_path_label_15";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_16 = "test_canvas_draw_path_label_16";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_17 = "test_canvas_draw_path_label_17";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_18 = "test_canvas_draw_path_label_18";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_19 = "test_canvas_draw_path_label_19";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_20 = "test_canvas_draw_path_label_20";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_21 = "test_canvas_draw_path_label_21";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_22 = "test_canvas_draw_path_label_22";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_23 = "test_canvas_draw_path_label_23";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_24 = "test_canvas_draw_path_label_24";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_25 = "test_canvas_draw_path_label_25";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_26 = "test_canvas_draw_path_label_26";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_27 = "test_canvas_draw_path_label_27";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_28 = "test_canvas_draw_path_label_28";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_29 = "test_canvas_draw_path_label_29";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_30 = "test_canvas_draw_path_label_30";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_31 = "test_canvas_draw_path_label_31";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_32 = "test_canvas_draw_path_label_32";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_33 = "test_canvas_draw_path_label_33";
constexpr char* UI_TEST_CANVAS_DRAW_PATH_LABEL_ID_34 = "test_canvas_draw_path_label_34";
}
class UITestCanvas : public UITest {
public:
    UITestCanvas() {}
    ~UITestCanvas() {}
    void SetUp() override;
    void TearDown() override;
    const UIView* GetTestView() override;

    void UIKitCanvasTestDrawLine001();
    void UIKitCanvasTestDrawLine002();
    void UIKitCanvasTestDrawCurve001();
    void UIKitCanvasTestDrawCurve002();
    void UIKitCanvasTestDrawRect001();
    void UIKitCanvasTestDrawRect002();
    void UIKitCanvasTestDrawRect003();
    void UIKitCanvasTestDrawCircle001();
    void UIKitCanvasTestDrawCircle002();
    void UIKitCanvasTestDrawCircle003();
    void UIKitCanvasTestDrawArc001();
    void UIKitCanvasTestDrawImage001();
    void UIKitCanvasTestDrawLabel001();
    void UIKitCanvasTestDrawSector001();
    void UIKitCanvasTestClear001();
    void UIKitCanvasTestDrawPath001();
    void UIKitCanvasTestDrawPath002();
    void UIKitCanvasTestDrawPath003();
    void UIKitCanvasTestDrawPath004();
    void UIKitCanvasTestDrawPath005();
    void UIKitCanvasTestDrawPath006();
    void UIKitCanvasTestDrawPath007();
    void UIKitCanvasTestDrawPath008();
    void UIKitCanvasTestDrawPath009();
    void UIKitCanvasTestDrawPath010();
    void UIKitCanvasTestDrawPath011();
    void UIKitCanvasTestDrawPath012();
    void UIKitCanvasTestDrawPath013();
    void UIKitCanvasTestDrawPath014();
    void UIKitCanvasTestDrawPath015();
    void UIKitCanvasTestDrawPath016();
    void UIKitCanvasTestDrawPath017();
    void UIKitCanvasTestDrawPath018();
    void UIKitCanvasTestDrawPath019();
    void UIKitCanvasTestDrawPath020();
    void UIKitCanvasTestDrawPath021();
    void UIKitCanvasTestDrawPath022();
    void UIKitCanvasTestDrawPath023();
    void UIKitCanvasTestDrawPath024();
    void UIKitCanvasTestDrawPath025();
    void UIKitCanvasTestDrawPath026();
    void UIKitCanvasTestDrawPath027();
    void UIKitCanvasTestDrawPath028();
    void UIKitCanvasTestDrawPath029();
    void UIKitCanvasTestDrawPath030();
    void UIKitCanvasTestDrawPath031();
    void UIKitCanvasTestDrawPath032();
    void UIKitCanvasTestDrawPath033();
    void UIKitCanvasTestDrawPath034();

private:
    const static int16_t GAP = 10;
    const static int16_t TITLE_HEIGHT = 29;
    const static uint16_t CANVAS_WIDTH = 454;
    const static uint16_t CANVAS_HEIGHT = 200;

    static constexpr int16_t START1_X = 10;
    static constexpr int16_t START1_Y = 10;
    static constexpr int16_t START2_X = 80;
    static constexpr int16_t START2_Y = 10;
    static constexpr int16_t LINE1_X = 40;
    static constexpr int16_t LINE1_Y = 100;
    static constexpr int16_t LINE2_X = 100;
    static constexpr int16_t LINE2_Y = 120;
    static constexpr int16_t CENTER_X = 150;
    static constexpr int16_t CENTER_Y = 150;
    static constexpr int16_t RADIUS = 50;
    static constexpr int16_t START_ANGLE = 30;
    static constexpr int16_t END_ANGLE = 250;
    static constexpr int16_t RECT_X = 250;
    static constexpr int16_t RECT_Y = 50;
    static constexpr int16_t RECT_WIDTH = 100;
    static constexpr int16_t RECT_HEIGHT = 50;
    void CreateTitleLabel(const char* title, const char* id = nullptr);
    UICanvas* CreateCanvas();

    UIScrollView* container_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_CANVAS_H
