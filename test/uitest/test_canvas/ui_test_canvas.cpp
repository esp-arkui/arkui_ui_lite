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

#include "ui_test_canvas.h"
#include "common/screen.h"
#include "components/ui_label.h"
#include "test_resource_config.h"

#include <draw/clip_utils.h>
#include <draw/draw_utils.h>

namespace OHOS {
void UITestCanvas::SetUp()
{
    if (container_ == nullptr) {
        container_ = new UIScrollView();
        container_->Resize(Screen::GetInstance().GetWidth(), Screen::GetInstance().GetHeight() - BACK_BUTTON_HEIGHT);
        container_->SetHorizontalScrollState(false);
        container_->SetThrowDrag(true);
    }
    positionY_ = 0;
}

void UITestCanvas::TearDown()
{
    DeleteChildren(container_);
    container_ = nullptr;
}

const UIView* UITestCanvas::GetTestView()
{
//    UIKitCanvasTestGlobalAlpha001();
//    UIKitCanvasTestDrawDashLine001();
//    UIKitCanvasTestLineJoin001();
//    UIKitCanvasTestDrawLine001();
//    UIKitCanvasTestDrawLine002();
//    UIKitCanvasTestDrawCurve001();
//    UIKitCanvasTestDrawCurve002();

    UIKitCanvasTestComposite001();
    UIKitCanvasTestGlobalAlpha001();
    UIKitCanvasTestDrawDashLine001();
    UIKitCanvasTestLineJoin001();
    UIKitCanvasTestDrawLine001();
    UIKitCanvasTestDrawLine002();
    UIKitCanvasTestDrawCurve001();
    UIKitCanvasTestDrawCurve002();
    UIKitCanvasTestDrawShadow001();
//    UIKitCanvasTestDrawRect001();

//    UIKitCanvasTestLinearGradient();//线性渐变水平
//    UIKitCanvasTestLinearGradient2();//线性渐变倾斜
//    UIKitCanvasTestRadialGradient();//放射渐变
    UIKitCanvasTestcreatePattern5();
    UIKitCanvasTestcreatePattern1();
    UIKitCanvasTestcreatePattern6();
    UIKitCanvasTestcreatePattern2();
    UIKitCanvasTestcreatePattern7();
    UIKitCanvasTestcreatePattern3();
    UIKitCanvasTestcreatePattern8();
    UIKitCanvasTestcreatePattern4();
//    UIKitCanvasTestDrawRect002();
//    UIKitCanvasTestDrawRect003();
//    UIKitCanvasTestDrawRect004();
//    UIKitCanvasTestClearRect001();
//    UIKitCanvasTestDrawCircle001();
//    UIKitCanvasTestDrawCircle002();
//    UIKitCanvasTestDrawCircle003();
//    UIKitCanvasTestDrawArc001();
//    UIKitCanvasTestDrawImage001();
//    UIKitCanvasTestDrawLabel001();
//    UIKitCanvasTestDrawSector001();
//    UIKitCanvasTestClear001();
//    UIKitCanvasTestDrawPath001();
//    UIKitCanvasTestDrawPath002();
//    UIKitCanvasTestDrawPath003();
//    UIKitCanvasTestDrawPath004();
//    UIKitCanvasTestDrawPath005();
//    UIKitCanvasTestDrawPath006();
//    UIKitCanvasTestDrawPath007();
//    UIKitCanvasTestDrawPath008();
//    UIKitCanvasTestDrawPath009();
//    UIKitCanvasTestDrawPath010();
//    UIKitCanvasTestDrawPath011();
//    UIKitCanvasTestDrawPath012();
//    UIKitCanvasTestDrawPath013();
//    UIKitCanvasTestDrawPath014();
//    UIKitCanvasTestDrawPath015();
//    UIKitCanvasTestDrawPath016();
//    UIKitCanvasTestDrawPath017();
//    UIKitCanvasTestDrawPath018();
//    UIKitCanvasTestDrawPath019();
//    UIKitCanvasTestDrawPath020();
//    UIKitCanvasTestDrawPath021();
//    UIKitCanvasTestDrawPath022();
//    UIKitCanvasTestDrawPath023();
//    UIKitCanvasTestDrawPath024();
//    UIKitCanvasTestDrawPath025();
//    UIKitCanvasTestDrawPath026();
//    UIKitCanvasTestDrawPath027();
//    UIKitCanvasTestDrawPath028();
//    UIKitCanvasTestDrawPath029();
//    UIKitCanvasTestDrawPath030();
//    UIKitCanvasTestDrawPath031();
//    UIKitCanvasTestDrawPath032();
//    UIKitCanvasTestDrawPath033();
//    UIKitCanvasTestDrawPath034();



    return container_;
}

void UITestCanvas::UIKitCanvasTestComposite001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("Composite 的处理模式");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    //canvas->GlobalAlpha(0.8f,paint);
    paint.SetStyle(Paint::PaintStyle::STROKE_FILL_STYLE);
    paint.SetStrokeColor(Color::Blue());
    paint.SetFillColor(Color::Red());
    paint.globalCompositeOperation(BaseGfxExtendEngine::BlendMode::BlendSrcOver);
    canvas->DrawCircle({ 50, 50 }, 40, paint);
    paint.globalCompositeOperation(BaseGfxExtendEngine::BlendMode::BlendSrcOut);
    //paint.globalCompositeOperation(BaseGfxExtendEngine::BlendMode::BlendOverlay);
    paint.SetFillColor(Color::GetColorFromRGBA(0,255,0,128));
    canvas->DrawCircle({ 70, 70 }, 40, paint);

}

