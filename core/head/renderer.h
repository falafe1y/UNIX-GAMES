#pragma once

#include <string>
#include <vector>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "world_configuation.h"

namespace fgames::core
{

class Renderer
{
public:
    Renderer();

    void clear();

    void draw(
        int x,
        int y,
        char symbol
    );

    void draw_border();

    void draw_text(
        int x,
        int y,
        const std::string& text
    );

    void draw_exit_confirmation(
        bool selected_option
    );

    void draw_gameover(
        bool selected_option
    );

    // Игровой экран.
    void present();

    // Экран лаунчера.
    void present_menu();

    int width() const;
    int height() const;

private:
    ftxui::Element build_game_field() const;

    ftxui::Element build_menu() const;

private:
    int width_ =
        WorldConfig().world_width;

    int height_ =
        WorldConfig().world_height;

    std::vector<std::string> buffer_;

    ftxui::ScreenInteractive screen_;

    ftxui::Element overlay_;
};

}
