#include "raylib-cpp.hpp"

#include "Circle/Circle.hpp"

class Neuron : public primitive::Circle
{
public:
    Neuron() : Circle(100, 100, 30, raylib::Color::Gray(), raylib::Color::White()) {};
    ~Neuron() = default;

private:
    double weight;
    double bias;
};
