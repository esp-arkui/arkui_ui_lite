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

//    RM008UIKitCanvasTest001();
//    RM008UIKitCanvasTest002();
//    RM008UIKitCanvasTest003();
//    RM008UIKitCanvasTest004();
//    RM008UIKitCanvasTest005();
//    RM008UIKitCanvasTest006();
//    RM008UIKitCanvasTest007();
//    RM008UIKitCanvasShadowTest008();
//    RM009LineCapDrawPath();
//    RM009LineJoinDrawPath();
//    RM009LineDashDrawPath();
//    RM009StrokeAndClearRectDrawPath();
//    RM011StrokeText001();
//    RM011CanvasScale001();
//    RM011CanvasRotate001();
//    RM012globalAlpha001();
//    RM012SaveOrRestore002();
    RM012GlobalCompositeOperation003();
    RM012GlobalCompositeOperationSOURCE_IN();
    RM012GlobalCompositeOperationCopy();
//    UIKitCanvasTestDrawLine001();
//    UIKitCanvasTestDrawLine002();
//    UIKitCanvasTestDrawCurve001();
//    UIKitCanvasTestDrawCurve002();
//    UIKitCanvasTestDrawRect001();
//    UIKitCanvasTestDrawRect002();
//    UIKitCanvasTestDrawRect003();
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
    CreateTitleLabel("绘制直线");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStrokeWidth(5); // 5: line width
    canvas->SetStartPosition({ 50, 10 }); // {50, 10}: Start point coordinates x, y;
    canvas->DrawLine({ 100, 50 }, paint); // {100, 50}: end point coordinates x, y
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
    canvas->DrawRect({ 100, 10 }, 50, 50, paint);
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
    canvas->DrawRect({ 200, 10 }, 50, 50, paint);
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
    paint.SetStrokeWidth(10); // 10: line width
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
//    paint.SetOpacity(127); // 127: opacity
    paint.Rotate(20);
    // {200, 50}: start point coordinates
//    canvas->DrawImage({ 0, 60 }, RED_IMAGE_PATH, paint);

    paint.Scale(0.4,0.4);
//    paint.Rotate(-20);
    canvas->DrawImage({100, 20}, GIF_IMAGE_PATH, paint);
//    paint.Rotate(10);
//    canvas->DrawImage({100, 20}, GIF_IMAGE_PATH, paint);
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
    fontStyle.direct = TEXT_DIRECT_LTR;
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
    paint.SetStrokeWidth(1);
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

