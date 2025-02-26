#pragma once

#include "raylib-cpp.hpp"

#include "Helper/Helper.hpp"
#include "Circle/Circle.hpp"
#include "Canvas/Canvas.hpp"

#include <iomanip>
#include <sstream>

struct NeuronColor
{
    raylib::Color fillColor;
    raylib::Color borderColor;
};

class Neuron : public primitive::Circle
{
public:
    Neuron() : Circle(100, 100, 30, raylib::Color::Gray(), raylib::Color::White()), m_value{helper::GetRandom()} {};
    ~Neuron() = default;

    void Update();
    void Draw() const;

    void SetColor(const NeuronColor neuronColor);

private:
    double m_bias = 0;
    double m_value = 0;
    std::string m_text = "";
};
