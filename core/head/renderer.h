#pragma once

#include "terminal.h"
#include <world_configuation.h>

#include <string>
#include <vector>

namespace fgames::core
{

class Renderer
{
public:
    Renderer(Terminal& terminal);

    void clear();
    void present();
    void draw(int x, int y, char symbol);
    void draw_border();
    void draw_text(int x, int y, const std::string& text);
    void draw_exit_confirmation(bool selected_option);
    void draw_gameover(bool selected_option);

private:
    int width_ = WorldConfig().world_width;
    int height_ = WorldConfig().world_height;
    Terminal& terminal_;
    std::vector<std::string> buffer_;
};

}