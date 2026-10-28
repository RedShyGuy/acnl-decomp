#pragma once

#include "decomp.h"

namespace sead {
// A duration in system ticks (as pead::TickSpan; the names are ours).
class TickSpan
{
public:
    explicit TickSpan(s64 tick) : mSpan(tick) {}
    static TickSpan fromMilliSeconds(s32 msec) { return TickSpan(static_cast<s64>(msec) * sFrequency / 1000); }
    // (divided by 1000 twice)
    static TickSpan fromMicroSeconds(s32 usec) { return TickSpan(static_cast<s64>(usec) * sFrequency / 1000 / 1000); }

    // system ticks per second (268111856; the static initializer 0x007967D0 sets it)
    static const s64 sFrequency;

    s64 mSpan; // 0x0
};
} // namespace sead
