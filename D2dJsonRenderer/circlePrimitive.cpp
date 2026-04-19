#include "circlePrimitive.h"
#include "primitiveVisitor.h"

namespace Domain
{
    void CirclePrimitive::Accept(IPrimitiveVisitor& visitor) const
    {
        visitor.Visit(*this);
    }
}
