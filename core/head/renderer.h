#pragma once

#include <vector>
#include <iostream>

#include "../platform/linux/head/terminal.h"

namespace FGames::core
{

class Renderer
{
public:
    Renderer(int width = 40, int height = 20, Terminal& terminal);
    void clear();
    void draw(int x, int y, char symbol);
    void present();

private:
    int width_;
    int height_;
    Terminal& terminal_;
    std::vector<std::string> buffer_;
};

}