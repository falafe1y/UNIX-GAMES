#pragma once

#include <memory>

#include "../core/head/engine.h"
#include "../core/head/game.h"
#include "../games/demo/demo.h"
#include "../games/snake/snake.h"

namespace fgames::launcher
{

class Launcher
{
public:
    Launcher() = default;

    void run();

private:
    std::unique_ptr<fgames::core::Game> currentGame_;
    fgames::core::Engine engine_;
};

}