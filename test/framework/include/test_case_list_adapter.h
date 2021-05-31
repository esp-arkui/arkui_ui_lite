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

#ifndef TEST_CASE_LIST_ADAPTER_H
#define TEST_CASE_LIST_ADAPTER_H

#include "components/abstract_adapter.h"
#include "components/root_view.h"
#include "components/ui_label.h"
#include "components/ui_label_button.h"

namespace OHOS {
class TestCaseListAdapter : public AbstractAdapter {
public:
    TestCaseListAdapter(RootView* rootView, UIView* mainList, UILabelButton* backBtn,
        UILabel* label, UILabel* testTitleLabel)
        : rootView_(rootView), mainList_(mainList), backBtn_(backBtn), testCaseLabel_(label),
          testLabel_(testTitleLabel) {}
    ~TestCaseListAdapter() {}
    UIView* GetView(UIView* inView, int16_t index) override;
    uint16_t GetCount() override;
private:
    RootView* rootView_;
    UIView* mainList_;
    UILabelButton* backBtn_;
    UILabel* testCaseLabel_;
    UILabel* testLabel_;
};
} // namespace OHOS
#endif // TEST_CASE_LIST_ADAPTER_H