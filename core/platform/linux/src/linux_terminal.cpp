#include "../head/linux_terminal.h"

#include <unistd.h>
#include <iostream>

namespace fgames::platform::linux_platform
{

LinuxTerminal::LinuxTerminal()
{
    enable_raw_mode();
    clear();
    hide_cursor();
}

LinuxTerminal::~LinuxTerminal()
{
    show_cursor();
    clear();

    if (raw_mode_enabled_)
    {
        disable_raw_mode();
    }
}

void LinuxTerminal::enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &original_terminal_) == -1)
    {
        return;
    }

    termios raw = original_terminal_;

    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0)
    {
        raw_mode_enabled_ = true;
    }
}

void LinuxTerminal::disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_);
    raw_mode_enabled_ = false;
}

void LinuxTerminal::clear()
{
    std::cout << "\033[2J";
    std::cout << "\033[H";
    std::cout.flush();
}

void LinuxTerminal::move_cursor(int x, int y)
{
    // ANSI coordinates начинаются с 1

    std::cout
        << "\033["
        << y + 1
        << ";"
        << x + 1
        << "H";

    std::cout.flush();
}

void LinuxTerminal::hide_cursor()
{
    std::cout << "\033[?25l";
    std::cout.flush();
}

void LinuxTerminal::show_cursor()
{
    std::cout << "\033[?25h";
    std::cout.flush();
}

void LinuxTerminal::write(const std::string& text)
{
    std::cout << text;
    std::cout.flush();
}

}