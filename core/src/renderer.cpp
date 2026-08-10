#include "../head/renderer.h"

#include <ftxui/dom/elements.hpp>

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

// ============================================================
// BUFFER
// ============================================================

void Renderer::clear()
{
    for (auto& row : buffer_)
    {
        row.assign(width_, ' ');
    }

    overlay_ = ftxui::text("");
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
    if (width_ <= 0 || height_ <= 0)
        return;

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

// ============================================================
// EXIT CONFIRMATION
// ============================================================

void Renderer::draw_exit_confirmation(
    bool selected_option
)
{
    using namespace ftxui;

    const auto yes =
        selected_option
            ? text(" Yes ")
                | bold
                | color(Color::Black)
                | bgcolor(Color::Green)
            : text(" Yes ")
                | color(Color::White);

    const auto no =
        !selected_option
            ? text(" No ")
                | bold
                | color(Color::Black)
                | bgcolor(Color::Red)
            : text(" No ")
                | color(Color::White);

    overlay_ =
        window(
            text(" Exit game? ")
                | bold
                | color(Color::Yellow)
                | center,

            vbox({
                text("Do you really want to leave?")
                    | center,

                text("Your current game will be lost.")
                    | dim
                    | center,

                separator(),

                hbox({
                    yes,
                    text("     "),
                    no
                })
                | center,

                separator(),

                text("Arrow keys  Select    Enter  Confirm")
                    | dim
                    | center
            })
        )
        | center;
}

// ============================================================
// GAME OVER
// ============================================================

void Renderer::draw_gameover(
    bool selected_option
)
{
    using namespace ftxui;

    const auto restart =
        selected_option
            ? text(" Restart ")
                | bold
                | color(Color::Black)
                | bgcolor(Color::Green)
            : text(" Restart ")
                | color(Color::White);

    const auto menu =
        !selected_option
            ? text(" Menu ")
                | bold
                | color(Color::Black)
                | bgcolor(Color::Red)
            : text(" Menu ")
                | color(Color::White);

    overlay_ =
        window(
            text(" GAME OVER ")
                | bold
                | color(Color::Red)
                | center,

            vbox({
                text("The snake has crashed.")
                    | center,

                text("What would you like to do?")
                    | dim
                    | center,

                separator(),

                hbox({
                    restart,
                    text("     "),
                    menu
                })
                | center,

                separator(),

                text("Arrow keys  Select    Enter  Confirm")
                    | dim
                    | center
            })
        )
        | center;
}

// ============================================================
// GAME FIELD
// ============================================================

ftxui::Element Renderer::build_game_field() const
{
    using namespace ftxui;

    Elements rows;

    rows.reserve(buffer_.size());

    for (const auto& row : buffer_)
    {
        Elements cells;

        cells.reserve(row.size());

        for (const char symbol : row)
        {
            switch (symbol)
            {
                case '#':
                    cells.push_back(
                        text("  ")
                            | bgcolor(Color::GrayDark)
                    );
                    break;

                case '@':
                    cells.push_back(
                        text("  ")
                            | bgcolor(Color::Green)
                    );
                    break;

                case 'o':
                    cells.push_back(
                        text("  ")
                            | bgcolor(Color::GreenLight)
                    );
                    break;

                case '*':
                    cells.push_back(
                        text("  ")
                            | bgcolor(Color::Red)
                    );
                    break;

                default:
                    cells.push_back(
                        text("  ")
                    );
                    break;
            }
        }

        rows.push_back(
            hbox(std::move(cells))
        );
    }

    return vbox(std::move(rows))
        | border;
}

// ============================================================
// MENU
// ============================================================

ftxui::Element Renderer::build_menu() const
{
    using namespace ftxui;

    Elements rows;

    for (const auto& row : buffer_)
    {
        rows.push_back(
            text(row)
        );
    }

    return vbox(std::move(rows))
        | border
        | center;
}

// ============================================================
// GAME PRESENT
// ============================================================

void Renderer::present()
{
    using namespace ftxui;

    const auto game =
        build_game_field();

    Element document;

    if (overlay_)
    {
        document =
            dbox({
                game,
                overlay_
            });
    }
    else
    {
        document = game;
    }

    screen_.Clear();

    Render(
        screen_,
        document
    );

    screen_.ResetPosition();
    screen_.Print();
}

// ============================================================
// MENU PRESENT
// ============================================================

void Renderer::present_menu()
{
    using namespace ftxui;

    const auto menu =
        build_menu();

    screen_.Clear();

    Render(
        screen_,
        menu
    );

    screen_.ResetPosition();
    screen_.Print();
}

// ============================================================
// SIZE
// ============================================================

int Renderer::width() const
{
    return width_;
}

int Renderer::height() const
{
    return height_;
}

}