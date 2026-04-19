#pragma once
#include "primitiveBase.h"
#include "rect2D.h"

namespace Domain
{
    class TrianglePrimitive : public PrimitiveBase
    {
    public:
        Point2D A;
        Point2D B;
        Point2D C;
        bool IsFilled;

        TrianglePrimitive(Point2D a, Point2D b, Point2D c, bool isFilled, ArgbColor color)
            : PrimitiveBase(color), A(a), B(b), C(c), IsFilled(isFilled) {
        }

        Rect2D GetBounds() const override
        {
            return Rect2D::FromPoints({ A, B, C });
        }

        void Accept(IPrimitiveVisitor& visitor) const override;
    };
}
