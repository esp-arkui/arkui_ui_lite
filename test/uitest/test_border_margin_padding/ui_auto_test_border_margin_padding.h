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

#ifndef UI_AUTO_TEST_BORDER_MARGIN_PADDING_H
#define UI_AUTO_TEST_BORDER_MARGIN_PADDING_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestBorderMarginPadding : public UIAutoTest {
public:
    UIAutoTestBorderMarginPadding()
    {
        startPoint_ = {0};
        endPoint_ = {0};
    }
    ~UIAutoTestBorderMarginPadding() {}

    void Reset() const;
    void RunTestList();

    void UIKitBorderMarginPaddingAllOff001() const;
    void UIKitBorderMarginPaddingAllOn001() const;
    void UIKitBorderMarginPaddingOnMargin001() const;
    void UIKitBorderMarginPaddingOnBorder001() const;
    void UIKitBorderMarginPaddingOnPadding001() const;
    void UIKitBorderMarginPaddingOffMargin001() const;
    void UIKitBorderMarginPaddingOffBorder001() const;
    void UIKitBorderMarginPaddingOffPadding001() const;

private:
    void UIKitBorderMarginPaddingCompare(const char* name) const;
    Point startPoint_;
    Point endPoint_;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_BORDER_MARGIN_PADDING_H