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
    if (event.type == fgames::core::EventType::QuitRequested)
        return;

    if (event.type != fgames::core::EventType::KeyPressed)
        return;

    // =========================================================
    // PAUSED
    // =========================================================

    if (state_ == SnakeState::Paused)
    {
        switch (event.key)
        {
            case fgames::core::EventKey::Left:
            case fgames::core::EventKey::Up:

                // Yes
                menu_selection_ = true;

                break;

            case fgames::core::EventKey::Right:
            case fgames::core::EventKey::Down:

                // No
                menu_selection_ = false;

                break;

            case fgames::core::EventKey::Enter:

                if (menu_selection_)
                {
                    // Хотим выйти в меню.
                    result_ = fgames::core::GameResult::ExitToMenu;
                }
                else
                {
                    // Продолжаем игру.
                    state_ = SnakeState::Running;

                    // Начинаем отсчёт времени заново.
                    move_timer_ = 0.0f;
                }

                break;

            case fgames::core::EventKey::Escape:

                // Escape = No
                state_ = SnakeState::Running;

                menu_selection_ = false;
                move_timer_ = 0.0f;

                break;

            default:
                break;
        }

        return;
    }

    // =========================================================
    // GAME OVER
    // =========================================================

    if (state_ == SnakeState::GameOver)
    {
        switch (event.key)
        {
            case fgames::core::EventKey::Left:
            case fgames::core::EventKey::Up:

                // Restart
                menu_selection_ = true;

                break;

            case fgames::core::EventKey::Right:
            case fgames::core::EventKey::Down:

                // Exit to menu
                menu_selection_ = false;
                break;

            case fgames::core::EventKey::Enter:

                if (menu_selection_)
                {
                    restart();
                }
                else
                {
                    result_ = fgames::core::GameResult::ExitToMenu;
                }

                break;

            default:
                break;
        }

        return;
    }

    // =========================================================
    // RUNNING
    // =========================================================

    switch (event.key)
    {
        case fgames::core::EventKey::Escape:

            // Переходим на паузу.
            state_ = SnakeState::Paused;

            // По умолчанию выбираем No.
            menu_selection_ = false;

            break;


        case fgames::core::EventKey::Up:
            if (!is_opposite(
                    direction_,
                    Direction::Up))
            {
                next_direction_ = Direction::Up;
            }

            break;


        case fgames::core::EventKey::Down:
            if (!is_opposite(
                    direction_,
                    Direction::Down))
            {
                next_direction_ = Direction::Down;
            }

            break;

        case fgames::core::EventKey::Left:
            if (!is_opposite(
                    direction_,
                    Direction::Left))
            {
                next_direction_ = Direction::Left;
            }

            break;

        case fgames::core::EventKey::Right:
            if (!is_opposite(
                    direction_,
                    Direction::Right))
            {
                next_direction_ = Direction::Right;
            }

            break;

        default:
            break;
    }
}

void SnakeGame::update(float delta_time)
{
    if (state_ == SnakeState::GameOver) 
        return;

    move_timer_ += delta_time;

    if (move_timer_ < move_interval_) 
        return;

    move_timer_ -= move_interval_;
    direction_ = next_direction_;

    move();
}

void SnakeGame::move()
{
    SnakePosition head = snake_.front();

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

        // по умолчанию предлагаем Restart
        menu_selection_ = true;
        return;
    }

    snake_.push_front(head);

    if (head == food_)
    {   
        score_++;
        spawn_food();
    }
    else
    {
        snake_.pop_back();
    }
}

bool SnakeGame::is_collision(const SnakePosition& SnakePosition) const
{
    // Столкновение со стеной.
    if (SnakePosition.x < 0 || SnakePosition.x >= FIELD_WITDH_ ||
        SnakePosition.y < 0 || SnakePosition.y >= FIELD_HEIGHT_)
    {
        return true;
    }

    // Столкновение с собственным телом.
    for (const auto& segment : snake_)
    {
        if (segment == SnakePosition)
            return true;
    }

    return false;
}

void SnakeGame::spawn_food()
{
    std::uniform_int_distribution<int> x_distribution(0, FIELD_WITDH_ - 1);
    std::uniform_int_distribution<int> y_distribution(0, FIELD_HEIGHT_ - 1);

    SnakePosition new_food;

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

void SnakeGame::restart()
{
    snake_.clear();
    score_ = 0;
    snake_.push_back({2, 2});

    direction_ = Direction::Right;
    next_direction_ = Direction::Right;

    state_ = SnakeState::Running;

    move_timer_ = 0.0f;

    menu_selection_ = false;

    spawn_food();
}

bool SnakeGame::is_paused() const
{
    return state_ != SnakeState::Running;
}

void SnakeGame::render(fgames::core::Renderer& renderer)
{
    renderer.clear();
    renderer.set_score(score_);

    // Еда
    renderer.draw(food_.x, food_.y, ftxui::Color::Red);

    // Змея
    bool is_head = true;

    for (const auto& segment : snake_)
    {
        renderer.draw(segment.x, segment.y, is_head ? ftxui::Color::Yellow : ftxui::Color::Green);

        is_head = false;
    }

    if (state_ == SnakeState::Paused)
    {
        renderer.draw_exit_confirmation(menu_selection_);
    }
    
    if (state_ == SnakeState::GameOver)
    {
        renderer.draw_gameover(menu_selection_);

    }

    renderer.present();
}

}