#include "Neuron.hpp"

void Neuron::SetColor(const NeuronColor neuronColor)
{
    Circle::SetColor(neuronColor.fillColor, neuronColor.fillColor);
}