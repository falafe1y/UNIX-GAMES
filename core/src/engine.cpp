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
                // Игра уже отрисована.
                // Просто накладываем плашку.
                render_confirmation();
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

void Engine::handle_confirmation_events(const std::vector<Event>& events)
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
                    render_confirmation();
                }

                break;

            case EventKey::Left:
            case EventKey::Down:

                if (!confirmation_selection_)
                {
                    confirmation_selection_ = true;
                    render_confirmation();
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

                    // ВАЖНО:
                    // сбрасываем накопившееся время паузы.
                    timer_.delta_time();
                }

                break;

            case EventKey::Escape:

                // Esc = No
                state_ = State::Playing;

                // Не даём времени паузы
                // попасть в следующий update().
                timer_.delta_time();

                break;

            default:
                break;
        }
    }
}

void Engine::render_confirmation()
{
    const int width = 54;
    const int height = 7;

    const int world_width =
        WorldConfig().world_width;

    const int world_height =
        WorldConfig().world_height;

    const int start_x =
        (world_width - width) / 2;

    const int start_y =
        (world_height - height) / 2;

    // Если поле слишком маленькое,
    // просто начинаем с координаты 0.
    const int x = start_x < 0 ? 0 : start_x;
    const int y = start_y < 0 ? 0 : start_y;

    renderer_.draw_text(
        x,
        y,
        "+----------------------------------------------------+"
    );

    renderer_.draw_text(
        x,
        y + 1,
        "| Do you really want to close the game and loss all |"
    );

    renderer_.draw_text(
        x,
        y + 2,
        "| your experience?                                   |"
    );

    renderer_.draw_text(
        x,
        y + 3,
        "|                                                    |"
    );

    if (confirmation_selection_)
    {
        renderer_.draw_text(
            x,
            y + 4,
            "|             [ Yes ]       No                      |"
        );
    }
    else
    {
        renderer_.draw_text(
            x,
            y + 4,
            "|               Yes       [ No ]                    |"
        );
    }

    renderer_.draw_text(
        x,
        y + 5,
        "|                                                    |"
    );

    renderer_.draw_text(
        x,
        y + 6,
        "+----------------------------------------------------+"
    );

    renderer_.present();
}

}