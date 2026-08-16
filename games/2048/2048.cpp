#include "2048.h"

#include <algorithm>
#include <array>
#include <vector>

namespace fgames::games
{

Game2048::Game2048()
    :
    random_engine_(std::random_device{}())
{
    set_world_size(WORLD_WIDTH_, WORLD_HEIGHT_);
    init_game();
}

void Game2048::init_game()
{
    for (auto& row : board_)
    {
        row.fill(0);
    }

    score_ = 0;

    state_ = Game2048State::Running;

    menu_selection_ = false;

    result_ = fgames::core::GameResult::Running;

    spawn_tile();
    spawn_tile();
}

void Game2048::spawn_tile()
{
    std::vector<std::pair<int, int>> empty_cells;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            if (board_[row][column] == 0)
            {
                empty_cells.emplace_back(row, column);
            }
        }
    }

    if (empty_cells.empty())
    {
        return;
    }

    std::uniform_int_distribution<std::size_t> position_distribution(
        0,
        empty_cells.size() - 1
    );

    const auto [row, column] =
        empty_cells[position_distribution(random_engine_)];

    // 90% -> 2
    // 10% -> 4
    std::uniform_int_distribution<int> value_distribution(1, 10);

    board_[row][column] =
        value_distribution(random_engine_) == 10
            ? 4
            : 2;
}

bool Game2048::move_line(
    std::array<int, BOARD_SIZE>& line
)
{
    const auto original = line;

    std::array<int, BOARD_SIZE> compressed{};

    int compressed_size = 0;

    // --------------------------------------------------------
    // Убираем нули.
    // --------------------------------------------------------

    for (const int value : line)
    {
        if (value != 0)
        {
            compressed[compressed_size++] = value;
        }
    }

    // --------------------------------------------------------
    // Объединяем одинаковые соседние клетки.
    // --------------------------------------------------------

    std::array<int, BOARD_SIZE> merged{};

    int merged_size = 0;

    for (int i = 0; i < compressed_size; ++i)
    {
        if (i + 1 < compressed_size &&
            compressed[i] == compressed[i + 1])
        {
            const int value = compressed[i] * 2;

            merged[merged_size++] = value;

            score_ += value;

            ++i;
        }
        else
        {
            merged[merged_size++] = compressed[i];
        }
    }

    // --------------------------------------------------------
    // Возвращаем нули справа.
    // --------------------------------------------------------

    line.fill(0);

    for (int i = 0; i < merged_size; ++i)
    {
        line[i] = merged[i];
    }

    return line != original;
}

bool Game2048::move_left()
{
    bool changed = false;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        if (move_line(board_[row]))
        {
            changed = true;
        }
    }

    return changed;
}

bool Game2048::move_right()
{
    bool changed = false;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        std::reverse(
            board_[row].begin(),
            board_[row].end()
        );

        if (move_line(board_[row]))
        {
            changed = true;
        }

        std::reverse(
            board_[row].begin(),
            board_[row].end()
        );
    }

    return changed;
}

bool Game2048::move_up()
{
    bool changed = false;

    for (int column = 0; column < BOARD_SIZE; ++column)
    {
        std::array<int, BOARD_SIZE> line{};

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            line[row] = board_[row][column];
        }

        if (move_line(line))
        {
            changed = true;
        }

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            board_[row][column] = line[row];
        }
    }

    return changed;
}

bool Game2048::move_down()
{
    bool changed = false;

    for (int column = 0; column < BOARD_SIZE; ++column)
    {
        std::array<int, BOARD_SIZE> line{};

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            line[row] = board_[row][column];
        }

        std::reverse(
            line.begin(),
            line.end()
        );

        if (move_line(line))
        {
            changed = true;
        }

        std::reverse(
            line.begin(),
            line.end()
        );

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            board_[row][column] = line[row];
        }
    }

    return changed;
}

bool Game2048::has_empty_cell() const
{
    for (const auto& row : board_)
    {
        for (const int value : row)
        {
            if (value == 0)
            {
                return true;
            }
        }
    }

    return false;
}

bool Game2048::can_move() const
{
    if (has_empty_cell())
    {
        return true;
    }

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            const int value = board_[row][column];

            if (row + 1 < BOARD_SIZE &&
                board_[row + 1][column] == value)
            {
                return true;
            }

            if (column + 1 < BOARD_SIZE &&
                board_[row][column + 1] == value)
            {
                return true;
            }
        }
    }

    return false;
}

void Game2048::check_game_over()
{
    if (!can_move())
    {
        state_ = Game2048State::GameOver;
        menu_selection_ = true;
    }
}

