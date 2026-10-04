#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
// A duration in system ticks (268111856 per second). The member name and the inline functions
// are ours.
class TimeSpan
{
public:
    TimeSpan() : m_Tick(0) {}
    explicit TimeSpan(s64 tick) : m_Tick(tick) {}

    s64 GetTick() const { return m_Tick; }

    static const TimeSpan& GetTicksPerSec(); // 0x00429784 | fefates:bytes [tier B]
    static const TimeSpan& GetTicksPerMSec(); // 0x00429824 | fefates:bytes [tier B]

    s64 m_Tick; // 0x0
};
ASSERT_SIZE(TimeSpan, 0x8);
} // namespace common
} // namespace pia
} // namespace nn
