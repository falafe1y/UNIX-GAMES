#include "launcher.h"

namespace fgames::launcher
{

Launcher::Launcher()
    :
    renderer_(),
    engine_(renderer_)
{
    games_ =
    {
        {
            "Snake",
            [] {
                return std::make_unique<
                    fgames::games::SnakeGame
                >();
            }
        },

        {
            "Demo",
            [] {
                return std::make_unique<
                    fgames::games::DemoGame
                >();
            }
        },

        {
            "Tetris",
            [] {
                return std::make_unique<
                    fgames::games::TetrisGame
                >();
            }
        }
    };
}

void Launcher::run()
{
    std::vector<std::string> names;

    names.reserve(games_.size());

    for (const auto& game : games_)
    {
        names.push_back(game.name);
    }

    while (running_)
    {
        const int selected =
            renderer_.run_menu(names);

        /*
         * Пользователь вышел из Launcher.
         */
        if (selected < 0 ||
            selected >= static_cast<int>(games_.size()))
        {
            running_ = false;
            break;
        }

        current_game_ =
            games_[selected].create();

        /*
         * Engine запускает FTXUI event loop
         * для конкретной игры.
         */
        const bool return_to_menu =
            engine_.run(*current_game_);

        current_game_.reset();

        if (!return_to_menu)
        {
            running_ = false;
        }
    }
}

}