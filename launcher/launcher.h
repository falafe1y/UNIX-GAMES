#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>

#include "../core/head/engine.h"
#include "../core/head/game.h"
#include "../core/head/renderer.h"

#include "../games/demo/demo.h"
#include "../games/snake/snake.h"
#include "../games/tetris/tetris.h"

namespace fgames::launcher
{

class Launcher
{
public:
Launcher();

void run();

private:
enum class AppState
{
Menu,
Game
};

struct GameEntry
{
    std::string name;

    std::function<
        std::unique_ptr<fgames::core::Game>()
    > create;
};

private:
bool running_{true};

AppState state_{AppState::Menu};

std::vector<GameEntry> games_;

std::vector<std::string> game_names_;

std::size_t selected_game_{0};

std::unique_ptr<fgames::core::Game> current_game_;

fgames::core::Renderer renderer_;

fgames::core::Engine engine_;

};

}
