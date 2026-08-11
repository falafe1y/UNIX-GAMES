#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "../core/head/engine.h"
#include "../core/head/game.h"
#include "../core/head/renderer.h"

namespace fgames::launcher
{

class Launcher
{
public:
    Launcher();

    void run();

private:
    struct GameEntry
    {
        std::string name;

        std::function<
            std::unique_ptr<fgames::core::Game>()
        > create;
    };

private:
    void create_game(int index);

    void destroy_game();

private:
    fgames::core::Renderer renderer_;

    fgames::core::Engine engine_;

    std::vector<GameEntry> games_;

    std::vector<std::string> game_names_;

    std::unique_ptr<fgames::core::Game> current_game_;

    int selected_{0};

    bool in_game_{false};
};

}
