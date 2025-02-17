#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(const size_t numLayers, const std::vector<int> &layerSizes)
    : m_numLayers{numLayers},
      m_layerSizes{layerSizes}
{
    for (int size : m_layerSizes)
    {
        m_layers.push_back(Layer(size));
    }

    Resize(0, 0, 400, 400);

    SetConnections();
};

void NeuralNetwork::Resize(double graphMinX, double graphMinY, double graphMaxX, double graphMaxY)
{
    m_graphMinX = graphMinX;
    m_graphMinY = graphMinY;
    m_graphMaxX = graphMaxX;
    m_graphMaxY = graphMaxY;

    if (m_numLayers == 0)
        return;

    size_t maxNeurons = 0;
    for (int size : m_layerSizes)
    {
        if (size > maxNeurons)
            maxNeurons = size;
    }

    double horizontalSpacing = (m_graphMaxX - m_graphMinX) / (m_numLayers + 1);
    double verticalSpacing = (m_graphMaxY - m_graphMinY) / (maxNeurons + 1);

    double neuronRadius = std::min(horizontalSpacing, verticalSpacing) * 0.4;

    // Position neurons
    for (int i = 0; i < m_numLayers; i++)
    {
        NeuronColor layerColor = (i < m_numLayers - 1 && i > 0) ? m_layerColorMapping[1] : m_layerColorMapping[0];

        double layerX = m_graphMinX + (i + 1) * horizontalSpacing;
        size_t numNeurons = m_layerSizes[i];

        // Center neurons in the vertical space
        double layerStartY = (m_graphMaxY + m_graphMinY) / 2.0 - (numNeurons - 1) * verticalSpacing / 2.0;

        for (int j = 0; j < numNeurons; j++)
        {
            double neuronY = layerStartY + j * verticalSpacing;
            m_layers[i].GetNeuron(j).SetPosition(layerX, neuronY);
            m_layers[i].GetNeuron(j).SetRadius(neuronRadius);
            m_layers[i].GetNeuron(j).SetColor(layerColor);
        }
    }

    SetConnections();
};

void NeuralNetwork::SetConnections()
{
    m_connections.clear();

    for (int i = 0; i < m_numLayers - 1; i++)
    {
        Layer *currLayer = &m_layers[i];
        Layer *nextLayer = &m_layers[i + 1];
        for (int j = 0; j < m_layerSizes[i]; ++j)
        {
            // set lines and weights
            raylib::Vector2 startPos = currLayer->GetNeuron(j).GetPosition();

            for (int k = 0; k < m_layerSizes[i + 1]; k++)
            {
                raylib::Vector2 endPos = nextLayer->GetNeuron(k).GetPosition();
                m_connections.push_back(Connection(startPos.x, startPos.y, endPos.x, endPos.y));
                m_connections.back().SetConnection(NConn{i, j}, NConn{i + 1, k});
            }
        }
    }
};

void NeuralNetwork::Update()
{
    for (Layer &layer : m_layers)
    {
        layer.Update();
    }
};

void NeuralNetwork::Draw()
{
    for (Connection &connection : m_connections)
    {
        connection.Draw();
    }

    for (Layer &layer : m_layers)
    {
        layer.Draw();
    }
};