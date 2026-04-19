#pragma once

namespace Domain
{
    struct Point2D
    {
        double X;
        double Y;

        Point2D(double x = 0.0, double y = 0.0) : X(x), Y(y) {}
    };
}
