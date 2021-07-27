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

#include "ui_auto_test_texture_mapper.h"
#include "auto_test_counter.h"
#include "ui_test_texture_mapper.h"

namespace OHOS {
void UIAutoTestTextureMapper::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Texture_Mapper");
}

void UIAutoTestTextureMapper::RunTestList()
{
    Reset();
    UIKitTextureMapperTestStart001();
    UIKitTextureMapperTestStop002();
    UIKitTextureMapperTestlistener003();
    UIKitTextureMapperTestIncCenter004();
    UIKitTextureMapperTestdecCenter005();
    UIKitTextureMapperTestIncTime006();
    UIKitTextureMapperTestdecTime007();
    UIKitTextureMapperTestIncscale008();
    UIKitTextureMapperTestdecscale009();
    UIKitTextureMapperTestIncRotateEnd010();
    UIKitTextureMapperTestdecRotateEnd011();
    UIKitTextureMapperTestStartAngle012();
    UIKitTextureMapperTestStartScale013();
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestStart001() const
{
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 5 data of 1, 10, 20, 30, 40 frames as the comparison point.
    uint16_t frameArray[5] = {1, 10, 20, 30, 40};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_start",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestStop002() const
{
    ClickViewById("test_texture_mapper_start_btn_id");
    ClickViewById("test_texture_mapper_stop_btn_id");
    const char* fileName = "ui_test_texture_mapper_stop_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestlistener003() const
{
    Reset();
    ClickViewById("test_texture_mapper_listener_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_listener",
        frameArray, sizeof(frameArray));
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestIncCenter004() const
{
    ClickViewById("test_texture_mapper_inc_center_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_inc_center",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestdecCenter005() const
{
    ClickViewById("test_texture_mapper_dec_center_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_dec_center",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestIncTime006() const
{
    ClickViewById("test_texture_mapper_duration_time_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_duration_time",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestdecTime007() const
{
    ClickViewById("test_texture_mapper_delay_time_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_delay_time",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestIncscale008() const
{
    ClickViewById("test_texture_mapper_inc_scale_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_inc_scale",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestdecscale009() const
{
    ClickViewById("test_texture_mapper_dec_scale_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_dec_scale",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestIncRotateEnd010() const
{
    ClickViewById("test_texture_mapper_inc_rotate_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_inc_rotate",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestdecRotateEnd011() const
{
    ClickViewById("test_texture_mapper_dec_rotate_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_dec_rotate",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestStartAngle012() const
{
    ClickViewById("test_texture_mapper_start_angle_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_start_angle",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}

void UIAutoTestTextureMapper::UIKitTextureMapperTestStartScale013() const
{
    ClickViewById("test_texture_mapper_start_scale_btn_id");
    ClickViewById("test_texture_mapper_start_btn_id");
    // Take the 10 data of 1, 10, 20, 30, 40, 50, 60, 70, 80, 90 frames as the comparison point.
    uint16_t frameArray[10] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_texture_mapper_start_scale",
        frameArray, sizeof(frameArray));
    ClickViewById("test_texture_mapper_reset_btn_id");
}
} // namespace OHOS