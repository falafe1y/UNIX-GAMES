#pragma once

#include <deque>
#include <random>

#include "../../core/head/game.h"
#include "../../core/head/event.h"
#include "../../core/head/renderer.h"
#include <../../core/head/world_configuation.h>

namespace fgames::games
{

struct Position
{
    int x;
    int y;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
};

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

enum class SnakeState
{
    Running,
    GameOver
};

class SnakeGame : public fgames::core::Game
{
public:
    SnakeGame();

    void handle_event(
        const fgames::core::Event& event
    ) override;

    void update(
        float delta_time
    ) override;

    void render(
        fgames::core::Renderer& renderer
    ) override;

private:
    void move();

    void spawn_food();

    bool is_collision(
        const Position& position
    ) const;

    bool is_opposite(
        Direction first,
        Direction second
    ) const;

private:
    std::deque<Position> snake_;

    Position food_;

    Direction direction_;
    Direction next_direction_;

    SnakeState state_;

    float move_timer_;
    float move_interval_;

    int field_width_ = WorldConfig().world_width;
    int field_height_ = WorldConfig().world_height;

    std::mt19937 random_engine_;
};

}