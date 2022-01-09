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
 * @file renderer_base.h
 * @brief Defines 基础渲染器
 * @since 1.0
 * @version 1.0.
 */

#ifndef RENDERER_BASE_INCLUDED
#define RENDERER_BASE_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "render/graphic_render_buffer.h"

namespace OHOS {
    template <class PixelFormat>
    class RendererBase {
    public:
        typedef PixelFormat pixfmt_type;
        typedef typename pixfmt_type::ColorType color_type;
        typedef typename pixfmt_type::RowData RowData;

        RendererBase() :
            pixfmtType(0),
            clipBox(1, 1, 0, 0)
        {
        }
        explicit RendererBase(pixfmt_type& ren) :
            pixfmtType(&ren),
            clipBox(0, 0, ren.Width() - 1, ren.Height() - 1)
        {
        }

        /**
         * @brief 传入pixfmt_type参数
         */
        void Attach(pixfmt_type& ren)
        {
            pixfmtType = &ren;
            clipBox = RectI(0, 0, ren.GetWidth() - 1, ren.GetHeight() - 1);
        }

        /**
         * @brief 获取需要渲染的宽度
         */
        unsigned Width() const
        {
            return pixfmtType->Width();
        }

        /**
         * @brief 获取需要渲染的高度
         */
        unsigned Height() const
        {
            return pixfmtType->Height();
        }

        /**
         * @brief ClipBox 设定剪裁盒子的大小
         * @param x1矩形起点x坐标
         * @param y1矩形起点y坐标
         * @param x2矩形对角x坐标
         * @param y2矩形对角y坐标
         * @return 返回矩形是否创建成功
         */
        bool ClipBox(int x1, int y1, int x2, int y2)
        {
            RectI cb(x1, y1, x2, y2);
            cb.Normalize();
            if (cb.Clip(RectI(0, 0, Width() - 1, Height() - 1))) {
                clipBox = cb;
                return true;
            }
            ClipBoxNaked(1, 1, 0, 0);
            return false;
        }

        /**
         * @brief ResetClipping 根据可见性设定剪裁盒子的大小，可见正常设置，不可见设定为一个像素
         * @param visibility 可见性
         */
        void ResetClipping(bool visibility)
        {
            if (visibility) {
                ClipBoxNaked(0, 0, Width() - 1, Height() - 1);
            } else {
                ClipBoxNaked(1, 1, 0, 0);
            }
        }

        /**
         * @brief ClipBoxNaked 设置盒子的大小
         * @param x1矩形起点x坐标
         * @param y1矩形起点y坐标
         * @param x2矩形对角x坐标
         * @param y2矩形对角y坐标
         */
        void ClipBoxNaked(int x1, int y1, int x2, int y2)
        {
            clipBox.x1 = x1;
            clipBox.y1 = y1;
            clipBox.x2 = x2;
            clipBox.y2 = y2;
        }

        /**
         * @brief inbox 判断点（x,y）是否在矩形内
         * @param x 点的x坐标
         * @param y 点的y坐标
         * @return 返回是否
         */
        bool Inbox(int x, int y) const
        {
            return x >= clipBox.x1 && y >= clipBox.y1 && x <= clipBox.x2 && y <= clipBox.y2;
        }

        /**
         * @brief GetClipBox 获取剪切的盒子
         * @return 返回对应盒子
         */
        const RectI& GetClipBox() const
        {
            return clipBox;
        }
        /**
         * @return 返回盒子的x轴最小值
         */
        int GetXmin() const
        {
            return clipBox.x1;
        }
        /**
         * @return 返回盒子的y轴最小值
         */
        int GetYmin() const
        {
            return clipBox.y1;
        }
        /**
         * @return 返回盒子的x轴最大值
         */
        int GetXmax() const
        {
            return clipBox.x2;
        }
        /**
         * @return 返回盒子的y轴最大值
         */
        int GetYmax() const
        {
            return clipBox.y2;
        }

        /**
         * @brief clear 以颜色c清楚GetWidth()宽度下GetHeight()高度下的矩形内的像素
         * @param c 颜色
         */
        void Clear(const color_type& color)
        {
            unsigned y;
            if (Width()) {
                for (y = 0; y < Height(); y++) {
                    pixfmtType->CopyHline(0, y, Width(), color);
                }
            }
        }

