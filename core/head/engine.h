#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "game.h"
#include "renderer.h"
#include "timer.h"

namespace fgames::core
{

class Engine
{
public:
explicit Engine(Renderer& renderer);

// Обрабатывает FTXUI-событие игры.
// Возвращает true, если событие было обработано.
bool handle_event(
    Game& game,
    const ftxui::Event& event
);

// Обновляет игру и строит её FTXUI-представление.
ftxui::Element render(Game& game);

// Запускает периодический redraw.
void start_timer();

// Останавливает периодический redraw.
void stop_timer();

private:
Renderer& renderer_;

Timer timer_;

bool timer_running_{false};

};

}
