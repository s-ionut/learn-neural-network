#include "Neuron.hpp"

void Neuron::SetColor(const NeuronColor neuronColor)
{
    Circle::SetColor(neuronColor.fillColor, neuronColor.fillColor);
}

void Neuron::Update()
{
    Circle::Update();
}

void Neuron::Draw() const
{
    Circle::Draw();
}