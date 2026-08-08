#pragma once

#include "../../../head/terminal.h"

#include <termios.h>

namespace fgames::platform::linux_platform
{

class LinuxTerminal final : public core::Terminal
{
public:
    LinuxTerminal();
    ~LinuxTerminal() override;

    void clear() override;
    void move_cursor(int x, int y) override;
    void hide_cursor() override;
    void show_cursor() override;
    void write(const std::string& text) override;

private:
    void enable_raw_mode();
    void disable_raw_mode();

private:
    termios original_terminal_{};
    bool raw_mode_enabled_{false};
};

}