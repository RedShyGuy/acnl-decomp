#pragma once

#include "decomp.h"

namespace sead {
class TickTime
{
public:
    void setNow(); // the function of the binary is nn::pia::common::Time::SetNow (the same code)
    TickTime(); // the function of the binary is pead::TickTime::TickTime
};
} // namespace sead
