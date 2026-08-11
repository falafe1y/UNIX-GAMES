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

    // ========================================================
    // GAME
    // ========================================================

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

    // FTXUI representation текущего framebuffer.
    ftxui::Element present() const;

    // ========================================================
    // MENU
    // ========================================================

    int run_menu(
        const std::vector<std::string>& items
    );

    // ========================================================
    // SIZE
    // ========================================================

    int width() const;
    int height() const;

    // ========================================================
    // FTXUI
    // ========================================================

    void request_frame();

    ftxui::ScreenInteractive& screen();

private:
    ftxui::Element build_game_field() const;

    ftxui::Element overlay_;

    int width_ =
        WorldConfig().world_width;

    int height_ =
        WorldConfig().world_height;

    std::vector<std::string> buffer_;

    ftxui::ScreenInteractive screen_;
};

}