#pragma once
#include <d2d1.h>
#include <memory>

#include "scene.h"

namespace Presentation
{
    class SceneView
    {
    public:
        static constexpr double ZoomStep = 1.1;
        static constexpr double ZoomMin = 0.1;
        static constexpr double ZoomMax = 10.0;

        SceneView();
        ~SceneView();

        HRESULT Initialize(HWND hwnd);
        void    Render(const Domain::Scene* scene);
        void    Resize(UINT width, UINT height) const;
        void    DiscardResources();

        void    ZoomIn();
        void    ZoomOut();
        void    ResetZoom();
        void    ZoomAtPoint(int delta);
        double  GetZoom() const;

    private:
        HRESULT CreateDeviceResources(HWND hwnd);
        void ApplyZoom(double factor);

        HWND _hwnd;
        ID2D1Factory* _factory;
        ID2D1HwndRenderTarget* _renderTarget;
        double _zoom;
    };
}
