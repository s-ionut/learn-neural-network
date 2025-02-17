#include "Neuron.hpp"

void Neuron::SetColor(const NeuronColor neuronColor)
{
    Circle::SetColor(neuronColor.fillColor, neuronColor.fillColor);
}

void Neuron::Update()
{
    std::ostringstream trValue;
    trValue << std::fixed << std::setprecision(2) << m_value;
    m_text = trValue.str();
    Circle::Update();
}

void Neuron::Draw() const
{
    Circle::Draw();
    DrawText(m_text.c_str(), static_cast<int>(Circle::GetPosition().x) - 8, static_cast<int>(Circle::GetPosition().y) - 3, 1, raylib::Color::Black());
}