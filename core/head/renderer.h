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
    void draw(int x, int y, char symbol);
    void draw_border();
    void draw_text(int x, int y, const std::string& text);
    void present();

private:
    int width_ = WorldConfig().world_width;
    int height_ = WorldConfig().world_height;
    Terminal& terminal_;
    std::vector<std::string> buffer_;
};

}