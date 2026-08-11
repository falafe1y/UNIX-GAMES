#pragma once

#include "event.h"

namespace fgames::core
{

enum class GameResult
{
    Running,
    ExitToMenu,
    Restart
};

class Renderer;

class Game
{
public:
    virtual ~Game() = default;

    virtual void handle_event(const Event& event) = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(Renderer& renderer) = 0;

    GameResult result() const
    {
        return result_;
    }

    virtual bool is_paused() const = 0;
    
protected:
    GameResult result_ = GameResult::Running;
};

}