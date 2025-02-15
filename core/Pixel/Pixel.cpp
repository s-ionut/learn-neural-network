#include "Pixel.hpp"

using namespace primitive;

Pixel::Pixel(double posX, double posY, raylib::Color color)
    : raylib::Rectangle(posX, posY, PIXEL_SIZE, PIXEL_SIZE),
      m_color{color} {};

void Pixel::Draw() const
{
    raylib::Rectangle::Draw(m_color);
}