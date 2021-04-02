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

#ifndef UI_AUTO_TEST_IMAGE_H
#define UI_AUTO_TEST_IMAGE_H

#include "ui_auto_test.h"

namespace OHOS {
class UiAutoTestImage : public UIAutoTest {
public:
    UiAutoTestImage() {}
    ~UiAutoTestImage() {}

    void Reset() const;
    void RunTestList();
    void UIKitUIImageTestSetImage001() const;
    void UIKitUIImageTestSetImage002() const;
    void UIKitUIImageTestSetImage004() const;
    void UIKitUIImageTestSetImage005() const;
    void UIKitUIImageTestSetImage006() const;
    void UIKitUIImageTestSetImage007() const;
    void UIKitUIImageTestSetImage008() const;
    void UIKitUIImageTestSetImage009() const;
    void UIKitUIImageTestSetImage010() const;
    void UIKitUIImageTestSetImage011() const;
    void UIKitUIImageTestSetImage012() const;
    void UIKitUIImageTestSetImage013() const;
    void UIKitUIImageTestSetImage014() const;
    void UIKitUIImageTestUncompress001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_IMAGE_H