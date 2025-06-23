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
    
    // Activation function control
    enum class ActivationType { SIGMOID, RELU, LEAKY_RELU, ELU, TANH };
    void SetActivationType(ActivationType type);
    ActivationType GetActivationType() const;
    
    // Batch normalization
    void SetBatchNormalization(bool enabled);
    bool IsBatchNormEnabled() const;
    void ApplyBatchNorm(double mean, double variance, double epsilon = 1e-8);
    
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
    bool m_batchNormEnabled = false;
    ActivationType m_activationType = ActivationType::RELU;
    std::string m_text = "";
    
    double Sigmoid(double x) const;
    double SigmoidDerivative(double x) const;
    double ReLU(double x) const;
    double LeakyReLU(double x, double alpha = 0.01) const;
    double ELU(double x, double alpha = 1.0) const;
    double Tanh(double x) const;
    
    double ApplyActivation(double x) const;
    double ApplyActivationDerivative(double x) const;
};
