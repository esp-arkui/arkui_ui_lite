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
 * @file gif_canvas_image_animator.h
 *
 * @brief Defines the Canvas GIF image handling class.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GIF_CANVAS_IMAGE_ANIMATOR_H
#define GIF_CANVAS_IMAGE_ANIMATOR_H

#include <string>
#include "common/image.h"
#include "animator.h"
#include "components/ui_view.h"
#include "gif_lib.h"

namespace OHOS {
#define DECLARE_GIF_CANVAS_IMAGE_ANIMATOR class GifCanvasImageAnimator

DECLARE_GIF_CANVAS_IMAGE_ANIMATOR;
struct ImageParam : public HeapBase {
    Point start;
    uint16_t height;
    uint16_t width;
    int16_t newWidth;
    int16_t newHeight;
    Image* image = nullptr;
    GifCanvasImageAnimator* gifImageAnimator = nullptr;
    std::string path;
};

class GifCanvasImageAnimator : public Animator, public AnimatorCallback {
public:
    GifCanvasImageAnimator(ImageParam* image, UIView* view, const char* src)
        : Animator(this, view, 0, true),
        size_(PointValue(0, 0)),
        gifFileType_(nullptr),
        imageIndex_(0),
        delayTime_(0),
        lastRunTime_(0),
        deltaTime_(0),
        gifDataSize_(0),
        src_(src),
        image_(image)

    {
        OpenGifFile(image->path.c_str()); // Open once to get the size first
        CloseGifFile();
    }

    virtual ~GifCanvasImageAnimator() override
    {
        image_ = nullptr;
        CloseGifFile();
    }

    void Callback(UIView* view) override;

    void SetGifFileType(GifFileType* gifFileType)
    {
        gifFileType_ = gifFileType;
    }

    uint32_t SetGifFrame(GifFileType* gifFileType, int32_t imageIndex, UIView* imageView) const;
    void DealGifImageData(const GifFileType* gifFileType,
                          const GifImageDesc* gifImageDesc,
                          const SavedImage* savedImage,
                          GraphicsControlBlock gcb,
                          const ColorMapObject* colorMap) const;
    void DealGifImageLineData(const GifImageDesc* gifImageDesc,
                              const SavedImage* savedImage,
                              GraphicsControlBlock& gcb,
                              const ColorMapObject* colorMap,
                              bool& transparentColor,
                              int32_t& heightVale,
                              int32_t& widthVale,
                              uint8_t& colorIndex,
                              uint32_t& index) const;
    void OpenGifFile(const char* src);
    void CloseGifFile();
    void SetImage(ImageParam* image);
    Point GetSize()
    {
        return size_;
    }

private:
    GifFileType* GetGifFileType()
    {
        if (gifFileType_ == nullptr) {
            OpenGifFile(src_);
        }
        return gifFileType_;
    }

    Point size_;
    GifFileType* gifFileType_;
    int32_t imageIndex_;
    uint32_t delayTime_;
    uint32_t lastRunTime_;
    uint32_t deltaTime_;
    uint32_t gifDataSize_;
    uint8_t* gifImageData_ = nullptr;
    const char* src_;
    ImageParam* image_;
};
}
#endif // GIF_CANVAS_IMAGE_ANIMATOR_H
