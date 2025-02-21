#pragma once

#include "raylib-cpp.hpp"

#include "Canvas/Canvas.hpp"
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

    // Track panning (offset) and zoom (scale)
    raylib::Vector2 m_offset = raylib::Vector2(0.0f, 0.0f);
    float m_scale = 1.0f;

    // For click-and-drag
    bool m_dragging = false;
    raylib::Vector2 m_lastMousePosition = {0.0f, 0.0f};

    std::string_view m_windowName;

    NeuralNetwork m_nn;

    raylib::Window m_window;
};