#include "../head/linux_input.h"

#include <unistd.h>

namespace fgames::platform::linux_platform
{

int LinuxInput::readRawByte()
{
    unsigned char c;

    const ssize_t n = read(
        STDIN_FILENO,
        &c,
        1
    );

    if (n <= 0)
        return -1;

    return c;
}


std::vector<core::Event> LinuxInput::poll()
{
    std::vector<core::Event> events;

    const int c = readRawByte();

    if (c == -1)
        return events;


    // Ctrl+C
    if (c == 0x03)
    {
        events.push_back({
            core::EventType::QuitRequested,
        });

        return events;
    }


    // Escape / arrows
    if (c == 0x1b)
    {
        const int next = readRawByte();

        // Escape
        if (next == -1)
        {
            events.push_back({core::EventType::KeyPressed, core::Key::Escape});
            return events;
        }

        // Escape sequence
        if (next == '[')
        {
            const int arrow = readRawByte();

            switch (arrow)
            {
                case 'A':
                    events.push_back({
                        core::EventType::KeyPressed,
                        core::Key::Up
                    });
                    break;

                case 'B':
                    events.push_back({
                        core::EventType::KeyPressed,
                        core::Key::Down
                    });
                    break;

                case 'C':
                    events.push_back({
                        core::EventType::KeyPressed,
                        core::Key::Right
                    });
                    break;

                case 'D':
                    events.push_back({
                        core::EventType::KeyPressed,
                        core::Key::Left
                    });
                    break;
            }
        }

        return events;
    }


    // Enter
    if (c == '\r' || c == '\n')
    {
        events.push_back({
            core::EventType::KeyPressed,
            core::Key::Enter
        });

        return events;
    }


    return events;
}

}