#include "sceneRenderer.h"

namespace Presentation
{
    SceneRenderer::SceneRenderer(ID2D1RenderTarget* renderTarget,
        const Application::CoordinateTransformer& transformer)
        : _rt(renderTarget), _transformer(transformer)
    {
    }

    void SceneRenderer::Visit(const Domain::LinePrimitive& line)
    {
        ID2D1SolidColorBrush* brush = CreateBrush(line.Color);
        if (!brush) return;

        _rt->DrawLine(
            _transformer.Transform(line.A),
            _transformer.Transform(line.B),
            brush,
            1.0f);

        brush->Release();
    }

    void SceneRenderer::Visit(const Domain::CirclePrimitive& circle)
    {
        ID2D1SolidColorBrush* brush = CreateBrush(circle.Color);
        if (!brush) return;

        D2D1_POINT_2F center = _transformer.Transform(circle.Center);
        float radius = _transformer.ScaleLength(circle.Radius);

        D2D1_ELLIPSE ellipse = D2D1::Ellipse(center, radius, radius);

        if (circle.IsFilled)
            _rt->FillEllipse(ellipse, brush);

        _rt->DrawEllipse(ellipse, brush, 1.0f);

        brush->Release();
    }

    void SceneRenderer::Visit(const Domain::TrianglePrimitive& triangle)
    {
        ID2D1SolidColorBrush* brush = CreateBrush(triangle.Color);
        if (!brush) return;

        D2D1_POINT_2F a = _transformer.Transform(triangle.A);
        D2D1_POINT_2F b = _transformer.Transform(triangle.B);
        D2D1_POINT_2F c = _transformer.Transform(triangle.C);

        ID2D1Factory* factory = nullptr;
        _rt->GetFactory(&factory);

        ID2D1PathGeometry* geometry = nullptr;
        factory->CreatePathGeometry(&geometry);
        factory->Release();

        if (!geometry) { brush->Release(); return; }

        ID2D1GeometrySink* sink = nullptr;
        geometry->Open(&sink);

        sink->BeginFigure(a, D2D1_FIGURE_BEGIN_FILLED);
        sink->AddLine(b);
        sink->AddLine(c);
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        sink->Close();
        sink->Release();

        if (triangle.IsFilled)
            _rt->FillGeometry(geometry, brush);

        _rt->DrawGeometry(geometry, brush, 1.0f);

        geometry->Release();
        brush->Release();
    }

    ID2D1SolidColorBrush* SceneRenderer::CreateBrush(const Domain::ArgbColor& color) const
    {
        ID2D1SolidColorBrush* brush = nullptr;
        _rt->CreateSolidColorBrush(
            D2D1::ColorF(
                color.R / 255.0f,
                color.G / 255.0f,
                color.B / 255.0f,
                color.A / 255.0f),
            &brush);
        return brush;
    }
}
