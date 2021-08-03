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

#ifndef UI_AUTO_TEST_CANVAS_H
#define UI_AUTO_TEST_CANVAS_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestCanvas : public UIAutoTest {
public:
    UIAutoTestCanvas() {}
    ~UIAutoTestCanvas() {}

    void Reset() const;
    void RunTestList();

    void UIKitCanvasTestDrawLine001() const;
    void UIKitCanvasTestDrawLine002() const;
    void UIKitCanvasTestDrawCurve001() const;
    void UIKitCanvasTestDrawCurve002() const;
    void UIKitCanvasTestDrawRect001() const;
    void UIKitCanvasTestDrawRect002() const;
    void UIKitCanvasTestDrawRect003() const;
    void UIKitCanvasTestDrawCircle001() const;
    void UIKitCanvasTestDrawCircle002() const;
    void UIKitCanvasTestDrawCircle003() const;
    void UIKitCanvasTestDrawArc001() const;
    void UIKitCanvasTestDrawImage001() const;
    void UIKitCanvasTestDrawLabel001() const;
    void UIKitCanvasTestDrawSector001() const;
    void UIKitCanvasTestClear001() const;
    void UIKitCanvasTestDrawPath001() const;
    void UIKitCanvasTestDrawPath002() const;
    void UIKitCanvasTestDrawPath003() const;
    void UIKitCanvasTestDrawPath004() const;
    void UIKitCanvasTestDrawPath005() const;
    void UIKitCanvasTestDrawPath006() const;
    void UIKitCanvasTestDrawPath007() const;
    void UIKitCanvasTestDrawPath008() const;
    void UIKitCanvasTestDrawPath009() const;
    void UIKitCanvasTestDrawPath010() const;
    void UIKitCanvasTestDrawPath011() const;
    void UIKitCanvasTestDrawPath012() const;
    void UIKitCanvasTestDrawPath013() const;
    void UIKitCanvasTestDrawPath014() const;
    void UIKitCanvasTestDrawPath015() const;
    void UIKitCanvasTestDrawPath016() const;
    void UIKitCanvasTestDrawPath017() const;
    void UIKitCanvasTestDrawPath018() const;
    void UIKitCanvasTestDrawPath019() const;
    void UIKitCanvasTestDrawPath020() const;
    void UIKitCanvasTestDrawPath021() const;
    void UIKitCanvasTestDrawPath022() const;
    void UIKitCanvasTestDrawPath023() const;
    void UIKitCanvasTestDrawPath024() const;
    void UIKitCanvasTestDrawPath025() const;
    void UIKitCanvasTestDrawPath026() const;
    void UIKitCanvasTestDrawPath027() const;
    void UIKitCanvasTestDrawPath028() const;
    void UIKitCanvasTestDrawPath029() const;
    void UIKitCanvasTestDrawPath030() const;
    void UIKitCanvasTestDrawPath031() const;
    void UIKitCanvasTestDrawPath032() const;
    void UIKitCanvasTestDrawPath033() const;
    void UIKitCanvasTestDrawPath034() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_CANVAS_H