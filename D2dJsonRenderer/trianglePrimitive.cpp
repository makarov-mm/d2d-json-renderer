#include "trianglePrimitive.h"
#include "primitiveVisitor.h"

namespace Domain
{
    void TrianglePrimitive::Accept(IPrimitiveVisitor& visitor) const
    {
        visitor.Visit(*this);
    }
}
