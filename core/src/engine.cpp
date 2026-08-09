#include "../head/engine.h"

namespace fgames::core
{

Engine::Engine(Input& input, Renderer& renderer)
:
input_(input),
renderer_(renderer)
{
}

bool Engine::run(Game& game)
{
    running_ = true;

    timer_.reset();

    while (running_)
    {
        const auto events = input_.poll();

        for (const auto& event : events)
        {
            if (event.type == EventType::QuitRequested)
            {
                running_ = false;
                break;
            }

            game.handle_event(event);
        }

        if (!running_)
            break;

        if (!game.is_paused())
        {
            const float delta_time = timer_.delta_time();

            game.update(delta_time);
        }
        else
        {
            // Игра стоит.
            // Сбрасываем таймер, чтобы после выхода
            // из паузы не накопилось время.
            timer_.reset();
        }

        game.render(renderer_);

        if (game.result() == GameResult::ExitToMenu)
        {
            running_ = false;
        }
    }

    return game.result() == GameResult::ExitToMenu;
}

}