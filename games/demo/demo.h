#pragma once

#include "../../core/head/game.h"
#include "../../core/head/renderer.h"

namespace fgames::games
{

struct DemoPosition
{
    int x;
    int y;

    bool operator==(const DemoPosition& other) const
    {
        return x == other.x && y == other.y;
    }
};

enum class DemoState
{
    Running,
    Paused
};

class DemoGame : public core::Game
{
public:
    DemoGame();

    void handle_event(const core::Event& event) override;
    void update(float delta_time) override;
    void render(core::Renderer& renderer) override;

    bool is_paused() const override;

private:
    void move(int dx, int dy);
    bool is_inside_field(const DemoPosition& position) const;

private:
    DemoState state_{DemoState::Running};

    DemoPosition position_{};

    const int FIELD_WIDTH_ = WorldConfig().world_width;
    const int FIELD_HEIGHT_ = WorldConfig().world_height;
};

}