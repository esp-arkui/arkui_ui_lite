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
 * limitations under the License..
 */
#ifndef AGG_PIXFMT_TRANSPOSER_INCLUDED
#define AGG_PIXFMT_TRANSPOSER_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
namespace OHOS {
    template <class PixFmt>
    class PixfmtTransposer : public HeapBase {
    public:
        using PixfmtType = PixFmt;
        using ColorType = typename PixfmtType::ColorType;
        using RowData = typename PixfmtType::RowData;
        using ValueType = typename ColorType::ValueType;
        using CalcType = typename ColorType::CalcType;

        PixfmtTransposer() :
            pixf_(0)
        {}

        /**
         * @brief 构造PixfmtTransposer实例，用于初始化pixf_成员.
         * @Param 屏幕缓存区
         * @since 1.0
         * @version 1.0
         */
        explicit PixfmtTransposer(PixfmtType& pixf) :
            pixf_(&pixf)
        {}

        /**
         * @brief 附加屏幕
         * @Param 屏幕缓存区
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixfmtType& pixf)
        {
            pixf_ = &pixf;
        }

        /**
         * @brief 获取屏幕宽
         * @return 宽度
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE unsigned Width() const
        {
            return pixf_->Height();
        }

        /**
         * @brief 获取屏幕高
         * @return 高度
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE unsigned Height() const
        {
            return pixf_->Width();
        }

        /**
         * @brief 获取指定坐标颜色
         * @return 颜色
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE ColorType Pixel(int x, int y) const
        {
            return pixf_->Pixel(y, x);
        }

        /**
         * @brief 设置指定坐标颜色
         *
         * @since 1.0
         * @version 1.0.
         */
        GRAPHIC_GEOMETRY_INLINE void CopyPixel(int x, int y, const ColorType& c)
        {
            pixf_->CopyPixel(y, x, c);
        }

        /**
         * @brief 从指定x,y起混合颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendPixel(int x, int y,
                                                const ColorType& color,
                                                uint8_t cover)
        {
            pixf_->BlendPixel(y, x, color, cover);
        }

        /**
         * @brief 从指定x,y起横向拷贝len长度的颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyHline(int x, int y,
                                               unsigned len,
                                               const ColorType& color)
        {
            pixf_->copy_vline(y, x, len, color);
        }

        /**
         * @brief 从指定x,y起纵向拷贝len长度的颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyVline(int x, int y,
                                               unsigned len,
                                               const ColorType& color)
        {
            pixf_->CopyHline(y, x, len, color);
        }

        /**
         * @brief 从指定x,y起横向混合len长度的颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendHline(int x, int y,
                                                unsigned len,
                                                const ColorType& color, uint8_t cover)
        {
            pixf_->BlendVline(y, x, len, color, cover);
        }

        /**
         * @brief 从指定x,y起纵向混合len长度的颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendVline(int x, int y,
                                                unsigned len,
                                                const ColorType& color,
                                                uint8_t cover)
        {
            pixf_->BlendHline(y, x, len, color, cover);
        }

        /**
         * @brief 从指定x,y起横向混合len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendSolidHspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType& c,
                                                     const uint8_t* covers)
        {
            pixf_->BlendSolidVspan(y, x, len, c, covers);
        }

        /**
         * @brief 从指定x,y起纵向混合len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendSolidVspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType& c,
                                                     const uint8_t* covers)
        {
            pixf_->BlendSolidHspan(y, x, len, c, covers);
        }

        /**
         * @brief 从指定x,y起横向拷贝len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyColorHspan(int x, int y,
                                                    unsigned len,
                                                    const ColorType* colors)
        {
            pixf_->CopyColorVspan(y, x, len, colors);
        }

        /**
         * @brief 从指定x,y起纵向拷贝len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void CopyColorVspan(int x, int y,
                                                    unsigned len,
                                                    const ColorType* colors)
        {
            pixf_->CopyColorHspan(y, x, len, colors);
        }

        /**
         * @brief 从指定x,y起横向混合len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendColorHspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType* colors,
                                                     const uint8_t* covers,
                                                     uint8_t cover)
        {
            pixf_->BlendColorVspan(y, x, len, colors, covers, cover);
        }

        /**
         * @brief 从指定x,y起纵向混合len长度的一系列颜色
         *
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void BlendColorVspan(int x, int y,
                                                     unsigned len,
                                                     const ColorType* colors,
                                                     const uint8_t* covers,
                                                     uint8_t cover)
        {
            pixf_->BlendColorHspan(y, x, len, colors, covers, cover);
        }

    private:
        PixfmtType* pixf_;
    };
} // namespace OHOS

#endif
