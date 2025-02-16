#include "raylib-cpp.hpp"

#include "Circle/Circle.hpp"

struct NeuronColor
{
    raylib::Color fillColor;
    raylib::Color borderColor;
};

class Neuron : public primitive::Circle
{
public:
    Neuron() : Circle(100, 100, 30, raylib::Color::Gray(), raylib::Color::White()) {};
    ~Neuron() = default;

    void SetColor(const NeuronColor neuronColor);

private:
    double weight;
    double bias;
};
