#pragma once

#include "Layer/Layer.hpp"
#include "Connection/Connection.hpp"

#include <vector>
#include <algorithm>

class NeuralNetwork
{
public:
    NeuralNetwork(const size_t numLayers, const std::vector<int> &layerSizes);
    ~NeuralNetwork() = default;

    void Update();
    void Draw();

    void Resize(double graphMinX, double graphMinY, double graphMaxX, double graphMaxY);

private:
    void SetConnections();

private:
    size_t m_numLayers;
    std::vector<int> m_layerSizes;

    std::vector<Layer> m_layers;
    std::vector<Connection> m_connections;

    double m_graphMinX = 0.;
    double m_graphMinY = 0.;
    double m_graphMaxX = 400.;
    double m_graphMaxY = 400.;

private:
    std::vector<NeuronColor> m_layerColorMapping =
        {
            {
                raylib::Color::White(),
                raylib::Color::Gray(),
            }, // INPUT LAYER
            {
                raylib::Color::Green(),
                raylib::Color::Black(),
            }, // HIDDEN LAYER
    };
};