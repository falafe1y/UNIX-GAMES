#pragma once

#include <atomic>
#include <thread>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include "game.h"
#include "renderer.h"
#include "timer.h"

namespace fgames::core
{

class Engine
{
public:
    explicit Engine(
        Renderer& renderer,
        ftxui::ScreenInteractive& screen
    );

    ~Engine();

    bool handle_event(
        Game& game,
        const ftxui::Event& event
    );

    ftxui::Element render(Game& game);

    void start();
    void stop();

private:
    Renderer& renderer_;
    ftxui::ScreenInteractive& screen_;

    Timer timer_;

    std::atomic_bool running_{false};
    std::thread tick_thread_;
};

}