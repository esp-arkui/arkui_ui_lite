#ifndef GIF_CANVAS_IMAGE_ANIMATOR_H
#define GIF_CANVAS_IMAGE_ANIMATOR_H

#endif // GIF_CANVAS_IMAGE_ANIMATOR_H

#include "animator.h"
#include "gif_lib.h"
#include "components/ui_view.h"
#include "common/image.h"

namespace OHOS {

class GifCanvasImageAnimator;
struct ImageParam : public HeapBase {
    Point start;
    uint16_t height;
    uint16_t width;
    Image* image = nullptr;
    GifCanvasImageAnimator *gifImageAnimator = nullptr;
    char* path;
};

class GifCanvasImageAnimator : public Animator, public AnimatorCallback {
public:
    GifCanvasImageAnimator(ImageParam* image,UIView* view, const char* src)
        : Animator(this, view, 0, true),
          gifFileType_(nullptr),
          imageIndex_(0),
          delayTime_(0),
          lastRunTime_(0),
          deltaTime_(0),
          gifDataSize_(0),
          src_(src),
          image_(image),
          size_({0,0})
    {

        OpenGifFile(image->path);//打开一次先获取大小
        CloseGifFile();

    }

    virtual ~GifCanvasImageAnimator()
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
    const void OpenGifFile(const char* src);
    void CloseGifFile();
    void SetImage(ImageParam* image);
    Point GetSize(){
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
