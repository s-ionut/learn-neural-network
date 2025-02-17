#include "Connection.hpp"

Connection::Connection(double startX, double startY, double endX, double endY) : Line(startX, startY, endX, endY)
{
    Line::SetColor(raylib::Color::White());
};

void Connection::SetWeight(double weight)
{
    m_weight = weight;
};

void Connection::SetConnection(NConn fromNeuron, NConn toNeuron)
{
    m_fromNeuron = fromNeuron;
    m_toNeuron = toNeuron;
};