void UITestCanvas::UIKitCanvasTestGlobalAlpha001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("canvas globalAlpha全局ALPHA透明");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetFillColor(Color::Green());
    canvas->GlobalAlpha(0.8f,paint);
    canvas->DrawRect({ 50, 10 }, 50, 50, paint);
    paint.SetStyle(Paint::PaintStyle::STROKE_FILL_STYLE);
    paint.SetStrokeColor(Color::Blue());
    paint.SetFillColor(Color::Red());
    canvas->DrawCircle({ 50, 50 }, 40, paint);
}

void UITestCanvas::UIKitCanvasTestDrawDashLine001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("strokeRect() 绘制矩形（无填充）支持颜色和alpha");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Green());
    paint.SetOpacity(200);//0 是完全透明 255 不透明

    canvas->LineWidth(2,paint);
    canvas->LineDashOffset(10,paint);
    float ds[] = {5, 3.1f, 1.2f, 5.5f};
    canvas->SetLineDash(ds, 4,paint);
    canvas->DrawCircle({ 50, 50 }, 40, paint);
    unsigned nDashes=0;
    float* fLineDashes = canvas->GetLineDash(paint,nDashes);
    float ds2[] = {3, 5.1f, 2.2f, 4.5f};
    canvas->SetLineDash(ds2, 4,paint);
    paint.SetOpacity(255);//0 是完全透明 255 不透明
    canvas->StrokeRect({ 100, 100 }, 40,100, paint);

    canvas->SetLineDash(nullptr,0,paint);
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Blue());
    canvas->StrokeRect({ 20, 20 }, 150, 100, paint);
}

void UITestCanvas::CreateTitleLabel(const char* title)
{
    UILabel* titleLabel = new UILabel();
    titleLabel->SetPosition(TEXT_DISTANCE_TO_LEFT_SIDE, positionY_, Screen::GetInstance().GetWidth(), TITLE_HEIGHT);
    titleLabel->SetFont(DEFAULT_VECTOR_FONT_FILENAME, FONT_DEFAULT_SIZE);
    titleLabel->SetText(title);
    container_->Add(titleLabel);
    positionY_ += TITLE_HEIGHT + 8; // 8: gap
}

UICanvas* UITestCanvas::CreateCanvas()
{
    UICanvas* canvas = new UICanvas();
    canvas->SetHeight(CANVAS_HEIGHT);
    canvas->SetWidth(CANVAS_WIDTH);
    canvas->SetPosition(VIEW_DISTANCE_TO_LEFT_SIDE, positionY_);
    canvas->SetStyle(STYLE_BACKGROUND_COLOR, Color::Gray().full);
    container_->Add(canvas);
    positionY_ += CANVAS_HEIGHT + GAP;
    return canvas;
}

void UITestCanvas::UIKitCanvasTestDrawLine001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制直线");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    // {0, 10}: Start point coordinates x, y; {50, 10}: end point coordinates x, y
    canvas->DrawLine({0, 10}, {50, 10}, paint);
}

