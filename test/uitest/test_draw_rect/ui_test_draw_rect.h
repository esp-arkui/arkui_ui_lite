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

#ifndef UI_TEST_DRAW_RECT_H
#define UI_TEST_DRAW_RECT_H

#include "components/ui_button.h"
#include "components/ui_label.h"
#include "components/ui_scroll_view.h"
#include "ui_test.h"

namespace OHOS {
namespace {
    constexpr char* UI_TEST_DRAW_RECT_ID = "Draw_Rect";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_01 = "test_draw_rect_rect_no_border_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_02 = "test_draw_rect_rect_no_border_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_03 = "test_draw_rect_rect_no_border_03";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_04 = "test_draw_rect_rect_no_border_04";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_05 = "test_draw_rect_rect_no_border_05";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_06 = "test_draw_rect_rect_no_border_06";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_07 = "test_draw_rect_rect_no_border_07";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_08 = "test_draw_rect_rect_no_border_08";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_09 = "test_draw_rect_rect_no_border_09";
    constexpr char* UI_TEST_DRAW_RECT_RECTNOBORDER_LABEL_ID_10 = "test_draw_rect_rect_no_border_10";
    constexpr char* UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_01 = "test_draw_rect_border_no_radius_01";
    constexpr char* UI_TEST_DRAW_RECT_BORDERNORADIUS_LABEL_ID_02 = "test_draw_rect_border_no_radius_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_01 = "test_draw_rect_rect_border_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_02 = "test_draw_rect_rect_border_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_03 = "test_draw_rect_rect_border_03";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_04 = "test_draw_rect_rect_border_04";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_05 = "test_draw_rect_rect_border_05";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_06 = "test_draw_rect_rect_border_06";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_07 = "test_draw_rect_rect_border_07";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_08 = "test_draw_rect_rect_border_08";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_09 = "test_draw_rect_rect_border_09";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_10 = "test_draw_rect_rect_border_10";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDER_LABEL_ID_11 = "test_draw_rect_rect_border_11";
    constexpr char* UI_TEST_DRAW_RECT_RECT_LABEL_ID_01 = "test_draw_rect_rect_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTCOLOR_LABEL_ID_01 = "test_draw_rect_rect_color_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_01 = "test_draw_rect_rect_radius_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_02 = "test_draw_rect_rect_radius_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_03 = "test_draw_rect_rect_radius_03";
    constexpr char* UI_TEST_DRAW_RECT_RECTRADIUS_LABEL_ID_04 = "test_draw_rect_rect_radius_04";
    constexpr char* UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_01 = "test_draw_rect_rect_opacity_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_02 = "test_draw_rect_rect_opacity_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTOPACITY_LABEL_ID_03 = "test_draw_rect_rect_opacity_03";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDERCOLOR_LABEL_ID_01 = "test_draw_rect_rect_border_color_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_01 = "test_draw_rect_rect_border_width_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDERWIDTH_LABEL_ID_02 = "test_draw_rect_rect_border_width_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_01 = "test_draw_rect_rect_border_opacity_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_02 = "test_draw_rect_rect_border_opacity_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTBORDEROPACITY_LABEL_ID_03 = "test_draw_rect_rect_border_opacity_03";
    constexpr char* UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_01 = "test_draw_rect_rect_padding_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTPADDING_LABEL_ID_02 = "test_draw_rect_rect_padding_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_01 = "test_draw_rect_rect_margin_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTMARGIN_LABEL_ID_02 = "test_draw_rect_rect_margin_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTPADDINGMARGIN_LABEL_ID_01 = "test_draw_rect_rect_padding_margin_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTPADDINGMARGIN_LABEL_ID_02 = "test_draw_rect_rect_padding_margin_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_01 = "test_draw_rect_rect_width_and_height_01";
    constexpr char* UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_02 = "test_draw_rect_rect_width_and_height_02";
    constexpr char* UI_TEST_DRAW_RECT_RECTWIDTHANDHEIGHT_LABEL_ID_03 = "test_draw_rect_rect_width_and_height_03";
}
class UITestDrawRect : public UITest {
public:
    UITestDrawRect() {}
    ~UITestDrawRect() {}
    void SetUp() override;
    void TearDown() override;
    UIView* GetTestView() override;

