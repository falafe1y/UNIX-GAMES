#pragma once

#include <deque>
#include <random>

#include "../../core/head/game.h"
#include "../../core/head/event.h"
#include "../../core/head/renderer.h"

namespace FGames::games
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

class SnakeGame : public FGames::core::Game
{
public:
    SnakeGame();

    void handle_event(
        const FGames::core::Event& event
    ) override;

    void update(
        float delta_time
    ) override;

    void render(
        FGames::core::Renderer& renderer
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

    int field_width_;
    int field_height_;

    std::mt19937 random_engine_;
};

}