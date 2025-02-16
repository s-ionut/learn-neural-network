#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName})
{
    SetTargetFPS(60);

    m_neurons.push_back(Neuron());
};

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Render();
    }
};

void Application::Update() {
    // TODO
};

void Application::Render()
{
    BeginDrawing();

    m_window.ClearBackground(raylib::Color::Black());

    m_neurons.back().Draw();

    EndDrawing();
};