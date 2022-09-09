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

#ifndef GRAPHIC_LITE_UI_TEST_MESSAGE_H
#define GRAPHIC_LITE_UI_TEST_MESSAGE_H
#include <vector>
#include <algorithm>
#include <memory>
#include <list>
#include <string>
#include <functional>

namespace OHOS {
enum TestEventID : uint8_t {
    TEST_CLICK_EVENT,
    TEST_MOVE_EVENT,
};

enum TestMode: uint8_t {
    TEST_MODE_BASE = 1,
    TEST_MODE_RUN,
};

struct TestSteps {
    std::string viewID;
    TestEventID eventID;
    std::vector<int> eventValue;
    bool saveCheckPoint;
};

struct TestMsgInfo {
    std::string className;
    std::vector<std::string> pageNav;
    std::vector<TestSteps> steps;
};

struct TestConfigInfo {
    TestConfigInfo()
    {
        testMode = TEST_MODE_BASE;
    }
    TestMode testMode;
    std::string baseDir;
    std::string runDir;
    std::string logDir;
};

const size_t S_C_MAIN_ID_SEND_CONFIG_INFO = 1;              // Send config information
const size_t C_S_MAIN_ID_REQUEST_TEST_INFO = 2;             // Request to start test
const size_t S_C_MAIN_ID_SEND_TEST_INFO = 3;                // Distribute test data
const size_t C_S_MAIN_ID_TEST_FINISH_INFO = 4;              // Test a set of data
const size_t S_C_MAIN_ID_All_TESTS_COMPLETE = 5;            // All tests completed
} // namespace OHOS

#endif // GRAPHIC_LITE_UI_TEST_MESSAGE_H
