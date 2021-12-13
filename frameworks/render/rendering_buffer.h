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
* @version 1.0
*/

#ifndef RENDERING_BUFFER_INCLUDED
#define RENDERING_BUFFER_INCLUDED

#include <cstring>

#include "gfx_utils/graphics/graphic_geometry/agg_array.h"

namespace OHOS {
    /**
     * 行访问器
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
         * @brief RowAccessor 行访问器
         * @param renBuf 缓冲区
         * @param areaWidth 缓冲区内像素区域的宽度
         * @param areaHeight 缓冲区内像素区域的高度
         * @param stride 缓冲区的步幅
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
         * @brief attach 传入参数
         * @param renBuf 缓冲区
         * @param areaWidth 缓冲区内像素区域的宽度
         * @param areaHeight 缓冲区内像素区域的高度
         * @param stride 缓冲区的步幅
         */
        void Attach(T* renBuf, unsigned areaWidth, unsigned areaHeight, int stride)
        {
            renBuf = start_ = renBuf;
            width_ = areaWidth;
            height_ = areaHeight;
            bufStride_ = stride;
            if (stride < 0) {
                start_ = renBuf - (GRAPHIC_GEOMETRY_INT64)(areaHeight - 1) * stride;
            }
        }
        /**
         * @brief GetBuf 获取渲染缓冲区的指针
         */
        GRAPHIC_GEOMETRY_INLINE T* GetBuf()
        {
            return renBuf_;
        }
        /**
         * @brief GetBuf 获取渲染缓冲区的指针
         */
        GRAPHIC_GEOMETRY_INLINE const T* GetBuf() const
        {
            return renBuf_;
        }
        /**
         * @brief GetBuf 获取区域宽度
         */
        GRAPHIC_GEOMETRY_INLINE unsigned GetWidth() const
        {
            return width_;
        }
        /**
         * @brief GetBuf 获取区域高度
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
         * @brief row_ptr 获取行首地址
         */

        GRAPHIC_GEOMETRY_INLINE T* RowPtr(int, int y, unsigned)
        {
            return start_ + y * (GRAPHIC_GEOMETRY_INT64)bufStride_;
        }
        /**
         * @brief row_ptr 返回指向第y行起点的指针
         */

        GRAPHIC_GEOMETRY_INLINE T* RowPtr(int y)
        {
            return start_ + y * (GRAPHIC_GEOMETRY_INT64)bufStride_;
        }
        /**
         * @brief row_ptr 返回指向第y行起点的指针
         */
        GRAPHIC_GEOMETRY_INLINE const T* RowPtr(int y) const
        {
            return start_ + y * (GRAPHIC_GEOMETRY_INT64)bufStride_;
        }
        /**
         * @brief row 获取行数据
         */

        GRAPHIC_GEOMETRY_INLINE rowData Row(int y) const
        {
            return rowData(0, width_ - 1, RowPtr(y));
        }

        /**
         *从另一rendering_buffer中复制数据
         */
        template <class RenBuf>
        void CopyFrom(const RenBuf& renBuf)
        {
            unsigned h = GetHeight();
            if (renBuf.height() < h) {
                h = renBuf.height();
            }
            unsigned l = GetStrideAbs();
            if (renBuf.stride_abs() < l) {
                l = renBuf.stride_abs();
            }
            l *= sizeof(T);
            unsigned y;
            unsigned w = GetWidth();
            for (y = 0; y < h; y++) {
                std::memcpy(RowPtr(0, y, w), renBuf.row_ptr(y), l);
            }
        }

        /**
         * @brief Clear 以value值填充整个内存块
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
        T* renBuf_;       // 指向渲染缓冲区的指针
        T* start_;        // 根据步幅指向第一个像素
        unsigned width_;  // 区域宽度
        unsigned height_; // 区域高度
        int bufStride_;   // 每行字节数。
    };

#ifdef RENDERING_BUFFER
    using RenderingBuffer = RENDERING_BUFFER;
#else
    using RenderingBuffer = RowAccessor<int8u>;
#endif

} // namespace OHOS

#endif
