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

#include "components/ui_swipe_view.h"
#include "dock/focus_manager.h"
#include "dock/vibrator_manager.h"
#include "gfx_utils/graphic_log.h"

namespace OHOS {
UISwipeView::UISwipeView(uint8_t direction)
    : swipeListener_(nullptr), curIndex_(0), blankSize_(DEFAULT_BLANK_SIZE), curView_(nullptr), loop_(false)
{
#if ENABLE_ROTATE_INPUT
    rotateFactor_ = DEFAULT_SWIPE_VIEW_ROTATE_FACTOR;
    lastRotateLen_ = 0;
#endif
#if ENABLE_VIBRATOR
    lastIndex_ = 0;
    needVibration_ = false;
#endif
    direction_ = direction;
    tickTime_ = ANIMATOR_TIME;
    swipeAccCoefficient_ = DRAG_ACC_FACTOR;
}

UISwipeView::~UISwipeView() {}

void UISwipeView::Add(UIView* view)
{
    if (view == nullptr) {
        return;
    }
    view->SetDragParentInstead(true);
    UIViewGroup::Add(view);
    SortChild();
    Invalidate();
}

void UISwipeView::Insert(UIView* prevView, UIView* insertView)
{
    if (insertView == nullptr) {
        return;
    }
    insertView->SetDragParentInstead(true);
    UIViewGroup::Insert(prevView, insertView);
    SortChild();
    Invalidate();
}

void UISwipeView::Remove(UIView* view)
{
    if (view == nullptr) {
        return;
    }
    UIViewGroup::Remove(view);
    SortChild();
    Invalidate();
}

void UISwipeView::MoveHeadOrTailChild()
{
    if (loop_) {
        if (direction_ == HORIZONTAL) {
            while (childrenHead_->GetX() >= 0) {
                MoveLastChildToFirst();
            }
            while (childrenTail_->GetX() + childrenTail_->GetWidth() <= GetWidth()) {
                MoveFirstChildToLast();
            }
        } else {
            while (childrenHead_->GetY() >= 0) {
                MoveLastChildToFirst();
            }
            while (childrenTail_->GetY() + childrenTail_->GetHeight() <= GetHeight()) {
                MoveFirstChildToLast();
            }
        }
    }
}

void UISwipeView::SetCurrentPage(uint16_t index, bool needAnimator)
{
    if (needAnimator) {
        MoveHeadOrTailChild();
    }
    SwitchToPage(index, needAnimator);
    Invalidate();
}

bool UISwipeView::DragXInner(int16_t distance)
{
    if (distance == 0) {
        return true;
    }
    if (!loop_) {
        if ((distance > 0) && (childrenHead_ != nullptr)) {
            if (childrenHead_->GetX() >= blankSize_) {
                distance = 0;
            } else if (childrenHead_ && (childrenHead_->GetX() + distance > blankSize_)) {
                distance = blankSize_ - childrenHead_->GetX();
            }
        } else if (childrenTail_ != nullptr) {
            int16_t width = GetWidth();
            if (childrenTail_->GetRelativeRect().GetRight() < width - blankSize_) {
                distance = 0;
            } else if (width - (childrenTail_->GetX() + childrenTail_->GetWidth() + distance) > blankSize_) {
                distance = width - blankSize_ - childrenTail_->GetX() - childrenTail_->GetWidth();
            }
        }
    }
    CalculateInvalidate();
    MoveChildByOffset(distance, 0);
    CalculateInvalidate();
    return true;
}

bool UISwipeView::DragYInner(int16_t distance)
{
    if (distance == 0) {
        return true;
    }
    if (!loop_) {
        if ((distance > 0) && (childrenHead_ != nullptr)) {
            if (childrenHead_->GetY() >= blankSize_) {
                distance = 0;
            } else if ((childrenHead_ != nullptr) && (childrenHead_->GetY() + distance > blankSize_)) {
                distance = blankSize_ - childrenHead_->GetY();
            }
        } else if (childrenTail_ != nullptr) {
            int16_t height = GetHeight();
            if (childrenTail_->GetRelativeRect().GetBottom() < height - blankSize_) {
                distance = 0;
            } else if (height - (childrenTail_->GetY() + childrenTail_->GetHeight() + distance) > blankSize_) {
                distance = height - blankSize_ - childrenTail_->GetY() - childrenTail_->GetHeight();
            }
        }
    }
    CalculateInvalidate();
    MoveChildByOffset(0, distance);
    CalculateInvalidate();
    return true;
}

bool UISwipeView::OnDragEvent(const DragEvent& event)
{
    UIView* currentView = GetViewByIndex(curIndex_);
    if (currentView == nullptr) {
        return UIView::OnDragEvent(event);
    }
    if (scrollAnimator_.GetState() != Animator::STOP) {
        UIAbstractScroll::StopAnimator();
    }

    if (direction_ == HORIZONTAL) {
        DragXInner(event.GetDeltaX());
        RefreshDelta(event.GetDeltaX());
    } else {
        DragYInner(event.GetDeltaY());
        RefreshDelta(event.GetDeltaY());
    }
    return UIView::OnDragEvent(event);
}

bool UISwipeView::OnDragEndEvent(const DragEvent& event)
{
    int16_t distance = 0;
    if (direction_ == HORIZONTAL) {
        distance = event.GetCurrentPos().x - event.GetPreLastPoint().x;
    } else {
        distance = event.GetCurrentPos().y - event.GetPreLastPoint().y;
    }
    RefreshCurrentView(distance, event.GetDragDirection());

    if (curView_ == nullptr) {
        return UIView::OnDragEndEvent(event);
    }

    SwitchToPage(curIndex_);

    Invalidate();
    return UIView::OnDragEndEvent(event);
}

#if ENABLE_ROTATE_INPUT
bool UISwipeView::OnRotateStartEvent(const RotateEvent& event)
{
    if (scrollAnimator_.GetState() != Animator::STOP) {
        UIAbstractScroll::StopAnimator();
    }
#if ENABLE_VIBRATOR
    needVibration_ = true;
#endif
    return UIView::OnRotateStartEvent(event);
}

bool UISwipeView::OnRotateEndEvent(const RotateEvent& event)
{
    uint8_t dir;
    if (direction_ == HORIZONTAL) {
        dir = (lastRotateLen_ >= 0) ? DragEvent::DIRECTION_LEFT_TO_RIGHT : DragEvent::DIRECTION_RIGHT_TO_LEFT;
    } else {
        dir = (lastRotateLen_ >= 0) ? DragEvent::DIRECTION_TOP_TO_BOTTOM : DragEvent::DIRECTION_BOTTOM_TO_TOP;
    }
    RefreshCurrentView(lastRotateLen_, dir);
    if (curView_ == nullptr) {
        return UIView::OnRotateEndEvent(event);
    }
    SwitchToPage(curIndex_);
    lastRotateLen_ = 0;
#if ENABLE_VIBRATOR
    needVibration_ = false;
#endif
    return UIView::OnRotateEndEvent(event);
}
#endif

UIView* UISwipeView::GetViewByIndex(uint16_t index) const
{
    UIView* child = childrenHead_;
    while (child != nullptr) {
        if (child->GetViewIndex() == index) {
            return child;
        }
        child = child->GetNextSibling();
    }
    return nullptr;
}

void UISwipeView::SetAnimatorTime(uint16_t time)
{
    tickTime_ = time / DEFAULT_TASK_PERIOD;
    if (tickTime_ == 0) {
        tickTime_ = 1;
    }
    animatorCallback_.SetDragTimes(tickTime_);
}

void UISwipeView::SwitchToPage(int16_t dst, bool needAnimator)
{
    if (isNeedLoop()) {
        dst = (dst + childrenNum_) % childrenNum_;
    } else if (dst < 0) {
        dst = 0;
    } else if (dst >= childrenNum_) {
        dst = childrenNum_ - 1;
    }

    UIView* dstView = GetViewByIndex(dst);
    if (dstView == nullptr) {
        return;
    }
    curIndex_ = dst;
    int16_t xOffset = 0;
    int16_t yOffset = 0;

    if (direction_ == HORIZONTAL) {
        if (alignMode_ == ALIGN_LEFT) {
            xOffset = -dstView->GetX();
        } else if (alignMode_ == ALIGN_RIGHT) {
            xOffset = GetWidth() - (dstView->GetX() + dstView->GetWidthWithMargin());
        } else {
            xOffset = (GetWidth() >> 1) - (dstView->GetX() + (dstView->GetWidthWithMargin() >> 1));
        }
    } else {
        yOffset = (GetHeight() >> 1) - (dstView->GetY() + (dstView->GetHeightWithMargin() >> 1));
    }

    if ((xOffset != 0) || (yOffset != 0)) {
        if (scrollAnimator_.GetState() != Animator::STOP) {
            scrollAnimator_.Stop();
        }
        if (needAnimator) {
            animatorCallback_.SetDragTimes(tickTime_);
            animatorCallback_.SetDragStartValue(0, 0);
            animatorCallback_.SetDragEndValue(xOffset, yOffset);
            scrollAnimator_.Start();
        } else {
            MoveChildByOffset(xOffset, yOffset);
        }
    }
}

void UISwipeView::StopAnimator()
{
    UIAbstractScroll::StopAnimator();
    if (swipeListener_ != nullptr) {
        swipeListener_->OnSwipe(*this);
    }
}

void UISwipeView::SortChild()
{
    if (childrenHead_ == nullptr) {
        return;
    }
    int16_t index = 0;
    UIView* pre = childrenHead_;
    UIView* next = childrenHead_->GetNextSibling();
    if (direction_ == HORIZONTAL) {
        pre->SetX(0);
    } else {
        pre->SetY(0);
    }
    pre->SetViewIndex(index);
    index++;

    while (next != nullptr) {
        if (direction_ == HORIZONTAL) {
            next->SetX(pre->GetRelativeRect().GetRight() + pre->GetStyle(STYLE_MARGIN_RIGHT) + 1);
        } else {
            next->SetY(pre->GetRelativeRect().GetBottom() + pre->GetStyle(STYLE_MARGIN_BOTTOM) + 1);
        }
        pre = next;
        next->SetViewIndex(index);
        next = next->GetNextSibling();
        index++;
    }
    bool tmpLoop = loop_;
    loop_ = false;
    SwitchToPage(curIndex_, false);
    loop_ = tmpLoop;
}

void UISwipeView::RefreshCurrentViewInner(int16_t distance,
                                          uint8_t dragDirection,
                                          int16_t (UIView::*pfnGetXOrY)() const,
                                          int16_t (UIView::*pfnGetWidthOrHeight)())
{
    if (childrenHead_ == nullptr) {
        curIndex_ = 0;
        curView_ = nullptr;
        return;
    }

    curIndex_ = 0;
    curView_ = nullptr;

    /*
     * It needs to be modified that swipemid should be calculated by the width and height of the current
     * sub view itself, not the width and height of the parent, especially for ALIGN_LEFT and ALIGN_RIGHT.
     */
    uint16_t swipeMid;
    if (alignMode_ == ALIGN_LEFT) {
        swipeMid = 0;
    } else if (alignMode_ == ALIGN_RIGHT) {
        swipeMid = (this->*pfnGetWidthOrHeight)();
    } else {
        swipeMid = (this->*pfnGetWidthOrHeight)() >> 1;
    }
    UIView* view = childrenHead_;

    if ((childrenHead_->*pfnGetXOrY)() > swipeMid) {
        curIndex_ = childrenHead_->GetViewIndex();
        curView_ = childrenHead_;
    } else if ((childrenTail_->*pfnGetXOrY)() + (childrenHead_->*pfnGetWidthOrHeight)() < swipeMid) {
        curIndex_ = childrenTail_->GetViewIndex();
        curView_ = childrenTail_;
    } else {
        while (view != nullptr) {
            if ((swipeMid >= (view->*pfnGetXOrY)()) &&
                (swipeMid <= (view->*pfnGetXOrY)() + (view->*pfnGetWidthOrHeight)())) {
                curIndex_ = view->GetViewIndex();
                curView_ = view;
                break;
            }
            view = view->GetNextSibling();
        }
    }
    if (curView_ == nullptr) {
        return;
    }

    int16_t accelerationOffset = GetMaxDelta() * GetSwipeACCLevel() / DRAG_ACC_FACTOR;
    if (distance < 0) {
        /*
         * 7, 10 : Check whether the current view is dragged by more than 1/5,
         * that is, the x or y coordinate plus 7/10 width or height.
         */
        if (((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() >> 1) < swipeMid) &&
            ((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() * 7 / 10) - accelerationOffset <
            swipeMid)) {
            curIndex_++;
        }
    } else if (distance > 0) {
        /*
         * 3, 10 : Check whether the current view is dragged by more than 1/5,
         * that is, the x or y coordinate plus 3/10 width or height.
         */
        if (((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() >> 1) > swipeMid) &&
            ((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() * 3 / 10) + accelerationOffset >
            swipeMid)) {
            curIndex_--;
        }
    } else {
        if (alignMode_ == ALIGN_LEFT) {
            if (((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() >> 1) < swipeMid)) {
                curIndex_++;
            }
        } else if (alignMode_ == ALIGN_RIGHT) {
            if ((curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() >> 1) > swipeMid) {
                curIndex_--;
            }
        } else {
            /*
             * If the absolute value of the offset is greater than the page turning threshold,
             * page turning is considered.
             */
            int16_t offset = (curView_->*pfnGetXOrY)() + ((curView_->*pfnGetWidthOrHeight)() >> 1) - swipeMid;
            int16_t threshold = (this->*pfnGetWidthOrHeight)() >> 2; // 2: 1/4 width or height
            if (offset > threshold && (dragDirection == DragEvent::DIRECTION_TOP_TO_BOTTOM ||
                                       dragDirection == DragEvent::DIRECTION_LEFT_TO_RIGHT)) {
                curIndex_--;
            } else if ((offset < -threshold) && (dragDirection == DragEvent::DIRECTION_BOTTOM_TO_TOP ||
                                                 dragDirection == DragEvent::DIRECTION_RIGHT_TO_LEFT)) {
                curIndex_++;
            }
        }
    }
#if ENABLE_VIBRATOR
    VibratorFunc vibratorFunc = VibratorManager::GetInstance()->GetVibratorFunc();
    if (vibratorFunc != nullptr && needVibration_ && curIndex_ != lastIndex_) {
        if (!loop_ && (curIndex_ == 0 || curIndex_ == childrenNum_ - 1)) {
            vibratorFunc(VibratorType::VIBRATOR_TYPE_THREE);
            GRAPHIC_LOGI("UISwipeView::RefreshCurrentViewInner calls TYPE_THREE vibrator");
        } else {
            vibratorFunc(VibratorType::VIBRATOR_TYPE_ONE);
            GRAPHIC_LOGI("UISwipeView::RefreshCurrentViewInner calls TYPE_ONE vibrator");
        }
        lastIndex_ = curIndex_;
    }
#endif
}

void UISwipeView::RefreshCurrentView(int16_t distance, uint8_t dragDirection)
{
    if (direction_ == HORIZONTAL) {
        RefreshCurrentViewInner(distance, dragDirection, &UIView::GetX, &UIView::GetWidthWithMargin);
    } else {
        RefreshCurrentViewInner(distance, dragDirection, &UIView::GetY, &UIView::GetHeightWithMargin);
    }
}

void UISwipeView::MoveChildByOffset(int16_t xOffset, int16_t yOffset)
{
    UIViewGroup::MoveChildByOffset(xOffset, yOffset);
    MoveHeadOrTailChild();
}

bool UISwipeView::isNeedLoop()
{
    if (!loop_ || (childrenHead_ == nullptr) || (childrenTail_ == nullptr)) {
        return false;
    }
    Rect childRect = GetAllChildRelativeRect();
    if (direction_ == HORIZONTAL) {
        if ((childRect.GetWidth() - childrenHead_->GetWidth() >= GetWidth()) &&
            (childRect.GetWidth() - childrenTail_->GetWidth() >= GetWidth())) {
            return true;
        }
    } else {
        if ((childRect.GetHeight() - childrenHead_->GetHeight() >= GetHeight()) &&
            (childRect.GetHeight() - childrenTail_->GetHeight() >= GetHeight())) {
            return true;
        }
    }
    return false;
}

void UISwipeView::MoveFirstChildToLast()
{
    if ((childrenTail_ == nullptr) || (childrenHead_ == nullptr)) {
        return;
    }
    if (direction_ == HORIZONTAL) {
        childrenHead_->SetX(childrenTail_->GetX() + childrenTail_->GetWidth());
    } else {
        childrenHead_->SetY(childrenTail_->GetY() + childrenTail_->GetHeight());
    }
    UIView* head = childrenHead_;
    UIViewGroup::Remove(childrenHead_);
    UIViewGroup::Add(head);
}

void UISwipeView::MoveLastChildToFirst()
{
    if ((childrenTail_ == nullptr) || (childrenHead_ == nullptr)) {
        return;
    }
    if (direction_ == HORIZONTAL) {
        childrenTail_->SetX(childrenHead_->GetX() - childrenTail_->GetWidth());
    } else {
        childrenTail_->SetY(childrenHead_->GetY() - childrenTail_->GetHeight());
    }
    UIView* last = childrenTail_;
    UIViewGroup::Remove(childrenTail_);
    UIViewGroup::Insert(nullptr, last);
}

void UISwipeView::CalculateInvalidate()
{
    Rect swipeRect(0, 0, GetRelativeRect().GetWidth() - 1, GetRelativeRect().GetHeight() - 1);
    UIView* view = childrenHead_;
    bool isFound = false;
    while (view != nullptr) {
        Rect rect = view->GetRelativeRect();
        if (rect.IsIntersect(swipeRect)) {
            if (view->IsVisible() && (view->GetOpaScale() != OPA_TRANSPARENT)) {
                view->Invalidate();
            }
            isFound = true;
        } else if (isFound) {
            return;
        }

        view = view->GetNextSibling();
    }
}
} // namespace OHOS
