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

#include "auto_test_counter.h"
#include "securec.h"

namespace OHOS {
AutoTestCounter& AutoTestCounter::GetInstance()
{
    static AutoTestCounter instance;
    return instance;
}

void AutoTestCounter::Reset()
{
    counter_ = 0;
    frameCount_ = 0;
    curFrame_ = 0;
    frameArray_ = nullptr;
}

bool AutoTestCounter::SetFrameArray(const char* fileName, uint16_t* frameArray, uint32_t count)
{
    if (memset_s(static_cast<void *>(screenshotName),
        DEFAULT_FILE_NAME_MAX_LENGTH, 0, DEFAULT_FILE_NAME_MAX_LENGTH) != EOK) {
        return false;
    }
    if (memcpy_s(static_cast<void *>(screenshotName),
        DEFAULT_FILE_NAME_MAX_LENGTH, fileName, strlen(fileName)) != EOK) {
        return false;
    }
    frameCount_ = count - 1;
    frameArray_ = frameArray;
    curFrame_ = 0;
    ThreadSuspend(threadId_);
}

bool AutoTestCounter::Increase()
{
    if (frameCount_ == 0 || frameArray_ == nullptr) {
        return false;
    }
    counter_++;
    if (frameArray_[curFrame_] == counter_) {
        char dest[8] = {0}; // 8: "xxx.bmp"
        char filePath[DEFAULT_FILE_NAME_MAX_LENGTH] = {0};
        sprintf_s(dest, sizeof(dest), "%d.bmp", frameArray_[curFrame_]);
        CompareTools::StrnCatPath(filePath,
            DEFAULT_FILE_NAME_MAX_LENGTH, screenshotName, strlen(screenshotName));
        if (strncat_s(filePath, DEFAULT_FILE_NAME_MAX_LENGTH, dest, sizeof(dest)) != EOK) {
            return false;
        }
        if (CompareTools::CheckFileExist(filePath, strlen(filePath))) {
            CompareTools::CompareFile(filePath, strlen(filePath));
        } else {
            CompareTools::SaveFile(filePath, strlen(filePath));
        }
        if (curFrame_ == frameCount_) {
            Reset();
            ThreadResume(threadId_);
        }
        curFrame_++;
    }
    return true;
}

void AutoTestCounter::SetThreadId(ThreadId threadId)
{
    threadId_ = threadId;
}
}
