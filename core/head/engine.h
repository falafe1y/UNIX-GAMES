#pragma once

#include "game.h"
#include "input.h"
#include "renderer.h"
#include "timer.h"

namespace fgames::core
{

class Engine
{
public:
    Engine(Input& input, Renderer& renderer);

    // Запускает переданную игру.
    // true  -> игра хочет вернуться в Launcher.
    // false -> Engine завершился обычным образом.
    bool run(Game& game);

private:
    bool running_{true};

    Input& input_;
    Renderer& renderer_;

    Timer timer_;
};

}