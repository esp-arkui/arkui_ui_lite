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

/**
 * @addtogroup UI_DFX
 * @{
 *
 * @brief
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file xxx.h
 *
 * @brief xxx.
 *
 * @since 3.0
 * @version 1.0
 */
#ifndef GRAPHIC_LITE_UI_VIEW_BOUNDS_H
#define GRAPHIC_LITE_UI_VIEW_BOUNDS_H

#include "graphic_config.h"
#if ENABLE_DEBUG
#include "gfx_utils/heap_base.h"
namespace OHOS {
/**
 * @brief
 *
 * @since 3.0
 * @version 1.0
 */
class UIViewBounds : public HeapBase {
public:
    /**
     * @brief Obtains a singleton <b>UIViewBounds</b> instance.
     *
     * @return Returns the <b>UIViewBounds</b> instance.
     * @since 1.0
     * @version 1.0
     */
    static UIViewBounds* GetInstance();

    /**
     * @brief 获取当前边框显示状态 
     * 
     * @return true 显示，false 不显示
     */
    bool GetShowState()
    {
        return showViewBounds_;
    }

    /**
     * @brief 设置当前边框显示状态 
     * 
     */
    void SetShowState(bool show);

private:
    UIViewBounds() {}
    virtual ~UIViewBounds() {}

    UIViewBounds(const UIViewBounds&) = delete;
    UIViewBounds& operator=(const UIViewBounds&) = delete;
    UIViewBounds(UIViewBounds&&) = delete;
    UIViewBounds& operator=(UIViewBounds&&) = delete;

    bool showViewBounds_ = true;
};
} // namespace OHOS
#endif // ENABLE_DEBUG
#endif // GRAPHIC_LITE_UI_VIEW_BOUNDS_H