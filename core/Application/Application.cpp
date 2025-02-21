#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName}),
      m_nn(5, std::vector<int>{2, 3, 2, 13, 1})
{
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);

    int canvasWidth = 1000;
    int canvasHeight = 800;

    Canvas::getInstance().Resize(canvasWidth, canvasHeight);
    m_nn.Resize(0, 0, canvasWidth, canvasHeight);
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
    // -- Handle Zoom --
    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0.0f)
    {
        m_scale += mouseWheel * 0.1f;
        if (m_scale < 0.1f)
            m_scale = 0.1f;
    }

    // -- Handle Click & Drag to Pan --
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        m_dragging = true;
        m_lastMousePosition = GetMousePosition();
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        m_dragging = false;
    }

    if (m_dragging)
    {
        raylib::Vector2 currentMousePosition = GetMousePosition();

        raylib::Vector2 delta = currentMousePosition - m_lastMousePosition;

        m_offset += delta;

        m_lastMousePosition = currentMousePosition;
    }

    m_nn.Update();
    Canvas::getInstance().Update();
};

void Application::Render()
{

    BeginDrawing();

    m_window.ClearBackground(raylib::Color::Black());

    Canvas::getInstance().Draw(m_offset, m_scale);
    m_nn.Draw();

    EndDrawing();
};