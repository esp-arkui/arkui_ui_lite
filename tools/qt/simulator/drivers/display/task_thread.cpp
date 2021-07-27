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

#include "task_thread.h"
#include "include/auto_test_counter.h"

namespace OHOS {
void TaskThread::run()
{
    taskQuitQry = false;
    while (!taskQuitQry) {
        TaskManager::GetInstance()->TaskHandler();
        AutoTestCounter::GetInstance().Increase();
        Sleep(DEFAULT_TASK_PERIOD);
    }
}

void TaskThread::Quit()
{
    taskQuitQry = true;
}
} // namespace OHOS
