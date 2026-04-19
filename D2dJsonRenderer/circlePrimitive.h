#pragma once
#include "primitiveBase.h"
#include "rect2D.h"

namespace Domain
{
    class CirclePrimitive : public PrimitiveBase
    {
    public:
        Point2D Center;
        double  Radius;
        bool    IsFilled;

        CirclePrimitive(Point2D center, double radius, bool isFilled, ArgbColor color)
            : PrimitiveBase(color), Center(center), Radius(radius), IsFilled(isFilled) {
        }

        Rect2D GetBounds() const override
        {
            return {
                Center.X - Radius, Center.Y - Radius,
                Center.X + Radius, Center.Y + Radius
            };
        }

        void Accept(IPrimitiveVisitor& visitor) const override;
    };
}
