#pragma once

#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace fgames::core
{

class Renderer
{
public:
    Renderer();

    void clear();
    
    void draw(int x, int y, ftxui::Color color);
    void draw_exit_confirmation(bool selected_option);
    void draw_gameover(bool selected_option);

    ftxui::Element present() const;
    ftxui::Element build_menu(const std::vector<std::string>& items, int selected) const;
    
    void resize(int width, int height);
    
    void set_score(int score = 0) {current_game_score_ = score;};
    int width() const;
    int height() const;

private:
    ftxui::Element build_game_field() const;
    ftxui::Element build_score_panel() const;

private:
    int width_ = 0;
    int height_ = 0;
    
    std::vector<std::vector<ftxui::Color>> bg_buffer_;
    ftxui::Element overlay_;

    int current_game_score_ = 0;
};

}