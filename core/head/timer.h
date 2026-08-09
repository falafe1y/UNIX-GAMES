#pragma once

#include <chrono>

namespace fgames::core
{

class Timer
{
public:
    float delta_time();
    void reset();

private:
    std::chrono::steady_clock::time_point lastTime_ = std::chrono::steady_clock::now();
};

}