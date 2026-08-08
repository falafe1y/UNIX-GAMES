#pragma once

#include <vector>
#include "event.h"

namespace fgames::core
{

class Input
{
public:
    virtual ~Input() = default;
    virtual std::vector<Event> poll() = 0;

};

}