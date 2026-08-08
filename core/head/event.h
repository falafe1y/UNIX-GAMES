#pragma once

namespace fgames::core
{

enum class EventType
{
    KeyPressed,
    QuitRequested
};


enum class Key
{
    Unknown,

    Up,
    Down,
    Left,
    Right,

    Enter,
    Escape
};


struct Event
{
    EventType type;
    Key key = Key::Unknown;
};

}