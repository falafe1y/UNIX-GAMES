#include "../head/input.h"
#include "../platform/linux/head/input.h"

namespace fgames::core
{

std::vector<Event> Input::poll()
{
    std::vector<Event> events;

    int c = readRawByte();

    if (c == -1) return events;

    // Ctrl+C - exit
    if (c == 0x03)
    {
        events.push_back({EventType::QuitRequested});
        return events;
    }

    // Escape or arrows
    if (c == 0x1b)
    {
        int next = readRawByte();

        // single escape - exit
        if (next == -1)
        {
            events.push_back({EventType::QuitRequested, EventKey::Escape});
            return events;
        }

        if (next == '[')
        {
            int arrow = readRawByte();

            switch (arrow)
            {
            case 'A':
                events.push_back({EventType::KeyPressed, EventKey::Up});
                break;

            case 'B':
                events.push_back({EventType::KeyPressed, EventKey::Down});
                break;

            case 'C':
                events.push_back({EventType::KeyPressed, EventKey::Right});
                break;

            case 'D':
                events.push_back({EventType::KeyPressed, EventKey::Left});
                break;

            default:
                break;
            }
        }

        return events;
    }

    if (c == '\r' || c == '\n')
    {
        events.push_back({EventType::KeyPressed, EventKey::Enter});
        return events;
    }

    return events;
}

}