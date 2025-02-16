#include "Layer.hpp"

Layer::Layer(size_t layerSize)
    : m_layerSize{layerSize},
      m_neurons{m_layerSize, Neuron()} {};

Neuron &Layer::GetNeuron(const int position)
{
    return m_neurons[position];
}

void Layer::Update()
{
    for (Neuron &neuron : m_neurons)
    {
        neuron.Update();
    }
}

void Layer::Draw()
{
    for (Neuron &neuron : m_neurons)
    {
        neuron.Draw();
    }
}