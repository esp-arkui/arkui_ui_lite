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
* @file rendering_buffer.h
* @brief Defines 渲染器缓冲
* @since 1.0
* @version 1.0.
*/

#ifndef RENDERING_BUFFER_INCLUDED
#define RENDERING_BUFFER_INCLUDED

#include <cstring>

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"

namespace OHOS {
    /**
     * Row accessor.
     */
    template <class T>
    class RowAccessor {
    public:
        using rowData = ConstRowInfo<T>;
        RowAccessor() :
            renBuf_(0),
            start_(0),
            width_(0),
            height_(0),
            bufStride_(0)
        {
        }
        /**
         * @brief RowAccessor Row accessor.
         * @param renBuf buffer.
         * @param areaWidth The width of the pixel area in the buffer.
         * @param areaHeight The height of the pixel area in the buffer.
         * @param stride Buffer stride.
         */
        RowAccessor(T* renBuf, unsigned areaWidth, unsigned areaHeight, int stride) :
            renBuf_(0),
            start_(0),
            width_(0),
            height_(0),
            bufStride_(0)
        {
            Attach(renBuf, areaWidth, areaHeight, stride);
        }

        /**
         * @brief attach Incoming parameters.
         * @param renBuf buffer
         * @param areaWidth The width of the pixel area in the buffer.
         * @param areaHeightThe height of the pixel area in the buffer.
         * @param stride Buffer stride.
         */
        void Attach(T* renBuf, unsigned areaWidth, unsigned areaHeight, int stride)
        {
            renBuf_ = start_ = renBuf;
            width_ = areaWidth;
            height_ = areaHeight;
            bufStride_ = stride;
            if (stride < 0) {
                start_ = renBuf - (int64_t)(areaHeight - 1) * stride;
            }
        }
        /**
         * @brief GetBuf Gets a pointer to the render buffer.
         */
        GRAPHIC_GEOMETRY_INLINE T* GetBuf()
        {
            return renBuf_;
        }
        /**
         * @brief GetBuf Gets a pointer to the render buffer.
         */
        GRAPHIC_GEOMETRY_INLINE const T* GetBuf() const
        {
            return renBuf_;
        }
        /**
         * @brief GetBuf Get area width.
         */
        GRAPHIC_GEOMETRY_INLINE unsigned GetWidth() const
        {
            return width_;
        }
        /**
         * @brief GetBuf GetBuf Get area height.
         */
        GRAPHIC_GEOMETRY_INLINE unsigned GetHeight() const
        {
            return height_;
        }
        GRAPHIC_GEOMETRY_INLINE int GetStride() const
        {
            return bufStride_;
        }
        GRAPHIC_GEOMETRY_INLINE unsigned GetStrideAbs() const
        {
            return (bufStride_ < 0) ? unsigned(-bufStride_) : unsigned(bufStride_);
        }

        /**
         * @brief row_ptr Get line start address.
         */

        GRAPHIC_GEOMETRY_INLINE T* RowPtr(int, int y, unsigned = 1)
        {
            return start_ + y * (int64_t)bufStride_;
        }
        /**
         * @brief row_ptr Returns a pointer to the beginning of line y.
         */

        GRAPHIC_GEOMETRY_INLINE T* RowPtr(int y)
        {
            return start_ + y * (int64_t)bufStride_;
        }
        /**
         * @brief row_ptr Returns a pointer to the beginning of line y。
         */
        GRAPHIC_GEOMETRY_INLINE const T* RowPtr(int y) const
        {
            return start_ + y * (int64_t)bufStride_;
        }
        /**
         * @brief row Get row data
         */

        GRAPHIC_GEOMETRY_INLINE rowData Row(int y) const
        {
            return rowData(0, width_ - 1, RowPtr(y));
        }

        /**
         * @brief Clear Fill the entire memory block with the value
         * @param value
         */
        void Clear(T value)
        {
            unsigned y;
            unsigned w = GetWidth();
            unsigned stride = GetStrideAbs();
            for (y = 0; y < GetHeight(); y++) {
                T* p = RowPtr(0, y, w);
                unsigned x;
                for (x = 0; x < stride; x++) {
                    *p++ = value;
                }
            }
        }

    private:
        T* renBuf_;       // Pointer to render buffer.
        T* start_;        // Point to the first pixel according to the stride.
        unsigned width_;  // Area width.
        unsigned height_; // Area height.
        int bufStride_;   // Number of bytes per line.
    };
    using RenderingBuffer = RowAccessor<uint8_t>;
} // namespace OHOS

#endif
