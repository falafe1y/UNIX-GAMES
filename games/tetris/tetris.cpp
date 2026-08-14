#include "tetris.h"

namespace fgames::games
{

// Статические фигуры (относительные координаты)
std::vector<TetrisPosition> TetrisGame::Tetromino::get_shape(TetrominoType type, int rotation)
{
    std::vector<TetrisPosition> shape;
    
    switch (type)
    {
        case TetrominoType::I:
            if (rotation == 0 || rotation == 2)
                shape = {{0,0}, {0,1}, {0,2}, {0,3}};
            else
                shape = {{0,0}, {1,0}, {2,0}, {3,0}};
            break;
            
        case TetrominoType::O:
            shape = {{0,0}, {0,1}, {1,0}, {1,1}};
            break;
            
        case TetrominoType::T:
            if (rotation == 0)
                shape = {{0,0}, {0,1}, {0,2}, {1,1}};
            else if (rotation == 1)
                shape = {{0,0}, {1,0}, {2,0}, {1,1}};
            else if (rotation == 2)
                shape = {{0,1}, {1,0}, {1,1}, {1,2}};
            else
                shape = {{0,1}, {1,0}, {1,1}, {2,1}};
            break;
            
        case TetrominoType::S:
            if (rotation == 0 || rotation == 2)
                shape = {{0,1}, {0,2}, {1,0}, {1,1}};
            else
                shape = {{0,0}, {1,0}, {1,1}, {2,1}};
            break;
            
        case TetrominoType::Z:
            if (rotation == 0 || rotation == 2)
                shape = {{0,0}, {0,1}, {1,1}, {1,2}};
            else
                shape = {{0,1}, {1,0}, {1,1}, {2,0}};
            break;
            
        case TetrominoType::J:
            if (rotation == 0)
                shape = {{0,0}, {0,1}, {0,2}, {1,0}};
            else if (rotation == 1)
                shape = {{0,0}, {1,0}, {2,0}, {2,1}};
            else if (rotation == 2)
                shape = {{0,2}, {1,0}, {1,1}, {1,2}};
            else
                shape = {{0,0}, {0,1}, {1,1}, {2,1}};
            break;
            
        case TetrominoType::L:
            if (rotation == 0)
                shape = {{0,0}, {0,1}, {0,2}, {1,2}};
            else if (rotation == 1)
                shape = {{0,0}, {0,1}, {1,0}, {2,0}};
            else if (rotation == 2)
                shape = {{0,0}, {1,0}, {1,1}, {1,2}};
            else
                shape = {{0,1}, {1,1}, {2,0}, {2,1}};
            break;
    }
    
    return shape;
}

TetrisGame::TetrisGame()
:
field_(FIELD_HEIGHT_, std::vector<bool>(FIELD_WIDTH_, false)),
random_engine_(std::random_device{}())
{
    // Проверка, что размеры корректны
    if (FIELD_WIDTH_ <= 0 || FIELD_HEIGHT_ <= 0) {
        std::cerr << "ERROR: Invalid field dimensions!" << std::endl;
        // Выход из программы или исправление
        std::exit(1);
    }
    
    init_game();
}

void TetrisGame::init_game()
{
    // Очистка поля
    for (auto& row : field_)
        std::fill(row.begin(), row.end(), false);
    
    state_ = TetrisState::Running;
    score_ = 0;
    level_ = 1;
    lines_cleared_ = 0;
    fall_interval_ = 0.8f;
    fall_timer_ = 0.0f;
    menu_selection_ = false;
    
    // Генерация первой фигуры
    std::uniform_int_distribution<int> dist(0, 6);
    next_tetromino_ = Tetromino(static_cast<TetrominoType>(dist(random_engine_)));
    spawn_tetromino();
}

void TetrisGame::spawn_tetromino()
{
    current_tetromino_ = next_tetromino_;
    
    // Позиционируем по центру сверху
    current_tetromino_.position.x = (FIELD_WIDTH_ - 4) / 2;
    current_tetromino_.position.y = 0;
    
    // Генерируем следующую фигуру
    std::uniform_int_distribution<int> dist(0, 6);
    next_tetromino_ = Tetromino(static_cast<TetrominoType>(dist(random_engine_)));
    
    // Проверка на Game Over
    if (is_collision(current_tetromino_, 0, 0))
    {
        state_ = TetrisState::GameOver;
        menu_selection_ = true;  // По умолчанию предлагаем рестарт
    }
}

bool TetrisGame::is_collision(const Tetromino& tetromino, int dx, int dy) const
{
    for (const auto& block : tetromino.blocks)
    {
        int new_x = tetromino.position.x + block.x + dx;
        int new_y = tetromino.position.y + block.y + dy;
        
        // Проверка границ
        if (new_x < 0 || new_x >= FIELD_WIDTH_ ||
            new_y < 0 || new_y >= FIELD_HEIGHT_)
        {
            return true;
        }
        
        // Проверка занятости (игнорируем текущую фигуру)
        if (field_[new_y][new_x])
        {
            return true;
        }
    }
    
    return false;
}

// ============================================================
// LOCK
// ============================================================

void TetrisGame::lock_tetromino()
{
    for (const auto& block : current_tetromino_.blocks)
    {
        int x = current_tetromino_.position.x + block.x;
        int y = current_tetromino_.position.y + block.y;
        
        if (y >= 0 && y < FIELD_HEIGHT_ && x >= 0 && x < FIELD_WIDTH_)
        {
            field_[y][x] = true;
        }
    }
}

// ============================================================
// CLEAR LINES
// ============================================================

int TetrisGame::clear_lines()
{
    int cleared = 0;
    
    for (int y = FIELD_HEIGHT_ - 1; y >= 0; )
    {
        bool full = true;
        for (int x = 0; x < FIELD_WIDTH_; ++x)
        {
            if (!field_[y][x])
            {
                full = false;
                break;
            }
        }
        
        if (full)
        {
            // Сдвигаем все строки вниз
            for (int row = y; row > 0; --row)
            {
                field_[row] = field_[row - 1];
            }
            std::fill(field_[0].begin(), field_[0].end(), false);
            
            cleared++;
            // Не увеличиваем y, так как на эту позицию сдвинулась строка сверху
        }
        else
        {
            y--;
        }
    }
    
    return cleared;
}

// ============================================================
// HANDLE EVENT
// ============================================================

void TetrisGame::handle_event(const fgames::core::Event& event)
{
    if (event.type == fgames::core::EventType::QuitRequested)
        return;

    if (event.type != fgames::core::EventType::KeyPressed)
        return;

    // =========================================================
    // PAUSED
    // =========================================================

    if (state_ == TetrisState::Paused)
    {
        switch (event.key)
        {
            case fgames::core::EventKey::Left:
            case fgames::core::EventKey::Up:
                menu_selection_ = true;
                break;

            case fgames::core::EventKey::Right:
            case fgames::core::EventKey::Down:
                menu_selection_ = false;
                break;

            case fgames::core::EventKey::Enter:
                if (menu_selection_)
                {
                    result_ = fgames::core::GameResult::ExitToMenu;
                }
                else
                {
                    state_ = TetrisState::Running;
                    fall_timer_ = 0.0f;
                }
                break;

            case fgames::core::EventKey::Escape:
                state_ = TetrisState::Running;
                menu_selection_ = false;
                fall_timer_ = 0.0f;
                break;

            default:
                break;
        }
        return;
    }

    // =========================================================
    // GAME OVER
    // =========================================================

    if (state_ == TetrisState::GameOver)
    {
        switch (event.key)
        {
            case fgames::core::EventKey::Left:
            case fgames::core::EventKey::Up:
                menu_selection_ = true;
                break;

            case fgames::core::EventKey::Right:
            case fgames::core::EventKey::Down:
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
            state_ = TetrisState::Paused;
            menu_selection_ = false;
            break;

        case fgames::core::EventKey::Left:
            if (!is_collision(current_tetromino_, -1, 0))
            {
                current_tetromino_.position.x--;
            }
            break;

        case fgames::core::EventKey::Right:
            if (!is_collision(current_tetromino_, 1, 0))
            {
                current_tetromino_.position.x++;
            }
            break;

        case fgames::core::EventKey::Down:
            if (!is_collision(current_tetromino_, 0, 1))
            {
                current_tetromino_.position.y++;
                fall_timer_ = 0.0f;
            }
            break;

        case fgames::core::EventKey::Up:
            {
                Tetromino rotated = current_tetromino_;
                rotated.rotate();
                if (!is_collision(rotated, 0, 0))
                {
                    current_tetromino_ = rotated;
                }
            }
            break;

        case fgames::core::EventKey::Space:
            {
                while (!is_collision(current_tetromino_, 0, 1))
                {
                    current_tetromino_.position.y++;
                }
                lock_tetromino();
                int lines = clear_lines();
                if (lines > 0)
                {
                    lines_cleared_ += lines;
                    if (lines_cleared_ >= level_ * 10)
                    {
                        level_++;
                        fall_interval_ = std::max(0.1f, 0.8f - (level_ - 1) * 0.07f);
                    }
                    score_ += lines * 2 * level_;
                }
                spawn_tetromino();
            }
            break;

        default:
            break;
    }
}

// ============================================================
// UPDATE
// ============================================================

void TetrisGame::update(float delta_time)
{
    if (state_ != TetrisState::Running)
        return;

    fall_timer_ += delta_time;

    if (fall_timer_ < fall_interval_)
        return;

    fall_timer_ = 0.0f;

    // Падение вниз
    if (!is_collision(current_tetromino_, 0, 1))
    {
        current_tetromino_.position.y++;
    }
    else
    {
        // Фигура достигла дна
        lock_tetromino();
        
        int lines = clear_lines();
        if (lines > 0)
        {
            lines_cleared_ += lines;
            
            // Обновление уровня и скорости
            if (lines_cleared_ >= level_ * 10)
            {
                level_++;
                fall_interval_ = std::max(0.1f, 0.8f - (level_ - 1) * 0.07f);
            }
            
            score_ += lines * 2 * level_;
        }
        
        spawn_tetromino();
    }
}

void TetrisGame::restart()
{
    init_game();
}

bool TetrisGame::is_paused() const
{
    return state_ != TetrisState::Running;
}

void TetrisGame::render(fgames::core::Renderer& renderer)
{
    renderer.clear();
    renderer.set_score(score_);

    // Отрисовка игрового поля
    for (int y = 0; y < FIELD_HEIGHT_; ++y)
    {
        for (int x = 0; x < FIELD_WIDTH_; ++x)
        {
            if (field_[y][x])
            {
                renderer.draw(x, y, ftxui::Color::GrayLight);
            }
        }
    }

    // Отрисовка текущей фигуры
    for (const auto& block : current_tetromino_.blocks)
    {
        int x = current_tetromino_.position.x + block.x;
        int y = current_tetromino_.position.y + block.y;
        
        if (y >= 0 && y < FIELD_HEIGHT_ && x >= 0 && x < FIELD_WIDTH_)
        {
            renderer.draw(x, y, ftxui::Color::Green);
        }
    }

    if (state_ == TetrisState::Paused)
    {
        renderer.draw_exit_confirmation(menu_selection_);
    }
    
    if (state_ == TetrisState::GameOver)
    {
        score_ = 0;
        renderer.draw_gameover(menu_selection_);
    }

    renderer.present();
}

} 