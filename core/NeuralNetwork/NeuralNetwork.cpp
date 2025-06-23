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
    
    // Position neurons
    for (int i = 0; i < m_numLayers; i++)
    {
        NeuronColor layerColor = (i < m_numLayers - 1 && i > 0) ? m_layerColorMapping[1] : m_layerColorMapping[0];

        double layerX = m_graphMinX + (i + 1) * horizontalSpacing;
        size_t numNeurons = m_layerSizes[i];

        // Adaptive sizing and spacing based on layer size
        double availableHeight = m_graphMaxY - m_graphMinY;
        double verticalSpacing, neuronRadius;
        
        if (numNeurons > 50) 
        {
            // For large layers (like input layer), make neurons very small and pack tightly
            verticalSpacing = availableHeight / (numNeurons + 1);
            neuronRadius = std::min(3.0, verticalSpacing * 0.3);
        }
        else if (numNeurons > 20)
        {
            // Medium layers
            verticalSpacing = availableHeight / (numNeurons + 1);
            neuronRadius = std::min(8.0, verticalSpacing * 0.4);
        }
        else
        {
            // Small layers (like output), make them larger and more spaced
            verticalSpacing = availableHeight / (numNeurons + 1);
            neuronRadius = std::min(20.0, verticalSpacing * 0.4);
        }

        // Center neurons in the vertical space
        double layerStartY = m_graphMinY + verticalSpacing;

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
    for (Connection &connection : m_connections)
    {
        connection.Update();
    }
    for (Layer &layer : m_layers)
    {
        layer.Update();
    }
};

void NeuralNetwork::Draw()
{
    for (Layer &layer : m_layers)
    {
        layer.Draw();
    }
};

void NeuralNetwork::DrawDirect()
{
    // Draw connections first (behind neurons)
    for (const Connection &connection : m_connections)
    {
        connection.DrawDirect();
    }
    
    // Draw neurons on top
    for (Layer &layer : m_layers)
    {
        layer.DrawDirect();
    }
};

std::vector<double> NeuralNetwork::Forward(const std::vector<double>& inputs)
{
    SetInputs(inputs);
    ForwardPropagate();
    return GetOutputs();
};

void NeuralNetwork::SetInputs(const std::vector<double>& inputs)
{
    if (inputs.size() != m_layerSizes[0])
    {
        return; // Input size mismatch
    }
    
    // Set input layer values
    for (size_t i = 0; i < inputs.size(); i++)
    {
        m_layers[0].GetNeuron(i).SetValue(inputs[i]);
    }
};

std::vector<double> NeuralNetwork::GetOutputs() const
{
    std::vector<double> outputs;
    int outputLayerIndex = m_numLayers - 1;
    
    for (int i = 0; i < m_layerSizes[outputLayerIndex]; i++)
    {
        outputs.push_back(m_layers[outputLayerIndex].GetNeuron(i).GetValue());
    }
    
    return outputs;
};

void NeuralNetwork::ForwardPropagate()
{
    // Reset all neurons except input layer and mark them inactive
    for (size_t layerIdx = 1; layerIdx < m_numLayers; layerIdx++)
    {
        for (int neuronIdx = 0; neuronIdx < m_layerSizes[layerIdx]; neuronIdx++)
        {
            m_layers[layerIdx].GetNeuron(neuronIdx).ResetInputs();
            m_layers[layerIdx].GetNeuron(neuronIdx).SetIsActive(false);
        }
    }
    
    // Mark input layer as active
    for (int neuronIdx = 0; neuronIdx < m_layerSizes[0]; neuronIdx++)
    {
        m_layers[0].GetNeuron(neuronIdx).SetIsActive(true);
    }
    
    // Forward propagate through layers
    for (size_t layerIdx = 0; layerIdx < m_numLayers - 1; layerIdx++)
    {
        for (int fromNeuron = 0; fromNeuron < m_layerSizes[layerIdx]; fromNeuron++)
        {
            double fromValue = m_layers[layerIdx].GetNeuron(fromNeuron).GetValue();
            
            for (int toNeuron = 0; toNeuron < m_layerSizes[layerIdx + 1]; toNeuron++)
            {
                int connIdx = GetConnectionIndex(layerIdx, fromNeuron, layerIdx + 1, toNeuron);
                if (connIdx >= 0)
                {
                    double weight = m_connections[connIdx].GetWeight();
                    m_layers[layerIdx + 1].GetNeuron(toNeuron).AddInput(fromValue * weight);
                }
            }
        }
        
        // Calculate outputs for next layer and mark as active
        for (int neuronIdx = 0; neuronIdx < m_layerSizes[layerIdx + 1]; neuronIdx++)
        {
            m_layers[layerIdx + 1].GetNeuron(neuronIdx).CalculateOutput();
            m_layers[layerIdx + 1].GetNeuron(neuronIdx).SetIsActive(true);
        }
    }
};

