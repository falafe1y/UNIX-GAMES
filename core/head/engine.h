#pragma once

#include "game.h"
#include "renderer.h"
#include "timer.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace fgames::core
{

class Engine
{
public:
    explicit Engine(Renderer& renderer);

    // true  -> игра хочет вернуться в Launcher
    // false -> приложение хочет завершиться
    bool run(Game& game);

private:
    Renderer& renderer_;

    Timer timer_;

    bool running_{false};
};

}