#pragma once

#include "raylib-cpp.hpp"

#include "Helper/Helper.hpp"
#include "Circle/Circle.hpp"
#include "Canvas/Canvas.hpp"

#include <iomanip>
#include <sstream>
#include <cmath>
#include <vector>

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
    void DrawDirect() const;

    void SetColor(const NeuronColor neuronColor);
    void SetValue(double value);
    double GetValue() const;
    void AddInput(double input);
    void CalculateOutput();
    void ResetInputs();
    
    // Training functionality
    void SetError(double error);
    double GetError() const;
    void CalculateGradient();
    double GetGradient() const;
    double GetRadius() const;
    
    // Activity visualization
    void SetIsActive(bool active);
    bool IsActive() const;
    raylib::Color GetActivityColor() const;

private:
    double m_bias = 0;
    double m_value = 0;
    double m_inputSum = 0;
    double m_error = 0;
    double m_gradient = 0;
    bool m_isActive = false;
    std::string m_text = "";
    
    double Sigmoid(double x) const;
    double SigmoidDerivative(double x) const;
    double ReLU(double x) const;
};
