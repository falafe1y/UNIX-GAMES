#include "demo.h"

namespace fgames::games
{

DemoGame::DemoGame()
    : position_{2, 2}
{
}

void DemoGame::handle_event(const core::Event& event)
{
    if (event.type == core::EventType::QuitRequested)
    {
        result_ = core::GameResult::ExitToMenu;
        return;
    }

    if (event.type != core::EventType::KeyPressed)
        return;

    if (state_ == DemoState::Paused)
    {
        switch (event.key)
        {
            case core::EventKey::Enter:
                state_ = DemoState::Running;
                break;

            case core::EventKey::Escape:
                result_ = core::GameResult::ExitToMenu;
                break;

            default:
                break;
        }

        return;
    }

    switch (event.key)
    {
        case core::EventKey::Up:
            move(0, -1);
            break;

        case core::EventKey::Down:
            move(0, 1);
            break;

        case core::EventKey::Left:
            move(-1, 0);
            break;

        case core::EventKey::Right:
            move(1, 0);
            break;

        case core::EventKey::Escape:
            state_ = DemoState::Paused;
            break;

        default:
            break;
    }
}

void DemoGame::update(float delta_time)
{
    // DemoGame не использует таймер.
    // Движение происходит непосредственно по нажатию стрелок.
    (void)delta_time;
}

void DemoGame::render(core::Renderer& renderer)
{
    renderer.clear();
    // renderer.draw_border();
    renderer.draw(
        position_.x,
        position_.y,
        ftxui::Color::Green
    );
    renderer.present();
}

bool DemoGame::is_paused() const
{
    return state_ == DemoState::Paused;
}

void DemoGame::move(int dx, int dy)
{
    DemoPosition next_position{
        position_.x + dx,
        position_.y + dy
    };

    if (!is_inside_field(next_position))
        return;

    position_ = next_position;
}

bool DemoGame::is_inside_field(const DemoPosition& position) const
{
    return position.x >= 0 &&
           position.x < FIELD_WIDTH_ &&
           position.y >= 0 &&
           position.y < FIELD_HEIGHT_;
}

}