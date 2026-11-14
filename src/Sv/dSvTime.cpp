#include "time/time_Extern.h"

#include <cstring>

#include "netgame/dGameFunctions.h"
#include "time/time_Api.h"

namespace {
// (inline) the time is cleared and gets the value (the game copies it with memcpy: the 64-bit value has an alignment of
// 4, see SvS64)
DECOMP_ALWAYS_INLINE void StoreTime(SvTime* time, s64 value)
{
    std::memset(static_cast<void*>(time), 0, sizeof(*time));
    std::memcpy(static_cast<void*>(time), &value, sizeof(*time));
}
} // namespace

// 0x005CE884 (name is ours)
void time::SvTimeSet(SvTime* time, const nn::fnd::DateTimeParameters& date)
{
    StoreTime(time, ToTime(date));
}

// 0x005CE8B8 (name is ours)
void time::SvTimeAddDays(SvTime* time, s32 days)
{
    StoreTime(time, Func56D4F8(SvTimeGetValue(time), days, 0, 0, 0));
}

// 0x005CE904 (name is ours)
void time::SvTimeSetNow(SvTime* time)
{
    StoreTime(time, netgame::GetClock());
}

// 0x005CE934 (name is ours)
void time::SvTimeAdd(SvTime* time, s32 days, s32 hours, s32 minutes, s32 seconds)
{
    StoreTime(time, Func56D4F8(SvTimeGetValue(time), days, hours, minutes, seconds));
}

// 0x005CE990 (name is ours)
void time::SvTimeSetClock(SvTime* time, s8 hour, s8 minute, s8 second)
{
    nn::fnd::DateTimeParameters date = SvTimeToParameters(time);
    SetHour(&date, hour);
    SetMinute(&date, minute);
    SetSecond(&date, second);
    SetMilliSecond(&date, 0);
    StoreTime(time, ToTime(date));
}

// 0x005CEA18 (name is ours)
SvTime::SvTime(s64 time)
{
    StoreTime(this, time);
}
