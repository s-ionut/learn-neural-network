#include "raylib-cpp.hpp"

#include <string>

class Application
{
public:
    Application(int width, int height, std::string_view windowName);
    void Run();

private:
    void Update();
    void Render();

private:
    int m_width;
    int m_height;
    std::string_view m_windowName;

    raylib::Window m_window;
};