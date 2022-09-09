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

#include "tcp_socket_manager.h"
#include "graphic_thread.h"
#include "main.cpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace OHOS {
TcpSocketClientManager* TcpSocketClientManager::GetInstance()
{
    static TcpSocketClientManager instance;
    return &instance;
}

TcpSocketClientManager::~TcpSocketClientManager()
{
    if (tcpSocket_) {
        delete tcpSocket_;
    }
}

void TcpSocketClientManager::InitSocket()
{
    if (tcpSocket_ == nullptr) {
        tcpSocket_ = new TcpScoketClient();
    }

    tcpSocket_->CreateTCPScoket();
    tcpSocket_->SetScoketManager(GetInstance());
}

void TcpSocketClientManager::ConnectSocket()
{
    if (tcpSocket_ == nullptr) {
        return;
    }

    tcpSocket_->ConnectSocket();
}

void TcpSocketClientManager::OnGetEventValue(QJsonArray array, std::vector<int>& values)
{
    foreach (auto it, array) {
        values.push_back(it.toInt());
    }
}

void TcpSocketClientManager::OnGetPageNav(QJsonArray array, std::vector<std::string>& pageNav)
{
    foreach (auto it, array) {
        pageNav.push_back(it.toString().toStdString());
    }
}

void TcpSocketClientManager::OnGetTestSetps(QJsonArray array, std::vector<TestSteps>& steps)
{
    foreach (auto it, array) {
        TestSteps testSteps;
        QJsonObject arrobj = it.toObject();
        if (arrobj.contains("viewID")) {
            testSteps.viewID = arrobj.value("viewID").toString().toStdString();
        }
        if (arrobj.contains("eventID")) {
            testSteps.eventID = static_cast<TestEventID>(arrobj.value("eventID").toInt());
        }
        if (arrobj.contains("eventValue")) {
            QJsonValue arrays_value = arrobj.take("eventValue");
            if (arrays_value.isArray()) {
                QJsonArray array = arrays_value.toArray();
                OnGetEventValue(array, testSteps.eventValue);
            }
        }
        if (arrobj.contains("saveCheckPoint")) {
            testSteps.saveCheckPoint = static_cast<bool>(arrobj.value("saveCheckPoint").toBool());
        }
        steps.push_back(testSteps);
    }
}

void TcpSocketClientManager::RecvMsg(QByteArray recv)
{
    std::shared_ptr<QByteArray> recvmsg = std::make_shared<QByteArray>(recv);
    QMutexLocker mutexLocker(&mutex_);
    recv_.push_back(recvmsg);
}

void TcpSocketClientManager::OnGetMsgInfo(const std::shared_ptr<QByteArray> recv)
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(recv->toStdString().data(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject object;
    if (document.isObject()) {
        object = document.object();
    }

    size_t mainID = 0;
    if (object.contains("main_id")) {
        mainID = static_cast<size_t>(object.value("main_id").toInt());
    }

    printf("OnGetMsgInfo------mainID = [%zu]\n", mainID);
    fflush(stdout);
    switch (mainID) {
        case OHOS::S_C_MAIN_ID_SEND_CONFIG_INFO:
            OnGetConfigInfo(object);
            break;
        case OHOS::S_C_MAIN_ID_SEND_TEST_INFO:
            OnGetTestInfo(object);
            break;
        case OHOS::S_C_MAIN_ID_All_TESTS_COMPLETE:
            dispatchCompleteFunc_();
            break;
        default:
            break;
    }
}

void TcpSocketClientManager::OnGetConfigInfo(const QJsonObject object)
{
    if (object.empty()) {
        return;
    }

    std::shared_ptr<TestConfigInfo> configInfo = std::make_shared<TestConfigInfo>();
    if (object.contains("testMode")) {
        configInfo->testMode = static_cast<TestMode>(object.value("testMode").toInt());
    }

    if (object.contains("base_dir")) {
        configInfo->baseDir =  object.value("base_dir").toString().toStdString();
    }

    if (object.contains("run_dir")) {
        configInfo->runDir =  object.value("run_dir").toString().toStdString();
    }

    if (object.contains("log_dir")) {
        configInfo->logDir =  object.value("log_dir").toString().toStdString();
    }

    dispatchConfigFunc_(configInfo);
}

void TcpSocketClientManager::OnGetTestInfo(QJsonObject object)
{
    if (object.empty()) {
        return;
    }

    std::vector<std::shared_ptr<TestMsgInfo>> testMsgInfo;
    if (object.contains("testInfo")) {
        QJsonValue arrays_value = object.take("testInfo");
        if (!arrays_value.isArray()) {
            return;
        }

        QJsonArray assintArray = arrays_value.toArray();
        foreach(auto it, assintArray) {
            std::shared_ptr<TestMsgInfo> msgInfo = std::make_shared<TestMsgInfo>();
            QJsonObject arrobj = it.toObject();
            if (arrobj.contains("className")) {
                msgInfo->className = arrobj.value("className").toString().toStdString();
            }

            if (arrobj.contains("pageNav")) {
                QJsonValue value = arrobj.take("pageNav");
                if (value.isArray()) {
                    QJsonArray array = value.toArray();
                    OnGetPageNav(array, msgInfo->pageNav);
                }
            }

            if (arrobj.contains("testSteps")) {
                QJsonValue value = arrobj.take("testSteps");
                if (value.isArray()) {
                    QJsonArray array = value.toArray();
                    OnGetTestSetps(array, msgInfo->steps);
                }
            }
            testMsgInfo.push_back(msgInfo);
        }
    }
    OnPrintTestInfo(testMsgInfo);

    dispatchFunc_(testMsgInfo);
}

void TcpSocketClientManager::OnPrintTestInfo(const std::vector<std::shared_ptr<TestMsgInfo>> testMsgInfo) const
{
    for (auto it : testMsgInfo) {
        printf("TcpSocketClientManager::OnRunRecvMsg()-className=[%s],---\n", it->className.c_str());
        fflush(stdout);
        for (auto it2 : it->pageNav) {
            printf("pageNav--%s----------", it2.c_str());
        }
        printf("\n");
        for (auto it3: it->steps) {
            printf("steps---viewID=[%s], eventID=[%d]", it3.viewID.c_str(), it3.eventID);
            for (auto it4 : it3.eventValue) {
                printf("steps---eventValue=[%d]", it4);
            }
            printf("steps---saveCheckPoint=[%d]--------\n", it3.saveCheckPoint);
        }
    }
}

void TcpSocketClientManager::DispatchMsg()
{
    OnRunRecvMsg();
}

void TcpSocketClientManager::SetDispatchFuncCallBack(DispatchFunc dispatchFunc)
{
    dispatchFunc_ = dispatchFunc;
}

void TcpSocketClientManager::SetDispatchConfigFuncCallBack(DispatchConfigFunc dispatchConfigFunc)
{
    dispatchConfigFunc_ = dispatchConfigFunc;
}

void TcpSocketClientManager::SetDispatchCompleteFuncCallBack(DispatchCompleteFunc dispatchCompleteFunc)
{
    dispatchCompleteFunc_ = dispatchCompleteFunc;
}

void TcpSocketClientManager::OnRunRecvMsg()
{
    if (recv_.empty()) {
        return;
    }

    QMutexLocker mutexLocker(&mutex_);
    std::shared_ptr<QByteArray> recv = recv_.front();
    recv_.pop_front();

    OnGetMsgInfo(recv);
}
} // namespace OHOS