void UITestCanvas::RM009LineCapDrawPath(){
    if (container_ == nullptr) {
        return;
    }

    CreateTitleLabel("RM009LineCap_平直的边缘_正方形_圆形线帽");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStrokeWidth(12);
    paint.SetStrokeColor(Color::Green());
    paint.SetLineCap(LineCapEnum::BUTT_CAP);

    canvas->BeginPath();
    canvas->MoveTo({20,20});//竖线
    canvas->LineTo({20, 80});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({20,100});//横线
    canvas->LineTo({80, 100});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({180,40}); //斜线
    canvas->LineTo({240, 100});
    canvas->DrawPath(paint);

    paint.SetStrokeColor(Color::Red());
    paint.SetLineCap(LineCapEnum::SQUARE_CAP);

    canvas->BeginPath();
    canvas->MoveTo({40,20});//竖线
    canvas->LineTo({40, 80});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({20,120});//横线
    canvas->LineTo({80, 120});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({200,40}); //斜线
    canvas->LineTo({260, 100});
    canvas->DrawPath(paint);

    paint.SetStrokeColor(Color::Blue());
    paint.SetLineCap(LineCapEnum::ROUND_CAP);

    canvas->BeginPath();
    canvas->MoveTo({60,20});//竖线
    canvas->LineTo({60, 80});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({20,140});//横线
    canvas->LineTo({80, 140});
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({240,40}); //斜线
    canvas->LineTo({300, 100});
    canvas->DrawPath(paint);
}
void UITestCanvas::RM009LineJoinDrawPath(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM009LineJoin_圆角_斜角_尖角_限制尖角长度");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetStrokeWidth(10);
    paint.SetStrokeColor(Color::Green());
    paint.SetLineJoin(LineJoinEnum::ROUND_JOIN);

    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 80});
    canvas->LineTo({40, 40});
    canvas->LineTo({60, 80});
    canvas->DrawPath(paint);

    paint.SetLineJoin(LineJoinEnum::BEVEL_JOIN);

    canvas->BeginPath();
    canvas->MoveTo({100, 20});
    canvas->LineTo({100, 80});
    canvas->LineTo({120, 40});
    canvas->LineTo({140, 80});
    canvas->DrawPath(paint);

    paint.SetLineJoin(LineJoinEnum::MITER_JOIN);
    canvas->BeginPath();
    canvas->MoveTo({180, 20});
    canvas->LineTo({180, 80});
    canvas->LineTo({200, 40});
    canvas->LineTo({220, 80});
    canvas->DrawPath(paint);

    //TODO::当斜接角度超过MiterLimit时应该转化为平角
    paint.SetLineJoin(LineJoinEnum::MITER_JOIN);
    paint.SetMiterLimit(4);
    canvas->BeginPath();
    canvas->MoveTo({260, 20});
    canvas->LineTo({260, 80});
    canvas->LineTo({280, 40});
    canvas->LineTo({330, 40});
    canvas->DrawPath(paint);
}
void UITestCanvas::RM009LineDashDrawPath(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM009LineDash_虚实线(10,5,5,2)_更改虚实线起点的偏移量(5)");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStrokeWidth(2);
    paint.SetStrokeColor(Color::Green());
    float ds2[] = {10, 5.0f, 5.0f, 2.0f};
    paint.SetLineDash(ds2,4);
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({200, 20});
    canvas->LineTo({200, 80});
    canvas->LineTo({20, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    paint.SetLineDashOffset(5);
    canvas->BeginPath();
    canvas->MoveTo({20, 120});
    canvas->LineTo({200, 120});
    canvas->LineTo({200, 180});
    canvas->LineTo({20, 180});
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::RM009StrokeAndClearRectDrawPath(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM009_StrokeRect_ClearRect");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStrokeWidth(2);
    paint.SetStrokeColor(Color::Orange());
    canvas->StrokeRect({10,10},40,50,paint);

    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({30,60});
    canvas->LineTo({200,60});
    canvas->LineTo({200,200});
    canvas->LineTo({30,200});
    canvas->ClosePath();
    canvas->FillPath(paint);

    canvas->ClearRect({50,80},100,70,paint);
}

void UITestCanvas::RM008UIKitCanvasTest001(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_FillStyle_Solid_单色绘制多边形和填充多边形");
    UICanvas* canvas = CreateCanvas();
    Paint paint;

    paint.SetStyle(Paint::STROKE_FILL_STYLE);
    paint.SetStrokeWidth(2);
    paint.SetStrokeColor(Color::Orange());
    paint.SetFillColor(Color::Blue());
    canvas->BeginPath();

    canvas->MoveTo({20, 20});
    canvas->LineTo({60, 80});
    canvas->LineTo({120, 20});
    canvas->ArcTo({120, 100},80,0,180);
    canvas->LineTo({120, 180});
    canvas->LineTo({100, 70});
    canvas->LineTo({20, 130});
    canvas->LineTo({5, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
    canvas->DrawPath(paint);
}

void UITestCanvas::RM008UIKitCanvasTest002(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_FillStyle_RM_013_Gradient_渐变填充多边形");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::GRADIENT);
    //线性渐变
    paint.createLinearGradient(50, 50, 150, 150);
    paint.addColorStop(0, Color::Yellow());
    paint.addColorStop(0.3, Color::White());
    paint.addColorStop(0.6, Color::Green());
    paint.addColorStop(1, Color::Blue());

    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({60, 80});
    canvas->LineTo({120, 20});
    canvas->ArcTo({120, 100},80,0,180);
    canvas->LineTo({120, 180});
    canvas->LineTo({50, 120});
    canvas->LineTo({20, 130});
    canvas->LineTo({5, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
    //放射渐变
    paint.createRadialGradient(300, 140, 5, 270, 100, 80);

    canvas->BeginPath();
    canvas->MoveTo({230, 20});
    canvas->LineTo({270, 80});
    canvas->LineTo({330, 20});
    canvas->ArcTo({330, 100},80,0,180);
    canvas->LineTo({330, 180});
    canvas->LineTo({260, 120});
    canvas->LineTo({230, 130});
    canvas->LineTo({215, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
}

void UITestCanvas::RM008UIKitCanvasTest003(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_StrokeStyle_RM_013_Gradient_渐变绘制多边形路径");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::GRADIENT);
    paint.SetStrokeWidth(8);
    //线性渐变
    paint.createLinearGradient(50, 50, 150, 150);
    paint.addColorStop(0, Color::Yellow());
    paint.addColorStop(0.3, Color::White());
    paint.addColorStop(0.6, Color::Green());
    paint.addColorStop(1, Color::Blue());

    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({60, 80});
    canvas->LineTo({120, 20});
    canvas->ArcTo({120, 100},80,0,180);
    canvas->LineTo({120, 180});
    canvas->LineTo({50, 120});
    canvas->LineTo({20, 130});
    canvas->LineTo({5, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);
    //放射渐变
    paint.createRadialGradient(300, 140, 5, 270, 100, 80);

    canvas->BeginPath();
    canvas->MoveTo({230, 20});
    canvas->LineTo({270, 80});
    canvas->LineTo({330, 20});
    canvas->ArcTo({330, 100},80,0,180);
    canvas->LineTo({330, 180});
    canvas->LineTo({260, 120});
    canvas->LineTo({230, 130});
    canvas->LineTo({215, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::RM008UIKitCanvasTest004(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_Pattern_模式REPEAT(图像)绘制多边形路径和填充");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PATTERN);
    paint.SetStrokeWidth(8);
    paint.CreatePattern(RED_IMAGE_PATH, Paint::REPEAT);
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({60, 80});
    canvas->LineTo({120, 20});
    canvas->ArcTo({120, 100},80,0,180);
    canvas->LineTo({120, 180});
    canvas->LineTo({50, 120});
    canvas->LineTo({20, 130});
    canvas->LineTo({5, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({230, 20});
    canvas->LineTo({270, 80});
    canvas->LineTo({330, 20});
    canvas->ArcTo({330, 100},80,0,180);
    canvas->LineTo({330, 180});
    canvas->LineTo({260, 120});
    canvas->LineTo({230, 130});
    canvas->LineTo({215, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
}
void UITestCanvas::RM008UIKitCanvasTest005(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_Pattern_模式REPEAT_X(图像)绘制多边形路径和填充");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PATTERN);
    paint.SetStrokeWidth(8);
    paint.CreatePattern(RED_IMAGE_PATH, Paint::REPEAT_X);
    canvas->BeginPath();

    canvas->MoveTo({20, 20});
    canvas->LineTo({60, 80});
    canvas->LineTo({120, 20});
    canvas->ArcTo({120, 100},80,0,180);
    canvas->LineTo({120, 180});
    canvas->LineTo({50, 120});
    canvas->LineTo({20, 130});
    canvas->LineTo({5, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({230, 20});
    canvas->LineTo({270, 80});
    canvas->LineTo({330, 20});
    canvas->ArcTo({330, 100},80,0,180);
    canvas->LineTo({330, 180});
    canvas->LineTo({260, 120});
    canvas->LineTo({230, 130});
    canvas->LineTo({215, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
}
void UITestCanvas::RM008UIKitCanvasTest006(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_Pattern_模式REPEAT_Y(图像)绘制多边形路径和填充");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PATTERN);
    paint.SetStrokeWidth(8);
    paint.CreatePattern(RED_IMAGE_PATH, Paint::REPEAT_Y);
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 60});
    canvas->LineTo({200, 60});
    canvas->LineTo({200, 20});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({20, 80});
    canvas->LineTo({20, 180});
    canvas->LineTo({200, 180});
    canvas->LineTo({200, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
}
void UITestCanvas::RM008UIKitCanvasTest007(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_Pattern_模式NO_REPEAT(图像)绘制多边形路径和填充");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetStyle(Paint::PATTERN);
    paint.SetStrokeWidth(8);
    paint.CreatePattern(RED_IMAGE_PATH, Paint::NO_REPEAT);
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 60});
    canvas->LineTo({200, 60});
    canvas->LineTo({200, 20});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    canvas->BeginPath();
    canvas->MoveTo({20, 80});
    canvas->LineTo({20, 180});
    canvas->LineTo({200, 180});
    canvas->LineTo({200, 80});
    canvas->ClosePath();
    canvas->FillPath(paint);
}

void UITestCanvas::RM008UIKitCanvasShadowTest008(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM008_阴影不偏移_偏移_改变模糊级别_改变模糊颜色");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.FillStyle(Color::Orange());
    paint.SetStrokeWidth(8);
    paint.SetShadowBlur(5);
    paint.SetShadowColor(Color::Black());
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 60});
    canvas->LineTo({100, 60});
    canvas->LineTo({100, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetShadowOffsetX(20);
    paint.SetShadowOffsetY(10);
    canvas->BeginPath();
    canvas->MoveTo({120, 20});
    canvas->LineTo({120, 60});
    canvas->LineTo({200, 60});
    canvas->LineTo({200, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetShadowOffsetX(0);
    paint.SetShadowOffsetY(0);
    canvas->BeginPath();
    canvas->MoveTo({20, 80});
    canvas->LineTo({20, 120});
    canvas->LineTo({100, 120});
    canvas->LineTo({100, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);


    paint.SetShadowOffsetX(20);
    paint.SetShadowOffsetY(10);
    canvas->BeginPath();
    canvas->MoveTo({120, 80});
    canvas->LineTo({120, 120});
    canvas->LineTo({200, 120});
    canvas->LineTo({200, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    paint.SetShadowBlur(15);
    paint.SetShadowOffsetX(20);
    paint.SetShadowOffsetY(10);
    canvas->BeginPath();
    canvas->MoveTo({250, 20});
    canvas->LineTo({250, 60});
    canvas->LineTo({330, 60});
    canvas->LineTo({330, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetShadowBlur(5);
    paint.SetShadowColor(Color::Blue());
    paint.SetShadowOffsetX(20);
    paint.SetShadowOffsetY(10);
    canvas->BeginPath();
    canvas->MoveTo({250, 80});
    canvas->LineTo({250, 120});
    canvas->LineTo({330, 120});
    canvas->LineTo({330, 80});
    canvas->ClosePath();
    canvas->DrawPath(paint);
}

void UITestCanvas::RM011StrokeText001(){
    CreateTitleLabel("RM011_StrokeText_多国文字加旋转放大");
    UICanvas* canvas = CreateCanvas();
    UICanvas::FontStyle fontStyle;
    fontStyle.align = TEXT_ALIGNMENT_CENTER;
    fontStyle.direct = TEXT_DIRECT_LTR;
    fontStyle.fontName = DEFAULT_VECTOR_FONT_FILENAME;
    fontStyle.fontSize = 15;
    fontStyle.letterSpace = 2;
    Paint paint;
    paint.SetFillColor(Color::Blue());
    canvas->StrokeText("葡萄牙语：Hongmeng, teste", {0, 20}, fontStyle, paint);
    canvas->StrokeText("西班牙语：Hong Meng, test", {0, 40}, fontStyle, paint);
    canvas->StrokeText("法语：HongMeng, test", {0, 60}, fontStyle, paint);
    canvas->StrokeText("瓜拉尼语：mitapoañda, pens", {0, 80}, fontStyle, paint);
    canvas->StrokeText("荷兰语：Hongmeng, kom op.", {0, 100}, fontStyle, paint);
    canvas->StrokeText("樊瓦什语：Кайалла", {0, 120}, fontStyle, paint);
    canvas->StrokeText("白俄罗斯语：Прывітанне", {0, 140}, fontStyle, paint);
    canvas->StrokeText("希腊语：Γεια.", {0, 80}, fontStyle, paint);
    canvas->StrokeText("瑞典语:Hej, Hongmeng.", {0, 160}, fontStyle, paint);
    canvas->StrokeText("俄语: Привет, hongmon ", {0, 180}, fontStyle, paint);
    paint.Scale(2,1);
    paint.Rotate(45);
    canvas->StrokeText("中国 你好，鸿蒙。。", {20, 0}, fontStyle, paint);
}
void UITestCanvas::RM011CanvasScale001(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM011放大和缩小Scale");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.StrokeStyle(Color::Red());
    paint.SetStrokeWidth(3);
    canvas->BeginPath();
    canvas->MoveTo({5, 5});
    canvas->LineTo({50, 5});
    canvas->LineTo({50, 35});
    canvas->LineTo({5, 35});
    canvas->ClosePath();
    canvas->DrawPath(paint);
    paint.StrokeStyle(Color::Green());
    paint.SetGlobalAlpha(0.5);
    paint.Scale(2,4);
    canvas->BeginPath();
    canvas->MoveTo({5, 5});
    canvas->LineTo({50, 5});
    canvas->LineTo({50, 35});
    canvas->LineTo({5, 35});
    canvas->ClosePath();
    canvas->DrawPath(paint);
    paint.StrokeStyle(Color::Blue());
    paint.SetGlobalAlpha(0.5);
    paint.Scale(0.5,0.5);
    canvas->BeginPath();
    canvas->MoveTo({5, 5});
    canvas->LineTo({50, 5});
    canvas->LineTo({50, 35});
    canvas->LineTo({5, 35});
    canvas->ClosePath();
    canvas->DrawPath(paint);
}
void UITestCanvas::RM011CanvasRotate001(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM011原(红)_旋转(绿)_平移(蓝)_单位矩阵(黄)_重置矩阵后再设置矩阵（橘色）");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.StrokeStyle(Color::Red());
    paint.SetStrokeWidth(3);
    canvas->BeginPath();
    canvas->MoveTo({15, 15});
    canvas->LineTo({60, 15});
    canvas->LineTo({60, 45});
    canvas->LineTo({15, 45});
    canvas->ClosePath();
    canvas->DrawPath(paint);
    paint.StrokeStyle(Color::Green());
    paint.Rotate(20);
    canvas->BeginPath();
    canvas->MoveTo({15, 15});
    canvas->LineTo({60, 15});
    canvas->LineTo({60, 45});
    canvas->LineTo({15, 45});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    paint.Translate(50,50);
    paint.StrokeStyle(Color::Blue());
    canvas->BeginPath();
    canvas->MoveTo({15, 15});
    canvas->LineTo({60, 15});
    canvas->LineTo({60, 45});
    canvas->LineTo({15, 45});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    paint.Transform(1,0.5,0.5,2,0,0);
    paint.StrokeStyle(Color::Yellow());
    canvas->BeginPath();
    canvas->MoveTo({15, 15});
    canvas->LineTo({60, 15});
    canvas->LineTo({60, 45});
    canvas->LineTo({15, 45});
    canvas->ClosePath();
    canvas->DrawPath(paint);

    paint.SetTransform(1,0.5,0.5,2,0,0);
    paint.StrokeStyle(Color::Orange());
    canvas->BeginPath();
    canvas->MoveTo({15, 15});
    canvas->LineTo({60, 15});
    canvas->LineTo({60, 45});
    canvas->LineTo({15, 45});
    canvas->ClosePath();
    canvas->DrawPath(paint);

}

void UITestCanvas::RM012globalAlpha001(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM012_设置图像透明度_红不透明_绿蓝_一半透明度");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 80});
    canvas->LineTo({150, 80});
    canvas->LineTo({150, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalAlpha(0.2);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({100, 60});
    canvas->LineTo({100, 120});
    canvas->LineTo({230, 120});
    canvas->LineTo({230, 60});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetFillColor(Color::Blue());
    canvas->BeginPath();
    canvas->MoveTo({180, 100});
    canvas->LineTo({180, 160});
    canvas->LineTo({310, 160});
    canvas->LineTo({310, 100});
    canvas->ClosePath();
    canvas->FillPath(paint);
}

void UITestCanvas::RM012GlobalCompositeOperation003(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM012_设置Composite");
    UICanvas* canvas = CreateCanvas();

    Paint paint;
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({10, 10});
    canvas->LineTo({10, 50});
    canvas->LineTo({50, 50});
    canvas->LineTo({50, 10});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({30, 30});
    canvas->LineTo({30, 70});
    canvas->LineTo({70, 70});
    canvas->LineTo({70, 30});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({90, 10});
    canvas->LineTo({90, 50});
    canvas->LineTo({130, 50});
    canvas->LineTo({130, 10});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::SOURCE_ATOP);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({110, 30});
    canvas->LineTo({110, 70});
    canvas->LineTo({150, 70});
    canvas->LineTo({150, 30});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({250, 10});
    canvas->LineTo({250, 50});
    canvas->LineTo({290, 50});
    canvas->LineTo({290, 10});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::SOURCE_OUT);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({280, 30});
    canvas->LineTo({280, 70});
    canvas->LineTo({310, 70});
    canvas->LineTo({310, 30});
    canvas->ClosePath();
    canvas->FillPath(paint);


    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({330, 10});
    canvas->LineTo({330, 50});
    canvas->LineTo({370, 50});
    canvas->LineTo({370, 10});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::DESTINATION_OVER);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({360, 30});
    canvas->LineTo({360, 70});
    canvas->LineTo({390, 70});
    canvas->LineTo({390, 30});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({10, 90});
    canvas->LineTo({10, 130});
    canvas->LineTo({50, 130});
    canvas->LineTo({50, 90});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::DESTINATION_ATOP);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({30, 110});
    canvas->LineTo({30, 150});
    canvas->LineTo({70, 150});
    canvas->LineTo({70, 110});
    canvas->ClosePath();
    canvas->FillPath(paint);


    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({90, 90});
    canvas->LineTo({90, 130});
    canvas->LineTo({130, 130});
    canvas->LineTo({130, 90});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::DESTINATION_IN);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({110, 110});
    canvas->LineTo({110, 150});
    canvas->LineTo({150, 150});
    canvas->LineTo({150, 110});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({170, 90});
    canvas->LineTo({170, 130});
    canvas->LineTo({210, 130});
    canvas->LineTo({210, 90});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::DESTINATION_OUT);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({190, 110});
    canvas->LineTo({190, 150});
    canvas->LineTo({230, 150});
    canvas->LineTo({230, 110});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({250, 90});
    canvas->LineTo({250, 130});
    canvas->LineTo({290, 130});
    canvas->LineTo({290, 90});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::LIGHTER);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({270, 110});
    canvas->LineTo({270, 150});
    canvas->LineTo({310, 150});
    canvas->LineTo({310, 110});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalCompositeOperation(Paint::SOURCE_OVER);
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({330, 90});
    canvas->LineTo({330, 130});
    canvas->LineTo({370, 130});
    canvas->LineTo({370, 90});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::XOR);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({350, 110});
    canvas->LineTo({350, 150});
    canvas->LineTo({390, 150});
    canvas->LineTo({390, 110});
    canvas->ClosePath();
    canvas->FillPath(paint);
}
void UITestCanvas::RM012GlobalCompositeOperationSOURCE_IN(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM012_设置Composite_SOURCE_IN_只显示第二个图源绿色");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 150});
    canvas->LineTo({300, 150});
    canvas->LineTo({300, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);
    paint.SetGlobalCompositeOperation(Paint::SOURCE_IN);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({100, 60});
    canvas->LineTo({100, 120});
    canvas->LineTo({230, 120});
    canvas->LineTo({230, 60});
    canvas->ClosePath();
    canvas->FillPath(paint);

}
void UITestCanvas::RM012GlobalCompositeOperationCopy(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM012_设置Composite_COPY_只显示第二个图源绿色");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 80});
    canvas->LineTo({150, 80});
    canvas->LineTo({150, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

//    paint.SetGlobalAlpha(0.2);
    paint.SetGlobalCompositeOperation(Paint::COPY);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({100, 60});
    canvas->LineTo({100, 120});
    canvas->LineTo({230, 120});
    canvas->LineTo({230, 60});
    canvas->ClosePath();
    canvas->FillPath(paint);

}

void UITestCanvas::RM012SaveOrRestore002(){
    if (container_ == nullptr) {
        return;
    }
    CreateTitleLabel("RM012_设置图像透明度_红不透明_绿蓝_一半透明度Save_Restore");
    UICanvas* canvas = CreateCanvas();
    Paint paint;
    paint.SetFillColor(Color::Red());
    canvas->BeginPath();
    canvas->MoveTo({20, 20});
    canvas->LineTo({20, 80});
    canvas->LineTo({150, 80});
    canvas->LineTo({150, 20});
    canvas->ClosePath();
    canvas->FillPath(paint);

    paint.SetGlobalAlpha(0.2);
    paint.SetFillColor(Color::Green());
    canvas->BeginPath();
    canvas->MoveTo({100, 60});
    canvas->LineTo({100, 120});
    canvas->LineTo({230, 120});
    canvas->LineTo({230, 60});
    canvas->ClosePath();
    canvas->FillPath(paint);

    canvas->Save(paint);
    paint.SetFillColor(Color::Blue());
    canvas->BeginPath();
    canvas->MoveTo({180, 100});
    canvas->LineTo({180, 160});
    canvas->LineTo({310, 160});
    canvas->LineTo({310, 100});
    canvas->ClosePath();
    paint=canvas->Restore();
    canvas->FillPath(paint);


}

} // namespace OHOS
