#pragma once
#include <initializer_list>

#include "point2D.h"

namespace Domain
{
    struct Rect2D
    {
        double MinX;
        double MinY;
        double MaxX;
        double MaxY;

        double Width()  const { return MaxX - MinX; }
        double Height() const { return MaxY - MinY; }

        Rect2D(double minX = 0, double minY = 0, double maxX = 0, double maxY = 0)
            : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {
        }

        Rect2D Expand(const Rect2D& other) const
        {
            return Rect2D(
                MinX < other.MinX ? MinX : other.MinX,
                MinY < other.MinY ? MinY : other.MinY,
                MaxX > other.MaxX ? MaxX : other.MaxX,
                MaxY > other.MaxY ? MaxY : other.MaxY);
        }

        static Rect2D FromPoints(std::initializer_list<Point2D> points)
        {
            double minX = 1e18, minY = 1e18, maxX = -1e18, maxY = -1e18;

            for (const Point2D& p : points)
            {
                if (p.X < minX) minX = p.X;
                if (p.Y < minY) minY = p.Y;
                if (p.X > maxX) maxX = p.X;
                if (p.Y > maxY) maxY = p.Y;
            }

            return { minX, minY, maxX, maxY };
        }
    };
}
