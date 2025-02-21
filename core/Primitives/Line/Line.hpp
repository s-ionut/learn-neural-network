#pragma once

#include "Canvas/Canvas.hpp"

class Line
{
public:
    Line(double startX, double startY, double endX, double endY);
    ~Line() = default;

    void Update();

    void SetColor(raylib::Color lineColor);

private:
    void GeneratePixels();

private:
    double m_startX;
    double m_startY;
    double m_endX;
    double m_endY;

    raylib::Color m_lineColor = raylib::Color::White();
};