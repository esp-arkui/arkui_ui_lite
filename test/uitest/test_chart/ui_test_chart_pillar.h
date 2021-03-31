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

#ifndef UI_TEST_CHART_PILLAR_H
#define UI_TEST_CHART_PILLAR_H

#include "components/ui_chart.h"
#include "components/ui_label.h"
#include "components/ui_label_button.h"
#include "components/ui_scroll_view.h"
#include "ui_test.h"

namespace OHOS {
namespace {
    constexpr char* UI_TEST_CHART_POLLAR_ID = "Chart_Pillar";
    constexpr char* UI_TEST_CHART_PILLAR_ADD_DATA_BTN_ID_01 = "test_chart_pillar_add_data_btn_01";
    constexpr char* UI_TEST_CHART_PILLAR_DEL_DATA_BTN_ID_01 = "test_chart_pillar_del_data_btn_01";
    constexpr char* UI_TEST_CHART_PILLAR_CLR_DATA_BTN_ID_01 = "test_chart_pillar_clr_data_btn_01";
    constexpr char* UI_TEST_CHART_PILLAR_REVERSE_BTN_ID_01 = "test_chart_pillar_reverse_btn_01";
    constexpr char* UI_TEST_CHART_PILLAR_AXIS_COLOR_BTN_ID_01 = "test_chart_pillar_axis_color_btn_01";
    constexpr char* UI_TEST_CHART_PILLAR_AXIS_VISIBLE_BTN_ID_01 = "test_chart_pillar_axis_visible_btn_01";
}
class UITestChartPillar : public UITest, public UIView::OnClickListener {
public:
    UITestChartPillar() : dataSerial_() {}
    ~UITestChartPillar() {}
    void SetUp() override;
    void TearDown() override;
    UIView* GetTestView() override;

    void SetUpButton(UILabelButton* btn, const char* title, const char* id = nullptr);

    bool OnClick(UIView& view, const ClickEvent& event) override;

    void UIKit_ChartPillar_Test_AddDataSerial_001();
    void UIKit_ChartPillar_Test_EnableReverse_002();
    void UIKit_ChartPillar_Test_SetAxisLineColor_003();
    void UIKit_ChartPillar_Test_SetAxisLineVisible_004();

private:
    void SetLastPos(UIView* view);
    void InnerDeleteChildren(UIView* view) const;
    static constexpr uint8_t DATA_NUM = 3;
    UIChartDataSerial *dataSerial_[DATA_NUM];
    uint8_t curDataIndex_ = 0;

    UIScrollView* container_ = nullptr;
    UIChartPillar* chart_ = nullptr;
    UILabelButton* addDataSerialBtn_ = nullptr;
    UILabelButton* deleteDataSerialBtn_ = nullptr;
    UILabelButton* clearDataSerialBtn_ = nullptr;
    UILabelButton* reverseBtn_ = nullptr;
    UILabelButton* setAxisColorBtn_ = nullptr;
    UILabelButton* setAxisVisibleBtn_ = nullptr;

    int16_t lastX_ = 0;
    int16_t lastY_ = 0;
};
} // namespace OHOS
#endif // UI_TEST_CHART_PILLAR_H
