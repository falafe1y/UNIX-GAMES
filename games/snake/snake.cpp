#include "snake.h"

namespace fgames::games
{

SnakeGame::SnakeGame()
:
direction_(Direction::Right),
next_direction_(Direction::Right),
state_(SnakeState::Running),
move_timer_(0.0f),
move_interval_(0.15f),
random_engine_(std::random_device{}())
{
    snake_.push_back({2, 2});

    spawn_food();
}

void SnakeGame::handle_event(
    const fgames::core::Event& event
)
{
    if (event.type == fgames::core::EventType::QuitRequested) return;
    if (event.type != fgames::core::EventType::KeyPressed) return;

    switch (event.key)
    {
        case fgames::core::EventKey::Up:
            if (!is_opposite(direction_, Direction::Up)) {
                next_direction_ = Direction::Up;
            }

            break;

        case fgames::core::EventKey::Down:
            if (!is_opposite(direction_, Direction::Down)) {
                next_direction_ = Direction::Down;
            }

            break;

        case fgames::core::EventKey::Left:
            if (!is_opposite(direction_, Direction::Left)) {
                next_direction_ = Direction::Left;
            }

            break;

        case fgames::core::EventKey::Right:
            if (!is_opposite(direction_, Direction::Right)) {
                next_direction_ = Direction::Right;
            }

            break;

        default:
            break;
    }
}

void SnakeGame::update(float delta_time)
{
    if (state_ == SnakeState::GameOver) return;

    move_timer_ += delta_time;

    if (move_timer_ < move_interval_) return;

    move_timer_ -= move_interval_;
    direction_ = next_direction_;

    move();
}

void SnakeGame::move()
{
    Position head = snake_.front();

    switch (direction_)
    {
        case Direction::Up:
            --head.y;
            break;

        case Direction::Down:
            ++head.y;
            break;

        case Direction::Left:
            --head.x;
            break;

        case Direction::Right:
            ++head.x;
            break;
    }

    if (is_collision(head))
    {
        state_ = SnakeState::GameOver;
        return;
    }

    snake_.push_front(head);

    if (head == food_)
    {
        spawn_food();
    }
    else
    {
        snake_.pop_back();
    }
}

bool SnakeGame::is_collision(const Position& position) const
{
    // Столкновение со стеной.
    if (position.x <= 0 ||
        position.x >= field_width_ - 1 ||
        position.y <= 0 ||
        position.y >= field_height_ - 1)
    {
        return true;
    }

    // Столкновение с собственным телом.
    for (const auto& segment : snake_)
    {
        if (segment == position)
            return true;
    }

    return false;
}

void SnakeGame::spawn_food()
{
    std::uniform_int_distribution<int> x_distribution(1, field_width_ - 2);
    std::uniform_int_distribution<int> y_distribution(1, field_height_ - 2);

    Position new_food;

    do
    {
        new_food.x = x_distribution(random_engine_);
        new_food.y = y_distribution(random_engine_);

    } while (is_collision(new_food));

    food_ = new_food;
}

bool SnakeGame::is_opposite(Direction first, Direction second) const
{
    return
        (first == Direction::Up &&
         second == Direction::Down) ||

        (first == Direction::Down &&
         second == Direction::Up) ||

        (first == Direction::Left &&
         second == Direction::Right) ||

        (first == Direction::Right &&
         second == Direction::Left);
}

void SnakeGame::render(fgames::core::Renderer& renderer)
{
    renderer.clear();

    renderer.draw_border();

    // Еда
    renderer.draw(food_.x, food_.y, '*');

    // Змея
    bool is_head = true;

    for (const auto& segment : snake_)
    {
        renderer.draw(segment.x, segment.y, is_head ? '@' : 'o');

        is_head = false;
    }

    renderer.present();
}

}