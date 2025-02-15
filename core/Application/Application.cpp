#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName}),
      m_circle(100, 100, 30, raylib::Color(150, 150, 150), raylib::Color())
{
    SetTargetFPS(60);
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
    // TODO
}

void Application::Render()
{
    BeginDrawing();

    m_window.ClearBackground(RAYWHITE);

    m_circle.Draw();

    EndDrawing();
}