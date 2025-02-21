#include "Line.hpp"
#include <iostream>

Line::Line(double startX, double startY, double endX, double endY)
    : m_startX(startX),
      m_startY(startY),
      m_endX(endX),
      m_endY(endY) {};

void Line::GeneratePixels()
{
    Canvas &canvas = Canvas::getInstance();
    int startX = static_cast<int>(m_startX);
    int endX = static_cast<int>(m_endX);
    int startY = static_cast<int>(m_startY);
    int endY = static_cast<int>(m_endY);

    int dx = std::abs(endX - startX);
    int sx = (startX < endX) ? 1 : -1;
    int dy = -std::abs(endY - startY);
    int sy = (startY < endY) ? 1 : -1;

    int err = dx + dy;
    int e2; // Initial error term

    while (true)
    {
        canvas.DrawPixel(startX, startY, m_lineColor);

        if (startX == endX && startY == endY)
            break; // Stop when reaching endpoint

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            startX += sx;
        } // Move in x direction

        if (e2 <= dx)
        {
            err += dx;
            startY += sy;
        } // Move in y direction
    }
}

void Line::SetColor(raylib::Color lineColor)
{
    m_lineColor = lineColor;
};

void Line::Update()
{
    // Use Bresenham’s Line Algorithm (integer based)
    GeneratePixels();
};