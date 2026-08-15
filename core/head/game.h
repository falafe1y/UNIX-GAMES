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

    int world_width() const {return world_width_;};
    int world_height() const {return world_height_;};

protected:
    GameResult result_ = GameResult::Running;
    void set_world_size(int width, int height) {
        world_width_ = width;
        world_height_ = height;
    };

private:
    int world_width_ = 0;
    int world_height_ = 0;
};

}