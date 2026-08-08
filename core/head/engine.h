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
    Engine() : renderer_(terminal_) {}

    void run(FGames::core::Game& game);

private:
    bool running_ = true;
    Terminal terminal_;
    Renderer renderer_;
    Input input_;
    Timer timer_;
};

}