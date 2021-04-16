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

#ifndef UI_AUTO_TEST_OPACITY_H
#define UI_AUTO_TEST_OPACITY_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestOpacity : public UIAutoTest {
public:
    UIAutoTestOpacity() {}
    ~UIAutoTestOpacity() {}

    void Reset() const;
    void RunTestList();

    /**
     * @brief Auto Test UILabel
     */
    void UIKitOpacityTestUILabel001() const;

    /**
     * @brief Auto Test UILabel exception value
     */
    void UIKitOpacityTestUILabel002() const;
    /**
     * @brief Auto Test UIButton
     */
    void UIKitOpacityTestUIButton001() const;

    /**
     * @brief Auto Test UIButton image
     */
    void UIKitOpacityTestUIButton002() const;

    /**
     * @brief Auto Test UILabelButton
     */
    void UIKitOpacityTestUILabelButton001() const;

    /**
     * @brief Auto Test UIArcLabel
     */
    void UIKitOpacityTestUIArcLabel001() const;

    /**
     * @brief Auto Test UIImageView
     */
    void UIKitOpacityTestUIImageView001() const;

    /**
     * @brief Auto Test UIImageAnimator
     */
    void UIKitOpacityTestUIImageAnimator001() const;

    /**
     * @brief Auto Test UIView
     */
    void UIKitOpacityTestUIView001() const;

    /**
     * @brief Auto Test UIViewGroup
     */
    void UIKitOpacityTestUIViewGroup001() const;

    /**
     * @brief Auto Test UIList
     */
    void UIKitOpacityTestUIList001() const;

    /**
     * @brief Auto Test UIScrollView
     */
    void UIKitOpacityTestUIScrollView001() const;

    /**
     * @brief Auto Test UISwipeView
     */
    void UIKitOpacityTestUISwipeView001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_OPACITY_H