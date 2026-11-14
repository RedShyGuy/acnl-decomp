#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"

// Functions of the game that the clock calls (not decompiled yet; names are ours, FuncXXXXXX while the purpose is
// unknown).
namespace time {
// SvTime: nanoseconds since 2000-01-01 (0x7FFFFFFFFFFFFFFF: none); the game copies it with memcpy (packed)
void SvTimeClear(SvTime* time); // 0x00303EEC
bool SvTimeIsValid(const SvTime* time); // 0x002FB354
nn::fnd::DateTimeParameters SvTimeToParameters(const SvTime* time); // 0x002FB3C8
void SvTimeSet(SvTime* time, const nn::fnd::DateTimeParameters& date); // 0x005CE884
// (names are ours) the time goes on by days / by days, hours, minutes and seconds / becomes the clock of the game (GetClock) /
// gets an hour, a minute and a second of its day (the milliseconds become 0)
void SvTimeAddDays(SvTime* time, s32 days); // 0x005CE8B8
void SvTimeAdd(SvTime* time, s32 days, s32 hours, s32 minutes, s32 seconds); // 0x005CE934
void SvTimeSetNow(SvTime* time); // 0x005CE904
void SvTimeSetClock(SvTime* time, s8 hour, s8 minute, s8 second); // 0x005CE990
// (copy in the shared area) the value of the time; none or 0 give 0x0056C808
s64 SvTimeGetValue(const SvTime* time); // 0x0075A680
void SvTimeNormalize(SvTime* time); // 0x00314B78
// the flag next to SvTownData::currentTime (bit 0 of unk_0xED84)
bool IsTimeFlagSet(const s64* time); // 0x00713EFC
void ClearTimeFlag(s64* time); // 0x0020DEC0
void SetTimeFlag(s64* time); // 0x0020DED0
void ClearTimeFlag2(s64* time); // 0x0020DEE0
bool Func630200(u8 part); // 0x00630200
void Func308EF0(nn::fnd::DateTimeParameters* date); // 0x00308EF0
// (the date goes on by days, hours, minutes, seconds; the days are counted in nanoseconds of a day)
void Func30A5AC(nn::fnd::DateTimeParameters* date, s32 days, s32 hours, s32 minutes, s32 seconds); // 0x0030A5AC
void Func3004A8(nn::fnd::DateTimeParameters* date); // 0x003004A8
} // namespace time
