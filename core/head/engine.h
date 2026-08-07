#pragma once

#include "game.h"
#include "renderer.h"
#include "input.h"
#include "timer.h"

namespace FGames::core
{

class Engine
{
public:
    void run(FGames::core::Game& game);

private:
    bool running_ = true;
    Renderer renderer_;
    Input input_;
    Timer timer_;
};

}