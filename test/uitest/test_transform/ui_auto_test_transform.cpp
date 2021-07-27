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

#include "ui_auto_test_transform.h"
#include "ui_test_transform.h"

namespace OHOS {
void UIAutoTestTransform::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Transform");
}

void UIAutoTestTransform::RunTestList()
{
    UIKitTransformTestRotate001();
    UIKitTransformTestScale001();
    UIKitTransformTestTranslate001();
}
void UIAutoTestTransform::UIKitTransformTestRotate001() const
{
    Reset();
    ClickViewById("test_tranform_rotate_button_id");
    const char* fileName = "ui_test_transform_rotate_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestTransform::UIKitTransformTestScale001() const
{
    Reset();
    ClickViewById("test_tranform_scale_button_id");
    const char* fileName = "ui_test_transform_scale_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestTransform::UIKitTransformTestTranslate001() const
{
    Reset();
    ClickViewById("test_tranform_translate_button_id");
    const char* fileName = "ui_test_transform_translate_001.bmp";
    CompareByBinary(fileName);
}
} // namespace OHOS