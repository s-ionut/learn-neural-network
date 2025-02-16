#pragma once

#include "raylib-cpp.hpp"

namespace primitive
{
    const int PIXEL_SIZE = 1;

    class Pixel : public raylib::Rectangle
    {
    public:
        Pixel(double posX, double posY, raylib::Color color)
            : raylib::Rectangle(static_cast<float>(posX), static_cast<float>(posY), PIXEL_SIZE, PIXEL_SIZE),
              m_color{color} {};
        ~Pixel() = default;

        void Draw() const;

    private:
        raylib::Color m_color;
    };
}; // namespace primitive