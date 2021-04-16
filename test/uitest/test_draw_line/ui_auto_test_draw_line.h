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

#ifndef UI_AUTO_TEST_DRAW_LINE_H
#define UI_AUTO_TEST_DRAW_LINE_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestDrawLine : public UIAutoTest {
public:
    UIAutoTestDrawLine() {}
    ~UIAutoTestDrawLine() {}

    void Reset() const;
    void RunTestList();

    void UIKitDrawLineTestLine001() const;
    void UIKitDrawLineTestLine002() const;
    void UIKitDrawLineTestLine003() const;
    void UIKitDrawLineTestLine004() const;
    void UIKitDrawLineTestLine005() const;
    void UIKitDrawLineTestLine006() const;
    void UIKitDrawLineTestLine007() const;
    void UIKitDrawLineTestLine008() const;
    void UIKitDrawLineTestLine009() const;
    void UIKitDrawLineTestLine010() const;
    void UIKitDrawLineTestLine011() const;
    void UIKitDrawLineTestLine012() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_DRAW_LINE_H