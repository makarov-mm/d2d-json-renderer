#pragma once
#include <d2d1.h>

#include "argbColor.h"
#include "coordinateTransformer.h"
#include "primitiveVisitor.h"

namespace Presentation
{
    class SceneRenderer : public Domain::IPrimitiveVisitor
    {
    public:
        SceneRenderer(ID2D1RenderTarget* renderTarget, const Application::CoordinateTransformer& transformer);

        void Visit(const Domain::LinePrimitive& line) override;
        void Visit(const Domain::CirclePrimitive& circle) override;
        void Visit(const Domain::TrianglePrimitive& triangle) override;

    private:
        ID2D1RenderTarget* _rt;
        const Application::CoordinateTransformer& _transformer;

        ID2D1SolidColorBrush* CreateBrush(const Domain::ArgbColor& color) const;
    };
}
