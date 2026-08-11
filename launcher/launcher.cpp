#include "launcher.h"

#ifdef FGames_PLATFORM_LINUX

#include "../core/platform/linux/head/linux_input.h"
#include "../core/platform/linux/head/linux_terminal.h"

#elif defined(FGames_PLATFORM_WINDOWS)

#include "../core/platform/win/head/win_input.h"
#include "../core/platform/win/head/win_terminal.h"

#else

#error "Unknown platform"

#endif

namespace fgames::launcher
{

Launcher::Launcher()

#ifdef FGames_PLATFORM_LINUX
:
input_(std::make_unique<fgames::platform::linux_platform::LinuxInput>()),
terminal_(std::make_unique<fgames::platform::linux_platform::LinuxTerminal>()),
renderer_(),
engine_(*input_, renderer_)

#elif defined(FGames_PLATFORM_WINDOWS)
:
input_(std::make_unique<fgames::platform::win_platform::WinInput>()),
terminal_(std::make_unique<fgames::platform::win_platform::WinTerminal>()),
renderer_(),
engine_(*input_, renderer_)

#endif

{
    games_ =
    {
        {
            "Snake", [] {return std::make_unique<fgames::games::SnakeGame>();}
        },

        {
            "Demo", [] {return std::make_unique<fgames::games::DemoGame>();}
        },

        {
            "Tetris", [] {return std::make_unique<fgames::games::TetrisGame>();}
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

        if (selected < 0 ||
            selected >= static_cast<int>(games_.size()))
        {
            break;
        }

        current_game_ =
            games_[selected].create();

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