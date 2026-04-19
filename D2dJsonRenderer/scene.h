#pragma once
#include <memory>
#include <vector>

#include "primitiveBase.h"
#include "rect2D.h"

namespace Domain
{
    class Scene
    {
    public:
        std::vector<std::unique_ptr<PrimitiveBase>> Primitives;

        Rect2D GetBounds() const
        {
            if (Primitives.empty())
            {
                return { 0, 0, 0, 0 };
            }

            Rect2D bounds = Primitives[0]->GetBounds();

            for (size_t i = 1; i < Primitives.size(); ++i)
            {
                bounds = bounds.Expand(Primitives[i]->GetBounds());
            }

            return bounds;
        }
    };
}
