#include "../head/engine.h"

#ifdef FGames_PLATFORM_LINUX

#include "../platform/linux/head/linux_input.h"
#include "../platform/linux/head/linux_terminal.h"

#elif defined(FGames_PLATFORM_WINDOWS)

#include "../platform/win/head/win_input.h"
#include "../platform/win/head/win_terminal.h"

#else

#error "Unknown platform"

#endif


namespace fgames::core
{

Engine::Engine()
#ifdef FGames_PLATFORM_LINUX
    :
    input_(std::make_unique<fgames::platform::linux_platform::LinuxInput>()),
    terminal_(std::make_unique<fgames::platform::linux_platform::LinuxTerminal>()),
    renderer_(40, 20, *terminal_)

#elif defined(FGames_PLATFORM_WINDOWS)
    :
    input_(std::make_unique<fgames::platform::win_platform::WinInput>()),
    terminal_(std::make_unique<fgames::platform::win_platform::WinTerminal>()),
    renderer_(40, 20, *terminal_)

#endif

{
}

void Engine::run(Game& game)
{
    running_ = true;

    while (running_)
    {
        const float delta_time = timer_.delta_time();
        const auto events = input_->poll();

        for (const auto& event : events)
        {
            if (event.type == EventType::QuitRequested)
            {
                running_ = false;
                break;
            }

            game.handle_event(event);
        }

        if (!running_) break;
        
        game.update(delta_time);
        game.render(renderer_);
    }
}

}