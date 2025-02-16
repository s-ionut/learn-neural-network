#pragma once

#include "Pixel/Pixel.hpp"

#include <vector>

class Line
{
public:
    Line(double startX, double startY, double endX, double endY);
    ~Line() = default;

    void Update();
    void Draw() const;

    void SetColor(raylib::Color lineColor);

private:
    void GeneratePixels();

private:
    double m_startX;
    double m_startY;
    double m_endX;
    double m_endY;

    std::vector<primitive::Pixel> m_pixels;

    raylib::Color m_lineColor = raylib::Color::White();
};