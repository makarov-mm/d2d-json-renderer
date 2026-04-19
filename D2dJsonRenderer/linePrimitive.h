#pragma once
#include "primitiveBase.h"
#include "rect2D.h"

namespace Domain
{
    class LinePrimitive : public PrimitiveBase
    {
    public:
        Point2D A;
        Point2D B;

        LinePrimitive(Point2D a, Point2D b, ArgbColor color)
            : PrimitiveBase(color), A(a), B(b) {
        }

        Rect2D GetBounds() const override
        {
            return Rect2D::FromPoints({ A, B });
        }

        void Accept(IPrimitiveVisitor& visitor) const override;
    };
}
