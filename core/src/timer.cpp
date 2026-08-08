#include "../head/timer.h"

namespace fgames::core
{

float Timer::delta_time()
{
    auto current = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = current - lastTime_;
    lastTime_ = current;
    return elapsed.count();
}

}