void UITestCanvas::UIKitCanvasTestDrawLine002()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制直线，包括linecap,线条宽度");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    // {0, 10}: Start point coordinates x, y; {50, 10}: end point coordinates x, y

    canvas->LineWidth(15,paint);
    paint.SetStrokeColor(Color::Red());
    canvas->SetStartPosition({ 50, 10 }); // {50, 10}: Start point coordinates x, y;
    canvas->DrawLine({ 100, 50 }, paint); // {100, 50}: end point coordinates x, y
    paint.SetStrokeColor(Color::Green());
    canvas->LineWidth(5,paint);
    paint.SetLineCap(BaseGfxExtendEngine::CapRound);
    canvas->SetStartPosition({ 150, 80 });
    canvas->DrawLine({ 50, 100 }, paint);
}

void UITestCanvas::UIKitCanvasTestLineJoin001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("LineCap lineJoin miterlimit设置或返回两条线相交时，所创建的拐角类型度");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    // {0, 10}: Start point coordinates x, y; {50, 10}: end point coordinates x, y
    canvas->LineWidth(10,paint);
    paint.SetLineCap(BaseGfxExtendEngine::CapRound);
    paint.SetLineJoin(BaseGfxExtendEngine::JoinBevel);
    paint.SetStrokeColor(Color::Blue());
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    canvas->BeginPath();
    canvas->MoveTo({ 50, 100 });
    canvas->LineTo({ 150, 80 });
    canvas->LineTo({ 75, 40 });
    canvas->ClosePath();
    canvas->DrawPath(paint);

    canvas->LineWidth(8,paint);
    paint.SetLineCap(BaseGfxExtendEngine::CapSquare);
    paint.SetLineJoin(BaseGfxExtendEngine::JoinMiter);
    paint.SetMiterLimit(5);
    paint.SetStrokeColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({ 5, 30 });
    canvas->LineTo({ 40, 180 });
    canvas->LineTo({ 105, 40 });
    canvas->LineTo({ 15, 100 });
    canvas->ClosePath();
    canvas->DrawPath(paint);

}
void UITestCanvas::UIKitCanvasTestDrawCurve001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制曲线");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStrokeColor(Color::Red());
    canvas->DrawCurve({ 100, 50 }, { 150, 50 }, { 150, 50 }, { 150, 100 }, paint);
}
void UITestCanvas::UIKitCanvasTestDrawShadow001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制矩形和阴影");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
   // paint.SetStrokeColor(Color::Red());
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetFillColor(Color::Green());
    paint.SetShadowColor(Color::Black());
    paint.SetStrokeWidth(1);
    paint.SetShadowOffsetX(10);
    paint.SetShadowOffsetY(10);
    paint.SetShadowBlurRadius(5);
    canvas->BeginPath();
    canvas->MoveTo({60,60});
    canvas->LineTo({60,110});
    canvas->LineTo({160,110});
    canvas->LineTo({160,60});
    canvas->ClosePath();
    canvas->DrawPath(paint);
    canvas->FillPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawCurve002()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制曲线");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStrokeColor(Color::Red());
    canvas->DrawCurve({ 100, 50 }, { 150, 50 }, { 150, 100 }, paint);
}

void UITestCanvas::UIKitCanvasTestDrawRect001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("矩形填充");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetFillColor(Color::Yellow());
    paint.SetStrokeWidth(30); // 30: line width
    // {100, 10}: left corner coordinates point, 50: width, 50: rectangle style
    canvas->DrawRect({ 100, 50 }, 50, 100, paint);
}

void UITestCanvas::UIKitCanvasTestDrawRect002()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("矩形描边");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Blue());
    // {200, 10}: left corner coordinates point, 50: width, 50: rectangle style
    canvas->DrawRect({ 100, 50 }, 50, 100, paint);
}

void UITestCanvas::UIKitCanvasTestDrawRect003()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("矩形填充 + 描边");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_FILL_STYLE);
    paint.SetFillColor(Color::Yellow());
    paint.SetStrokeColor(Color::Blue());
    // {300, 10}: left corner coordinates point, 50: width, 50: rectangle style

    canvas->DrawRect({ 300, 10 }, 50, 50, paint);
}

void UITestCanvas::UIKitCanvasTestDrawRect004()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("strokeRect() 绘制矩形（无填充）支持颜色和alpha");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Green());
    paint.SetOpacity(255);//0 是完全透明 255 不透明
    paint.SetStrokeWidth(2);
    canvas->StrokeRect({ 20, 20 }, 150, 100, paint);

}

