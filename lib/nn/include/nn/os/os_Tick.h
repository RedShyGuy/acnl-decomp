#pragma once

// nn::os::Tick - a count of system ticks (268 MHz). The class name is from the binary (e.g.
// nw::snd::internal::driver::SoundThread::CalcProcessCost(const nn::os::Tick&)); it is 8 bytes,
// so functions return it through memory.

#include "types.h"

namespace nn {
namespace os {

class Tick
{
public:
    Tick() : mTick(0) {}
    explicit Tick(s64 tick) : mTick(tick) {}

    operator s64() const { return mTick; }

private:
    s64 mTick;
};

} // namespace os
} // namespace nn
