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
    state_ = State::Playing;
    exit_confirmed_ = false;
    confirmation_selection_ = false;

    timer_.reset();

    while (running_)
    {
        if (state_ == State::Playing)
        {
            const float delta_time = timer_.delta_time();
            const auto events = input_.poll();

            handle_game_events(game, events);

            if (!running_)
                break;

            if (state_ == State::Playing)
            {
                game.update(delta_time);
                game.render(renderer_);
            }
            else if (state_ == State::ConfirmExit)
            {
                // Игра уже была отрисована.
                // Накладываем плашку поверх текущего кадра.
                renderer_.draw_exit_confirmation(
                    confirmation_selection_
                );

                renderer_.present();
            }
        }
        else if (state_ == State::ConfirmExit)
        {
            const auto events = input_.poll();

            handle_confirmation_events(events);
        }
    }

    return exit_confirmed_;
}

void Engine::handle_game_events(Game& game, const std::vector<Event>& events)
{
    for (const auto& event : events)
    {
        if (event.type == EventType::QuitRequested)
        {
            running_ = false;
            exit_confirmed_ = false;
            return;
        }

        if (event.type == EventType::KeyPressed &&
            event.key == EventKey::Escape)
        {
            state_ = State::ConfirmExit;

            // По умолчанию выбираем No.
            confirmation_selection_ = false;

            return;
        }

        game.handle_event(event);
    }
}

void Engine::handle_confirmation_events(
    const std::vector<Event>& events)
{
    for (const auto& event : events)
    {
        if (event.type == EventType::QuitRequested)
        {
            running_ = false;
            exit_confirmed_ = false;
            return;
        }

        if (event.type != EventType::KeyPressed)
            continue;

        switch (event.key)
        {
            case EventKey::Right:
            case EventKey::Up:

                if (confirmation_selection_)
                {
                    confirmation_selection_ = false;

                    renderer_.draw_exit_confirmation(
                        confirmation_selection_
                    );

                    renderer_.present();
                }

                break;

            case EventKey::Left:
            case EventKey::Down:

                if (!confirmation_selection_)
                {
                    confirmation_selection_ = true;

                    renderer_.draw_exit_confirmation(
                        confirmation_selection_
                    );

                    renderer_.present();
                }

                break;

            case EventKey::Enter:

                if (confirmation_selection_)
                {
                    // Yes
                    exit_confirmed_ = true;
                    running_ = false;
                }
                else
                {
                    // No
                    state_ = State::Playing;
                    timer_.reset();
                }

                break;

            case EventKey::Escape:

                // Escape = No
                state_ = State::Playing;
                timer_.reset();
                break;

            default:
                break;
        }
    }
}

}