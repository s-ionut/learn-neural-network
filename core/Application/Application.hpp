#include "raylib-cpp.hpp"

#include "Neuron/Neuron.hpp"

#include <string>

class Application
{
public:
    Application(int width, int height, std::string_view windowName);
    ~Application() = default;
    void Run();

private:
    void Update();
    void Render();

private:
    int m_width;
    int m_height;
    std::string_view m_windowName;

    std::vector<Neuron> m_neurons;

    raylib::Window m_window;
};