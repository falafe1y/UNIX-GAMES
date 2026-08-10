#include "../head/renderer.h"

namespace fgames::core
{

Renderer::Renderer()
:
buffer_(
    height_,
    std::string(width_, ' ')
),
screen_(
    ftxui::ScreenInteractive::Fullscreen()
)
{
}

void Renderer::clear()
{
    for (auto& row : buffer_)
    {
        row.assign(width_, ' ');
    }
}

void Renderer::draw(
    int x,
    int y,
    char symbol
)
{
    if (x < 0 || x >= width_)
        return;

    if (y < 0 || y >= height_)
        return;

    buffer_[y][x] = symbol;
}

void Renderer::draw_border()
{
    // Верхняя и нижняя границы.
    for (int x = 0; x < width_; ++x)
    {
        buffer_[0][x] = '#';
        buffer_[height_ - 1][x] = '#';
    }

    // Левая и правая границы.
    for (int y = 0; y < height_; ++y)
    {
        buffer_[y][0] = '#';
        buffer_[y][width_ - 1] = '#';
    }
}

void Renderer::draw_text(
    int x,
    int y,
    const std::string& text
)
{
    for (std::size_t i = 0; i < text.size(); ++i)
    {
        draw(
            x + static_cast<int>(i),
            y,
            text[i]
        );
    }
}

void Renderer::draw_exit_confirmation(
    bool selected_option
)
{
    const std::string line1 =
        "+--------------------------------------+";

    const std::string line2 =
        "| Do you really want to close the game?|";

    const std::string line3 =
        "|       and lose all your experience?  |";

    const std::string line4 =
        "|                                      |";

    const std::string line5 =
        selected_option
            ? "|          [ Yes ]     No              |"
            : "|            Yes     [ No ]            |";

    const std::string line6 =
        "|                                      |";

    const std::string line7 =
        "+--------------------------------------+";

    const int box_width =
        static_cast<int>(line1.size());

    const int box_height = 7;

    const int start_x =
        (width_ - box_width) / 2;

    const int start_y =
        (height_ - box_height) / 2;

    draw_text(start_x, start_y,     line1);
    draw_text(start_x, start_y + 1, line2);
    draw_text(start_x, start_y + 2, line3);
    draw_text(start_x, start_y + 3, line4);
    draw_text(start_x, start_y + 4, line5);
    draw_text(start_x, start_y + 5, line6);
    draw_text(start_x, start_y + 6, line7);
}

void Renderer::draw_gameover(
    bool selected_option
)
{
    const std::string line1 =
        "+--------------------------------------+";

    const std::string line2 =
        "|              Game Over!              |";

    const std::string line3 =
        "|       Do you want to restart?        |";

    const std::string line4 =
        "|                                      |";

    const std::string line5 =
        selected_option
            ? "|          [ Yes ]     No              |"
            : "|            Yes     [ No ]            |";

    const std::string line6 =
        "|                                      |";

    const std::string line7 =
        "+--------------------------------------+";

    const int box_width =
        static_cast<int>(line1.size());

    const int box_height = 7;

    const int start_x =
        (width_ - box_width) / 2;

    const int start_y =
        (height_ - box_height) / 2;

    draw_text(start_x, start_y,     line1);
    draw_text(start_x, start_y + 1, line2);
    draw_text(start_x, start_y + 2, line3);
    draw_text(start_x, start_y + 3, line4);
    draw_text(start_x, start_y + 4, line5);
    draw_text(start_x, start_y + 5, line6);
    draw_text(start_x, start_y + 6, line7);
}

void Renderer::present()
{
    using namespace ftxui;

    Elements rows;

    for (const auto& row : buffer_)
    {
        rows.push_back(
            text(row)
        );
    }

    auto document =
        vbox(rows);

    screen_.Clear();

    Render(
        screen_,
        document
    );

    screen_.ResetPosition();

    screen_.Print();
}

}
