#include "../head/engine.h"

namespace FGames::core
{

void Engine::run(Game& game)
{
    while(running_)
    {
        float delta_time = timer_.delta_time();
        auto events = input_.poll();

        for(const auto& event : events)
        {
            if(event.type == EventType::QuitRequested)
            {
                running_ = false;
                break;
            }

            game.handle_event(event);
        }

        game.update(delta_time);

        game.render(renderer_);
    }
}

}