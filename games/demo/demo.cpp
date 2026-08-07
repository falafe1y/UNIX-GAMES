#include "demo.h"

#include <iostream>

namespace FGames::games
{

void DemoGame::handle_event(
    const core::Event& event)
{
    if (event.type != core::EventType::KeyPressed)
        return;

    switch (event.key)
    {
    case core::Key::Left:
        --x_;
        break;

    case core::Key::Right:
        ++x_;
        break;

    case core::Key::Up:
        --y_;
        break;

    case core::Key::Down:
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

    renderer.draw(
        x_,
        y_,
        '@');

    renderer.present();
}

}