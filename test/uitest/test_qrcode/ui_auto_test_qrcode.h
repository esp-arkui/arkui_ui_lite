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

#ifndef UI_AUTO_TEST_QRCODE_H
#define UI_AUTO_TEST_QRCODE_H

#include "ui_auto_test.h"

namespace OHOS {
class UiAutoTestQrcode : public UIAutoTest {
public:
    UiAutoTestQrcode() {}
    ~UiAutoTestQrcode() {}

    void Reset() const;
    void RunTestList();
    void UIKitUIQrcodeTestDisplay001() const;
    void UIKitUIQrcodeTestDisplay002() const;
    void UIKitUIQrcodeTestDisplay003() const;
    void UIKitUIQrcodeTestDisplay004() const;
    void UIKitUIQrcodeTestDisplay005() const;
    void UIKitUIQrcodeTestDisplay006() const;
    void UIKitUIQrcodeTestDisplay007() const;
    void UIKitUIQrcodeTestDisplay008() const;
    void UIKitUIQrcodeTestDisplay009() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_QRCODE_H