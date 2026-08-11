#pragma once

#include <chrono>

namespace fgames::core
{

class Timer
{
public:
    float tick();
    void reset();

private:
    std::chrono::steady_clock::time_point last_time_ = std::chrono::steady_clock::now();
};

}