        /**
         * @brief ReplaceColor 替换原本有颜色的区域的颜色为color
         * @param c 颜色
         */
        void ReplaceColor(const color_type& color)
        {
            unsigned y;
            if (Width()) {
                for (y = 0; y < Height(); y++) {
                    pixfmtType->ReplaceHlineColor(0, y, Width(), color);
                }
            }
        }

        /**
         * @brief fill 填充GetWidth()宽度下GetHeight()高度下的矩形颜色c
         * @param c 需要填充的颜色
         */
        void Fill(const color_type& color)
        {
            unsigned y;
            if (Width()) {
                for (y = 0; y < Height(); y++) {
                    pixfmtType->blend_hline(0, y, Width(), color, COVER_MASK);
                }
            }
        }

        /**
         * @brief pixel 返回（x,y）的坐标对应的像素
         * @param x 坐标x
         * @param y 坐标y
         * @return
         */
        color_type Pixel(int x, int y) const
        {
            return Inbox(x, y) ? pixfmtType->pixel(x, y) : color_type::no_color();
        }

        /**
         * @brief BlendHline 渲染y上x轴一定范围内的扫描线
         * @param x1 扫描线左侧坐标
         * @param y 扫描线 y轴坐标
         * @param x2 扫描线右侧坐标
         * @param c 渲染扫描线的颜色
         * @param colors 扫描线对应颜色数组
         */
        void BlendHline(int x1, int y, int x2, const color_type& color, uint8_t cover)
        {
            if (x1 > x2) {
                int t = x2;
                x2 = x1;
                x1 = t;
            }
            if (y > GetYmax() || y < GetYmin() || x1 > GetXmax() || x2 < GetXmin()) {
                return;
            }
            if (x1 < GetXmin()) {
                x1 = GetXmin();
            }

            if (x2 > GetXmax()) {
                x2 = GetXmax();
            }
            pixfmtType->BlendHline(x1, y, x2 - x1 + 1, color, cover);
        }

        /**
         * @brief BlendSolidHspan 渲染y轴一定范围内实线的扫描线
         * @param x 扫描线 x轴开始坐标
         * @param y 扫描线 y轴坐标
         * @param len 扫描线长度
         * @param c 渲染扫描线的颜色
         * @param colors 扫描线对应颜色数组
         */
        void BlendSolidHspan(int x, int y, int len, const color_type& color, const uint8_t* covers)
        {
            if (y > GetYmax() || y < GetYmin()) {
                return;
            }
            if (x < GetXmin()) {
                len -= GetXmin() - x;
                if (len <= 0) {
                    return;
                }
                covers += GetXmin() - x;
                x = GetXmin();
            }
            if (x + len > GetXmax()) {
                len = GetXmax() - x + 1;
                if (len <= 0) {
                    return;
                }
            }
            pixfmtType->BlendSolidHspan(x, y, len, color, covers);
        }

        /**
         * @brief CopyColorHspan 复制一定y轴范围内颜色
         * @param x 扫描线 x轴开始坐标
         * @param y 扫描线 y轴坐标
         * @param len 扫描线长度
         * @param colors 扫描线对应颜色数组
         */
        void CopyColorHspan(int x, int y, int len, const color_type* colors)
        {
            if (y > GetYmax() || y < GetYmin()) {
                return;
            }
            if (x < GetXmin()) {
                int d = GetXmin() - x;
                len -= d;
                if (len <= 0) {
                    return;
                }
                colors += d;
                x = GetXmin();
            }
            if (x + len > GetXmax()) {
                len = GetXmax() - x + 1;
                if (len <= 0) {
                    return;
                }
            }
            pixfmtType->copy_color_hspan(x, y, len, colors);
        }

        /**
         * @brief BlendColorHspan 混合一定高度内颜色
         * @param x 扫描线 x轴开始坐标
         * @param y 扫描线 y轴坐标
         * @param len 扫描线长度
         * @param colors 扫描线对应颜色数组
         * @param covers 扫描线对应覆盖率数组
         * @param cover 覆盖率
         */
        void BlendColorHspan(int x, int y, int len, const color_type* colors, const uint8_t* covers,
                             uint8_t cover = OHOS::COVER_FULL)
        {
            if (y > GetYmax() || y < GetYmin()) {
                return;
            }
            if (x < GetXmin()) {
                int d = GetXmin() - x;
                len -= d;
                if (len <= 0) {
                    return;
                }
                if (covers) {
                    covers += d;
                }
                colors += d;
                x = GetXmin();
            }
            if (x + len > GetXmax()) {
                len = GetXmax() - x + 1;
                if (len <= 0) {
                    return;
                }
            }
            pixfmtType->BlendColorHspan(x, y, len, colors, covers, cover);
        }

