#include "NeuralNetwork.hpp"
#include <iostream>

NeuralNetwork::NeuralNetwork(const int inputLayer, const int hiddenLayer, const int outputLayer)
    : m_inputLayer{inputLayer},
      m_hiddenLayer{hiddenLayer},
      m_outputLayer{outputLayer}
{
    m_layers.push_back(std::vector<Neuron>(m_inputLayer));
    m_layers.push_back(std::vector<Neuron>(m_hiddenLayer));
    m_layers.push_back(std::vector<Neuron>(m_outputLayer));

    Resize(0, 0, 400, 400);
};

void NeuralNetwork::Resize(double graphMinX, double graphMinY, double graphMaxX, double graphMaxY)
{
    m_graphMinX = graphMinX;
    m_graphMinY = graphMinY;
    m_graphMaxX = graphMaxX;
    m_graphMaxY = graphMaxY;

    size_t numLayers = m_layers.size();
    if (numLayers == 0)
        return;

    size_t maxNeurons = 0;
    for (const auto &layer : m_layers)
    {
        if (layer.size() > maxNeurons)
            maxNeurons = layer.size();
    }

    double horizontalSpacing = (m_graphMaxX - m_graphMinX) / (numLayers + 1);
    double verticalSpacing = (m_graphMaxY - m_graphMinY) / (maxNeurons + 1);

    double neuronRadius = std::min(horizontalSpacing, verticalSpacing) * 0.4;

    // Position neurons
    for (int i = 0; i < numLayers; i++)
    {
        double layerX = m_graphMinX + (i + 1) * horizontalSpacing;
        size_t numNeurons = m_layers[i].size();

        // Center neurons in the vertical space
        double layerStartY = (m_graphMaxY + m_graphMinY) / 2.0 - (numNeurons - 1) * verticalSpacing / 2.0;

        for (int j = 0; j < numNeurons; j++)
        {
            double neuronY = layerStartY + j * verticalSpacing;
            m_layers[i][j].SetPosition(layerX, neuronY);
            m_layers[i][j].SetRadius(neuronRadius);
        }
    }
}

void NeuralNetwork::Update()
{
    for (int i = 0; i < 3; i++)
    {
        for (Neuron &neuron : m_layers[i])
        {
            neuron.Update();
        }
    }
};

void NeuralNetwork::Draw()
{
    for (int i = 0; i < 3; i++)
    {
        for (const Neuron &neuron : m_layers[i])
        {
            neuron.Draw();
        }
    }
};