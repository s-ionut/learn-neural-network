#include "raylib-cpp.hpp"

#include "Pixel/Pixel.hpp"

namespace primitive
{
    class Circle
    {
    public:
        Circle(double originX, double originY, double radius, raylib::Color fillColor, raylib::Color borderColor);
        ~Circle() = default;

        void Update();
        void Draw() const;

        void SetPosition(double originX, double originY);
        void SetRadius(double radius);

    private:
        void GeneratePixels();

        double m_radius;
        double m_originX;
        double m_originY;

        raylib::Color m_fillColor;
        raylib::Color m_borderColor;
        std::vector<primitive::Pixel> m_pixels;
    };
} // namespace primitive