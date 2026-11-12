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
void SvTimeNormalize(SvTime* time); // 0x00314B78
// the flag next to SvTownData::currentTime (bit 0 of unk_0xED84)
bool IsTimeFlagSet(const s64* time); // 0x00713EFC
void ClearTimeFlag(s64* time); // 0x0020DEC0
void SetTimeFlag(s64* time); // 0x0020DED0
void ClearTimeFlag2(s64* time); // 0x0020DEE0
bool Func630200(u8 part); // 0x00630200
void Func308EF0(nn::fnd::DateTimeParameters* date); // 0x00308EF0
void Func30A5AC(nn::fnd::DateTimeParameters* date, s32 a, s32 b, s32 c, f32 d); // 0x0030A5AC
void Func3004A8(nn::fnd::DateTimeParameters* date); // 0x003004A8
} // namespace time
