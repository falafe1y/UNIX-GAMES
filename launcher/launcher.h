#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../core/head/engine.h"
#include "../core/head/game.h"
#include "../core/head/input.h"
#include "../core/head/renderer.h"
#include "../core/head/terminal.h"

#include "../games/demo/demo.h"
#include "../games/snake/snake.h"

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
        std::function<std::unique_ptr<fgames::core::Game>()> create;
    };

    void render_menu();
    void handle_menu_events(const std::vector<fgames::core::Event>& events);

private:
    bool running_{true};

    std::vector<GameEntry> games_;
    std::size_t selected_game_{0};

    std::unique_ptr<fgames::core::Input> input_;
    std::unique_ptr<fgames::core::Terminal> terminal_;

    fgames::core::Renderer renderer_;
    fgames::core::Engine engine_;

    std::unique_ptr<fgames::core::Game> current_game_;
};

}