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

game_names_.reserve(games_.size());

for (const auto& game : games_)
{
    game_names_.push_back(game.name);
}

}

void Launcher::run()
{
using namespace ftxui;

/*
 * ========================================================
 * ROOT RENDERER
 * ========================================================
 */

auto view = Renderer(
    [&]
    {
        /*
         * MENU
         */
        if (state_ == AppState::Menu)
        {
            return renderer_.build_menu(
                game_names_,
                static_cast<int>(selected_game_)
            );
        }

        /*
         * GAME
         */
        if (current_game_)
        {
            return engine_.render(
                *current_game_
            );
        }

        return text("");
    }
);

/*
 * ========================================================
 * INPUT
 * ========================================================
 */

auto root = CatchEvent(
    view,
    [&](const Event& event)
    {
        /*
         * =================================================
         * GAME INPUT
         * =================================================
         */

        if (state_ == AppState::Game)
        {
            /*
             * Передаём ВСЕ события игре.
             *
             * В том числе Escape.
             *
             * Мы не вмешиваемся в существующую
             * pause / confirmation логику.
             */
            engine_.handle_event(
                *current_game_,
                event
            );

            /*
             * Только если Game действительно
             * решила выйти в Launcher —
             * уничтожаем её.
             */
            if (current_game_->result() ==
                core::GameResult::ExitToMenu)
            {
                engine_.stop_timer();

                current_game_.reset();

                state_ = AppState::Menu;

                renderer_.request_frame();
            }

            return true;
        }

        /*
         * =================================================
         * MENU INPUT
         * =================================================
         */

        if (event == Event::ArrowUp)
        {
            if (selected_game_ > 0)
            {
                --selected_game_;
            }

            renderer_.request_frame();

            return true;
        }

        if (event == Event::ArrowDown)
        {
            if (selected_game_ + 1 < games_.size())
            {
                ++selected_game_;
            }

            renderer_.request_frame();

            return true;
        }

        /*
         * ENTER -> GAME
         */
        if (event == Event::Return)
        {
            current_game_ =
                games_[selected_game_].create();

            state_ = AppState::Game;

            engine_.start_timer();

            renderer_.request_frame();

            return true;
        }

        /*
         * ESC / Q -> EXIT APPLICATION
         */
        if (event == Event::Escape ||
            event == Event::Character('q') ||
            event == Event::Character('Q'))
        {
            running_ = false;

            renderer_.screen()
                .ExitLoopClosure()();

            return true;
        }

        return false;
    }
);

/*
 * ========================================================
 * ONE AND ONLY ONE LOOP
 * ========================================================
 */

renderer_.screen().Loop(root);

engine_.stop_timer();

current_game_.reset();

}


}
