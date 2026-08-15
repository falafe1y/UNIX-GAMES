#include "launcher.h"

#include "../games/snake/snake.h"
#include "../games/tetris/tetris.h"
#include "../games/demo/demo.h"
#include "../games/2048/2048.h"

namespace fgames::launcher
{

Launcher::Launcher()
    :
    screen_(ftxui::ScreenInteractive::Fullscreen()),
    renderer_(),
    engine_(renderer_, screen_)
{
    games_ =
    {
        {
            "DEMO",
            []()
            {
                return std::make_unique<
                    fgames::games::DemoGame
                >();
            }
        },

        {
            "Snake",
            []()
            {
                return std::make_unique<
                    fgames::games::SnakeGame
                >();
            }
        },

        {
            "Tetris",
            []()
            {
                return std::make_unique<
                    fgames::games::TetrisGame
                >();
            }
        },

        {
            "2048",
            []()
            {
                return std::make_unique<
                    fgames::games::Game2048
                >();
            }
        }
    };

    game_names_.reserve(games_.size());

    for (const auto& game : games_)
    {
        game_names_.push_back(game.name);
    }
}

void Launcher::create_game(int index)
{
    if (index < 0 ||
        index >= static_cast<int>(games_.size()))
    {
        return;
    }

    current_game_ =
        games_[index].create();

    in_game_ = true;

    engine_.start();
}

void Launcher::destroy_game()
{
    engine_.stop();

    current_game_.reset();

    in_game_ = false;
}

void Launcher::run()
{
    using namespace ftxui;

    auto renderer_component =
        Renderer(
            [&]()
            {
                if (in_game_ && current_game_)
                {
                    return engine_.render(
                        *current_game_
                    );
                }

                return renderer_.build_menu(
                    game_names_,
                    selected_
                );
            }
        );

    auto component =
        CatchEvent(
            renderer_component,

            [&](const Event& event)
            {
                // ==================================================
                // GAME
                // ==================================================

                if (in_game_ && current_game_)
                {
                    // Передаём событие игре.
                    engine_.handle_event(
                        *current_game_,
                        event
                    );

                    // Проверяем результат игры.
                    switch (current_game_->result())
                    {
                        case core::GameResult::Running:
                        {
                            break;
                        }

                        case core::GameResult::Restart:
                        {
                            const int game_index =
                                selected_;

                            destroy_game();

                            create_game(game_index);

                            break;
                        }

                        case core::GameResult::ExitToMenu:
                        {
                            destroy_game();

                            break;
                        }
                    }

                    return true;
                }

                // ==================================================
                // MENU
                // ==================================================

                if (event == Event::ArrowUp)
                {
                    if (!game_names_.empty())
                    {
                        selected_--;

                        if (selected_ < 0)
                        {
                            selected_ =
                                static_cast<int>(
                                    game_names_.size()
                                ) - 1;
                        }
                    }

                    return true;
                }

                if (event == Event::ArrowDown)
                {
                    if (!game_names_.empty())
                    {
                        selected_++;

                        if (selected_ >=
                            static_cast<int>(
                                game_names_.size()
                            ))
                        {
                            selected_ = 0;
                        }
                    }

                    return true;
                }

                // ENTER -> запуск игры
                if (event == Event::Return)
                {
                    create_game(selected_);

                    return true;
                }

                // ESC / Q -> выход из программы
                if (event == Event::Escape ||
                    event == Event::Character('q') ||
                    event == Event::Character('Q'))
                {
                    screen_.ExitLoopClosure()();
                    return true;
                }

                return false;
            }
        );

    screen_.Loop(component);

    // На всякий случай.
    destroy_game();
}

}
