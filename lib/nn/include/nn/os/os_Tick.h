#pragma once

// nn::os::Tick - a count of system ticks (268 MHz). The class name is from the binary (e.g.
// nw::snd::internal::driver::SoundThread::CalcProcessCost(const nn::os::Tick&)); it is 8 bytes,
// so functions return it through memory.

#include "types.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace os {

class Tick
{
public:
    Tick() : mTick(0) {}
    explicit Tick(s64 tick) : mTick(tick) {}

    operator s64() const { return mTick; }

    // the time of the ticks (268111856 Hz): the product with the 32.32 fixed point factor
    // 0x3.BAD34AEE (10^9 / 268111856); inline in the callers (applet, boss; the name is ours)
    nn::fnd::TimeSpan ToTimeSpan() const
    {
        u32 low = static_cast<u32>(mTick);
        s32 high = static_cast<s32>(mTick >> 32);
        s64 nanoSeconds = static_cast<s64>((static_cast<u64>(low) * 0xBAD34AEEu) >> 32);
        nanoSeconds += static_cast<s64>(low) * 3;
        nanoSeconds += static_cast<s64>(high) * 0xBAD34AEELL;
        nanoSeconds += static_cast<s64>(static_cast<u64>(static_cast<s64>(high) * 3) << 32);
        return nn::fnd::TimeSpan::FromNanoSeconds(nanoSeconds);
    }

private:
    s64 mTick;
};

} // namespace os
} // namespace nn
