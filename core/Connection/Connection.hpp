#pragma once

#include "Helper/Helper.hpp"
#include "Line/Line.hpp"

struct NConn
{
    int layer;
    int neuron;
};

class Connection : public Line
{
public:
    Connection(double startX, double startY, double endX, double endY);
    ~Connection() = default;

    void SetWeight(double weight);
    void SetConnection(NConn fromNeuron, NConn toNeuron);

private:
    double m_weight = helper::GetRandom();

    NConn m_fromNeuron;
    NConn m_toNeuron;
};