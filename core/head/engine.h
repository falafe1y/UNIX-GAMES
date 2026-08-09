#pragma once

#include "game.h"
#include "input.h"
#include "renderer.h"
#include "timer.h"
// #include "terminal.h"

#include <memory>

namespace fgames::core
{

class Engine
{
public:
    Engine(Input& input, Renderer& renderer);

    bool run(Game& game);

private:
    enum class State
    {
        Playing,
        ConfirmExit
    };

    void handle_game_events(Game& game, const std::vector<Event>& events);
    void handle_confirmation_events(const std::vector<Event>& events);
    void render_confirmation();

private:
    bool running_{true};
    Input& input_;
    Renderer& renderer_;
    Timer timer_;
    
    // false = No
    // true  = Yes
    State state_{State::Playing};
    bool confirmation_selection_{false};
    bool exit_confirmed_{false};
};

}