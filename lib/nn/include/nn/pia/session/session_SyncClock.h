#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
// The clock shared by the stations (SyncClockProtocol): the time since the base time in ms. The
// layout is from the constructor; the member names are ours.
class SyncClock
{
public:
    SyncClock(); // 0x0044D304 | fefates:bytes [tier B]

    void SetBaseTime(const nn::pia::common::Time& time); // 0x0044D2D0 | fefates:bytes [tier B]
    void ClearBaseTime(); // 0x0044D2EC | fefates:bytes [tier B]
    // ms since the base time, -1 without one
    s64 GetTime() const; // 0x00734B00 | fefates:bytes

    u32 m_Unknown0x0;          // 0x00 (not set here)
    u32 m_Unknown0x4;          // 0x04
    common::Time m_BaseTime;   // 0x08
    bool m_IsBaseTimeSet;      // 0x10
};
ASSERT_SIZE(SyncClock, 0x18);
} // namespace session
} // namespace pia
} // namespace nn
