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

#ifndef UI_AUTO_TEST_ARC_LABEL_H
#define UI_AUTO_TEST_ARC_LABEL_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestArcLabel : public UIAutoTest {
public:
    UIAutoTestArcLabel() {}
    ~UIAutoTestArcLabel() {}

    void Reset() const;
    void RunTestList();

    void UIKitUIArcLabelAutoTestDisplay001() const;
    void UIKitUIArcLabelAutoTestDisplay002() const;
    void UIKitUIArcLabelAutoTestDisplay003() const;
    void UIKitUIArcLabelAutoTestDisplay004() const;
    void UIKitUIArcLabelAutoTestAlign001() const;
    void UIKitUIArcLabelAutoTestAlign002() const;
    void UIKitUIArcLabelAutoTestAlign003() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_ARC_LABEL_H