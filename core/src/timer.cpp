#include "../head/timer.h"

namespace fgames::core
{

float Timer::tick()
{
    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<float> elapsed = now - last_time_;
    last_time_ = now;
    return elapsed.count();
}

void Timer::reset()
{
    last_time_ = std::chrono::steady_clock::now();
}

}