#include "Application.hpp"

Application::Application(int width, int height, std::string_view windowName)
    : m_width{width},
      m_height{height},
      m_windowName{windowName},
      m_window(m_width, m_height, std::string{m_windowName}),
      m_nn(4, std::vector<int>{16, 3, 2, 10})
{
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);

    int canvasWidth = 1400;
    int canvasHeight = 1000;

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

    // Run neural network with sample data
    RunNeuralNetworkDemo();

    m_nn.Update();
    // Canvas no longer needed for direct rendering
};

void Application::RunNeuralNetworkDemo()
{
    // Train neural network with sample data every few frames
    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter % 2 == 0) // Every 1 second at 60 FPS
    {
        TrainWithSampleData();
    }
};

void Application::TrainWithSampleData()
{
    // Generate random "handwritten digit" data for demonstration
    std::vector<double> inputs = GenerateRandomImage();

    // Random target digit (0-9)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 9);
    int targetDigit = dis(gen);

    std::vector<double> targets = CreateTargetVector(targetDigit);

    // Train the network
    m_nn.Train(inputs, targets, 0.01);

    // Also run forward pass to see current output
    std::vector<double> outputs = m_nn.Forward(inputs);
};

std::vector<double> Application::GenerateRandomImage()
{
    // Generate random input data for 16-input network
    std::vector<double> inputs(16);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < 16; i++)
    {
        inputs[i] = dis(gen);
    }

    return inputs;
};

std::vector<double> Application::CreateTargetVector(int digit)
{
    // Create one-hot encoded target vector for digits 0-9
    std::vector<double> target(10, 0.0);
    if (digit >= 0 && digit <= 9)
    {
        target[digit] = 1.0;
    }
    return target;
};

void Application::Render()
{
    BeginDrawing();

    m_window.ClearBackground(raylib::Color::Black());

    // Apply camera transform for zoom/pan
    BeginMode2D(raylib::Camera2D{m_offset, {0, 0}, 0.0f, m_scale});

    // Draw neural network with vector graphics (not canvas)
    m_nn.DrawDirect();

    EndMode2D();

    EndDrawing();
};