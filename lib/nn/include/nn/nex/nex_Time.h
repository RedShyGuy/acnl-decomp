#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// A time in milliseconds (SystemClock: since the first call, starting at 1). Passed by reference
// like ARMCC does for a class with a copy constructor.
class Time
{
public:
    Time() : m_Value(0) {}
    Time(u64 value) : m_Value(value) {}
    Time(const Time& other) : m_Value(other.m_Value) {}
    Time& operator=(const Time& other)
    {
        m_Value = other.m_Value;
        return *this;
    }

    // (inline) the time milliseconds later (earlier if negative)
    Time operator+(int milliseconds) const
    {
        if (milliseconds < 0) {
            return Time(m_Value - static_cast<u64>(-milliseconds));
        }
        return Time(m_Value + milliseconds);
    }
    // (inline; the order of std::multimap<Time, ...>)
    bool operator<(const Time& other) const { return m_Value < other.m_Value; }

    static Time GetTime(); // 0x003CE128 | fefates:callgraph [tier C]
    // the time timeout milliseconds from now (0 for an infinite timeout)
    static Time ConvertTimeoutToDeadline(unsigned int timeout); // 0x003CE0B8 | fefates:bytes [tier B]
    // the milliseconds until the deadline (-1 for none, 0 if it passed)
    static unsigned int ConvertDeadlineToTimeout(Time deadline); // 0x003CE074 | fefates:callgraph [tier C]
    // (where the original inlines it; defined below SystemClock)
    static unsigned int ConvertDeadlineToTimeoutInline(Time deadline);

    u64 m_Value; // 0x0
};
ASSERT_SIZE(Time, 0x8);

// The source of the time of SystemClock if set (name is ours; nothing in the binary sets it).
class TimeSource
{
public:
    virtual void vf_0x00();
    virtual void vf_0x04();
    virtual Time GetTime();
};

// The clock of nex: the milliseconds of the system tick, corrected so that the time starts at 1
// and (with s_IsMonotonic) never goes back. The names of the members and of the functions without
// a reference name are ours.
class SystemClock
{
public:
    // (symbols.json names it nn::nex::Time::GetTime; fefates names it like this)
    static Time GetTime(); // 0x0035BF84 | fefates:bytes-fuzzy [tier B]
    static Time GetUncorrectedTime(); // 0x0035BEE4 | fefates:callseq [tier B]
    // the time under s_Lock
    static Time GetTimeLocked(); // 0x0035BE20
    // the corrected time that never goes back; corrects the offset under the lock (isLocked)
    static Time GetMonotonicTime(bool isLocked); // 0x0035BCA4

    static bool s_IsInitialized;
    static bool s_IsMonotonic;
    static bool s_IsTickClock;
    static TimeSource* s_pTimeSource;
    static Time s_LastTime;
    static s64 s_Offset;
};

inline unsigned int Time::ConvertDeadlineToTimeoutInline(Time deadline)
{
    if (deadline.m_Value == 0) {
        return static_cast<unsigned int>(-1);
    }
    Time now = SystemClock::GetTime();
    if (now.m_Value >= deadline.m_Value) {
        return 0;
    }
    return deadline.m_Value - now.m_Value;
}
} // namespace nex
} // namespace nn
