#pragma once

#include "event.h"

namespace fgames::core
{

enum class GameResult
{
    Continue,
    ExitToMenu
};

class Renderer;

class Game
{
public:
    // деструктор виртуальный чтобы очищать ресурсы наследников
    virtual ~Game() = default;

    // сюда будут приходить события, которые игра будет обрабатывать
    virtual void handle_event(const Event& event) = 0;

    // логика обновления кадра, каждая игра переопределяет сама
    // delta_time - время, за которое нужно отрисовать следующий кадр
    virtual void update(float delta_time) = 0;

    // отрисовка
    virtual void render(Renderer& renderer) = 0;

    GameResult result() const
    {
        return result_;
    }

    virtual bool is_paused() const = 0;

protected:
    GameResult result_ = GameResult::Continue;
};

}