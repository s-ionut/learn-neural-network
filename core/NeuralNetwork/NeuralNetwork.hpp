#include "Neuron/Neuron.hpp"

#include <vector>

class NeuralNetwork
{
public:
    NeuralNetwork(const int inputLayer, const int hiddenLayer, const int outputLayer);
    ~NeuralNetwork() = default;

    void Update();
    void Draw();

    void Resize(double graphMinX, double graphMinY, double graphMaxX, double graphMaxY);

private:
    int m_inputLayer;
    int m_hiddenLayer;
    int m_outputLayer;

    std::vector<std::vector<Neuron>> m_layers;

    double m_graphMinX = 0.;
    double m_graphMinY = 0.;
    double m_graphMaxX = 400.;
    double m_graphMaxY = 400.;

    double m_verticalSpacing = 20.;
    double m_horizontalSpacing = 30.;
};