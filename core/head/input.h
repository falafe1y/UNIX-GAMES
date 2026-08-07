#pragma once

#include <vector>
#include "event.h"

namespace FGames::core
{

class Input
{
public:
    std::vector<Event> poll();

};

}