#pragma once

#include "../../../head/terminal.h"

namespace fgames::platform::win_platform
{

class WinTerminal final : public core::Terminal
{
public:
    WinTerminal() = default;
    ~WinTerminal() override = default;

    void clear() override;
    void move_cursor(int x, int y) override;
    void hide_cursor() override;
    void show_cursor() override;
    void write(const std::string& text) override;
};

}