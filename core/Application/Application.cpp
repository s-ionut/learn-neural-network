#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName) : m_width{width},
                                                                               m_height{height},
                                                                               m_windowName{windowName},
                                                                               m_window(m_width, m_height, std::string{m_windowName})
{
    SetTargetFPS(60);
};

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        BeginDrawing();

        m_window.ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
};