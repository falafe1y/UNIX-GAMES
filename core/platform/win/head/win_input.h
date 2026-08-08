#pragma once

#include "../../../head/input.h"

namespace fgames::platform::win_platform
{

class WinInput final : public core::Input
{
public:
    std::vector<core::Event> poll() override;
};

}