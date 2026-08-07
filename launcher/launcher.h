#pragma once

#include <memory>
#include <iostream>

#include "../core/head/engine.h"
#include "../core/head/game.h"
#include "../games/demo/demo.h"

namespace FGames::launcher
{

class Launcher
{
public:
    void run();

private:
    std::unique_ptr<FGames::core::Game> currentGame_;
    FGames::core::Engine engine_;
};

}