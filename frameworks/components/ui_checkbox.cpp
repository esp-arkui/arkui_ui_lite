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

#include "common/image.h"
#include "components/ui_checkbox.h"
#include "default_resource/check_box_res.h"
#include "draw/draw_arc.h"
#include "draw/draw_image.h"
#include "draw/draw_line.h"
#include "draw/draw_rect.h"
#include "imgdecode/cache_manager.h"

namespace OHOS {
UICheckBox::UICheckBox()
    : state_(UNSELECTED), onStateChangeListener_(nullptr), width_(DEFAULT_HOT_WIDTH), height_(DEFAULT_HOT_HEIGHT),
      borderWidth_(DEFAULT_BORDER_WIDTH), animator_(this, this, 0, true)
{
    touchable_ = true;
    style_ = &(StyleDefault::GetBackgroundTransparentStyle());
    image_[UNSELECTED].SetSrc(GetCheckBoxOffInfo());
    image_[SELECTED].SetSrc(GetCheckBoxOnInfo());
    ImageHeader header = { 0 };
    image_[UNSELECTED].GetHeader(header);
    Resize(header.width, header.height);
    AnimatorManager::GetInstance()->Add(&animator_);
}

UICheckBox::~UICheckBox()
{
    animator_.Stop();
    AnimatorManager::GetInstance()->Remove(&animator_);
}

void UICheckBox::SetState(UICheckBoxState state)
{
    if (state != state_) {
        state_ = state;
        if (onStateChangeListener_ != nullptr) {
            onStateChangeListener_->OnChange(state);
        }
        Invalidate();
    }
}

void UICheckBox::ReverseState()
{
    state_ = (state_ == SELECTED) ? UNSELECTED : SELECTED;
}

bool UICheckBox::OnClickEvent(const ClickEvent& event)
{
    ReverseState();
    RsetCallback();
    CalculateSize();
    animator_.Start();
    Invalidate();
    return UIView::OnClickEvent(event);
}

void UICheckBox::SetImages(const char* selectedImageSrc, const char* unselectedImageSrc)
{
    image_[SELECTED].SetSrc(selectedImageSrc);
    image_[UNSELECTED].SetSrc(unselectedImageSrc);
}

void UICheckBox::SetImages(const ImageInfo* selectedImageSrc, const ImageInfo* unselectedImageSrc)
{
    image_[SELECTED].SetSrc(selectedImageSrc);
    image_[UNSELECTED].SetSrc(unselectedImageSrc);
}

void UICheckBox::CalculateSize()
{
    int16_t width = GetWidth();
    int16_t height = GetHeight();
    if ((width_ == width) && (height_ == height)) {
        return;
    }
    width_ = width;
    height_ = height;
    int16_t minValue = (width_ > height_) ? height_ : width_;
    borderWidth_ =  DEFAULT_BORDER_WIDTH * minValue / DEFAULT_HOT_WIDTH;
}

void UICheckBox::SelectedStateSoftwareDrawing(Rect rect, Rect trunc, int16_t borderRadius, int16_t rectLineWidth)
{
    Style styleSelect = StyleDefault::GetBackgroundTransparentStyle();
    styleSelect.borderRadius_ = borderRadius;
    styleSelect.bgColor_ = Color::GetColorFromRGB(0x1F, 0x71, 0xFF);
    styleSelect.bgOpa_ = OPA_OPAQUE;
    Rect contentRect = GetContentRect();
    bool isIntersect = trunc.Intersect(trunc, contentRect);
    if (isIntersect) {
        DrawRect::Draw(rect, trunc, styleSelect, opaScale_);
    }
    int16_t dx = borderWidth_ * 0.22; // 0.22 : coefficient,
    int16_t dy = borderWidth_ * 0.5; // 0.5 : coefficient,
    Point start = { static_cast<int16_t>(rect.GetX() + dx), static_cast<int16_t>(rect.GetY() + dy) };
    dx = borderWidth_ * 0.2; // 0.2 : coefficient
    Point mid = { static_cast<int16_t>(start.x + dx), static_cast<int16_t>(start.y + dx) };
    dx = borderWidth_ * 0.38; // 0.38 : coefficient
    Point end = { static_cast<int16_t>(mid.x + dx), static_cast<int16_t>(mid.y - dx) };
    const int16_t half = 2; // 2 ：half
    ArcInfo arcInfoLeft = {
        start, { 0, 0 }, static_cast<uint16_t>(rectLineWidth), SEMICIRCLE_IN_DEGREE + QUARTER_IN_DEGREE / half,
        QUARTER_IN_DEGREE / half, nullptr
    };
    ArcInfo arcInfoMid = {
        mid, { 0, 0 }, static_cast<uint16_t>(rectLineWidth), SEMICIRCLE_IN_DEGREE - QUARTER_IN_DEGREE / half,
        SEMICIRCLE_IN_DEGREE + QUARTER_IN_DEGREE / half, nullptr
    };
    ArcInfo arcInfoRight = {
        end, { 0, 0 }, static_cast<uint16_t>(rectLineWidth), CIRCLE_IN_DEGREE - QUARTER_IN_DEGREE / half,
        SEMICIRCLE_IN_DEGREE - QUARTER_IN_DEGREE / half, nullptr
    };
    styleSelect.lineColor_ = Color::White();
    if (isIntersect) {
        DrawArc::GetInstance()->Draw(arcInfoLeft, trunc, styleSelect, OPA_OPAQUE, CapType::CAP_NONE);
        DrawLine::Draw(start, mid, trunc, rectLineWidth * 2, Color::White(), opaScale_); // 2 : double
        DrawArc::GetInstance()->Draw(arcInfoMid, trunc, styleSelect, OPA_OPAQUE, CapType::CAP_NONE);
        DrawLine::Draw(mid, end, trunc, rectLineWidth * 2, Color::White(), opaScale_); // 2 : double
        DrawArc::GetInstance()->Draw(arcInfoRight, trunc, styleSelect, OPA_OPAQUE, CapType::CAP_NONE);
    }
}

void UICheckBox::OnDraw(const Rect& invalidatedArea)
{
    Rect trunc = invalidatedArea;
    if ((image_[SELECTED].GetSrcType() != IMG_SRC_UNKNOWN) && (image_[UNSELECTED].GetSrcType() != IMG_SRC_UNKNOWN)) {
        ImageHeader header = {0};
        image_[state_].GetHeader(header);
        int16_t imgWidth = header.width;
        int16_t imgHeight = header.height;
        Rect coords = GetContentRect();
        coords.SetWidth(imgWidth);
        coords.SetHeight(imgHeight);
        DrawRect::Draw(GetRect(), invalidatedArea, *style_, opaScale_);
        int16_t offsetLeft = (GetWidth() - imgWidth) / 2; // 2 : half
        int16_t offsetTop = (GetHeight() - imgHeight) / 2; // 2 : half
        coords.SetX(coords.GetX() + offsetLeft);
        coords.SetY(coords.GetY() + offsetTop);
        if (trunc.Intersect(trunc, coords)) {
            image_[state_].DrawImage(coords, trunc, *style_, opaScale_);
        }
    } else {
        CalculateSize();
        int16_t rectLineWidth = borderWidth_ / DEFAULT_BORDER_WIDTH;
        int16_t borderRadius  = rectLineWidth * 4; // 4 : coefficient
        DrawRect::Draw(GetRect(), invalidatedArea, *style_, opaScale_);
        Rect contentRect = GetContentRect();
        int16_t x = contentRect.GetX() + (width_ - borderWidth_) / 2; // 2: half
        int16_t y = contentRect.GetY() + (height_ - borderWidth_) / 2; // 2: half
        Rect rect(x, y, x + borderWidth_,  y + borderWidth_);
        switch (state_) {
            case SELECTED: {
                SelectedStateSoftwareDrawing(rect, trunc, borderRadius, rectLineWidth);
                break;
            }
            case UNSELECTED: {
                Style styleUnSelect  = StyleDefault::GetBackgroundTransparentStyle();
                styleUnSelect.borderWidth_ = rectLineWidth;
                styleUnSelect.borderRadius_ = borderRadius;
                styleUnSelect.borderColor_ = Color::White();
                styleUnSelect.borderOpa_ = 0xa8; // 0xa8: opacity
                if (trunc.Intersect(trunc, contentRect)) {
                    DrawRect::Draw(rect, trunc, styleUnSelect, opaScale_);
                }
                break;
            }
            default:
                break;
        }
    }
}

void UICheckBox::Callback(UIView* view)
{

}
} // namespace OHOS
