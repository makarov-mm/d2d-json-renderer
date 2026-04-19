#include "sceneView.h"
#include "sceneRenderer.h"

#include <algorithm>

namespace Presentation
{
    SceneView::SceneView()
		: _hwnd(nullptr)
        , _factory(nullptr)
        , _renderTarget(nullptr)
        , _zoom(1.0)
    {
    }

    SceneView::~SceneView()
    {
        DiscardResources();
        if (_factory) { _factory->Release(); _factory = nullptr; }
    }

    HRESULT SceneView::Initialize(HWND hwnd)
    {
        _hwnd = hwnd;
        return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory);
    }

    HRESULT SceneView::CreateDeviceResources(HWND hwnd)
    {
        if (_renderTarget) return S_OK;

        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        return _factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &_renderTarget);
    }

    void SceneView::Render(const Domain::Scene* scene)
    {
        if (FAILED(CreateDeviceResources(_hwnd))) return;

        D2D1_SIZE_F size = _renderTarget->GetSize();

        _renderTarget->BeginDraw();
        _renderTarget->Clear(D2D1::ColorF(0.68f, 0.85f, 0.90f));

        D2D1::Matrix3x2F transform =
            D2D1::Matrix3x2F::Scale(static_cast<float>(_zoom), static_cast<float>(_zoom));

        _renderTarget->SetTransform(transform);

        if (scene && !scene->Primitives.empty())
        {
            Domain::Rect2D bounds = scene->GetBounds();
            Application::ViewportTransform vt = Application::CoordinateTransformer::Calculate(
                bounds, size.width / _zoom, size.height / _zoom);

            Application::CoordinateTransformer transformer(vt);
            SceneRenderer renderer(_renderTarget, transformer);

            for (const auto& primitive : scene->Primitives)
            {
                primitive->Accept(renderer);
            }
        }

        _renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        HRESULT hr = _renderTarget->EndDraw();

        if (hr == D2DERR_RECREATE_TARGET)
        {
            DiscardResources();
        }
    }

    void SceneView::Resize(UINT width, UINT height) const
    {
        if (_renderTarget)
        {
            _renderTarget->Resize(D2D1::SizeU(width, height));
        }
    }

    void SceneView::DiscardResources()
    {
        if (_renderTarget)
        {
	        _renderTarget->Release(); 
        	_renderTarget = nullptr;
        }
    }
    void SceneView::ZoomIn()
    {
        ApplyZoom(ZoomStep);
    }

    void SceneView::ZoomOut()
    {
        ApplyZoom(1.0 / ZoomStep);
    }

    void SceneView::ResetZoom()
    {
        _zoom = 1.0;
    }

    void SceneView::ZoomAtPoint(int delta)
    {
        double factor = delta > 0 ? ZoomStep : 1.0 / ZoomStep;
        ApplyZoom(factor);
    }

    double SceneView::GetZoom() const
    {
	    return _zoom;
    }

    void SceneView::ApplyZoom(double factor)
    {
        double newZoom = _zoom * factor;

        newZoom = std::max<double>(newZoom, ZoomMin);
        newZoom = std::min<double>(newZoom, ZoomMax);

        RECT rc;
        GetClientRect(_hwnd, &rc);

        _zoom = newZoom;
    }
}
