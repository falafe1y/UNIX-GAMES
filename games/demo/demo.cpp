#include "demo.h"

#include <iostream>

namespace fgames::games
{

void DemoGame::handle_event(
    const core::Event& event)
{
    if (event.type != core::EventType::KeyPressed) return;

    switch (event.key)
    {
    case core::EventKey::Left:
        --x_;
        break;

    case core::EventKey::Right:
        ++x_;
        break;

    case core::EventKey::Up:
        --y_;
        break;

    case core::EventKey::Down:
        ++y_;
        break;

    default:
        break;
    }
}

void DemoGame::update(float)
{
}

void DemoGame::render(core::Renderer& renderer)
{
    renderer.clear();
    renderer.draw(x_, y_, '@');
    renderer.draw_border();
    renderer.present();
}

}