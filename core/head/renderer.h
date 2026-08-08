#pragma once

#include <string>
#include <vector>

#include "terminal.h"

namespace FGames::core
{

class Renderer
{
public:
    Renderer(
        int width,
        int height,
        Terminal& terminal
    );

    void clear();

    void draw(
        int x,
        int y,
        char symbol
    );

    void present();

private:
    int width_;
    int height_;

    Terminal& terminal_;

    std::vector<std::string> buffer_;
};

}