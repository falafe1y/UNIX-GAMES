#include "../head/input.h"

#include "../platform/linux/head/input.h"

namespace FGames::core
{

std::vector<Event> Input::poll()
{
    std::vector<Event> events;

    int c = readRawByte();

    if (c == -1)
        return events;

    // Ctrl+C - выходим из игры
    if (c == 0x03)
    {
        events.push_back({EventType::QuitRequested});
        return events;
    }

    // Escape или стрелки (escape-последовательность вида ESC [ A/B/C/D)
    if (c == 0x1b)
    {
        int next = readRawByte();

        // одиночный Esc, без продолжения - выходим из игры
        if (next == -1)
        {
            events.push_back({EventType::QuitRequested, Key::Escape});
            return events;
        }

        if (next == '[')
        {
            int arrow = readRawByte();

            switch (arrow)
            {
            case 'A':
                events.push_back({EventType::KeyPressed, Key::Up});
                break;

            case 'B':
                events.push_back({EventType::KeyPressed, Key::Down});
                break;

            case 'C':
                events.push_back({EventType::KeyPressed, Key::Right});
                break;

            case 'D':
                events.push_back({EventType::KeyPressed, Key::Left});
                break;

            default:
                break;
            }
        }

        return events;
    }

    if (c == '\r' || c == '\n')
    {
        events.push_back({EventType::KeyPressed, Key::Enter});
        return events;
    }

    return events;
}

}