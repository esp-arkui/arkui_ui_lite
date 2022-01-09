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

#include "animator/gif_canvas_image_animator.h"

#include <securec.h>

#include "draw/draw_utils.h"

namespace OHOS {
    const int HUNDREDTHS = 10;
    const int GIF_PIX_SIZE = 4;
    void GifCanvasImageAnimator::OpenGifFile(const char* src)
    {
        int32_t error = D_GIF_SUCCEEDED;
        GifFileType* gifFileType = DGifOpenFileName(src, &error);
        if (error != D_GIF_SUCCEEDED) {
            return;
        }
        DGifSlurp(gifFileType);
        /* 3 : when change single pixel to byte, the buffer should divided by 8, equal to shift right 3 bits. */
        uint8_t pixelByteSize = DrawUtils::GetPxSizeByColorMode(ARGB8888) >> 3;
        gifDataSize_ = static_cast<uint32_t>(gifFileType->SWidth * gifFileType->SHeight * pixelByteSize);
        size_ = {static_cast<int16_t>(gifFileType->SWidth), static_cast<int16_t>(gifFileType->SHeight)};
        gifImageData_ = static_cast<uint8_t*>(UIMalloc(gifDataSize_));
        if (gifImageData_ == nullptr) {
            CloseGifFile();
            return;
        }
        SetGifFileType(gifFileType);
    }

    void GifCanvasImageAnimator::CloseGifFile()
    {
        GifFileType* gifFileType = GetGifFileType();
        if (gifFileType != nullptr) {
            DGifCloseFile(gifFileType, nullptr);
        }
        if (gifImageData_ != nullptr) {
            UIFree(reinterpret_cast<void*>(const_cast<uint8_t*>(gifImageData_)));
            gifImageData_ = nullptr;
        }
        imageIndex_ = 0;
        gifFileType_ = nullptr;
    }

    void GifCanvasImageAnimator::Callback(UIView* view)
    {
        if (view == nullptr) {
            return;
        }
        if (view == nullptr) {
            return;
        }
        uint32_t curTime = GetRunTime();
        if (curTime != 0) {
            if (curTime + deltaTime_ - lastRunTime_ >= delayTime_) {
                deltaTime_ = curTime + deltaTime_ - lastRunTime_ - delayTime_;
                lastRunTime_ = curTime;
            } else {
                return;
            }
        }
        GifFileType* gifFileType = GetGifFileType();
        if (gifFileType != nullptr) {
            delayTime_ = SetGifFrame(gifFileType, imageIndex_, view);
            imageIndex_ = (imageIndex_ < gifFileType->ImageCount - 1) ? (imageIndex_ + 1) : 0;
        }
    }

    uint32_t GifCanvasImageAnimator::SetGifFrame(GifFileType* gifFileType, int32_t imageIndex, UIView* imageView) const
    {
        SavedImage* savedImage = &(gifFileType->SavedImages[imageIndex]);
        if (savedImage == nullptr) {
            return 0;
        }
        GifImageDesc* gifImageDesc = &(savedImage->ImageDesc);
        if (gifImageDesc == nullptr) {
            return 0;
        }
        GraphicsControlBlock gcb;
        int32_t ret = DGifSavedExtensionToGCB(gifFileType, imageIndex, &gcb);
        if (ret != GIF_OK) {
            return 0;
        }
        ColorMapObject* colorMap = nullptr;
        if (gifImageDesc->ColorMap != nullptr) {
            colorMap = gifImageDesc->ColorMap;
        } else {
            colorMap = gifFileType->SColorMap;
        }

        DealGifImageData(gifFileType, gifImageDesc, savedImage, gcb, colorMap);
        if (gifImageData_ == nullptr || image_ == nullptr || image_->image == nullptr) {
            return 0;
        }
        ImageInfo gifFrame;
        gifFrame.header.width = static_cast<uint16_t>(gifFileType->SWidth);
        gifFrame.header.height = static_cast<uint16_t>(gifFileType->SHeight);
        gifFrame.header.colorMode = ARGB8888;
        gifFrame.dataSize = gifDataSize_;
        gifFrame.data = gifImageData_;
        image_->image->SetSrc(&gifFrame);
        image_->width = static_cast<uint16_t>(gifFileType->SWidth);
        image_->height = static_cast<uint16_t>(gifFileType->SHeight);

        if (gcb.DelayTime >= 0) {
            imageView->Invalidate();
            // 10: change hundredths (1/100) of a second to millisecond
            return static_cast<uint32_t>(gcb.DelayTime) * HUNDREDTHS;
        } else {
            return 0;
        }
    }

    void GifCanvasImageAnimator::DealGifImageData(const GifFileType* gifFileType,
                                                  const GifImageDesc* gifImageDesc,
                                                  const SavedImage* savedImage,
                                                  GraphicsControlBlock gcb,
                                                  const ColorMapObject* colorMap) const
    {
        if ((gifFileType == nullptr) || (gifImageDesc == nullptr)
                || (savedImage == nullptr) || (savedImage->RasterBits == nullptr)
                || (colorMap == nullptr) || (colorMap->Colors == nullptr)) {
            return;
        }
        uint8_t colorIndex = 0;
        GifColorType* gifColorType = nullptr;
        uint32_t index = 0;
        bool transparentColor = true;
        int32_t loc = 0;
        for (int32_t x = 0; x < gifFileType->SHeight; x++) {
            for (int32_t y = 0; y < gifFileType->SWidth; y++) {
                transparentColor = true;
                if ((x >= gifImageDesc->Top) && (x < gifImageDesc->Top + gifImageDesc->Height) &&
                    (y >= gifImageDesc->Left) && (y < gifImageDesc->Left + gifImageDesc->Width)) {
                    loc = (x - gifImageDesc->Top) * gifImageDesc->Width + (y - gifImageDesc->Left);
                    colorIndex = savedImage->RasterBits[loc];
                    if ((gcb.TransparentColor == NO_TRANSPARENT_COLOR) || (colorIndex != gcb.TransparentColor)) {
                        transparentColor = false;
                    }
                }
                if (transparentColor) {
                    index += GIF_PIX_SIZE; // GIF_PIX_SIZE: skip color index, keep last frame color
                } else {
                    gifColorType = &colorMap->Colors[colorIndex];
                    gifImageData_[index++] = gifColorType->Blue;
                    gifImageData_[index++] = gifColorType->Green;
                    gifImageData_[index++] = gifColorType->Red;
                    gifImageData_[index++] = OPA_OPAQUE;
                }
            }
        }
    }

    void GifCanvasImageAnimator::SetImage(ImageParam* image)
    {
        image_ = image;
    }
} // namespace OHOS
