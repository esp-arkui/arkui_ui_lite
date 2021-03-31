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
void UiAutoTestChartPolyline::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_CHART_POLYLINE_ID);
}

void UiAutoTestChartPolyline::RunTestList()
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
void UiAutoTestChartPolyline::UIKitChartPolylineTestAddDataSerial001() const
{
    Reset();
    const char* fileName = "ui_test_chart_ployline_add_data_01.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_add_data_02.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_ployline_add_data_03.bin";
    CompareByBinary(fileName3);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestDeleteDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_del_data_01.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_del_data_02.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_POLYLINE_DEL_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_ployline_del_data_03.bin";
    CompareByBinary(fileName3);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestClearDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_POLYLINE_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_POLYLINE_CLR_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_clr_data_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestTopPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_TOP_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_top_point_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestBotPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_BOT_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_bot_point_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestHeadPoint001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_HEAD_POINT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_head_point_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestReverse001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_REVERSE_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_reverse_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestGradientBottom001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_POLYLINE_GRADIENT_BOT_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_gradient_bot_01.bin";
    CompareByBinary(fileName);
}

void UiAutoTestChartPolyline::UIKitChartPolylineTestAddPoints001() const
{
    Reset();
    for (uint8_t i = 0; i < 8; i++) { // 8: click button times;
        ClickViewById(UI_TEST_CHART_POLYLINE_ADD_POINTS_BTN_ID_01);
    }
    const char* fileName = "ui_test_chart_ployline_add_points_01.bin";
    CompareByBinary(fileName);

    ClickViewById(UI_TEST_CHART_POLYLINE_SMOOTH_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_ployline_smooth_01.bin";
    CompareByBinary(fileName2);
}
} // namespace OHOS
