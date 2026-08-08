#pragma once

#include "event.h"

#include <vector>

namespace fgames::core
{

class Input
{
public:
    virtual ~Input() = default;
    virtual std::vector<Event> poll() = 0;
};

}