void Game2048::handle_event(
    const fgames::core::Event& event
)
{
    if (event.type == fgames::core::EventType::QuitRequested)
    {
        return;
    }

    if (event.type != fgames::core::EventType::KeyPressed)
    {
        return;
    }

    if (state_ == Game2048State::Paused)
    {
        handle_paused_event(event);
        return;
    }

    if (state_ == Game2048State::GameOver)
    {
        handle_gameover_event(event);
        return;
    }

    handle_running_event(event);
}

void Game2048::handle_running_event(
    const fgames::core::Event& event
)
{
    bool moved = false;

    switch (event.key)
    {
        case fgames::core::EventKey::Left:
            moved = move_left();
            break;

        case fgames::core::EventKey::Right:
            moved = move_right();
            break;

        case fgames::core::EventKey::Up:
            moved = move_up();
            break;

        case fgames::core::EventKey::Down:
            moved = move_down();
            break;

        case fgames::core::EventKey::Escape:

            state_ = Game2048State::Paused;

            // По умолчанию выбираем No.
            menu_selection_ = false;

            return;

        default:
            return;
    }

    if (!moved)
    {
        check_game_over();
        return;
    }

    spawn_tile();

    check_game_over();
}

void Game2048::handle_paused_event(
    const fgames::core::Event& event
)
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
                result_ = fgames::core::GameResult::ExitToMenu;
            }
            else
            {
                state_ = Game2048State::Running;
            }

            break;

        case fgames::core::EventKey::Escape:

            state_ = Game2048State::Running;

            menu_selection_ = false;

            break;

        default:
            break;
    }
}

void Game2048::handle_gameover_event(
    const fgames::core::Event& event
)
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

            // Menu
            menu_selection_ = false;

            break;

        case fgames::core::EventKey::Enter:

            if (menu_selection_)
            {
                init_game();
            }
            else
            {
                result_ = fgames::core::GameResult::ExitToMenu;
            }

            break;

        default:
            break;
    }
}

void Game2048::update(float /*delta_time*/)
{
    // 2048 не имеет автоматического движения.
    //
    // Вся игровая логика происходит непосредственно
    // в handle_event().
}

void Game2048::render(
    fgames::core::Renderer& renderer
)
{
    renderer.clear();

    renderer.set_score(score_);

    draw_board(renderer);

    if (state_ == Game2048State::Paused)
    {
        renderer.draw_exit_confirmation(menu_selection_);
    }

    if (state_ == Game2048State::GameOver)
    {
        renderer.draw_gameover(menu_selection_);
    }
}

void Game2048::draw_board(
    fgames::core::Renderer& renderer
) const
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int column = 0; column < BOARD_SIZE; ++column)
        {
            draw_tile(
                renderer,
                row,
                column,
                board_[row][column]
            );
        }
    }
}

void Game2048::draw_tile(
    fgames::core::Renderer& renderer,
    int row,
    int column,
    int value
) const
{
    const int start_x =
        column * TILE_WIDTH;

    const int start_y =
        row * TILE_HEIGHT;

    const auto color = tile_color(value);

    // --------------------------------------------------------
    // Tile background.
    // --------------------------------------------------------

    for (int y = 0; y < TILE_HEIGHT; ++y)
    {
        for (int x = 0; x < TILE_WIDTH; ++x)
        {
            renderer.draw(
                start_x + x,
                start_y + y,
                color
            );
        }
    }

    if (value == 0)
        return;

    // --------------------------------------------------------
    // Tile value.
    // --------------------------------------------------------

    const std::string text =
        std::to_string(value);

    const int text_x =
        start_x +
        (TILE_WIDTH - static_cast<int>(text.size())) / 2;

    const int text_y =
        start_y + TILE_HEIGHT / 2;

    renderer.draw_text(
        text_x,
        text_y,
        text,
        ftxui::Color::Black
    );
}

ftxui::Color Game2048::tile_color(int value) const
{
    using ftxui::Color;

    switch (value)
    {
        case 0:
            return Color::Default;

        case 2:
            return Color::GrayLight;

        case 4:
            return Color::White;

        case 8:
            return Color::Yellow;

        case 16:
            return Color::Green;

        case 32:
            return Color::GreenLight;

        case 64:
            return Color::Red;

        case 128:
            return Color::Blue;

        case 256:
            return Color::Cyan;

        case 512:
            return Color::Magenta;

        case 1024:
            return Color::Yellow;

        case 2048:
            return Color::Green;

        default:
            return Color::White;
    }
}

// ============================================================
// OTHER
// ============================================================

bool Game2048::is_paused() const
{
    return state_ != Game2048State::Running;
}

}