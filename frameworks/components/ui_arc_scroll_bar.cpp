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

#include "components/ui_arc_scroll_bar.h"

#include "draw/draw_arc.h"
#include "engines/gfx/gfx_engine_manager.h"

namespace OHOS {
void UIArcScrollBar::SetPosition(int16_t x, int16_t y, int16_t width, int16_t radius)
{
    if ((width > 0) && (radius > 0)) {
        center_.x = x;
        center_.y = y;
        width_ = width;
        radius_ = radius;
    }
}

void UIArcScrollBar::OnDraw(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea, uint8_t backgroundOpa)
{
    backgroundOpa = (backgroundOpa == OPA_OPAQUE) ? opacity_ : (static_cast<uint16_t>(backgroundOpa) * opacity_) >> 8;
    DrawBackground(gfxDstBuffer, invalidatedArea, backgroundOpa);
    DrawForeground(gfxDstBuffer, invalidatedArea, backgroundOpa);
}

void UIArcScrollBar::DrawForeground(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea, uint8_t backgroundOpa)
{
    uint16_t foregoundAngleRange = foregroundProportion_ * (END_ANGLE_IN_DEGREE - START_ANGLE_IN_DEGREE);
    int16_t minAngle = START_ANGLE_IN_DEGREE;
    int16_t maxAngle = END_ANGLE_IN_DEGREE - foregoundAngleRange;
    int16_t startAngle = minAngle + scrollProgress_ * (maxAngle - minAngle);
    int16_t endAngle = startAngle + foregoundAngleRange;

    ArcInfo arcInfo = {0};
    arcInfo.radius = radius_;
    arcInfo.center = center_;
    arcInfo.startAngle = MATH_MAX(startAngle, START_ANGLE_IN_DEGREE);
    arcInfo.endAngle = MATH_MIN(endAngle, END_ANGLE_IN_DEGREE);
    foregroundStyle_->lineCap_ = CapType::CAP_ROUND;
    foregroundStyle_->lineWidth_ = width_;
    BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, *foregroundStyle_, backgroundOpa,
                                          foregroundStyle_->lineCap_);
}

void UIArcScrollBar::DrawBackground(BufferInfo& gfxDstBuffer, const Rect& invalidatedArea, uint8_t backgroundOpa)
{
    ArcInfo arcInfo = {0};
    arcInfo.radius = radius_;
    arcInfo.center = center_;
    arcInfo.startAngle = START_ANGLE_IN_DEGREE;
    arcInfo.endAngle = END_ANGLE_IN_DEGREE;
    backgroundStyle_->lineCap_ = CapType::CAP_ROUND;
    backgroundStyle_->lineWidth_ = width_;
    BaseGfxEngine::GetInstance()->DrawArc(gfxDstBuffer, arcInfo, invalidatedArea, *backgroundStyle_, backgroundOpa,
                                          backgroundStyle_->lineCap_);
}
} // namespace OHOS
