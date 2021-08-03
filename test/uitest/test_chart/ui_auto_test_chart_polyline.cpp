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

#include "ui_auto_test_chart_polyline.h"
#include "ui_test_chart_polyline.h"

namespace OHOS {
void UIAutoTestChartPolyline::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Chart_Polyline");
}

void UIAutoTestChartPolyline::RunTestList()
{
    Reset();
    UIKitChartPolylineTestAddDataSerial001();
    UIKitChartPolylineTestDeleteDataSerial001();
    UIKitChartPolylineTestClearDataSerial001();
    UIKitChartPolylineTestTopPoint001();
    UIKitChartPolylineTestBotPoint001();
    UIKitChartPolylineTestHeadPoint001();
    UIKitChartPolylineTestReverse001();
    UIKitChartPolylineTestGradientBottom001();
    UIKitChartPolylineTestAddPoints001();
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestAddDataSerial001() const
{
    Reset();
    const char* fileName = "ui_test_chart_ployline_add_data_01.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_add_data_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_ployline_add_data_03.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestDeleteDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_del_data_01.bmp";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_del_data_02.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_ployline_del_data_03.bmp";
    CompareByBinary(fileName3);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestClearDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_POLYLINE_CLR_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_clr_data_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestTopPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_TOP_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_top_point_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestBotPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_BOT_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_bot_point_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestHeadPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_HEAD_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_head_point_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestReverse001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_REVERSE_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_reverse_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestGradientBottom001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_GRADIENT_BOT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_gradient_bot_01.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestChartPolyline::UIKitChartPolylineTestAddPoints001() const
{
    Reset();
    for (uint8_t i = 0; i < 8; i++) { // 8: click button times;
        ClickViewById(UI_TEST_CHART_POLYLINE_ADD_POINTS_BTN_ID_01);
    }
    const char* fileName = "ui_test_chart_ployline_add_points_01.bmp";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_CHART_POLYLINE_SMOOTH_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_smooth_01.bmp";
    CompareByBinary(fileName2);
}
} // namespace OHOS
