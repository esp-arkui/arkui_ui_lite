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

#include "components/ui_abstract_scroll.h"

namespace OHOS {
UIAbstractScroll::UIAbstractScroll()
{
#if ENABLE_FOCUS_MANAGER
    focusable_ = true;
#endif
    isViewGroup_ = true;
    touchable_ = true;
    draggable_ = true;
    dragParentInstead_ = false;
}

UIAbstractScroll::~UIAbstractScroll()
{
    scrollAnimator_.Stop();
}

void UIAbstractScroll::MoveChildByOffset(int16_t offsetX, int16_t offsetY)
{
    if ((offsetX == 0) && (offsetY == 0)) {
        return;
    }
    UIView* view = GetChildrenHead();
    int16_t x;
    int16_t y;
    while (view != nullptr) {
        x = view->GetX() + offsetX;
        y = view->GetY() + offsetY;
        view->SetPosition(x, y);
        view = view->GetNextSibling();
    }
    Invalidate();
}

int16_t UIAbstractScroll::GetMaxDeltaY() const
{
    int16_t result = 0;
    for (int16_t i = 0; i < MAX_DELTA_Y_SIZE; i++) {
        if (result < MATH_ABS(lastDeltaY_[i])) {
            result = MATH_ABS(lastDeltaY_[i]);
        }
    }
    return result;
}

void UIAbstractScroll::StopAnimator()
{
    scrollAnimator_.Stop();
    RsetCallback();
    isDragging_ = false;
}

bool UIAbstractScroll::DragThrowAnimator(Point currentPos, Point lastPos)
{
    if (!throwDrag_ && (reboundSize_ == 0)) {
        return false;
    }
    int16_t dragDistanceX = 0;
    int16_t dragDistanceY = 0;
    if (throwDrag_) {
        CalculateDragDistance(currentPos, lastPos, dragDistanceX, dragDistanceY);
    }
    if (reboundSize_ != 0) {
        CalculateReboundDistance(dragDistanceX, dragDistanceY);
    }
    StartAnimator(dragDistanceX, dragDistanceY);
    return true;
}

void UIAbstractScroll::StartAnimator(int16_t dragDistanceX, int16_t dragDistanceY)
{
    int16_t dragTimes = MATH_MAX(MATH_ABS(dragDistanceX), MATH_ABS(dragDistanceY)) / DRAG_TIMES_COEFFICIENT;
    if (dragTimes < MIN_DRAG_TIMES) {
        dragTimes = MIN_DRAG_TIMES;
    }
    RsetCallback();
    SetDragStartValue(0, 0);
    SetDragEndValue(dragDistanceX, dragDistanceY);
    dragTimes_ = dragTimes * DRAG_ACC_FACTOR / GetDragACCLevel();
    scrollAnimator_.Start();
    isDragging_ = true;
}

void UIAbstractScroll::CalculateDragDistance(Point currentPos,
                                             Point lastPos,
                                             int16_t& dragDistanceX,
                                             int16_t& dragDistanceY)
{
    if ((direction_ == VERTICAL) || (direction_ == HORIZONTAL_AND_VERTICAL)) {
        dragDistanceY = (currentPos.y - lastPos.y) * DRAG_DISTANCE_COEFFICIENT;
        if (dragDistanceY > 0) {
            dragDistanceY += GetMaxDeltaY() * GetSwipeACCLevel() / DRAG_ACC_FACTOR;
        } else {
            dragDistanceY -= GetMaxDeltaY() * GetSwipeACCLevel() / DRAG_ACC_FACTOR;
        }
    }

    if ((direction_ == HORIZONTAL) || (direction_ == HORIZONTAL_AND_VERTICAL)) {
        dragDistanceX = (currentPos.x - lastPos.x) * DRAG_DISTANCE_COEFFICIENT;
    }

    if (maxScrollDistance_ != 0) {
        if (MATH_ABS(dragDistanceY) > maxScrollDistance_) {
            int16_t calculatedValue = (dragDistanceY > 0) ? 1 : -1;
            dragDistanceY = calculatedValue * maxScrollDistance_;
        }
        if (MATH_ABS(dragDistanceX) > maxScrollDistance_) {
            int16_t calculatedValue = (dragDistanceX > 0) ? 1 : -1;
            dragDistanceX = calculatedValue * maxScrollDistance_;
        }
    }
}

void UIAbstractScroll::Callback(UIView* view)
{
    if (view == nullptr) {
        return;
    }

    UIAbstractScroll* scrollView = static_cast<UIAbstractScroll*>(view);
    if (curtTime_ <= dragTimes_) {
        bool needStopX = false;
        bool needStopY = false;
        if (startValueY_ != endValueY_) {
            int16_t actY = scrollView->easingFunc_(startValueY_, endValueY_, curtTime_, dragTimes_);
            if ((actY != previousValueY_) && !scrollView->DragYInner(actY - previousValueY_)) {
                needStopY = true;
            }
            previousValueY_ = actY;
        } else {
            needStopY = true;
        }
        if (startValueX_ != endValueX_) {
            int16_t actX = scrollView->easingFunc_(startValueX_, endValueX_, curtTime_, dragTimes_);
            if ((actX != previousValueX_) && !scrollView->DragXInner(actX - previousValueX_)) {
                needStopX = true;
            }
            previousValueX_ = actX;
        } else {
            needStopX = true;
        }
        if (needStopX && needStopY) {
            scrollView->StopAnimator();
        } else {
            curtTime_++;
        }
    } else {
        scrollView->StopAnimator();
    }
}

void UIAbstractScroll::OnStop(UIView& view)
{
    RsetCallback();
    isDragging_ = false;
}
} // namespace OHOS
