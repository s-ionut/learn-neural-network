#include "Pixel.hpp"

using namespace primitive;

void Pixel::Draw() const
{
    raylib::Rectangle::Draw(m_color);
};