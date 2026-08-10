#pragma once

namespace fgames::core
{

enum class EventType
{
    KeyPressed,
    QuitRequested
};

enum class EventKey
{
    Unknown,

    Up,
    Down,
    Left,
    Right,

    Space,

    Enter,
    Escape
};

struct Event
{
    EventType type;
    EventKey key = EventKey::Unknown;
};

}