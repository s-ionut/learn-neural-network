#include "raylib-cpp.hpp"

namespace primitive
{
    constexpr int PIXEL_SIZE = 1;

    class Pixel : raylib::Rectangle
    {
    public:
        Pixel(double posX, double posY, raylib::Color color);
        ~Pixel() = default;

        void Draw() const;

    private:
        raylib::Color m_color;
    };
} // namespace primitive