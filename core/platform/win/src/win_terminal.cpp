#include "../head/win_terminal.h"

#include <iostream>

namespace fgames::platform::win_platform
{

void WinTerminal::clear()
{
    std::cout << "\033[2J";
    std::cout << "\033[H";
}

void WinTerminal::move_cursor(
    int x,
    int y
)
{
    std::cout
        << "\033["
        << y + 1
        << ";"
        << x + 1
        << "H";
}

void WinTerminal::hide_cursor()
{
}

void WinTerminal::show_cursor()
{
}

void WinTerminal::write(
    const std::string& text
)
{
    std::cout << text;
    std::cout.flush();
}

}