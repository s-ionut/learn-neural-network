#include "Neuron/Neuron.hpp"

#include <vector>

class Layer
{
public:
    Layer(size_t layerSize);
    ~Layer() = default;

    void Draw();
    void Update();

    Neuron &GetNeuron(const int position);

private:
    size_t m_layerSize = 0;
    std::vector<Neuron> m_neurons;
    std::vector<double> m_weights;
};