#include "../head/renderer.h"

#include <iostream>

namespace FGames::core
{

Renderer::Renderer(Terminal& terminal, int width, int height)
    : width_(width),
      height_(height),
      terminal_(terminal),
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
    terminal_.moveCursor(0,0);
    std::string frame;

    for(const auto& row : buffer_)
    {
        frame += row;
        frame += '\n';
    }

    terminal_.write(frame);
}

}