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

#include "ui_auto_test_view_bitmap.h"
#include "ui_test_view_bitmap.h"

namespace OHOS {
void UIAutoTestViewBitmap::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("View Bitmap");
}

void UIAutoTestViewBitmap::RunTestList()
{
    UIKitViewBitmapTest001();
    UIKitViewBitmapTest002();
}

void UIAutoTestViewBitmap::UIKitViewBitmapTest001() const
{
    Reset();
    ClickViewById(UI_TEST_VIEW_BITMAP_BTN_ID_01);
    const char* fileName1 = "ui_test_view_bitmap_01.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestViewBitmap::UIKitViewBitmapTest002() const
{
    Reset();
    ClickViewById(UI_TEST_VIEW_BITMAP_BTN_ID_02);
    const char* fileName1 = "ui_test_view_bitmap_02.bmp";
    CompareByBinary(fileName1);
}
} // namespace OHOS