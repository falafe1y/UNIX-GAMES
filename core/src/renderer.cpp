#include "../head/renderer.h"

namespace fgames::core
{

Renderer::Renderer()
    :
    buffer_(height_, std::string(width_, ' ')),
    bg_buffer_(height_, std::vector<ftxui::Color>(width_, ftxui::Color::Default))
{
}

void Renderer::clear()
{
    for (auto& row : bg_buffer_)
    {
        std::fill(row.begin(), row.end(), ftxui::Color::Default);
    }

    overlay_ = ftxui::text("");
}

void Renderer::draw(int x, int y, ftxui::Color color)
{
    if (x < 0 || x >= width_) 
        return;

    if (y < 0 || y >= height_) 
        return;

    bg_buffer_[y][x] = color;
}

void Renderer::draw_border()
{
    if (width_ <= 0 || height_ <= 0)
        return;

    for (int x = 0; x < width_; ++x)
    {
        draw(x, 0, ftxui::Color::GrayDark);
        draw(x, height_ -1, ftxui::Color::GrayDark);
    }

    for (int y = 0; y < height_; ++y)
    {
        draw(0, y, ftxui::Color::GrayDark);
        draw(width_ - 1, y, ftxui::Color::GrayDark);
    }
}

// void Renderer::draw_text(int x, int y, const std::string& text)
// {
//     for (std::size_t i = 0; i < text.size(); ++i)
//     {
//         draw(x + static_cast<int>(i), y, ftxui::Color::White);

//     }
// }

void Renderer::draw_exit_confirmation(bool selected_option)
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
            text(" Exit game ")
                | bold
                | color(Color::Yellow)
                | center,

            vbox(
                Elements{
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
                }
            )
        )
        | center;
}

void Renderer::draw_gameover(bool selected_option)
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

            vbox(
                Elements{
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
                }
            )
        )
        | center;
}

ftxui::Element Renderer::build_game_field() const
{
    ftxui::Elements rows;
    rows.reserve(buffer_.size());

    for (const auto& row : bg_buffer_)
    {
        ftxui::Elements cells;
        cells.reserve(row.size());

        for (const auto& color : row)
        {
            cells.push_back(ftxui::text("  ") | ftxui::bgcolor(color));
        }

        rows.push_back(hbox(std::move(cells)) | ftxui::center);
    }

    return vbox(std::move(rows)) | ftxui::border;
}

// ============================================================
// GAME PRESENT
// ============================================================

ftxui::Element Renderer::present() const
{
    using namespace ftxui;

    const auto game = build_game_field();

    if (overlay_)
    {
        return dbox({
            game,
            overlay_
        });
    }

    return game;
}

// menu
ftxui::Element Renderer::build_menu(const std::vector<std::string>& items, int selected) const
{
using namespace ftxui;

Elements menu_items;

for (std::size_t i = 0; i < items.size(); ++i)
{
    const bool focused = static_cast<int>(i) == selected;

    Element item =
        text(items[i])
        | center;

    if (focused)
    {
        item =
            hbox({
                text("  "),
                text("> ")
                    | bold | color(Color::Green),
                text(items[i])
                    | bold | center
            })
            | bgcolor(Color::GrayDark);
    }

    menu_items.push_back(std::move(item));
}

return vbox({
    vbox({
        text("F G A M E S")
            | bold
            | color(Color::Green)
            | center,

        text("Terminal Game Collection")
            | dim
            | center
    })
    | border
    | color(Color::Green)
    | size(WIDTH, EQUAL, 40),

    text(" "),

    vbox({
        text("GAMES")
            | bold
            | color(Color::Yellow)
            | center,

        separator(),

        vbox(std::move(menu_items)),

        separator(),

        text("LEFT/RIGHT    Select")
            | dim
            | center,

        text("ENTER    Start")
            | dim
            | center,

        text("ESC/Q    Quit")
            | dim
            | center
    })
    | border
    // | size(WIDTH, EQUAL, 40)
})
| center;

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
