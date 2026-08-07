#include "../head/renderer.h"

#include <iostream>

namespace FGames::core
{

Renderer::Renderer(int width, int height)
    : width_(width),
      height_(height),
      buffer_(height, std::string(width, ' '))
{
}

void Renderer::clear() 
{
    for (auto& row : buffer_)
    {
        row.assign(width_, ' ');
    }
}

void Renderer::draw(int x, int y, char symbol)
{
    if (x < 0 || x >= width_)
        return;

    if (y < 0 || y >= height_)
        return;

    buffer_[y][x] = symbol;
}

void Renderer::present()
{
    // очистить экран консоли
    std::cout << "\033[2J";
    // вернуть каретку в начальную позицию
    std::cout << "\033[H";

    for (const auto& row : buffer_)
    {
        std::cout << row << '\n';
    }

    std::cout.flush();
}

}