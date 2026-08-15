#include "../head/renderer.h"

namespace fgames::core
{

Renderer::Renderer()
    :
    bg_buffer_(height_, std::vector<ftxui::Color>(width_, ftxui::Color::Default)),
    text_buffer_(height_, std::vector<std::string>(width_))
{
}

void Renderer::clear()
{
    for (auto& row : bg_buffer_)
    {
        std::fill(row.begin(), row.end(), ftxui::Color::Default);
    }

    for (auto& row : text_buffer_)
    {
        std::fill(row.begin(), row.end(), "");
    }

    overlay_ = ftxui::text("");
}

// ====================================== DRAWING ======================================

void Renderer::draw(int x, int y, ftxui::Color color, const std::string& text)
{
    if (x < 0 || x >= width_) 
        return;

    if (y < 0 || y >= height_) 
        return;

    bg_buffer_[y][x] = color;
}

void Renderer::draw_text(int x, int y, const std::string& text, ftxui::Color color)
{
    if (y < 0 || y >= height_)
        return;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        const int current_x =
            x + static_cast<int>(i);

        if (current_x < 0 || current_x >= width_)
            continue;

        text_buffer_[y][current_x] =
            text.substr(i, 1);
    }
}

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

// ====================================== BUILD ======================================

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

ftxui::Element Renderer::build_game_field() const
{
    using namespace ftxui;

    Elements rows;
    rows.reserve(bg_buffer_.size());

    for (std::size_t y = 0; y < bg_buffer_.size(); ++y)
    {
        Elements cells;
        cells.reserve(bg_buffer_[y].size());

        for (std::size_t x = 0; x < bg_buffer_[y].size(); ++x)
        {
            const auto& color = bg_buffer_[y][x];
            const auto& character = text_buffer_[y][x];

            Element cell;

            if (character.empty())
            {
                cell =
                    text("  ")
                    | bgcolor(color);
            }
            else
            {
                cell =
                    text(" " + character)
                    | bold
                    // | color(Color::Black)
                    | bgcolor(color);
            }

            cells.push_back(std::move(cell));
        }

        rows.push_back(
            hbox(std::move(cells))
        );
    }

    return vbox(std::move(rows))
        | border
        | center;
}

ftxui::Element Renderer::build_score_panel() const
{
    using namespace ftxui;

    const auto score_text = text(std::to_string(current_game_score_))
        | bold
        | color(Color::White)
        | bgcolor(Color::DarkBlue);

    const auto title = text("Score")
        | bold
        | color(Color::Yellow)
        | center;

    return window(
        title,
        vbox({
            separator(),
            hbox({
                text("   "),
                score_text,
                text("   ")
            })
            | center
        })
    );
}

// ====================================== RENDERING ======================================

ftxui::Element Renderer::present() const
{
    using namespace ftxui;

    auto game_field = build_game_field();
    auto score_panel = build_score_panel();

    auto content = vbox({
        score_panel | center,
        center(game_field) | flex
    });

    if (overlay_) {
        content = dbox({ content, overlay_ });
    }

    return border(content);
}

// ====================================== OTHER ======================================

void Renderer::resize(int width, int height)
{
    if (width_ == width && height_ == height)
        return;

    width_ = width;
    height_ = height;

    bg_buffer_.assign(height_, std::vector<ftxui::Color>(width_, ftxui::Color::Default));
    text_buffer_.assign(height_, std::vector<std::string>(width_));
}

int Renderer::width() const
{
    return width_;
}

int Renderer::height() const
{
    return height_;
}

}
