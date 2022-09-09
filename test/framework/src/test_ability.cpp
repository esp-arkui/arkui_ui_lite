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

#include "ui_test_app.h"
#include "graphic_thread.h"
#include "auto_test_manager.h"
#include <functional>

void RunApp()
{
    OHOS::UITestApp::GetInstance()->Start();
}

void DispatchAppMsg(std::vector<std::shared_ptr<OHOS::TestMsgInfo>> msgInfo)
{
    OHOS::AutoTestManager::GetInstance()->StartTest(msgInfo);
}

void DipatchAppConfigMsg(std::shared_ptr<OHOS::TestConfigInfo> configInfo)
{
    OHOS::AutoTestManager::GetInstance()->SetConfigInfo(configInfo);
}

void DispatchCompleteMsg()
{
    OHOS::AutoTestManager::GetInstance()->TestComplete();
}

void BindSendMsg(void (*func)(size_t mainID))
{
    OHOS::AutoTestManager::GetInstance()->SetSendMsgFuncCallBack(std::bind(func, std::placeholders::_1));
}

