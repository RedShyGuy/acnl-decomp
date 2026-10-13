#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/os/os_Tick.h"

namespace nn {
namespace nex {
// 0x0096C919 (name is ours)
bool SystemClock::s_IsInitialized;
// 0x0096C91A (name is ours)
bool SystemClock::s_IsMonotonic;
// 0x0096C939 (name is ours)
bool SystemClock::s_IsTickClock = true;
// 0x0096C978 (name is ours)
TimeSource* SystemClock::s_pTimeSource;
// 0x0096CAA8 (name is ours)
Time SystemClock::s_LastTime;
// 0x0096CAB0 (name is ours)
s64 SystemClock::s_Offset;

namespace {
// locks the correction of the time (name is ours)
// 0x00ADE8D0 (name is ours)
CriticalSection s_TimeLock(1);
} // namespace

// 0x0035BCA4 (name is ours)
Time nn::nex::SystemClock::GetMonotonicTime(bool isLocked)
{
    // 0x0096CAD8 (name is ours)
    static Time s_MonotonicTime;
    Time last = s_MonotonicTime;
    s64 offset = s_Offset;
    Time now = GetUncorrectedTime();
    if (!s_IsInitialized) {
        s_MonotonicTime = 1;
        s_IsInitialized = true;
        s_Offset = 1 - now.m_Value;
        return Time(1);
    }
    if (offset >= 0 || now.m_Value >= static_cast<u64>(-offset)) {
        u64 time = now.m_Value + offset;
        if (time >= last.m_Value) {
            s_MonotonicTime = time;
            return Time(time);
        }
    }
    if (!isLocked) {
        return GetTimeLocked();
    }
    u64 time = last.m_Value + 1;
    s_Offset = time - now.m_Value;
    s_MonotonicTime = time;
    return Time(time);
}

// 0x0035BE20 (name is ours)
Time nn::nex::SystemClock::GetTimeLocked()
{
    ScopedCS lock(s_TimeLock);
    if (s_IsMonotonic) {
        return GetMonotonicTime(true);
    }
    if (s_IsInitialized) {
        return Time(GetUncorrectedTime().m_Value + s_Offset);
    }
    s_IsInitialized = true;
    s_Offset = 1 - GetUncorrectedTime().m_Value;
    return Time(1);
}

// 0x0035BEE4 | fefates:callseq [tier B]
Time nn::nex::SystemClock::GetUncorrectedTime()
{
    if (s_pTimeSource != nullptr) {
        return s_pTimeSource->GetTime();
    }
    // (both settings read the tick; the other one calls a function first that does nothing in
    // this build)
    return Time(static_cast<u64>(nnosTickGetSystemCurrent()) * 1000 / nn::os::Tick::TICKS_PER_SECOND);
}

// 0x0035BF84 | fefates:bytes-fuzzy [tier B]
Time nn::nex::SystemClock::GetTime()
{
    Time time;
    if (s_IsMonotonic) {
        time = GetMonotonicTime(false);
    } else if (s_IsInitialized) {
        time = GetUncorrectedTime().m_Value + s_Offset;
    } else {
        s_IsInitialized = true;
        s_Offset = 1 - GetUncorrectedTime().m_Value;
        time = 1;
    }
    s_LastTime = time;
    return time;
}

// 0x003CE074 | fefates:callgraph [tier C]
unsigned int nn::nex::Time::ConvertDeadlineToTimeout(Time deadline)
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

// 0x003CE0B8 | fefates:bytes [tier B]
nn::nex::Time nn::nex::Time::ConvertTimeoutToDeadline(unsigned int timeout)
{
    if (timeout == static_cast<unsigned int>(-1)) {
        return Time(0);
    }
    return SystemClock::GetTime() + static_cast<int>(timeout);
}

// 0x003CE128 | fefates:callgraph [tier C]
nn::nex::Time nn::nex::Time::GetTime()
{
    return SystemClock::GetTime();
}

} // namespace nex
} // namespace nn
