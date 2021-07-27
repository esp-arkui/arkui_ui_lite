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

#ifndef UI_AUTO_TEST_CIRCLE_PROGRESS_H
#define UI_AUTO_TEST_CIRCLE_PROGRESS_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestCircleProgress : public UIAutoTest {
public:
    UIAutoTestCircleProgress() {}
    ~UIAutoTestCircleProgress() {}

    void Reset() const;
    void RunTestList();

    void UIKitCircleProgressTestUICircleProgress001() const;
    void UIKitCircleProgressTestSetValue002() const;
    void UIKitCircleProgressTestSetImage003() const;
    void UIKitCircleProgressTestSetStyle004() const;
    void UIKitCircleProgressTestGetStyle005() const;
    void UIKitCircleProgressTestSetCapType006() const;
    void UIKitCircleProgressTestSetStep007() const;
    void UIKitCircleProgressTestSetRange008() const;
    void UIKitCircleProgressTestEnableBackground009() const;
    void UIKitCircleProgressTestSetCenter010() const;
    void UIKitCircleProgressTestSetRadius011() const;
    void UIKitCircleProgressTestSetLineWidth012() const;
    void UIKitCircleProgressTestSetImagePosition013() const;
    void UIKitCircleProgressTestSetLineColor014() const;
    void UIKitCircleProgressTestSetAngle015() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_CIRCLE_PROGRESS_H