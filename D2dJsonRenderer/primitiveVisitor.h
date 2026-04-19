#pragma once
#include "circlePrimitive.h"
#include "linePrimitive.h"
#include "trianglePrimitive.h"

namespace Domain
{
    class IPrimitiveVisitor
    {
    public:
        virtual ~IPrimitiveVisitor() = default;
        virtual void Visit(const LinePrimitive& line) = 0;
        virtual void Visit(const CirclePrimitive& circle) = 0;
        virtual void Visit(const TrianglePrimitive& triangle) = 0;
    };
}
