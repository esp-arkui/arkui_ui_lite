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

#ifndef UI_AUTO_TEST_CHART_POLYLINE_H
#define UI_AUTO_TEST_CHART_POLYLINE_H

#include "ui_auto_test.h"

namespace OHOS {
class UIAutoTestChartPolyline : public UIAutoTest {
public:
    UIAutoTestChartPolyline() {}
    ~UIAutoTestChartPolyline() {}

    void Reset() const;
    void RunTestList();

    void UIKitChartPolylineTestAddDataSerial001() const;
    void UIKitChartPolylineTestDeleteDataSerial001() const;
    void UIKitChartPolylineTestClearDataSerial001() const;
    void UIKitChartPolylineTestTopPoint001() const;
    void UIKitChartPolylineTestBotPoint001() const;
    void UIKitChartPolylineTestHeadPoint001() const;
    void UIKitChartPolylineTestReverse001() const;
    void UIKitChartPolylineTestGradientBottom001() const;
    void UIKitChartPolylineTestAddPoints001() const;
};
} // namespace OHOS
#endif // UI_AUTO_TEST_CHART_POLYLINE_H