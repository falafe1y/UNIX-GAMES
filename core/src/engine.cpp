#include "../head/engine.h"

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

    if (event == ftxui::Event::Character('q') || event == ftxui::Event::Character('Q'))
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

Engine::Engine(Renderer& renderer, ftxui::ScreenInteractive& screen)
    :
    renderer_(renderer),
    screen_(screen)
{
}

Engine::~Engine()
{
    stop();
}

bool Engine::handle_event(Game& game, const ftxui::Event& event)
{
    if (event == ftxui::Event::Custom)
    {
        return false;
    }

    const Event core_event = convert_event(event);

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
    if (!game.is_paused())
    {
        const float delta_time =timer_.tick();
        game.update(delta_time);
    }
    else
    {
        timer_.reset();
    }

    renderer_.resize(game.world_width(), game.world_height());

    renderer_.clear();
    game.render(renderer_);

    return renderer_.present();
}

void Engine::start()
{
    if (running_)
    {
        return;
    }

    running_ = true;

    timer_.reset();

    tick_thread_ = std::thread([this]()
        {
            using namespace std::chrono_literals;

            while (running_)
            {
                std::this_thread::sleep_for(16ms);

                if (!running_)
                {
                    break;
                }

                screen_.PostEvent(ftxui::Event::Custom);
            }
        }
    );
}

void Engine::stop()
{
    running_ = false;

    if (tick_thread_.joinable())
    {
        tick_thread_.join();
    }
}

}