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

namespace OHOS
{
    /**
     * 行访问器
     */
    template <class T>
    class row_accessor
    {
    public:
        typedef const_row_info<T> row_data;
        row_accessor() :
            buf(0),
            start(0),
            width(0),
            height(0),
            bufStride(0)
        {
        }
        row_accessor(T* buf, unsigned areaWidth, unsigned areaHeight, int stride) :
            buf(0),
            start(0),
            width(0),
            height(0),
            bufStride(0)
        {
            attach(buf, areaWidth, areaHeight, stride);
        }

        /**
         * @brief attach 传入参数
         */
        void attach(T* buf, unsigned areaWidth, unsigned areaHeight, int stride)
        {
            buf = start = buf;
            width = areaWidth;
            height = areaHeight;
            bufStride = stride;
            if (stride < 0)
            {
                start = buf - (AGG_INT64)(areaHeight - 1) * stride;
            }
        }
        /**
         * @brief GetBuf 获取渲染缓冲区的指针
         */
        AGG_INLINE T* GetBuf()
        {
            return buf;
        }
        /**
         * @brief GetBuf 获取渲染缓冲区的指针
         */
        AGG_INLINE const T* GetBuf() const
        {
            return buf;
        }
        /**
         * @brief GetBuf 获取区域宽度
         */
        AGG_INLINE unsigned GetWidth() const
        {
            return width;
        }
        /**
         * @brief GetBuf 获取区域高度
         */
        AGG_INLINE unsigned GetHeight() const
        {
            return height;
        }
        AGG_INLINE int GetStride() const
        {
            return bufStride;
        }
        AGG_INLINE unsigned GetStrideAbs() const
        {
            return (bufStride < 0) ? unsigned(-bufStride) : unsigned(bufStride);
        }

        /**
         * @brief row_ptr 获取行首地址
         */
        AGG_INLINE T* row_ptr(int, int y, unsigned)
        {
            return start + y * (AGG_INT64)bufStride;
        }
        /**
         * @brief row_ptr 返回指向第y行起点的指针
         */
        AGG_INLINE T* row_ptr(int y)
        {
            return start + y * (AGG_INT64)bufStride;
        }
        /**
         * @brief row_ptr 返回指向第y行起点的指针
         */
        AGG_INLINE const T* row_ptr(int y) const
        {
            return start + y * (AGG_INT64)bufStride;
        }
        /**
         * @brief row 获取行数据
         */
        AGG_INLINE row_data row(int y) const
        {
            return row_data(0, width - 1, row_ptr(y));
        }

        /**
         *从另一rendering_buffer中复制数据
         */
        template <class RenBuf>
        void copy_from(const RenBuf& src)
        {
            unsigned h = GetHeight();
            if (src.height() < h)
                h = src.height();

            unsigned l = GetStrideAbs();
            if (src.stride_abs() < l)
                l = src.stride_abs();

            l *= sizeof(T);

            unsigned y;
            unsigned w = GetWidth();
            for (y = 0; y < h; y++)
            {
                std::memcpy(row_ptr(0, y, w), src.row_ptr(y), l);
            }
        }

        /**
         * @brief clear 以value值填充整个内存块
         * @param value
         */
        void clear(T value)
        {
            unsigned y;
            unsigned w = GetWidth();
            unsigned stride = GetStrideAbs();
            for (y = 0; y < GetHeight(); y++)
            {
                T* p = row_ptr(0, y, w);
                unsigned x;
                for (x = 0; x < stride; x++)
                {
                    *p++ = value;
                }
            }
        }

    private:
        T* buf;          // 指向渲染缓冲区的指针
        T* start;        // 根据步幅指向第一个像素
        unsigned width;  // 区域宽度
        unsigned height; // 区域高度
        int bufStride;   // 每行字节数。
    };

#ifdef RENDERING_BUFFER
    typedef RENDERING_BUFFER rendering_buffer;
#else
    typedef row_accessor<int8u> rendering_buffer;
#endif

} // namespace OHOS

#endif
