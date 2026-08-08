#pragma once

#include "../../../head/input.h"

namespace fgames::platform::linux_platform
{

class LinuxInput final : public core::Input
{
public:
    std::vector<core::Event> poll() override;

private:
    int readRawByte();
};

}