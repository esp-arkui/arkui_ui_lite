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

#ifndef UI_AUTO_TEST_BOX_PROGRESS_H
#define UI_AUTO_TEST_BOX_PROGRESS_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestBoxProgress : public UIAutoTest {
public:
    UIAutoTestBoxProgress() {}
    ~UIAutoTestBoxProgress() {}

    void Reset() const;
    void RunTestList();

    void UIKitBoxProgressTestUIBoxProgress001() const;
    void UIKitBoxProgressTestSetRange002() const;
    void UIKitBoxProgressTestSetValue003() const;
    void UIKitBoxProgressTestSetImage004() const;
    void UIKitBoxProgressTestSetStyle005() const;
    void UIKitBoxProgressTestGetStyle006() const;
    void UIKitBoxProgressTestSetCapType007() const;
    void UIKitBoxProgressTestSetDirection008() const;
    void UIKitBoxProgressTestEnableBackground009() const;
    void UIKitBoxProgressTestSetStep010() const;
    void UIKitBoxProgressTestSetValidSize011() const;
    void UIKitBoxProgressTestSetBorderAndPadding012() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_BOX_PROGRESS_H