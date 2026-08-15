#pragma once

#include <deque>
#include <random>

#include "../../core/head/game.h"
#include "../../core/head/event.h"
#include "../../core/head/renderer.h"

namespace fgames::games
{

struct SnakePosition
{
    int x;
    int y;

    bool operator==(const SnakePosition& other) const
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
    Paused,
    GameOver
};

class SnakeGame : public fgames::core::Game
{
public:
    SnakeGame();

    void handle_event(const fgames::core::Event& event) override;
    void update(float delta_time) override;
    void render(fgames::core::Renderer& renderer) override;
    bool is_paused() const override;

private:
    void move();
    void spawn_food();
    bool is_collision(const SnakePosition& position) const;
    bool is_opposite(Direction first, Direction second) const;
    void restart();

private:
    SnakeState state_{SnakeState::Running};

    // false = No
    // true  = Yes
    bool menu_selection_{false};

    std::deque<SnakePosition> snake_;
    SnakePosition food_;
    Direction direction_;
    Direction next_direction_;

    float move_timer_;
    float move_interval_;

    const int FIELD_WIDTH_ = 20;
    const int FIELD_HEIGHT_ = 20;

    std::mt19937 random_engine_;

    int score_ = 0;
};

}