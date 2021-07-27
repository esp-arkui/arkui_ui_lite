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

#include "ui_auto_test_clip.h"
#include "ui_test_clip.h"

namespace OHOS {
void UIAutoTestClip::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Clip");
}

void UIAutoTestClip::RunTestList()
{
    Reset();
    UIKitClipTest001();
    UIKitClipTest002();
    UIKitClipTest003();
    UIKitClipTest004();
    UIKitClipTest005();
}

void UIAutoTestClip::UIKitClipTest001() const
{
    const char* fileName1 = "ui_test_clip_round_radius_45.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_CLIP_RADIUS_ADD_ONE_BTN_ID);
    ClickViewById(UI_TEST_CLIP_RADIUS_ADD_ONE_BTN_ID);
    const char* fileName2 = "ui_test_clip_round_radius_47.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CLIP_RADIUS_ADD_FIVE_BTN_ID);
    const char* fileName4 = "ui_test_clip_round_radius_52.bmp";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_CLIP_RADIUS_REDUCE_ONE_BTN_ID);
    const char* fileName3 = "ui_test_clip_round_radius_51.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_CLIP_RADIUS_REDUCE_FIVE_BTN_ID);
    const char* fileName5 = "ui_test_clip_round_radius_46.bmp";
    CompareByBinary(fileName5);
}

void UIAutoTestClip::UIKitClipTest002() const
{
    DragViewToHead(UI_TEST_CLIP_ARC_LABEL_ID);
    const char* fileName1 = "ui_test_clip_sector_angle_0_50.bmp";
    CompareByBinary(fileName1);
    ClickViewById(UI_TEST_CLIP_START_ANGLE_ADD_BTN_ID);
    const char* fileName2 = "ui_test_clip_sector_angle_5_50.bmp";
    CompareByBinary(fileName2);
    ClickViewById(UI_TEST_CLIP_END_ANGLE_ADD_BTN_ID);
    const char* fileName3 = "ui_test_clip_sector_angle_5_55.bmp";
    CompareByBinary(fileName3);
    ClickViewById(UI_TEST_CLIP_START_ANGLE_REDUCE_BTN_ID);
    const char* fileName4 = "ui_test_clip_sector_angle_0_55.bmp";
    CompareByBinary(fileName4);
    ClickViewById(UI_TEST_CLIP_END_ANGLE_REDUCE_BTN_ID);
    const char* fileName5 = "ui_test_clip_sector_angle_0_50.bmp";
    CompareByBinary(fileName5);
}

void UIAutoTestClip::UIKitClipTest003() const
{
    DragViewToHead(UI_TEST_CLIP_POLYGON_FIVE_STAR_LABEL_ID);
    const char* fileName1 = "ui_test_clip_polygon_five_star.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestClip::UIKitClipTest004() const
{
    DragViewToHead(UI_TEST_CLIP_BESSEL_LABEL_ID);
    const char* fileName1 = "ui_test_clip_bessel.bmp";
    CompareByBinary(fileName1);
}

void UIAutoTestClip::UIKitClipTest005() const
{
    DragViewToHead(UI_TEST_CLIP_POLYGON_LABEL_ID);
    const char* fileName1 = "ui_test_clip_polygon.bmp";
    CompareByBinary(fileName1);
}
} // namespace OHOS