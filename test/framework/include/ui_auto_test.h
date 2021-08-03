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

#ifndef GRAPHIC_LITE_UI_AUTO_TEST_H
#define GRAPHIC_LITE_UI_AUTO_TEST_H

#include "auto_test_case_group.h"
#include "components/ui_view.h"

namespace OHOS {
enum DragDirection {
    BOTTOM_TO_TOP,
    TOP_TO_BOTTOM,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    UPPER_LEFT_CORNER_TO_BOTTOM_RIGHT_CORNER,
    BOTTOM_RIGHT_CORNER_TO_UPPER_LEFT_CORNER,
    UPPER_RIGHT_CORNER_TO_BOTTOM_LEFT_CORNER,
    BOTTOM_LEFT_CORNER_TO_UPPER_RIGHT_CORNER
};
class UIAutoTest : public AutoTestCaseGroup::AutoTestCase {
public:
    UIAutoTest() {}
    virtual ~UIAutoTest() {}

    static void SetUpTestCase();
    void ResetMainMenu() const;
    void EnterSubMenu(const char* id) const;
    void ClickViewById(const char* id) const;
    void DragViewToHead(const char* id) const;
    void DragViewById(const char* id, DragDirection direction) const;
    void CompareByBinary(const char* fileName) const;
};
} // namespace OHOS
#endif // GRAPHIC_LITE_UI_AUTO_TEST_H
