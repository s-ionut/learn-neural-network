#pragma once

#include "Line/Line.hpp"

class Connection : public Line
{
public:
    Connection(double startX, double startY, double endX, double endY);
    ~Connection() = default;

    void SetWeight(double weight);

private:
    double m_weight;
};