#include "Circle.hpp"

using namespace primitive;

Circle::Circle(double originX, double originY, double radius, raylib::Color fillColor, raylib::Color borderColor)
    : m_originX{originX},
      m_originY{originY},
      m_radius{radius},
      m_fillColor{fillColor},
      m_borderColor{borderColor} {};

void Circle::GeneratePixels()
{
    int x = 0;
    int y = static_cast<int>(m_radius);
    int d = 3 - 2 * static_cast<int>(m_radius);

    while (x <= y)
    {
        for (double i = m_originX - x; i <= m_originX + x; i++)
        {
            m_pixels.push_back(primitive::Pixel(i, m_originY + y, m_fillColor));
            m_pixels.push_back(primitive::Pixel(i, m_originY - y, m_fillColor));
        }
        for (double i = m_originX - y; i <= m_originX + y; i++)
        {
            m_pixels.push_back(primitive::Pixel(i, m_originY + x, m_fillColor));
            m_pixels.push_back(primitive::Pixel(i, m_originY - x, m_fillColor));
        }

        m_pixels.push_back(primitive::Pixel(m_originX + x, m_originY + y, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX - x, m_originY + y, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX + x, m_originY - y, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX - x, m_originY - y, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX + y, m_originY + x, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX - y, m_originY + x, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX + y, m_originY - x, m_borderColor));
        m_pixels.push_back(primitive::Pixel(m_originX - y, m_originY - x, m_borderColor));

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            --y;
        }
        ++x;
    }
};

raylib::Vector2 Circle::GetPosition() const
{
    return raylib::Vector2{static_cast<float>(m_originX), static_cast<float>(m_originY)};
};

void Circle::SetPosition(double originX, double originY)
{
    m_originX = originX;
    m_originY = originY;
};

void Circle::SetRadius(double radius)
{
    m_radius = radius;
};

void Circle::SetColor(raylib::Color fillColor, raylib::Color borderColor)
{
    m_fillColor = fillColor;
    m_borderColor = borderColor;
};

void Circle::Update()
{
    // Use Bresenham's circle algorithm
    GeneratePixels();
};

void Circle::Draw() const
{
    for (const auto &pixel : m_pixels)
    {
        pixel.Draw();
    }
};