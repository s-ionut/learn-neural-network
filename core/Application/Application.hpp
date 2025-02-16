#pragma once

#include "raylib-cpp.hpp"

#include "NeuralNetwork/NeuralNetwork.hpp"

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

    NeuralNetwork m_nn;

    raylib::Window m_window;
};