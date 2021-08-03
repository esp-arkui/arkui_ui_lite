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

#include "ui_auto_test_image_animator.h"
#include "auto_test_counter.h"
#include "ui_test_image_animator.h"

namespace OHOS {
void UIAutoTestImageAnimator::Reset() const
{
    ResetMainMenu();
    EnterSubMenu("Image_Animator");
}

void UIAutoTestImageAnimator::RunTestList()
{
    Reset();
    UIKitImageAnimatorTestStart001();
    UIKitImageAnimatorTestStop002();
    UIKitImageAnimatorTestPause003();
    UIKitImageAnimatorTestResume004();
    UIKitImageAnimatorTestSetImage005();
    UIKitImageAnimatorTestSetTickOfUpdate006();
    UIKitImageAnimatorTestSetSizeFixed007();
    UIKitImageAnimatorTestSetRepeat008();
    UIKitImageAnimatorTestSetReverse009();
    UIKitImageAnimatorTestSetAnimatorStopListener010();
    UIKitImageAnimatorTestSetRepeatTimes011();
    UIKitImageAnimatorTestSetFillModeTrueForward012();
    UIKitImageAnimatorTestSetFillModeTrueBackward013();
    UIKitImageAnimatorTestSetFillModeFalseForward014();
    UIKitImageAnimatorTestSetFillModeFalseBackward015();
    UIKitImageAnimatorTestSetImageInfo016();
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestStart001() const
{
    ClickViewById("test_image_animator_start_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_star",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestStop002() const
{
    ClickViewById("test_image_animator_stop_btn");
    const char* fileName = "ui_test_image_animator_stop_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestPause003() const
{
    ClickViewById("test_image_animator_start_btn");
    ClickViewById("test_image_animator_pause_btn");
    const char* fileName = "ui_test_image_animator_pause_001.bmp";
    CompareByBinary(fileName);
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestResume004() const
{
    ClickViewById("test_image_animator_resume_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_resume",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetImage005() const
{
    Reset();
    ClickViewById("test_image_animator_set_image_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_set_image",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetTickOfUpdate006() const
{
    Reset();
    ClickViewById("test_image_animator_set_image_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_set_tick_of_update",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetSizeFixed007() const
{
    ClickViewById("test_image_animator_fixe_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fixe",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetRepeat008() const
{
    Reset();
    ClickViewById("test_image_animator_no_repeat_btn");
    // Take the 8 data of 1, 5, 10, 15, 20, 25, 30, 35 frames as the comparison point.
    uint16_t frameArray[8] = {1, 5, 10, 15, 20, 25, 30, 35};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_no_repeat",
        frameArray, sizeof(frameArray));
    ClickViewById("test_image_animator_repeat_btn");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_repeat",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetReverse009() const
{
    Reset();
    ClickViewById("test_image_animator_positive_order_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_positive_order",
        frameArray, sizeof(frameArray));
    ClickViewById("test_image_animator_reverse_order_btn");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_reverse_order",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetAnimatorStopListener010() const
{
    Reset();
    ClickViewById("test_image_animator_listener_btn");
    ClickViewById("test_image_animator_no_repeat_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_listern",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetRepeatTimes011() const
{
    Reset();
    ClickViewById("test_image_animator_no_repeat_btn");
    // Take the 10 data of 1, 5, 10, 15, 20, 25, 30, 35, 40, 45 frames as the comparison point.
    uint16_t frameArray[10] = {1, 5, 10, 15, 20, 25, 30, 35, 40, 45};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_no_repeat",
        frameArray, sizeof(frameArray));
    ClickViewById("test_image_animator_repeat_times_btn");
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_listern",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetFillModeTrueForward012() const
{
    Reset();
    ClickViewById("test_image_animator_fill_mode_true_forward_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fill_mode_true_forward",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetFillModeTrueBackward013() const
{
    ClickViewById("test_image_animator_fill_mode_true_backward_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fill_mode_true_backward",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetFillModeFalseForward014() const
{
    ClickViewById("test_image_animator_fill_mode_false_forward_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fill_mode_false_backward",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetFillModeFalseBackward015() const
{
    ClickViewById("test_image_animator_fill_mode_false_backward_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fill_mode_false_backward",
        frameArray, sizeof(frameArray));
}

void UIAutoTestImageAnimator::UIKitImageAnimatorTestSetImageInfo016() const
{
    ClickViewById("test_image_animator_set_image_info_btn");
    // Take the 5 data of 1、5、10、15、20 frames as the comparison point.
    uint16_t frameArray[5] = {1, 5, 10, 15, 20};
    AutoTestCounter::GetInstance().SetFrameArray("ui_test_image_animator_fill_mode_false_backward",
        frameArray, sizeof(frameArray));
}
} // namespace OHOS