void UITestCanvas::UIKitCanvasTestClearRect001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("clearRect() 在给定的矩形内清除指定的像素");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetFillColor(Color::Red());
    // {300, 10}: left corner coordinates point, 50: width, 50: rectangle style
    canvas->DrawRect({ 5, 10 }, 250, 250, paint);
    canvas->ClearRect({10,20},150,150,paint);
}

void UITestCanvas::UIKitCanvasTestDrawCircle001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("圆形填充");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetStrokeColor(Color::Yellow());
    paint.SetFillColor(Color::Yellow());
    paint.SetStrokeWidth(10); // 10: line width
    paint.SetOpacity(127);    // 127: opacity
    // {100, 100}: circle center coordinates, 30: circle radius
    canvas->DrawCircle({ 100, 100 }, 30, paint);
}

void UITestCanvas::UIKitCanvasTestDrawCircle002()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("圆形描边");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Blue());
    paint.SetStrokeWidth(10); // 10: line width
    paint.SetOpacity(127);    // 127: opacity
    // {200, 100}: circle center coordinates, 30: circle radius
    canvas->DrawCircle({ 200, 100 }, 30, paint);
}

void UITestCanvas::UIKitCanvasTestDrawCircle003()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("圆形填充 + 描边");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_FILL_STYLE);
    paint.SetFillColor(Color::Yellow());
    paint.SetStrokeColor(Color::Blue());
    paint.SetStrokeWidth(10); // 10: line width
    paint.SetOpacity(127);    // 127: opacity
    // {300, 100}: circle center coordinates, 30: circle radius
    canvas->DrawCircle({ 300, 100 }, 30, paint);
}

void UITestCanvas::UIKitCanvasTestDrawArc001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制弧线");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_STYLE);
    paint.SetStrokeColor(Color::Red());
    canvas->LineWidth(10,paint);
    // {100, 150}: arc's center coordinates, 50: arc radius, 135: start angle, 270: end angle
    canvas->DrawArc({ 100, 150 }, 50, 135, 270, paint);
}

void UITestCanvas::UIKitCanvasTestDrawImage001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制图片");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
//    paint.SetOpacity(255); // 127: opacity
    // {200, 50}: start point coordinates
    canvas->DrawImage({ 10, 50 }, GREEN_IMAGE_PATH, paint);
}

void UITestCanvas::UIKitCanvasTestDrawLabel001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制文字");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetFillColor(Color::Blue());
    UICanvas::FontStyle fontStyle;
    fontStyle.align = TEXT_ALIGNMENT_RIGHT;
    fontStyle.direct = TEXT_DIRECT_RTL;
    fontStyle.fontName = DEFAULT_VECTOR_FONT_FILENAME;
    fontStyle.fontSize = 30;    // 30: font size
    fontStyle.letterSpace = 10; // 10 letter space
    // {50, 50}: start point coordinates, 100: max width
    canvas->DrawLabel({ 50, 50 }, "canvas绘制字体", 100, fontStyle, paint);
}

void UITestCanvas::UIKitCanvasTestDrawSector001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("扇形填充");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetFillColor(Color::Yellow());
    // {350, 150}: sector's center coordinates, 100: sector radius, 0: start angle, 30: end angle
    canvas->DrawSector({ 350, 150 }, 100, 0, 30, paint);
}

void UITestCanvas::UIKitCanvasTestClear001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("清空画布，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    // {0, 10}: Start point coordinates x, y; {50, 10}: end point coordinates x, y
    canvas->DrawLine({ 0, 10 }, { 50, 10 }, paint);
    canvas->Clear();
}

