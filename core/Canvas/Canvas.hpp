#pragma once

#include "raylib-cpp.hpp"

class Canvas
{
public:
    Canvas(const Canvas &) = delete;
    Canvas &operator=(const Canvas &) = delete;

    static Canvas &getInstance();
    static raylib::Image &getImageInst();
    static raylib::Texture &getTextureInst();

    void DrawPixel(int x, int y, raylib::Color color);
    void DrawText(std::string text, int posX, int posY, int fontSize, raylib::Color color);
    void Resize(int width, int height);

    void Update();
    void Draw(raylib::Vector2 offset, double scale);

private:
    Canvas();
    ~Canvas();

    raylib::Image m_image;
    raylib::Texture m_texture;
};