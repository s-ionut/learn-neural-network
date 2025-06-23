#include "Neuron.hpp"
#include <iostream>
#include <algorithm>
void Neuron::SetColor(const NeuronColor neuronColor)
{
    Circle::SetColor(neuronColor.fillColor, neuronColor.fillColor);
};

void Neuron::Update()
{
    std::ostringstream trValue;
    trValue << std::fixed << std::setprecision(2) << m_value;
    m_text = trValue.str();
    // No longer draw to canvas - using direct rendering in DrawDirect()
};

void Neuron::Draw() const {
    // TODO: fix this shit workaround
    //       this helps with drawing order
    //       (so text isn't overwritten by the actual neuron cicle)
};

void Neuron::DrawDirect() const
{
    raylib::Vector2 pos = Circle::GetPosition();
    double radius = GetRadius();
    
    // Draw circle with activity-based color
    raylib::Color fillColor = GetActivityColor();
    DrawCircleV(pos, static_cast<float>(radius), fillColor);
    
    // Border color based on activity
    raylib::Color borderColor = m_isActive ? raylib::Color::White() : raylib::Color::DarkGray();
    DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<float>(radius), borderColor);
    
    // Draw text if neuron is large enough
    if (radius >= 4.0)
    {
        int fontSize = static_cast<int>(radius * 0.8);
        fontSize = std::max(1, std::min(fontSize, 20)); // Allow larger text for zoom
        
        // Center text better
        int textWidth = MeasureText(m_text.c_str(), fontSize);
        int textX = static_cast<int>(pos.x) - textWidth / 2;
        int textY = static_cast<int>(pos.y) - fontSize / 2;
        
        DrawText(m_text.c_str(), textX, textY, fontSize, BLACK);
    }
};

void Neuron::SetValue(double value)
{
    m_value = value;
};

double Neuron::GetValue() const
{
    return m_value;
};

void Neuron::AddInput(double input)
{
    m_inputSum += input;
};

void Neuron::CalculateOutput()
{
    // Apply selected activation function to input sum + bias
    m_value = ApplyActivation(m_inputSum + m_bias);
};

void Neuron::ResetInputs()
{
    m_inputSum = 0;
};

double Neuron::Sigmoid(double x) const
{
    return 1.0 / (1.0 + std::exp(-x));
};

double Neuron::ReLU(double x) const
{
    return std::max(0.0, x);
};

double Neuron::LeakyReLU(double x, double alpha) const
{
    return x > 0 ? x : alpha * x;
};

double Neuron::ELU(double x, double alpha) const
{
    return x > 0 ? x : alpha * (std::exp(x) - 1.0);
};

double Neuron::Tanh(double x) const
{
    return std::tanh(x);
};

void Neuron::SetActivationType(ActivationType type)
{
    m_activationType = type;
};

Neuron::ActivationType Neuron::GetActivationType() const
{
    return m_activationType;
};

double Neuron::ApplyActivation(double x) const
{
    switch (m_activationType)
    {
        case ActivationType::SIGMOID: return Sigmoid(x);
        case ActivationType::RELU: return ReLU(x);
        case ActivationType::LEAKY_RELU: return LeakyReLU(x);
        case ActivationType::ELU: return ELU(x);
        case ActivationType::TANH: return Tanh(x);
        default: return ReLU(x);
    }
};

double Neuron::ApplyActivationDerivative(double x) const
{
    switch (m_activationType)
    {
        case ActivationType::SIGMOID: return SigmoidDerivative(x);
        case ActivationType::RELU: return x > 0 ? 1.0 : 0.0;
        case ActivationType::LEAKY_RELU: return x > 0 ? 1.0 : 0.01;
        case ActivationType::ELU: return x > 0 ? 1.0 : ELU(x) + 1.0;
        case ActivationType::TANH: return 1.0 - x * x; // derivative of tanh
        default: return x > 0 ? 1.0 : 0.0;
    }
};

void Neuron::SetBatchNormalization(bool enabled)
{
    m_batchNormEnabled = enabled;
};

bool Neuron::IsBatchNormEnabled() const
{
    return m_batchNormEnabled;
};

void Neuron::ApplyBatchNorm(double mean, double variance, double epsilon)
{
    if (m_batchNormEnabled)
    {
        // Normalize: (x - mean) / sqrt(variance + epsilon)
        m_value = (m_value - mean) / std::sqrt(variance + epsilon);
    }
};

void Neuron::SetError(double error)
{
    m_error = error;
};

double Neuron::GetError() const
{
    return m_error;
};

void Neuron::CalculateGradient()
{
    // Gradient = error * derivative of activation function
    m_gradient = m_error * ApplyActivationDerivative(m_value);
};

double Neuron::GetGradient() const
{
    return m_gradient;
};

double Neuron::SigmoidDerivative(double x) const
{
    return x * (1.0 - x); // Since x is already sigmoid(z), derivative is x*(1-x)
};

double Neuron::GetRadius() const
{
    return Circle::GetRadius();
};

void Neuron::SetIsActive(bool active)
{
    m_isActive = active;
};

bool Neuron::IsActive() const
{
    return m_isActive;
};

raylib::Color Neuron::GetActivityColor() const
{
    // Color based on activity and value
    if (m_isActive)
    {
        // Active neurons: color intensity based on activation value
        float intensity = static_cast<float>(std::abs(m_value));
        intensity = std::min(intensity, 1.0f);
        
        if (m_value > 0.5)
        {
            // High activation: bright green to yellow
            return raylib::Color{static_cast<unsigned char>(255 * intensity), 
                               255, 
                               static_cast<unsigned char>(100 * (1.0f - intensity)), 
                               255};
        }
        else
        {
            // Low activation: blue to cyan
            return raylib::Color{static_cast<unsigned char>(100 * intensity), 
                               static_cast<unsigned char>(200 * intensity), 
                               255, 
                               255};
        }
    }
    else
    {
        // Inactive neurons: gray
        return raylib::Color::Gray();
    }
};