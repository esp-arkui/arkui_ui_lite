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

#ifndef GRAPHIC_LITE_AUTO_TEST_COUNTER_H
#define GRAPHIC_LITE_AUTO_TEST_COUNTER_H

#include "graphic_thread.h"
#include "compare_tools.h"

namespace OHOS {
class AutoTestCounter {
public:
    static AutoTestCounter& GetInstance();
    void Reset();

    bool SetFrameArray(const char* fileName, uint16_t* frameArray, uint32_t count);

    bool Increase();

    void SetThreadId(ThreadId threadId);
private:
    AutoTestCounter() : counter_(0), frameArray_(nullptr), frameCount_(0), curFrame_(0) {}
    virtual ~AutoTestCounter() {}

    char screenshotName[DEFAULT_FILE_NAME_MAX_LENGTH] = {0};
    uint32_t counter_;
    uint16_t* frameArray_;
    uint32_t frameCount_;
    uint32_t curFrame_;
    ThreadId threadId_;

    AutoTestCounter(const AutoTestCounter&) = delete;
    AutoTestCounter& operator=(const AutoTestCounter&) = delete;
    AutoTestCounter(AutoTestCounter&&) = delete;
    AutoTestCounter& operator=(AutoTestCounter&&) = delete;
};
} // namespace OHOS
#endif // GRAPHIC_LITE_AUTO_TEST_COUNTER_H
