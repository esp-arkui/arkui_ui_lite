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

#include "ui_auto_test_image.h"
#include "ui_test_image.h"

namespace OHOS {
void UIAutoTestImage::Reset() const
{
    ResetMainMenu();
    EnterSubMenu(UI_TEST_IMAGE_ID);
}

void UIAutoTestImage::RunTestList()
{
    Reset();
    UIKitUIImageTestSetImage001();
    UIKitUIImageTestSetImage002();
    UIKitUIImageTestSetImage004();
    UIKitUIImageTestSetImage005();
    UIKitUIImageTestSetImage006();
    UIKitUIImageTestSetImage007();
    UIKitUIImageTestSetImage008();
    UIKitUIImageTestSetImage009();
    UIKitUIImageTestSetImage010();
    UIKitUIImageTestSetImage011();
    UIKitUIImageTestSetImage012();
    UIKitUIImageTestSetImage013();
    UIKitUIImageTestSetImage014();
    UIKitUIImageTestUncompress001();
}

void UIAutoTestImage::UIKitUIImageTestSetImage001() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_01);
    const char* fileName = "ui_test_image_set_image_01.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage002() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_02);
    const char* fileName = "ui_test_image_set_image_02.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage004() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_04);
    const char* fileName = "ui_test_image_set_image_04.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage005() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_05);
    const char* fileName = "ui_test_image_set_image_05.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage006() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_06);
    const char* fileName = "ui_test_image_set_image_06.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage007() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_07);
    const char* fileName = "ui_test_image_set_image_07.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage008() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_08);
    const char* fileName = "ui_test_image_set_image_08.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage009() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_09);
    const char* fileName = "ui_test_image_set_image_09.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage010() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_10);
    const char* fileName = "ui_test_image_set_image_10.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage011() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_11);
    const char* fileName = "ui_test_image_set_image_11.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage012() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_12);
    const char* fileName = "ui_test_image_set_image_12.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage013() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_13);
    const char* fileName = "ui_test_image_set_image_13.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestSetImage014() const
{
    DragViewToHead(UI_TEST_IMAGE_SETIMAGE_ID_14);
    const char* fileName = "ui_test_image_set_image_14.bin";
    CompareByBinary(fileName);
}

void UIAutoTestImage::UIKitUIImageTestUncompress001() const
{
    DragViewToHead(UI_TEST_IMAGE_UNCOMPARESS_ID_01);
    const char* fileName = "ui_test_image_uncompress_01.bin";
    CompareByBinary(fileName);
}
} // namespace OHOS