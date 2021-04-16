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

#ifndef UI_AUTO_TEST_SLIDER_H
#define UI_AUTO_TEST_SLIDER_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestSlider : public UIAutoTest {
public:
    UIAutoTestSlider() {}
    ~UIAutoTestSlider() {}

    void Reset() const;
    void RunTestList();
    void UIKitSliderTestSetRange001() const;
    void UIKitSliderTestSetValue001() const;
    void UIKitSliderTestSetImage001() const;
    void UIKitSliderTestSetStyle001() const;
    void UIKitSliderTestSetKnobWidth001() const;
    void UIKitSliderTestSetStep001() const;
    void UIKitSliderTestSetColor001() const;
    void UIKitSliderTestSetDirection001() const;
    void UIKitSliderTestSetValidSize001() const;
    void UIKitSliderTestSetRadius001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_SLIDER_H