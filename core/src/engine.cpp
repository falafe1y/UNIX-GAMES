#include "../head/engine.h"

#include <atomic>
#include <chrono>
#include <thread>

#include <ftxui/component/component.hpp>

namespace fgames::core
{

namespace
{

core::Event convert_event(const ftxui::Event& event)
{
    using FtxEvent = ftxui::Event;

    if (event == FtxEvent::ArrowUp)
    {
        return {
            EventType::KeyPressed,
            EventKey::Up
        };
    }

    if (event == FtxEvent::ArrowDown)
    {
        return {
            EventType::KeyPressed,
            EventKey::Down
        };
    }

    if (event == FtxEvent::ArrowLeft)
    {
        return {
            EventType::KeyPressed,
            EventKey::Left
        };
    }

    if (event == FtxEvent::ArrowRight)
    {
        return {
            EventType::KeyPressed,
            EventKey::Right
        };
    }

    if (event == FtxEvent::Return)
    {
        return {
            EventType::KeyPressed,
            EventKey::Enter
        };
    }

    if (event == FtxEvent::Character(' '))
    {
        return {
            EventType::KeyPressed,
            EventKey::Space
        };
    }

    if (event == FtxEvent::Escape)
    {
        return {
            EventType::KeyPressed,
            EventKey::Escape
        };
    }

    if (event == FtxEvent::Character('q') ||
        event == FtxEvent::Character('Q'))
    {
        return {
            EventType::QuitRequested,
            EventKey::Unknown
        };
    }

    return {
        EventType::KeyPressed,
        EventKey::Unknown
    };
}

}

Engine::Engine(Renderer& renderer)
    :
    renderer_(renderer)
{
}

bool Engine::run(Game& game)
{
    using namespace std::chrono_literals;

    running_ = true;

    timer_.reset();

    std::atomic<bool> stop_timer{false};

    /*
     * Будим FTXUI примерно 60 раз в секунду.
     *
     * Это позволяет игровому Renderer callback
     * регулярно вызываться даже когда пользователь
     * ничего не нажимает.
     */
    std::thread timer_thread(
        [&]()
        {
            while (!stop_timer)
            {
                std::this_thread::sleep_for(16ms);

                if (!stop_timer)
                {
                    renderer_.request_frame();
                }
            }
        }
    );

    /*
     * ========================================================
     * GAME RENDERER
     * ========================================================
     */

    auto game_renderer = ftxui::Renderer(
        [&]()
        {
            /*
             * Обновляем игровую логику.
             */
            if (!game.is_paused())
            {
                const float delta_time =
                    timer_.delta_time();

                game.update(delta_time);
            }
            else
            {
                timer_.reset();
            }

            /*
             * Очищаем framebuffer.
             */
            renderer_.clear();

            /*
             * Игра рисует себя в наш Renderer.
             */
            game.render(renderer_);

            /*
             * Игра хочет вернуться в Launcher.
             */
            if (game.result() == GameResult::ExitToMenu)
            {
                running_ = false;

                renderer_.screen()
                    .ExitLoopClosure()();
            }

            /*
             * Превращаем framebuffer в FTXUI DOM.
             */
            return renderer_.present();
        }
    );

    /*
     * ========================================================
     * INPUT
     * ========================================================
     */

    auto component = ftxui::CatchEvent(
        game_renderer,
        [&](const ftxui::Event& event)
        {
            /*
             * Custom используется только для
             * периодического обновления игры.
             *
             * Не передаём его в Game.
             */
            if (event == ftxui::Event::Custom)
            {
                return false;
            }

            /*
             * Q -> выход из игры.
             */
            if (event == ftxui::Event::Character('q') ||
                event == ftxui::Event::Character('Q'))
            {
                game.handle_event({
                    EventType::QuitRequested,
                    EventKey::Unknown
                });

                return true;
            }

            /*
             * Все остальные клавиши конвертируем
             * из FTXUI Event -> core::Event.
             */
            const core::Event core_event =
                convert_event(event);

            /*
             * Неизвестные события не отправляем игре.
             */
            if (core_event.key != EventKey::Unknown ||
                core_event.type == EventType::QuitRequested)
            {
                game.handle_event(core_event);
            }

            /*
             * Игра могла изменить свой результат
             * прямо во время обработки события.
             */
            if (game.result() == GameResult::ExitToMenu)
            {
                running_ = false;

                renderer_.screen()
                    .ExitLoopClosure()();
            }

            return true;
        }
    );

    /*
     * ========================================================
     * FTXUI EVENT LOOP
     * ========================================================
     */

    renderer_.screen().Loop(component);

    /*
     * ========================================================
     * STOP TIMER
     * ========================================================
     */

    stop_timer = true;

    if (timer_thread.joinable())
    {
        timer_thread.join();
    }

    return game.result() == GameResult::ExitToMenu;
}

}