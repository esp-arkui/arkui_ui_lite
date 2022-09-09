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

#include <QApplication>

#include "graphic_config.h"
#include "main_widget.h"
#include "monitor.h"
#include "tcp_socket_manager.h"
#include "tcp_socket.h"

extern void RunApp();
extern void DispatchAppMsg(std::vector<std::shared_ptr<OHOS::TestMsgInfo>> msgInfo);
extern void DipatchAppConfigMsg(std::shared_ptr<OHOS::TestConfigInfo> configInfo);
extern void BindSendMsg(void (*func)(size_t mainID));
extern void DispatchCompleteMsg();
static void DispatchMsg(std::vector<std::shared_ptr<OHOS::TestMsgInfo>> msgInfo)
{
    DispatchAppMsg(msgInfo);
}

static void DipatchConfigMsg(std::shared_ptr<OHOS::TestConfigInfo> configInfo)
{
    DipatchAppConfigMsg(configInfo);
}

static void DispatchComplete()
{
    DispatchCompleteMsg();
}

static void SendMsg(size_t mainID)
{
    OHOS::TcpSocketClientManager::GetInstance()->SendMsg(mainID);
}

int main(int argc, char* argv[])
{
    QApplication uitest(argc, argv);
    OHOS::GraphicStartUp::Init();
    OHOS::Monitor::GetInstance()->InitHal();
    OHOS::Monitor::GetInstance()->InitFontEngine();
    OHOS::Monitor::GetInstance()->InitImageDecodeAbility();

    OHOS::TcpSocketClientManager::GetInstance()->InitSocket();
    if (argv[1] != nullptr && strcmp(argv[1], "-f") == 0) {
        OHOS::TcpSocketClientManager::GetInstance()->ConnectSocket();
    }
    OHOS::TcpSocketClientManager::GetInstance()->SetDispatchFuncCallBack(std::bind(DispatchMsg, std::placeholders::_1));
    OHOS::TcpSocketClientManager::GetInstance()->SetDispatchConfigFuncCallBack(
        std::bind(DipatchConfigMsg, std::placeholders::_1));
    OHOS::TcpSocketClientManager::GetInstance()->SetDispatchCompleteFuncCallBack(std::bind(DispatchComplete));
    BindSendMsg(SendMsg);

    RunApp();
    OHOS::MainWidget mainWidget;
    mainWidget.resize(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION);
    mainWidget.show();
    return uitest.exec();
}