        /**
         * @brief ClipRectArea 剪辑矩形区域
         * @param dst
         * @param src
         * @param wsrc
         * @param hsrc
         * @return  返回剪辑的区域
         */
        RectI ClipRectArea(RectI& dst, RectI& src, int wsrc, int hsrc) const
        {
            RectI rc(0, 0, 0, 0);
            RectI cb = GetClipBox();
            ++cb.x2;
            ++cb.y2;

            if (src.x1 < 0) {
                dst.x1 -= src.x1;
                src.x1 = 0;
            }
            if (src.y1 < 0) {
                dst.y1 -= src.y1;
                src.y1 = 0;
            }

            if (src.x2 > wsrc) {
                src.x2 = wsrc;
            }
            if (src.y2 > hsrc) {
                src.y2 = hsrc;
            }
            if (dst.x1 < cb.x1) {
                src.x1 += cb.x1 - dst.x1;
                dst.x1 = cb.x1;
            }
            if (dst.y1 < cb.y1) {
                src.y1 += cb.y1 - dst.y1;
                dst.y1 = cb.y1;
            }

            if (dst.x2 > cb.x2) {
                dst.x2 = cb.x2;
            }
            if (dst.y2 > cb.y2) {
                dst.y2 = cb.y2;
            }

            rc.x2 = dst.x2 - dst.x1;
            rc.y2 = dst.y2 - dst.y1;

            if (rc.x2 > src.x2 - src.x1) {
                rc.x2 = src.x2 - src.x1;
            }

            if (rc.y2 > src.y2 - src.y1) {
                rc.y2 = src.y2 - src.y1;
            }
            return rc;
        }

        /**
         * @brief 从src中取一段到缓冲区进行混合
         * @param src 一段缓冲区
         * @param rectSrcPtr 矩形边界
         * @param dx 需要取出的x长度
         * @param dy 需要取出的y长度
         * @param cover 覆盖率
         */
        template <class SrcPixelFormatRenderer>
        void BlendFrom(const SrcPixelFormatRenderer& src, const RectI* rectSrcPtr = 0, int dx = 0, int dy = 0,
                       uint8_t cover = OHOS::COVER_FULL)
        {
            RectI rsrc(0, 0, src.Width(), src.Height());
            if (rectSrcPtr) {
                rsrc.x1 = rectSrcPtr->x1;
                rsrc.y1 = rectSrcPtr->y1;
                rsrc.x2 = rectSrcPtr->x2 + 1;
                rsrc.y2 = rectSrcPtr->y2 + 1;
            }

            RectI rdst(rsrc.x1 + dx, rsrc.y1 + dy, rsrc.x2 + dx, rsrc.y2 + dy);
            RectI rect = ClipRectArea(rdst, rsrc, src.Width(), src.Height());
            if (rect.x2 > 0) {
                int incy = 1;
                if (rdst.y1 > rsrc.y1) {
                    rsrc.y1 += rect.y2 - 1;
                    rdst.y1 += rect.y2 - 1;
                    incy = -1;
                }
                while (rect.y2 > 0) {
                    typename SrcPixelFormatRenderer::RowData rw = src.Row(rsrc.y1);
                    if (rw.ptr) {
                        int x1src = rsrc.x1;
                        int x1dst = rdst.x1;
                        int len = rect.x2;
                        if (rw.x1 > x1src) {
                            x1dst += rw.x1 - x1src;
                            len -= rw.x1 - x1src;
                            x1src = rw.x1;
                        }
                        if (len > 0) {
                            if (x1src + len - 1 > rw.x2) {
                                len -= x1src + len - rw.x2 - 1;
                            }
                            if (len > 0) {
                                pixfmtType->BlendFrom(src, x1dst, rdst.y1, x1src, rsrc.y1, len, cover);
                            }
                        }
                    }
                    rdst.y1 += incy;
                    rsrc.y1 += incy;
                    --rect.y2;
                }
            }
        }

    private:
        pixfmt_type* pixfmtType;
        RectI clipBox;
    };

} // namespace OHOS
#endif
