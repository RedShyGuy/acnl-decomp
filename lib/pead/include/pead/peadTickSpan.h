#pragma once

// pead::TickSpan - a duration in system ticks, and the thread sleep that takes one. The names are
// ours.

#include "decomp.h"

namespace pead {
class TickSpan
{
public:
    explicit TickSpan(s64 tick) : mSpan(tick) {}

    static TickSpan fromMilliSeconds(u32 msec) { return TickSpan(static_cast<s64>(msec) * sFrequency / 1000); }

    // system ticks per second (268111856; a static initializer at 0x007967EC sets it)
    static const s64 sFrequency;

    s64 mSpan; // 0x0
};

// sleeps the calling thread (name is ours)
void SleepThread(TickSpan span); // 0x0053C174
} // namespace pead
