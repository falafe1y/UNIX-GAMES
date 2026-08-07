#include "../head/input.h"

namespace FGames::core
{

std::vector<Event> Input::poll()
{
    return
    {
        {
            EventType::KeyPressed,
            Key::Right
        }
    };
}

}