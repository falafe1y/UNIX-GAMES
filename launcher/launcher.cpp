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
            "Snake",
            [] {
                return std::make_unique<fgames::games::SnakeGame>();
            }
        },

        {
            "Demo",
            [] {
                return std::make_unique<fgames::games::DemoGame>();
            }
        }
    };
}

void Launcher::run()
{
    while (running_)
    {
        render_menu();
        const auto events = input_->poll();
        handle_menu_events(events);

        if (!running_)
            break;

        if (current_game_)
        {
            const bool return_to_menu = engine_.run(*current_game_);
            current_game_.reset();

            if (!return_to_menu)
            {
                running_ = false;
            }
        }
    }
}

void Launcher::handle_menu_events(const std::vector<fgames::core::Event>& events)
{
    for (const auto& event : events)
    {
        if (event.type == fgames::core::EventType::QuitRequested)
        {
            running_ = false;
            return;
        }

        if (event.type != fgames::core::EventType::KeyPressed) continue;
        switch (event.key)
        {
            case fgames::core::EventKey::Up:
                if (selected_game_ > 0)
                {
                    --selected_game_;
                }

                break;

            case fgames::core::EventKey::Down:
                if (selected_game_ + 1 < games_.size())
                {
                    ++selected_game_;
                }

                break;

            case fgames::core::EventKey::Enter:
                current_game_ =
                    games_[selected_game_].create();

                break;

            case fgames::core::EventKey::Escape:
                running_ = false;
                break;

            default:
                break;
        }

        if (!running_)
            return;
    }
}

void Launcher::render_menu()
{
    renderer_.clear();
    renderer_.draw_border();
    const std::string title = "FGames";

    for (std::size_t i = 0; i < title.size(); ++i)
    {
        renderer_.draw(2 + static_cast<int>(i), 1, title[i]);
    }

    for (std::size_t i = 0; i < games_.size(); ++i)
    {
        const bool selected = i == selected_game_;

        const std::string prefix = selected ? "> " : "  ";
        const std::string& name = games_[i].name;

        const int y = 3 + static_cast<int>(i);

        for (std::size_t x = 0; x < prefix.size(); ++x)
        {
            renderer_.draw(2 + static_cast<int>(x), y, prefix[x]);
        }

        for (std::size_t x = 0; x < name.size(); ++x)
        {
            renderer_.draw(2 + static_cast<int>(prefix.size() + x), y, name[x]);
        }
    }

    renderer_.present();
}

}