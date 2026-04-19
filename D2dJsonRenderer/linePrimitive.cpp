#include "linePrimitive.h"
#include "primitiveVisitor.h"

namespace Domain
{
    void LinePrimitive::Accept(IPrimitiveVisitor& visitor) const
    {
        visitor.Visit(*this);
    }
}