    void UIKit_DrawRect_Test_RectNoBorder_001();
    void UIKit_DrawRect_Test_RectNoBorder_002();
    void UIKit_DrawRect_Test_RectNoBorder_003();
    void UIKit_DrawRect_Test_RectNoBorder_004();
    void UIKit_DrawRect_Test_RectNoBorder_005();
    void UIKit_DrawRect_Test_RectNoBorder_006();
    void UIKit_DrawRect_Test_RectNoBorder_007();
    void UIKit_DrawRect_Test_RectNoBorder_008();
    void UIKit_DrawRect_Test_RectNoBorder_009();
    void UIKit_DrawRect_Test_RectNoBorder_0010();
    void UIKit_DrawRect_Test_RectBorderNoRadius_001();
    void UIKit_DrawRect_Test_RectBorderNoRadius_002();
    void UIKit_DrawRect_Test_RectBorderRadius_001();
    void UIKit_DrawRect_Test_RectBorderRadius_002();
    void UIKit_DrawRect_Test_RectBorderRadius_003();
    void UIKit_DrawRect_Test_RectBorderRadius_004();
    void UIKit_DrawRect_Test_RectBorderRadius_005();
    void UIKit_DrawRect_Test_RectBorderRadius_006();
    void UIKit_DrawRect_Test_RectBorderRadius_007();
    void UIKit_DrawRect_Test_RectBorderRadius_008();
    void UIKit_DrawRect_Test_RectBorderRadius_009();
    void UIKit_DrawRect_Test_RectBorderRadius_010();
    void UIKit_DrawRect_Test_RectBorderRadius_011();

    /**
     * @brief Test draw rect function
     */
    void UIKit_DrawRect_Test_Rect_001();

    /**
     * @brief Test draw rect background color
     */
    void UIKit_DrawRect_Test_RectColor_001();

    /**
     * @brief Test draw rect radius is 0
     */
    void UIKit_DrawRect_Test_RectRadius_001();

    /**
     * @brief Test draw rect radius
     */
    void UIKit_DrawRect_Test_RectRadius_002();

    /**
     * @brief Test draw rect radius equal width
     */
    void UIKit_DrawRect_Test_RectRadius_003();

    /**
     * @brief Test draw rect radius equal width and height
     */
    void UIKit_DrawRect_Test_RectRadius_004();

    /**
     * @brief Test draw rect opacity is not transparent
     */
    void UIKit_DrawRect_Test_RectOpacity_001();

    /**
     * @brief Test draw rect opacity is transparent
     */
    void UIKit_DrawRect_Test_RectOpacity_002();

    /**
     * @brief Test draw rect opacity with different value
     */
    void UIKit_DrawRect_Test_RectOpacity_003();

    /**
     * @brief Test draw rect border color with different value
     */
    void UIKit_DrawRect_Test_RectBorderColor_001();

    /**
     * @brief Test draw rect border width is 0
     */
    void UIKit_DrawRect_Test_RectBorderWidth_001();

    /**
     * @brief Test draw rect border with different value
     */
    void UIKit_DrawRect_Test_RectBorderWidth_002();

    /**
     * @brief Test draw rect border opacity is not transparent
     */
    void UIKit_DrawRect_Test_RectBorderOpacity_001();

    /**
     * @brief Test draw rect border opacity is transparent
     */
    void UIKit_DrawRect_Test_RectBorderOpacity_002();

    /**
     * @brief Test draw rect border opacity with different value
     */
    void UIKit_DrawRect_Test_RectBorderOpacity_003();

    /**
     * @brief Test draw rect padding is 0
     */
    void UIKit_DrawRect_Test_RectPadding_001();

    /**
     * @brief Test draw rect padding with different value
     */
    void UIKit_DrawRect_Test_RectPadding_002();

    /**
     * @brief Test draw rect margin is 0
     */
    void UIKit_DrawRect_Test_RectMargin_001();

    /**
     * @brief Test draw rect margin is not 0
     */
    void UIKit_DrawRect_Test_RectMargin_002();

    /**
     * @brief Test draw rect padding and margin is 0
     */
    void UIKit_DrawRect_Test_RectPaddingMargin_001();

    /**
     * @brief Test draw rect padding and margin with different value
     */
    void UIKit_DrawRect_Test_RectPaddingMargin_002();

    /**
     * @brief Test rect width is 0
     */
    void UIKit_DrawRect_Test_RectWidthAndHeight_001();

    /**
     * @brief Test rect height is 0
     */
    void UIKit_DrawRect_Test_RectWidthAndHeight_002();

    /**
     * @brief Test rect width and height is 0
     */
    void UIKit_DrawRect_Test_RectWidthAndHeight_003();

private:
    UIViewGroup* CreateTestCaseGroup() const;
    UILabel* CreateTitleLabel() const;
    UIView* CreateUIView() const;
    UIScrollView* container_ = nullptr;
};
} // namespace OHOS
#endif // UI_TEST_DRAW_RECT_H
