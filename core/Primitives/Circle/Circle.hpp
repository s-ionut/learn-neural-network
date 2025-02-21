#pragma once

#include "raylib-cpp.hpp"

#include "Pixel/Pixel.hpp"
#include "Canvas/Canvas.hpp"

namespace primitive
{
    class Circle
    {
    public:
        Circle(double originX, double originY, double radius, raylib::Color fillColor, raylib::Color borderColor);
        ~Circle() = default;

        void Update();

        raylib::Vector2 GetPosition() const;

        void SetPosition(double originX, double originY);
        void SetRadius(double radius);
        void SetColor(raylib::Color fillColor, raylib::Color borderColor);

    private:
        void GeneratePixels();

        double m_radius;
        double m_originX;
        double m_originY;

        raylib::Color m_fillColor;
        raylib::Color m_borderColor;
    };
}; // namespace primitive