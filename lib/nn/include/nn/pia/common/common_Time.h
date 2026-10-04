#pragma once

// nn::pia::common::Time - a point in time in system ticks. The class and SetNow are from the
// fefates symbols; the member, the operators and the constants are ours.

#include "decomp.h"
#include "nn/pia/common/common_TimeSpan.h"

namespace nn {
namespace pia {
namespace common {
class Time
{
public:
    Time() : m_Tick(0) {}
    explicit Time(s64 tick) : m_Tick(tick) {}

    // the system tick count now (svc GetSystemTick)
    void SetNow(); // 0x00428DC4 | fefates:bytes [tier B]

    TimeSpan operator-(const Time& rhs) const { return TimeSpan(m_Tick - rhs.m_Tick); }
    Time operator+(const TimeSpan& rhs) const { return Time(m_Tick + rhs.m_Tick); }
    Time& operator+=(const TimeSpan& rhs)
    {
        m_Tick += rhs.m_Tick;
        return *this;
    }
    // the ticks compare unsigned (ARMCC: subs / sbcs / bcs)
    bool operator<(const Time& rhs) const { return static_cast<u64>(m_Tick) < static_cast<u64>(rhs.m_Tick); }
    bool operator>=(const Time& rhs) const { return !(*this < rhs); }

    // the latest and the earliest time (all bits set / 0); the static initializer 0x00791A38
    // sets them
    static const Time INFINITE_TIME;
    static const Time ZERO_TIME;

    s64 m_Tick; // 0x0
};
ASSERT_SIZE(Time, 0x8);
} // namespace common
} // namespace pia
} // namespace nn
