#pragma once

#include <d2d1.h>

#include "point2D.h"
#include "rect2D.h"

namespace Application
{

    struct ViewportTransform
    {
        double Scale;
        double OffsetX;
        double OffsetY;

        ViewportTransform(double scale = 1.0, double offsetX = 0.0, double offsetY = 0.0)
            : Scale(scale), OffsetX(offsetX), OffsetY(offsetY) {
        }
    };

    class CoordinateTransformer
    {
    public:
        explicit CoordinateTransformer(const ViewportTransform& transform)
            : _transform(transform) {
        }

        D2D1_POINT_2F Transform(const Domain::Point2D& point) const
        {
            return D2D1::Point2F(
                static_cast<float>(point.X * _transform.Scale + _transform.OffsetX),
                static_cast<float>(-point.Y * _transform.Scale + _transform.OffsetY));
        }

        float ScaleLength(double value) const
        {
            return static_cast<float>(value * _transform.Scale);
        }

        static ViewportTransform Calculate(const Domain::Rect2D& bounds,
            double viewportWidth,
            double viewportHeight)
        {
            const double Padding = 20.0;

            if (viewportWidth <= 0 || viewportHeight <= 0)
                return ViewportTransform(1.0, 0.0, 0.0);

            double contentWidth = bounds.Width() > 0 ? bounds.Width() : 1.0;
            double contentHeight = bounds.Height() > 0 ? bounds.Height() : 1.0;

            double availableWidth = viewportWidth - 2 * Padding;
            double availableHeight = viewportHeight - 2 * Padding;

            if (availableWidth <= 0) availableWidth = viewportWidth;
            if (availableHeight <= 0) availableHeight = viewportHeight;

            double scaleX = availableWidth / contentWidth;
            double scaleY = availableHeight / contentHeight;
            double scale = scaleX < scaleY ? scaleX : scaleY;

            scale = (((scale) < (1.0)) ? (scale) : (1.0));

            double renderedWidth = contentWidth * scale;
            double renderedHeight = contentHeight * scale;

            double offsetX = (viewportWidth - renderedWidth) / 2.0 - bounds.MinX * scale;
            double offsetY = (viewportHeight - renderedHeight) / 2.0 + bounds.MaxY * scale;

            return ViewportTransform(scale, offsetX, offsetY);
        }

    private:
        ViewportTransform _transform;
    };

}
