#pragma once
#include "argbColor.h"
#include "rect2D.h"

namespace Domain
{
    class PrimitiveBase
    {
    public:
        ArgbColor Color;

        explicit PrimitiveBase(ArgbColor color) : Color(color) {}
        virtual ~PrimitiveBase() = default;

        virtual Rect2D GetBounds() const = 0;
        virtual void Accept(class IPrimitiveVisitor& visitor) const = 0;
    };
}
