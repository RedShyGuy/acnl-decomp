#include "nn/pia/common/common_TimeSpan.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// system ticks per millisecond (268.111856 MHz)
const s64 TICKS_PER_MSEC = 268111;
} // namespace

// 0x00429784 | fefates:bytes [tier B]
const TimeSpan& nn::pia::common::TimeSpan::GetTicksPerSec()
{
    // 0x0097E3F8 (guard 0x0097E3EC)
    static const TimeSpan s_TicksPerSec(GetTicksPerMSec().m_Tick * 1000);
    return s_TicksPerSec;
}

// 0x00429824 | fefates:bytes [tier B]
const TimeSpan& nn::pia::common::TimeSpan::GetTicksPerMSec()
{
    // 0x0097E3F0 (guard 0x0097E3E8)
    static const TimeSpan s_TicksPerMSec(TICKS_PER_MSEC);
    return s_TicksPerMSec;
}

} // namespace common
} // namespace pia
} // namespace nn
