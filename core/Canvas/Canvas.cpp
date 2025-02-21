#include "Canvas.hpp"

Canvas &Canvas::getInstance()
{
    static Canvas instance;
    return instance;
};

raylib::Image &Canvas::getImageInst()
{
    return getInstance().m_image;
};

raylib::Texture &Canvas::getTextureInst()
{
    return getInstance().m_texture;
};

void Canvas::Draw(raylib::Vector2 offset, double scale)
{
    DrawTextureEx(m_texture, offset, 0.0f, scale, raylib::Color::White());
};

void Canvas::Update()
{
    m_texture.Update(m_image.data);
};

void Canvas::Resize(int width, int height)
{
    m_image = raylib::Image(GenImageColor(width, height, raylib::Color::Black()));
    if (m_texture.id > 0)
    {
        m_texture = raylib::Texture(m_image);
    }
};

void Canvas::DrawPixel(int x, int y, raylib::Color color)
{
    if (x >= 0 && x < m_image.width && y >= 0 && y < m_image.height)
    {
        raylib::Color *pixels = static_cast<raylib::Color *>(m_image.data);
        // Row-major: index = (y * width) + x
        int index = y * m_image.width + x;
        pixels[index] = color;
    }
};

void Canvas::DrawText(std::string text, int posX, int posY, int fontSize, raylib::Color color)
{
    ImageDrawText(&m_image, text.c_str(), posX, posY, fontSize, color);
}

Canvas::Canvas()
{
    m_image = raylib::Image(GenImageColor(1000, 1000, raylib::Color::Black()));
    m_texture = raylib::Texture(m_image);
};

Canvas::~Canvas()
{
    UnloadImage(m_image);
    UnloadTexture(m_texture);
};