int NeuralNetwork::GetConnectionIndex(int fromLayer, int fromNeuron, int toLayer, int toNeuron) const
{
    int index = 0;
    
    // Calculate the index based on the connection structure
    for (int i = 0; i < fromLayer; i++)
    {
        index += m_layerSizes[i] * m_layerSizes[i + 1];
    }
    
    index += fromNeuron * m_layerSizes[toLayer] + toNeuron;
    
    if (index >= 0 && index < m_connections.size())
    {
        return index;
    }
    
    return -1;
};

Connection& NeuralNetwork::GetConnection(int fromLayer, int fromNeuron, int toLayer, int toNeuron)
{
    int index = GetConnectionIndex(fromLayer, fromNeuron, toLayer, toNeuron);
    return m_connections[index];
};

void NeuralNetwork::Train(const std::vector<double>& inputs, const std::vector<double>& targets, double learningRate)
{
    // Forward pass
    Forward(inputs);
    
    // Backward pass
    Backpropagate(targets);
    
    // Update weights
    UpdateWeights(learningRate);
};

double NeuralNetwork::CalculateLoss(const std::vector<double>& outputs, const std::vector<double>& targets) const
{
    double loss = 0.0;
    for (size_t i = 0; i < outputs.size(); i++)
    {
        double diff = targets[i] - outputs[i];
        loss += diff * diff;
    }
    return loss * 0.5; // Mean squared error
};

void NeuralNetwork::Backpropagate(const std::vector<double>& targets)
{
    // Calculate output layer errors
    CalculateOutputErrors(targets);
    
    // Calculate hidden layer errors (backwards)
    CalculateHiddenErrors();
    
    // Calculate gradients for all neurons
    for (size_t layerIdx = 1; layerIdx < m_numLayers; layerIdx++)
    {
        for (int neuronIdx = 0; neuronIdx < m_layerSizes[layerIdx]; neuronIdx++)
        {
            m_layers[layerIdx].GetNeuron(neuronIdx).CalculateGradient();
        }
    }
};

void NeuralNetwork::CalculateOutputErrors(const std::vector<double>& targets)
{
    int outputLayerIdx = m_numLayers - 1;
    
    for (int i = 0; i < m_layerSizes[outputLayerIdx]; i++)
    {
        double output = m_layers[outputLayerIdx].GetNeuron(i).GetValue();
        double target = (i < targets.size()) ? targets[i] : 0.0;
        double error = target - output;
        m_layers[outputLayerIdx].GetNeuron(i).SetError(error);
    }
};

void NeuralNetwork::CalculateHiddenErrors()
{
    // Propagate errors backwards through hidden layers
    for (int layerIdx = m_numLayers - 2; layerIdx >= 1; layerIdx--)
    {
        for (int neuronIdx = 0; neuronIdx < m_layerSizes[layerIdx]; neuronIdx++)
        {
            double error = 0.0;
            
            // Sum weighted errors from next layer
            for (int nextNeuronIdx = 0; nextNeuronIdx < m_layerSizes[layerIdx + 1]; nextNeuronIdx++)
            {
                int connIdx = GetConnectionIndex(layerIdx, neuronIdx, layerIdx + 1, nextNeuronIdx);
                if (connIdx >= 0)
                {
                    double nextError = m_layers[layerIdx + 1].GetNeuron(nextNeuronIdx).GetError();
                    double weight = m_connections[connIdx].GetWeight();
                    error += nextError * weight;
                }
            }
            
            m_layers[layerIdx].GetNeuron(neuronIdx).SetError(error);
        }
    }
};

void NeuralNetwork::UpdateWeights(double learningRate)
{
    // Update all connection weights
    for (size_t layerIdx = 0; layerIdx < m_numLayers - 1; layerIdx++)
    {
        for (int fromNeuronIdx = 0; fromNeuronIdx < m_layerSizes[layerIdx]; fromNeuronIdx++)
        {
            for (int toNeuronIdx = 0; toNeuronIdx < m_layerSizes[layerIdx + 1]; toNeuronIdx++)
            {
                int connIdx = GetConnectionIndex(layerIdx, fromNeuronIdx, layerIdx + 1, toNeuronIdx);
                if (connIdx >= 0)
                {
                    double fromValue = m_layers[layerIdx].GetNeuron(fromNeuronIdx).GetValue();
                    double toGradient = m_layers[layerIdx + 1].GetNeuron(toNeuronIdx).GetGradient();
                    
                    double weightDelta = learningRate * toGradient * fromValue;
                    double newWeight = m_connections[connIdx].GetWeight() + weightDelta;
                    m_connections[connIdx].SetWeight(newWeight);
                }
            }
        }
    }
};