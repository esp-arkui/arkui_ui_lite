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
 * @brief Defines Base renderer
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
         * @brief afferent pixfmt_type param.
         */
        void Attach(pixfmt_type& ren)
        {
            pixfmtType = &ren;
            clipBox = RectI(0, 0, ren.GetWidth() - 1, ren.GetHeight() - 1);
        }

        /**
         * @brief Gets the width to render.
         */
        unsigned Width() const
        {
            return pixfmtType->Width();
        }

        /**
         * @brief Gets the height to render.
         */
        unsigned Height() const
        {
            return pixfmtType->Height();
        }

        /**
         * @brief ClipBox Sets the size of the clipping box.
         * @param x1 rectangular starting point x coordinate.
         * @param y1 rectangular starting point y coordinate.
         * @param x2 rectangular diagonal x coordinate.
         * @param y2 rectangular diagonal y coordinate.
         * @return Returns whether the rectangle was created successfully.
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
         * @brief ResetClipping Set the size of the clipping box according to the visibility,
         *  The visibility is set to normal, and the invisibility is set to one pixel.
         * @param visibility Visible.
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
         * @brief ClipBoxNaked Sets the size of the box.
         * @param x1 rectangular starting point x coordinate.
         * @param y1 rectangular starting point y coordinate.
         * @param x2 rectangular starting point x coordinate.
         * @param y2 rectangular starting point y coordinate.
         */
        void ClipBoxNaked(int x1, int y1, int x2, int y2)
        {
            clipBox.x1 = x1;
            clipBox.y1 = y1;
            clipBox.x2 = x2;
            clipBox.y2 = y2;
        }

        /**
         * @brief inbox determines whether the point (x, y) is within the rectangle.
         * @param x coordinate of x point.
         * @param y coordinate of y point.
         * @return Return whether.
         */
        bool Inbox(int x, int y) const
        {
            return x >= clipBox.x1 && y >= clipBox.y1 && x <= clipBox.x2 && y <= clipBox.y2;
        }

        /**
         * @brief GetClipBox Get cut box.
         * @return Return to the corresponding box.
         */
        const RectI& GetClipBox() const
        {
            return clipBox;
        }
        /**
         * @return Returns the minimum value of the x-axis of the box.
         */
        int GetXmin() const
        {
            return clipBox.x1;
        }
        /**
         * @return Returns the minimum value of the y-axis of the box.
         */
        int GetYmin() const
        {
            return clipBox.y1;
        }
        /**
         * @return Returns the maximum value of the x-axis of the box.
         */
        int GetXmax() const
        {
            return clipBox.x2;
        }
        /**
         * @return Returns the maximum value of the Y-axis of the box.
         */
        int GetYmax() const
        {
            return clipBox.y2;
        }

        /**
         * @brief Clear clear the pixels in the rectangle under getwidth() width and getheight() height with color.
         * @param color colour.
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
         * @brief ReplaceColor Replace the color of the original colored area with color.
         * @param color colour.
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
         * @brief fill fills the rectangle color under getwidth() width and getheight() height.
         * @param color Color to fill.
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
         * @brief Pixel returns the pixel corresponding to the coordinate of (x, y).
         * @param x Coordinate x
         * @param y Coordinate y
         * @return
         */
        color_type Pixel(int x, int y) const
        {
            return Inbox(x, y) ? pixfmtType->pixel(x, y) : color_type::no_color();
        }

        /**
         * @brief BlendHline Render scanlines within a certain range of the X axis on the Y axis.
         * @param x1 Scan line left coordinate.
         * @param y  Scan line Y-axis coordinates.
         * @param x2 Scan line right coordinate.
         * @param c The color of the rendered scanline.
         * @param colors Scan line corresponding color array.
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
         * @brief BlendSolidHspan Renders a scan line of a solid line within a certain range of the Y axis.
         * @param x scan line X-axis start coordinate.
         * @param y scan line Y-axis start coordinate.
         * @param len Scan line length.
         * @param c The color of the rendered scanline.
         * @param colors Scan line corresponding color array.
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
         * @brief CopyColorHspan Copy colors within a certain Y-axis range
         * @param x Scan line X-axis start coordinate
         * @param y Scan line Y-axis start coordinate
         * @param len Scan line length.
         * @param colors Scan line corresponding color array.
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
            pixfmtType->CopyColorHspan(x, y, len, colors);
        }

        /**
         * @brief BlendColorHspan Mix colors within a certain height.
         * @param x Scan line X-axis start coordinate
         * @param y Scan line Y-axis start coordinate
         * @param len Scan line length.
         * @param colors Scan line corresponding color array.
         * @param covers Scan line corresponding coverage array.
         * @param cover Coverage
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
         * @brief ClipRectArea Clip rectangular area
         * @param dst
         * @param src
         * @param wsrc
         * @param hsrc
         * @return Returns the area of the clip
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
         * @brief Take a segment from SRC to buffer for mixing.
         * @param src one segment buffer.
         * @param rectSrcPtr Rectangular boundary.
         * @param dx X length to be taken out
         * @param dy Y length to be taken out
         * @param cover Coverage
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
