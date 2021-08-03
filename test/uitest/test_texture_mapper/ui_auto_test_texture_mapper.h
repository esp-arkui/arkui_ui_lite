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

#ifndef UI_AUTO_TEST_TEXTURE_MAPPER_H
#define UI_AUTO_TEST_TEXTURE_MAPPER_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestTextureMapper : public UIAutoTest {
public:
    UIAutoTestTextureMapper() {}
    ~UIAutoTestTextureMapper() {}

    void Reset() const;
    void RunTestList();

    void UIKitTextureMapperTestStart001() const;
    void UIKitTextureMapperTestStop002() const;
    void UIKitTextureMapperTestlistener003() const;
    void UIKitTextureMapperTestIncCenter004() const;
    void UIKitTextureMapperTestdecCenter005() const;

    void UIKitTextureMapperTestIncTime006() const;
    void UIKitTextureMapperTestdecTime007() const;
    void UIKitTextureMapperTestIncscale008() const;
    void UIKitTextureMapperTestdecscale009() const;
    void UIKitTextureMapperTestIncRotateEnd010() const;
    void UIKitTextureMapperTestdecRotateEnd011() const;
    void UIKitTextureMapperTestStartAngle012() const;
    void UIKitTextureMapperTestStartScale013() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_TEXTURE_MAPPER_H