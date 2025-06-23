#pragma once

#include "Canvas/Canvas.hpp"

class Line
{
public:
    Line(double startX, double startY, double endX, double endY);
    ~Line() = default;

    void Update();

    void SetColor(raylib::Color lineColor);
    
    // Getters for direct rendering
    double GetStartX() const { return m_startX; }
    double GetStartY() const { return m_startY; }
    double GetEndX() const { return m_endX; }
    double GetEndY() const { return m_endY; }
    raylib::Color GetColor() const { return m_lineColor; }

private:
    void GeneratePixels();

private:
    double m_startX;
    double m_startY;
    double m_endX;
    double m_endY;

    raylib::Color m_lineColor = raylib::Color::White();
};