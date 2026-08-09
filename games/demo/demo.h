#pragma once

#include "../../core/head/game.h"
#include "../../core/head/renderer.h"

namespace fgames::games
{

class DemoGame : public core::Game
{
public:
    void handle_event(const core::Event& event) override;
    void update(float delta_time) override;
    void render(core::Renderer& renderer) override;
    bool is_paused() const override {
        return false;
    }
    
private:
    int x_ = 10;
    int y_ = 10;
};

}