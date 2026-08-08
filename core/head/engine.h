#pragma once

#include <memory>

#include "game.h"
#include "input.h"
#include "renderer.h"
#include "terminal.h"
#include "timer.h"

namespace fgames::core
{

class Engine
{
public:
    Engine();

    void run(Game& game);

private:
    bool running_{true};

    std::unique_ptr<Input> input_;
    std::unique_ptr<Terminal> terminal_;
    Renderer renderer_;
    Timer timer_;
};

}