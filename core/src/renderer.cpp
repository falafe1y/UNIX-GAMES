#include "../head/renderer.h"

namespace fgames::core
{

Renderer::Renderer(int width, int height, Terminal& terminal)
    :
    width_(width),
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
    if (x < 0 || x >= width_) return;
    if (y < 0 || y >= height_) return;
    buffer_[y][x] = symbol;
}

void Renderer::draw_border()
{
    // Верхняя и нижняя границы
    for (int x = 0; x < width_; ++x)
    {
        buffer_[0][x] = '#';
        buffer_[height_ - 1][x] = '#';
    }

    // Левая и правая границы
    for (int y = 0; y < height_; ++y)
    {
        buffer_[y][0] = '#';
        buffer_[y][width_ - 1] = '#';
    }
}

void Renderer::present()
{
    terminal_.move_cursor(0, 0);

    std::string frame;

    for (const auto& row : buffer_)
    {
        frame += row;
        frame += '\n';
    }

    terminal_.write(frame);
}

}