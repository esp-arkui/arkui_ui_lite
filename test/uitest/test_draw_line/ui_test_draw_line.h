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

#ifndef UI_TEST_DRAW_LINE_H
#define UI_TEST_DRAW_LINE_H

#include "components/ui_canvas.h"
#include "components/ui_label.h"
#include "components/ui_scroll_view.h"
#include "components/ui_view_group.h"
#include "ui_test.h"

namespace OHOS {
namespace {
    constexpr char* UI_TEST_DRAW_LINE_ID = "Draw_Line";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_01 = "test_draw_line_01";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_02 = "test_draw_line_02";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_03 = "test_draw_line_03";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_04 = "test_draw_line_04";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_05 = "test_draw_line_05";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_06 = "test_draw_line_06";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_07 = "test_draw_line_07";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_08 = "test_draw_line_08";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_09 = "test_draw_line_09";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_10 = "test_draw_line_10";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_11 = "test_draw_line_11";
    constexpr char* UI_TEST_DRAW_LINE_LABEL_ID_12 = "test_draw_line_12";
}
class UITestDrawLine : public UITest {
public:
    UITestDrawLine() {}
    ~UITestDrawLine() {}
    void SetUp() override;
    void TearDown() override;
    UIView* GetTestView() override;

    /**
     * @brief Test draw line Function
     */
    void UIKit_Draw_Line_Test_Line_001();
    void UIKit_Draw_Line_Test_Line_002();
    void UIKit_Draw_Line_Test_Line_003();
    void UIKit_Draw_Line_Test_Line_004();
    void UIKit_Draw_Line_Test_Line_005();
    void UIKit_Draw_Line_Test_Line_006();
    void UIKit_Draw_Line_Test_Line_007();
    void UIKit_Draw_Line_Test_Line_008();
    void UIKit_Draw_Line_Test_Line_009();
    void UIKit_Draw_Line_Test_Line_010();
    void UIKit_Draw_Line_Test_Line_011();
    void UIKit_Draw_Line_Test_Line_012();

private:
    UIViewGroup* CreateTestCaseGroup() const;
    UILabel* CreateTitleLabel() const;
    UICanvas* CreateCanvas() const;
    UIScrollView* container_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_DRAW_LINE_H