void UITestCanvas::UIKitCanvasTestDrawPath001()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath002()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("lineTo，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath003()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("arc");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath004()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("rect");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath005()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("closePath，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath006()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath007()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + arc");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath008()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + rect");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath009()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + closePath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->LineTo({ LINE2_X, LINE2_Y });
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath010()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("闭合路径调用closePath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->LineTo({ LINE2_X, LINE2_Y });
    canvas->LineTo({ START1_X, START1_Y });
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath011()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + moveTo + lineTo");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->MoveTo({ START2_X, START2_Y });
    canvas->LineTo({ LINE2_X, LINE2_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath012()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + arc");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath013()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + arc + closePath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath014()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + rect");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath015()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + rect + closePath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath016()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("rect + lineTo");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath017()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("rect + moveTo + lineTo");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath018()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("rect + arc");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath019()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("arc + rect");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath020()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + arc + closePath + lineTo");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->ClosePath();
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath021()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("不调用beginPath，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->ClosePath();
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath022()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("不调用drawPath，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->ClosePath();
    canvas->LineTo({ LINE1_X, LINE1_Y });
}

void UITestCanvas::UIKitCanvasTestDrawPath023()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + lineTo + closePath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath024()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("moveTo + closePath，无显示");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath025()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("多次drawPath");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->DrawPath(paint);
    paint.SetStrokeColor(Color::Blue());

    canvas->LineWidth(1,paint);
    canvas->LineTo({ LINE2_X, LINE2_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath026()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("arc起止角度互换");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, END_ANGLE);

    canvas->MoveTo({ START2_X, START2_Y });
    canvas->ArcTo({ CENTER_X + CENTER_X, CENTER_Y }, RADIUS, END_ANGLE, START_ANGLE);

    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath027()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("arc扫描范围超过360度 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE + QUARTER_IN_DEGREE);
    canvas->ArcTo({ CENTER_X + CENTER_X, CENTER_Y }, RADIUS, END_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);

    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath028()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("创建两条路径，只绘制后一条 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);

    canvas->BeginPath();
    canvas->MoveTo({ START2_X, START2_Y });
    canvas->ArcTo({ CENTER_X + CENTER_X, CENTER_Y }, RADIUS, END_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);

    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath029()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制两条不同样式的路径 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({ START2_X, START2_Y });
    canvas->ArcTo({ CENTER_X + CENTER_X, CENTER_Y }, RADIUS, END_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);
    paint.SetStrokeColor(Color::Blue());
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath030()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("同一条路径绘制100遍 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X, LINE1_Y });
    canvas->ArcTo({ CENTER_X, CENTER_Y }, RADIUS, START_ANGLE, START_ANGLE + CIRCLE_IN_DEGREE);
    canvas->AddRect({ RECT_X, RECT_Y }, RECT_HEIGHT, RECT_WIDTH);
    for (uint8_t i = 0; i < 100; i++) { // 100: number of times for drawing loops
        canvas->DrawPath(paint);
    }
}

void UITestCanvas::UIKitCanvasTestDrawPath031()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制直线超出canvas范围");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ START1_X, START1_Y });
    canvas->LineTo({ LINE1_X + HORIZONTAL_RESOLUTION, LINE1_Y });
    canvas->LineTo({ LINE2_X, LINE2_Y });
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath032()
{
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("绘制直线传入临界值 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->MoveTo({ INT16_MAX, INT16_MAX });
    canvas->LineTo({ 0, 0 });

    canvas->MoveTo({ 0, 0 });
    canvas->LineTo({ INT16_MAX, INT16_MAX });

    canvas->MoveTo({ INT16_MIN, INT16_MIN });
    canvas->LineTo({ 0, 0 });

    canvas->MoveTo({ 0, 0 });
    canvas->LineTo({ INT16_MIN, INT16_MIN });
    canvas->DrawPath(paint);
}


void UITestCanvas::UIKitCanvasTestDrawPath033()
{
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("绘制arc传入临界值 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->ArcTo({ INT16_MAX, INT16_MAX }, RADIUS, START_ANGLE, END_ANGLE);
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->ArcTo({ INT16_MIN, INT16_MIN }, RADIUS, START_ANGLE, END_ANGLE);
    paint.SetStrokeColor(Color::Red());
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->ArcTo({ CENTER_X, CENTER_Y }, UINT16_MAX, START_ANGLE, END_ANGLE);
    paint.SetStrokeColor(Color::Yellow());
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->ArcTo({ CENTER_X + CENTER_X, CENTER_Y }, RADIUS, INT16_MIN, INT16_MAX);
    paint.SetStrokeColor(Color::Blue());
    canvas->DrawPath(paint);
}

void UITestCanvas::UIKitCanvasTestDrawPath034()
{
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("绘制rect传入临界值 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    canvas->BeginPath();
    canvas->AddRect({ INT16_MAX, INT16_MAX }, RECT_HEIGHT, RECT_WIDTH);
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->AddRect({ RECT_X, RECT_Y }, INT16_MAX, INT16_MAX);
    paint.SetStrokeColor(Color::Red());
    canvas->DrawPath(paint);
}

/**
 * 调试线性渐变
 */
void UITestCanvas::UIKitCanvasTestLinearGradient(){
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("线性渐变水平 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetStrokeWidth(2);
    paint.createLinearGradient(100,50,200,50);
    paint.SetStrokeColor(Color::White());
    paint.addColorStop(0,Color::Yellow());
    paint.addColorStop(0.3,Color::White());
    paint.addColorStop(0.6,Color::Green());
    paint.addColorStop(1,Color::Blue());
    canvas->DrawRect({ 100, 50 }, 50, 100, paint);

}
/**
 * 调试线性渐变
 */
void UITestCanvas::UIKitCanvasTestLinearGradient2(){
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("线性渐变倾斜 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::STROKE_FILL_STYLE);
    paint.SetFillColor(Color::Yellow());
    paint.SetStrokeWidth(2);
    paint.createLinearGradient(100,50,200,150);
    paint.SetStrokeColor(Color::White());
    paint.addColorStop(0,Color::Yellow());
    paint.addColorStop(0.3,Color::White());
    paint.addColorStop(0.6,Color::Green());
    paint.addColorStop(1,Color::Blue());
    canvas->DrawRect({ 10, 10 }, 180, 300, paint);

}


/**
 * 调试放射渐变
 */
void UITestCanvas::UIKitCanvasTestRadialGradient(){
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("调试放射渐变 ");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStyle(Paint::PaintStyle::FILL_STYLE);
    paint.SetStrokeWidth(2);
    paint.createRadialGradient(180,140,10,150,100,80);
    paint.SetStrokeColor(Color::White());
    paint.addColorStop(0,Color::Yellow());
    paint.addColorStop(0.4,Color::Gray());
    paint.addColorStop(0.8,Color::Green());
    paint.addColorStop(1,Color::Blue());
    canvas->DrawRect({ 10, 10 }, 180, 300, paint);

}


/**
 * pattern
 */
void UITestCanvas::UIKitCanvasTestcreatePattern1()
{
    if (container_ == nullptr) {
       return;
    }
    CreateTitleLabel("pattern-重复");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PaintStyle::PATTERN);

    paint.createPattern(RED_IMAGE_PATH,"repeat");
    PolygonPath path;
    path.Circle({180, 80}, 70);
    canvas->fill(paint,&path);

}

void UITestCanvas::UIKitCanvasTestcreatePattern2()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-x轴重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);

       paint.createPattern(RED_IMAGE_PATH,"repeat-x");
       PolygonPath path;
       path.Circle({300, 80}, 70);
       canvas->fill(paint,&path);
}
void UITestCanvas::UIKitCanvasTestcreatePattern3()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-y轴重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);
       paint.createPattern(RED_IMAGE_PATH,"repeat-y");
       PolygonPath path;
       path.Circle({80, 80}, 70);
       canvas->fill(paint,&path);
}
void UITestCanvas::UIKitCanvasTestcreatePattern4()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-不重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);
       paint.createPattern(RED_IMAGE_PATH,"no-repeat");
       PolygonPath path;
       path.Circle({80, 80}, 70);
       canvas->fill(paint,&path);
}
void UITestCanvas::UIKitCanvasTestcreatePattern5()
{
    if (container_ == nullptr) {
       return;
    }
    CreateTitleLabel("pattern-重复");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PaintStyle::PATTERN);

    paint.createPattern(RED_IMAGE_PATH,"repeat");
    canvas->fill(paint);
}
void UITestCanvas::UIKitCanvasTestcreatePattern6()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-x轴重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);
       paint.createPattern(RED_IMAGE_PATH,"repeat-x");
       canvas->fill(paint);
}
void UITestCanvas::UIKitCanvasTestcreatePattern7()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-y轴重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);
       paint.createPattern(RED_IMAGE_PATH,"repeat-y");
       canvas->fill(paint);
}
void UITestCanvas::UIKitCanvasTestcreatePattern8()
{
       if (container_ == nullptr) {
           return;
       }
       CreateTitleLabel("pattern-不重复");
       UICanvas* canvas = CreateCanvas();
       Paint paint;
       paint.SetStyle(Paint::PaintStyle::PATTERN);
       paint.createPattern(RED_IMAGE_PATH,"no-repeat");
       canvas->fill(paint);
}

} // namespace OHOS
