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

#ifndef UI_AUTO_TEST_DRAW_RECT_H
#define UI_AUTO_TEST_DRAW_RECT_H

#include "ui_auto_test.h"

namespace OHOS {
class UiAutoTestDrawRect : public UIAutoTest {
public:
    UiAutoTestDrawRect() {}
    ~UiAutoTestDrawRect() {}

    void Reset() const;
    void RunTestList();

    void UIKitDrawRectTestRectNoBorder001() const;
    void UIKitDrawRectTestRectNoBorder002() const;
    void UIKitDrawRectTestRectNoBorder003() const;
    void UIKitDrawRectTestRectNoBorder004() const;
    void UIKitDrawRectTestRectNoBorder005() const;
    void UIKitDrawRectTestRectNoBorder006() const;
    void UIKitDrawRectTestRectNoBorder007() const;
    void UIKitDrawRectTestRectNoBorder008() const;
    void UIKitDrawRectTestRectNoBorder009() const;
    void UIKitDrawRectTestRectNoBorder010() const;
    void UIKitDrawRectTestRectBorderNoRadius001() const;
    void UIKitDrawRectTestRectBorderNoRadius002() const;
    void UIKitDrawRectTestRectBorderRadius001() const;
    void UIKitDrawRectTestRectBorderRadius002() const;
    void UIKitDrawRectTestRectBorderRadius003() const;
    void UIKitDrawRectTestRectBorderRadius004() const;
    void UIKitDrawRectTestRectBorderRadius005() const;
    void UIKitDrawRectTestRectBorderRadius006() const;
    void UIKitDrawRectTestRectBorderRadius007() const;
    void UIKitDrawRectTestRectBorderRadius008() const;
    void UIKitDrawRectTestRectBorderRadius009() const;
    void UIKitDrawRectTestRectBorderRadius010() const;
    void UIKitDrawRectTestRectBorderRadius011() const;
    void UIKitDrawRectTestRect001() const;
    void UIKitDrawRectTestRectColor001() const;
    void UIKitDrawRectTestRectRadius001() const;
    void UIKitDrawRectTestRectRadius002() const;
    void UIKitDrawRectTestRectRadius003() const;
    void UIKitDrawRectTestRectRadius004() const;
    void UIKitDrawRectTestRectOpacity001() const;
    void UIKitDrawRectTestRectOpacity002() const;
    void UIKitDrawRectTestRectOpacity003() const;
    void UIKitDrawRectTestRectBorderColor001() const;
    void UIKitDrawRectTestRectBorderWidth001() const;
    void UIKitDrawRectTestRectBorderWidth002() const;
    void UIKitDrawRectTestRectBorderOpacity001() const;
    void UIKitDrawRectTestRectBorderOpacity002() const;
    void UIKitDrawRectTestRectBorderOpacity003() const;
    void UIKitDrawRectTestRectPadding001() const;
    void UIKitDrawRectTestRectPadding002() const;
    void UIKitDrawRectTestRectMargin001() const;
    void UIKitDrawRectTestRectMargin002() const;
    void UIKitDrawRectTestRectPaddingMargin001() const;
    void UIKitDrawRectTestRectPaddingMargin002() const;
    void UIKitDrawRectTestRectWidthAndHeight001() const;
    void UIKitDrawRectTestRectWidthAndHeight002() const;
    void UIKitDrawRectTestRectWidthAndHeight003() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_DRAW_RECT_H