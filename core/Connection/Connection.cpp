#include "Connection.hpp"

Connection::Connection(double startX, double startY, double endX, double endY) : Line(startX, startY, endX, endY)
{
    Line::SetColor(raylib::Color::White());
};

void Connection::SetWeight(double weight)
{
    m_weight = weight;
};

double Connection::GetWeight() const
{
    return m_weight;
};

void Connection::SetConnection(NConn fromNeuron, NConn toNeuron)
{
    m_fromNeuron = fromNeuron;
    m_toNeuron = toNeuron;
};

void Connection::DrawDirect() const
{
    // Draw line directly with Raylib
    raylib::Vector2 start = {static_cast<float>(Line::GetStartX()), static_cast<float>(Line::GetStartY())};
    raylib::Vector2 end = {static_cast<float>(Line::GetEndX()), static_cast<float>(Line::GetEndY())};
    
    // Color and thickness based on weight strength
    raylib::Color lineColor = GetWeightColor();
    float thickness = std::min(3.0f, std::max(1.0f, static_cast<float>(std::abs(m_weight) * 2.0)));
    
    DrawLineEx(start, end, thickness, lineColor);
};

raylib::Color Connection::GetWeightColor() const
{
    // Color based on weight strength and sign
    float absWeight = static_cast<float>(std::abs(m_weight));
    float intensity = std::min(absWeight, 1.0f);
    
    if (m_weight > 0)
    {
        // Positive weights: green (excitatory)
        return raylib::Color{static_cast<unsigned char>(50 * (1.0f - intensity)), 
                           static_cast<unsigned char>(255 * intensity), 
                           static_cast<unsigned char>(50 * (1.0f - intensity)), 
                           static_cast<unsigned char>(150 + 105 * intensity)};
    }
    else
    {
        // Negative weights: red (inhibitory)
        return raylib::Color{static_cast<unsigned char>(255 * intensity), 
                           static_cast<unsigned char>(50 * (1.0f - intensity)), 
                           static_cast<unsigned char>(50 * (1.0f - intensity)), 
                           static_cast<unsigned char>(150 + 105 * intensity)};
    }
};
