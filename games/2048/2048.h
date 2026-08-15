#pragma once

#include "../../core/head/game.h"
#include "../../core/head/renderer.h"

#include <array>
#include <random>

namespace fgames::games
{

enum class Game2048State
{
    Running,
    Paused,
    GameOver
};

class Game2048 : public fgames::core::Game
{
public:
    Game2048();

    void handle_event(const fgames::core::Event& event) override;
    void update(float delta_time) override;
    void render(fgames::core::Renderer& renderer) override;

    bool is_paused() const override;

private:
    static constexpr int BOARD_SIZE = 4;
    const int TILE_WIDTH = 8;
    const int TILE_HEIGHT = 5;

    using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;

    void init_game();
    void spawn_tile();

    bool move_left();
    bool move_right();
    bool move_up();
    bool move_down();

    bool move_line(std::array<int, BOARD_SIZE>& line);

    bool can_move() const;
    bool has_empty_cell() const;

    void check_game_over();

    void handle_running_event(const fgames::core::Event& event);
    void handle_paused_event(const fgames::core::Event& event);
    void handle_gameover_event(const fgames::core::Event& event);

    void draw_board(fgames::core::Renderer& renderer) const;
    void draw_tile(
        fgames::core::Renderer& renderer,
        int row,
        int column,
        int value
    ) const;

    ftxui::Color tile_color(int value) const;

    // =========================================================
    // STATE
    // =========================================================

    Board board_{};

    Game2048State state_{Game2048State::Running};

    bool menu_selection_{false};

    int score_{0};

    std::mt19937 random_engine_;
};

}