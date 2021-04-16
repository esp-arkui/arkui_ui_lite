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

#ifndef UI_AUTO_TEST_BUTTON_H
#define UI_AUTO_TEST_BUTTON_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestButton : public UIAutoTest {
public:
    UIAutoTestButton() {}
    ~UIAutoTestButton() {}

    void Reset() const;
    void RunTestList();

    /**
     * @brief Auto Test Checkbox Function
     */
    void UIKitCheckBoxTest001() const;

    /**
     * @brief Auto Test Checkbox's SetImage Function
     */
    void UIKitCheckBoxTest002() const;

    /**
     * @brief Auto Test Radiobutton Function
     */
    void UIKitRadioButtonTest001() const;

    /**
     * @brief Auto Test Radiobutton's SetImage Function
     */
    void UIKitRadioButtonTest002() const;

    /**
     * @brief Auto Test Togglebutton Function
     */
    void UIKitToggleButtonTest001() const;

    /**
     * @brief Auto Test Togglebutton's SetImage Function
     */
    void UIKitToggleButtonTest002() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_BUTTON_H