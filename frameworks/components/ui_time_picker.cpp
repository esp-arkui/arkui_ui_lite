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

#include "components/ui_time_picker.h"
#include <cstdio>
#include <ctime>
#include "gfx_utils/graphic_log.h"
#include "securec.h"
#include "themes/theme_manager.h"

namespace OHOS {
UITimePicker::UITimePicker()
    : selectedValue_{0},
      selectedHour_{0},
      selectedMinute_{0},
      selectedSecond_{0},
      secVisible_(false),
      timePickerListener_(nullptr)
{
    Theme* theme = ThemeManager::GetInstance().GetCurrent();
    if (theme != nullptr) {
        style_ = &(theme->GetPickerBackgroundStyle());
    } else {
        style_ = &(StyleDefault::GetPickerBackgroundStyle());
    }
    uint8_t backFontId = style_->font_;
    ColorType backColor = style_->textColor_;

    if (theme != nullptr) {
        style_ = &(theme->GetPickerHighlightStyle());
    } else {
        style_ = &(StyleDefault::GetPickerHighlightStyle());
    }
    uint8_t frontFontId = style_->font_;
    ColorType frontColor = style_->textColor_;

#if ENABLE_FOCUS_MANAGER
    focusable_ = true;
#endif

    Add(&hourPicker_);
    Add(&minutePicker_);
    Add(&secondPicker_);

#if ENABLE_ROTATE_INPUT
    hourPicker_.GetChildrenHead()->SetViewId(HOUR_LIST_NAME);
    minutePicker_.GetChildrenHead()->SetViewId(MIN_LIST_NAME);
    secondPicker_.GetChildrenHead()->SetViewId(SEC_LIST_NAME);
#endif

    hourPicker_.RegisterSelectedListener(this);
    minutePicker_.RegisterSelectedListener(this);
    secondPicker_.RegisterSelectedListener(this);

    hourPicker_.SetValues(TIME_START, HOUR_END);
    minutePicker_.SetValues(TIME_START, MIN_END);
    secondPicker_.SetValues(TIME_START, SEC_END);

    SetTextStyle(backFontId, frontFontId, backColor, frontColor);
}

UITimePicker::~UITimePicker() {}

void UITimePicker::OnPickerStoped(UIPicker& picker)
{
    uint16_t hourSelect = hourPicker_.GetSelected();
    uint16_t minSelect = minutePicker_.GetSelected();
    GetValueByIndex(selectedHour_, BUF_SIZE, hourSelect, TIME_START, HOUR_END);
    GetValueByIndex(selectedMinute_, BUF_SIZE, minSelect, TIME_START, MIN_END);

    if (memset_s(selectedValue_, SELECTED_VALUE_SIZE, 0, SELECTED_VALUE_SIZE) != EOK) {
        return;
    }

    if (secVisible_) {
        uint16_t secSelect = secondPicker_.GetSelected();
        GetValueByIndex(selectedSecond_, BUF_SIZE, secSelect, TIME_START, SEC_END);
        if (sprintf_s(selectedValue_, SELECTED_VALUE_SIZE, "%s:%s:%s",
            selectedHour_, selectedMinute_, selectedSecond_) < 0) {
            return;
        }
    } else {
        if (sprintf_s(selectedValue_, SELECTED_VALUE_SIZE, "%s:%s", selectedHour_, selectedMinute_) < 0) {
            return;
        }
    }

    if (timePickerListener_ != nullptr) {
        timePickerListener_->OnTimePickerStoped(*this);
    }
}

void UITimePicker::GetValueByIndex(char* value, uint8_t len, uint16_t index, int16_t start, int16_t end)
{
    if ((value != nullptr) && (index < end - start + 1)) {
        if (sprintf_s(value, len, "%02d", index) < 0) {
            return;
        }
    }
}

bool UITimePicker::SetSelected(const char* value)
{
    if (value == nullptr) {
        return false;
    }

    if (memset_s(selectedValue_, SELECTED_VALUE_SIZE, 0, SELECTED_VALUE_SIZE) != EOK) {
        return false;
    }

    if (strcpy_s(selectedValue_, SELECTED_VALUE_SIZE, value) != EOK) {
        return false;
    }
    if (secVisible_) {
        if (sscanf_s(value, "%[^:]%*c%[^:]%*c%[^\n]", selectedHour_, BUF_SIZE,
                     selectedMinute_, BUF_SIZE, selectedSecond_, BUF_SIZE) < 3) { // 3: three variables
            return false;
        }
    } else {
        if (sscanf_s(value, "%[^:]%*c%[^\n]", selectedHour_, BUF_SIZE,
                     selectedMinute_, BUF_SIZE) < 2) { // 2: two variables
            return false;
        }
    }
    return RefreshSelected(selectedValue_);
}

bool UITimePicker::RefreshSelected(const char* value)
{
    uint32_t hourSelect;
    uint32_t minSelect;

    if (secVisible_) {
        uint32_t secSelect;
        // 3: three variables
        if (sscanf_s(value, "%d:%d:%d", &hourSelect, &minSelect, &secSelect) < 3) {
            return false;
        }
        secondPicker_.SetSelected(secSelect);
    } else {
        if (sscanf_s(value, "%d:%d", &hourSelect, &minSelect) < 2) { // 2: two variables
            return false;
        }
    }

    hourPicker_.SetSelected(hourSelect);
    minutePicker_.SetSelected(minSelect);
    return true;
}

void UITimePicker::SetItemHeight(int16_t height)
{
    hourPicker_.SetItemHeight(height);
    minutePicker_.SetItemHeight(height);
    secondPicker_.SetItemHeight(height);
}

void UITimePicker::EnableSecond(bool state)
{
    secVisible_ = state;
    RefreshPickerSize();
}

void UITimePicker::RefreshPickerSize()
{
    if (secVisible_) {
        int16_t w = GetWidth() / 3; // 3 : number of picker items
        hourPicker_.SetPosition(0, 0, w, GetHeight());
        minutePicker_.SetPosition(w, 0, w, GetHeight());
        secondPicker_.SetPosition(2 * w, 0, w, GetHeight()); // 2 : offset of picker item
        secondPicker_.SetVisible(true);
    } else {
        int16_t w = GetWidth() / 2; // 2 : number of picker items
        hourPicker_.SetPosition(0, 0, w, GetHeight());
        minutePicker_.SetPosition(w, 0, w, GetHeight());
        secondPicker_.SetVisible(false);
    }
}

void UITimePicker::SetTextStyle(uint8_t backgroundFontId, uint8_t highlightFontId,
    ColorType backgroundColor, ColorType highlightColor)
{
    hourPicker_.SetFontId(backgroundFontId, highlightFontId);
    minutePicker_.SetFontId(backgroundFontId, highlightFontId);
    secondPicker_.SetFontId(backgroundFontId, highlightFontId);

    SetTextColor(backgroundColor, highlightColor);
}

void UITimePicker::SetTextColor(ColorType backgroundColor, ColorType highlightColor)
{
    hourPicker_.SetTextColor(backgroundColor, highlightColor);
    minutePicker_.SetTextColor(backgroundColor, highlightColor);
    secondPicker_.SetTextColor(backgroundColor, highlightColor);
}

void UITimePicker::SetBackgroundFont(const char* name, uint8_t size)
{
    hourPicker_.SetBackgroundFont(name, size);
    minutePicker_.SetBackgroundFont(name, size);
    secondPicker_.SetBackgroundFont(name, size);
}

void UITimePicker::SetHighlightFont(const char* name, uint8_t size)
{
    hourPicker_.SetHighlightFont(name, size);
    minutePicker_.SetHighlightFont(name, size);
    secondPicker_.SetHighlightFont(name, size);
}

void UITimePicker::SetWidth(int16_t width)
{
    UIView::SetWidth(width);
    RefreshPickerSize();
}

void UITimePicker::SetHeight(int16_t height)
{
    UIView::SetHeight(height);
    RefreshPickerSize();
}

void UITimePicker::SetLoopState(const uint8_t pickerType, bool state)
{
    switch (pickerType) {
        case PICKER_HOUR:
            hourPicker_.SetLoopState(state);
            break;
        case PICKER_MIN:
            minutePicker_.SetLoopState(state);
            break;
        case PICKER_SEC:
            secondPicker_.SetLoopState(state);
            break;
        default:
            return;
    }
}
}
