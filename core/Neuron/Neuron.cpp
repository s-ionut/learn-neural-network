#include "Neuron.hpp"
#include <iostream>
void Neuron::SetColor(const NeuronColor neuronColor)
{
    Circle::SetColor(neuronColor.fillColor, neuronColor.fillColor);
};

void Neuron::Update()
{
    std::ostringstream trValue;
    trValue << std::fixed << std::setprecision(2) << m_value;
    m_text = trValue.str();
    Circle::Update();
    Canvas::getInstance().DrawText(m_text, static_cast<int>(Circle::GetPosition().x) - 8, static_cast<int>(Circle::GetPosition().y) - 3, 12, raylib::Color::Black());
};

void Neuron::Draw() const {
    // TODO: fix this shit workaround
    //       this helps with drawing order
    //       (so text isn't overwritten by the actual neuron cicle)
};