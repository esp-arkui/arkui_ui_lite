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

#include "ui_auto_test_chart_pillar.h"
#include "ui_test_chart_pillar.h"

namespace OHOS {
void UIAutoTestChartPillar::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_CHART_POLLAR_ID);
}

void UIAutoTestChartPillar::RunTestList()
{
    Reset();
    UIKitChartPillarTestAddDataSerial001();
    UIKitChartPillarTestDeleteDataSerial001();
    UIKitChartPillarTestClearDataSerial001();
    UIKitChartPillarTestReverse001();
    UIKitChartPillarTestSetAxisLineColor001();
    UIKitChartPillarTestSetAxisLineVisible001();
}

void UIAutoTestChartPillar::UIKitChartPillarTestAddDataSerial001() const
{
    Reset();
    const char* fileName = "ui_test_chart_pillar_add_data_01.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_pillar_add_data_02.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_pillar_add_data_03.bin";
    CompareByBinary(fileName3);
}

void UIAutoTestChartPillar::UIKitChartPillarTestDeleteDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_PILLAR_DEL_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_pillar_del_data_01.bin";
    CompareByBinary(fileName);
    ClickViewById(UI_TEST_CHART_PILLAR_DEL_DATA_BTN_ID_01);
    const char* fileName2 = "ui_test_chart_pillar_del_data_02.bin";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CHART_PILLAR_DEL_DATA_BTN_ID_01);
    const char* fileName3 = "ui_test_chart_pillar_del_data_03.bin";
    CompareByBinary(fileName3);
}

void UIAutoTestChartPillar::UIKitChartPillarTestClearDataSerial001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);
    ClickViewById(UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01);

    ClickViewById(UI_TEST_CHART_PILLAR_CLR_DATA_BTN_ID_01);
    const char* fileName = "ui_test_chart_pillar_clr_data_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestChartPillar::UIKitChartPillarTestReverse001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_PILLAR_REVERSE_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_reverse_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestChartPillar::UIKitChartPillarTestSetAxisLineColor001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_PILLAR_AXIS_COLOR_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_axis_color_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestChartPillar::UIKitChartPillarTestSetAxisLineVisible001() const
{
    Reset();
    ClickViewById(UI_TEST_CHART_PILLAR_AXIS_VISIBLE_BTN_ID_01);
    const char* fileName = "ui_test_chart_ployline_axis_visible_01.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS