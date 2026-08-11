#include "../head/engine.h"

#include <chrono>
#include <thread>

namespace fgames::core
{

namespace
{

Event convert_event(const ftxui::Event& event)
{
if (event == ftxui::Event::ArrowUp)
{
return {
EventType::KeyPressed,
EventKey::Up
};
}

if (event == ftxui::Event::ArrowDown)
{
    return {
        EventType::KeyPressed,
        EventKey::Down
    };
}

if (event == ftxui::Event::ArrowLeft)
{
    return {
        EventType::KeyPressed,
        EventKey::Left
    };
}

if (event == ftxui::Event::ArrowRight)
{
    return {
        EventType::KeyPressed,
        EventKey::Right
    };
}

if (event == ftxui::Event::Return)
{
    return {
        EventType::KeyPressed,
        EventKey::Enter
    };
}

if (event == ftxui::Event::Character(' '))
{
    return {
        EventType::KeyPressed,
        EventKey::Space
    };
}

if (event == ftxui::Event::Escape)
{
    return {
        EventType::KeyPressed,
        EventKey::Escape
    };
}

if (event == ftxui::Event::Character('q') ||
    event == ftxui::Event::Character('Q'))
{
    return {
        EventType::QuitRequested,
        EventKey::Unknown
    };
}

return {
    EventType::KeyPressed,
    EventKey::Unknown
};

}

}

Engine::Engine(Renderer& renderer)
:
renderer_(renderer)
{
}

bool Engine::handle_event(
Game& game,
const ftxui::Event& event
)
{
/*
* Custom — это внутренний тик FTXUI.
* В Game его передавать нельзя.
*/
if (event == ftxui::Event::Custom)
{
return false;
}

const Event core_event =
    convert_event(event);

if (core_event.type == EventType::KeyPressed &&
    core_event.key == EventKey::Unknown)
{
    return false;
}

game.handle_event(core_event);

return true;

}

ftxui::Element Engine::render(Game& game)
{
/*
* Время обновления считается только здесь.
*
* Поэтому Timer больше не связан с input.
*/
if (!game.is_paused())
{
const float delta_time =
timer_.delta_time();

    game.update(delta_time);
}
else
{
    /*
     * Чтобы после паузы не получить огромный delta_time.
     */
    timer_.reset();
}

renderer_.clear();

game.render(renderer_);

return renderer_.present();

}

void Engine::start_timer()
{
if (timer_running_)
{
return;
}

timer_running_ = true;

timer_.reset();

/*
 * FTXUI получает Custom примерно 60 раз
 * в секунду и благодаря этому перерисовывает игру.
 *
 * Поток НЕ трогает Game.
 * Он только будит ScreenInteractive.
 */
std::thread(
    [this]()
    {
        using namespace std::chrono_literals;

        while (timer_running_)
        {
            std::this_thread::sleep_for(16ms);

            if (timer_running_)
            {
                renderer_.request_frame();
            }
        }
    }
).detach();

}

void Engine::stop_timer()
{
timer_running_ = false;
}

}
