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

#ifndef UI_AUTO_TEST_SWIPE_VIEW_H
#define UI_AUTO_TEST_SWIPE_VIEW_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestSwipeView : public UIAutoTest {
public:
    UIAutoTestSwipeView() {}
    ~UIAutoTestSwipeView() {}

    void Reset() const;
    void RunTestList();

    void UIKitSwipeViewTestHorizontal001() const;
    void UIKitSwipeViewTestHorizontal002() const;
    void UIKitSwipeViewTestVer001() const;
    void UIKitSwipeViewTestVer002() const;
    void UIKitSwipeViewTestAdd001() const;
    void UIKitSwipeViewTestAdd002() const;
    void UIKitSwipeViewTestAdd003() const;
    void UIKitSwipeViewTestRemove001() const;
    void UIKitSwipeViewTestRemove002() const;
    void UIKitSwipeViewTestRemove003() const;
    void UIKitSwipeViewTestAlign001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_SWIPE_VIEW_H