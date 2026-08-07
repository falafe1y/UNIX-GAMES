#include "../head/terminal.h"

#include <unistd.h>
#include <iostream>

Terminal::Terminal()
{
    enableRawMode();
    hideCursor();
    clear();
}

Terminal::~Terminal()
{
    showCursor();
    clear();

    if (rawModeEnabled)
    {
        disableRawMode();
    }
}

void Terminal::enableRawMode()
{
    tcgetattr(STDIN_FILENO, &originalTerminal);

    termios raw = originalTerminal;

    // выключаем:
    // ECHO  - не показывать вводимые символы
    // ICANON - посимвольный ввод вместо строк
    // ISIG - отключаем Ctrl+C/Ctrl+Z сигналы
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);

    // отключаем обработку Ctrl+S/Ctrl+Q
    raw.c_iflag &= ~(IXON);

    // отключаем специальную обработку CR/LF
    raw.c_iflag &= ~(ICRNL);

    // читаем сразу
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    rawModeEnabled = true;
}

void Terminal::disableRawMode()
{
    tcsetattr(
        STDIN_FILENO,
        TCSAFLUSH,
        &originalTerminal
    );

    rawModeEnabled = false;
}

void Terminal::clear()
{
    std::cout << "\033[2J";
    std::cout << "\033[H";
    std::cout.flush();
}

void Terminal::moveCursor(int x, int y)
{
    std::cout
        << "\033["
        << y
        << ";"
        << x
        << "H";

    std::cout.flush();
}

void Terminal::hideCursor()
{
    std::cout << "\033[?25l";
    std::cout.flush();
}

void Terminal::showCursor()
{
    std::cout << "\033[?25h";
    std::cout.flush();
}

void Terminal::write(const std::string& text)
{
    std::cout << text;
    std::cout.flush();
}