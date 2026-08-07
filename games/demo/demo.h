#pragma once

#include "../../core/head/game.h"
#include "../../core/head/renderer.h"

namespace FGames::games
{

class DemoGame : public core::Game
{
public:
    void handle_event(const core::Event& event) override;
    void update(float delta_time) override;
    void render(core::Renderer& renderer) override;
    
private:
    int x_ = 10;
    int y_ = 10;
};

}