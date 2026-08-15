#pragma once

#include <deque>
#include <random>
#include <vector>
#include <map>
#include <iostream>

#include "../../core/head/game.h"
#include "../../core/head/event.h"
#include "../../core/head/renderer.h"

namespace fgames::games
{

struct TetrisPosition
{
    int x;
    int y;

    bool operator==(const TetrisPosition& other) const
    {
        return x == other.x && y == other.y;
    }
};

enum class TetrisState
{
    Running,
    Paused,
    GameOver
};


class TetrisGame : public fgames::core::Game
{
public:
    TetrisGame();

    void handle_event(const fgames::core::Event& event) override;
    void update(float delta_time) override;
    void render(fgames::core::Renderer& renderer) override;
    bool is_paused() const override;

private:
    enum class TetrominoType { I, O, T, S, Z, J, L };

    struct Tetromino
    {
        TetrominoType type;
        std::vector<TetrisPosition> blocks;  // Относительные координаты
        TetrisPosition position;              // Позиция на поле
        int rotation;                   // 0, 1, 2, 3

        Tetromino() : type(TetrominoType::I), rotation(0), position{0, 0} {}
        Tetromino(TetrominoType t) : type(t), rotation(0), position{0, 0} 
        {
            set_blocks();
        }

        void set_blocks()
        {
            blocks = get_shape(type, rotation);
        }

        void rotate()
        {
            rotation = (rotation + 1) % 4;
            blocks = get_shape(type, rotation);
        }

        static std::vector<TetrisPosition> get_shape(TetrominoType type, int rotation);
    };

private:
    void init_game();
    void spawn_tetromino();
    bool is_collision(const Tetromino& tetromino, int dx, int dy) const;
    void lock_tetromino();
    int clear_lines();
    void restart();

private:
    const int FIELD_WIDTH_ = 30;
    const int FIELD_HEIGHT_ = 25;

    TetrisState state_{TetrisState::Running};
    bool menu_selection_{false};

    std::vector<std::vector<bool>> field_;  // true = filled

    Tetromino current_tetromino_;
    Tetromino next_tetromino_;

    float fall_timer_{0.0f};
    float fall_interval_{0.8f};
    
    int score_{0};
    int level_{1};
    int lines_cleared_{0};

    std::mt19937 random_engine_;
};

}