#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName}),
      m_nn(5, std::vector<int>{2, 3, 2, 13, 1})
{
    SetTargetFPS(60);

    m_nn.Resize(0, 0, m_width, m_height);
};

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Render();
    }
};

void Application::Update()
{
    m_nn.Update();
};

void Application::Render()
{
    BeginDrawing();

    m_window.ClearBackground(raylib::Color::Black());

    m_nn.Draw();

    EndDrawing();
};