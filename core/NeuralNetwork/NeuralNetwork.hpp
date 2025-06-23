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
    void DrawDirect(); // Direct vector rendering for zoom/pan

    void Resize(double graphMinX, double graphMinY, double graphMaxX, double graphMaxY);
    
    // Neural network functionality
    std::vector<double> Forward(const std::vector<double>& inputs);
    void SetInputs(const std::vector<double>& inputs);
    std::vector<double> GetOutputs() const;
    Connection& GetConnection(int fromLayer, int fromNeuron, int toLayer, int toNeuron);
    
    // Training functionality
    void Train(const std::vector<double>& inputs, const std::vector<double>& targets, double learningRate = 0.01);
    double CalculateLoss(const std::vector<double>& outputs, const std::vector<double>& targets) const;
    void Backpropagate(const std::vector<double>& targets);
    void UpdateWeights(double learningRate);

private:
    void SetConnections();
    void ForwardPropagate();
    int GetConnectionIndex(int fromLayer, int fromNeuron, int toLayer, int toNeuron) const;
    void CalculateOutputErrors(const std::vector<double>& targets);
    void CalculateHiddenErrors();

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