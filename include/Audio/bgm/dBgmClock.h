#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

namespace audio {
namespace bgm {
// The clock of the music: once per frame UpdateClock compares the date of the game with the date of the frame before (in the
// file of the BGM manager; all names are ours).

// the last value of the hour (the second) in *last; true if it changed
bool UpdateHour(s8* last); // 0x00589F40
bool UpdateSecond(s8* last); // 0x00589FE4
// the minute and second of now in [start, end) (over the full hour if end < start)
bool IsInTime(u8 startMinute, u8 startSecond, u8 endMinute, u8 endSecond); // 0x00589F68
// the date moved by more than one second (the clock was set)
bool IsClockJumped(); // 0x00589FD4
bool IsTimeChanged(); // 0x0058A00C
void UpdateClock(); // 0x0058A01C
s8 GetHour(); // 0x0058A160
s8 GetMinute(); // 0x0058A170
s8 GetSecond(); // 0x0058A180

extern bool s_IsHourChanged;
extern bool s_IsTimeChanged;
extern bool s_IsClockJumped;
extern nn::fnd::DateTimeParameters s_LastDate;
} // namespace bgm
} // namespace audio
