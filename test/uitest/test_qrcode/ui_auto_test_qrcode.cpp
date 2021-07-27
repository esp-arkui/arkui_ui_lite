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

#include "ui_auto_test_qrcode.h"
#include "ui_test_qrcode.h"

namespace OHOS {
void UIAutoTestQrcode::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("UIQrcode");
}

void UIAutoTestQrcode::RunTestList()
{
    Reset();
    UIKitUIQrcodeTestDisplay001();
    UIKitUIQrcodeTestDisplay002();
    UIKitUIQrcodeTestDisplay003();
    UIKitUIQrcodeTestDisplay004();
    UIKitUIQrcodeTestDisplay005();
    UIKitUIQrcodeTestDisplay006();
    UIKitUIQrcodeTestDisplay007();
    UIKitUIQrcodeTestDisplay008();
    UIKitUIQrcodeTestDisplay009();
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay001() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay001");
    const char* fileName = "ui_test_qrcode_display_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay002() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay002");
    const char* fileName = "ui_test_qrcode_display_002.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay003() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay003");
    const char* fileName = "ui_test_qrcode_display_003.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay004() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay004");
    const char* fileName = "ui_test_qrcode_display_004.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay005() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay005");
    const char* fileName = "ui_test_qrcode_display_005.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay006() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay006");
    const char* fileName = "ui_test_qrcode_display_006.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay007() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay007");
    const char* fileName = "ui_test_qrcode_display_007.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay008() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay008");
    const char* fileName = "ui_test_qrcode_display_008.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestQrcode::UIKitUIQrcodeTestDisplay009() const
{
    DragViewToHead("UIKitUIQrcodeTestDisplay009");
    const char* fileName = "ui_test_qrcode_display_009.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS