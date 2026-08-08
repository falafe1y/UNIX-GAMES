#pragma once

#include <string>

namespace fgames::core
{

class Terminal
{
public:
    virtual ~Terminal() = default;

    virtual void clear() = 0;
    virtual void move_cursor(int x, int y) = 0;
    virtual void hide_cursor() = 0;
    virtual void show_cursor() = 0;
    virtual void write(const std::string& text) = 0;
};

}
