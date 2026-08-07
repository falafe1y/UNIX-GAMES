#pragma once

#include <termios.h>
#include <string>

class Terminal
{
public:
    Terminal();
    ~Terminal();

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    void clear();
    void moveCursor(int x, int y);

    void hideCursor();
    void showCursor();

    void write(const std::string& text);

private:
    void enableRawMode();
    void disableRawMode();

private:
    termios originalTerminal{};
    bool rawModeEnabled{false};
};