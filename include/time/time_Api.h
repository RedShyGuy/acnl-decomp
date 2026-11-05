#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "sead/seadTickTime.h"

// The clock of the game (namespace after the RTTI of time::TimeHioNode; all other names are ours).
//
// Times are SvTimes (nanoseconds) since 2000-01-01 00:00 (State::baseDateTime). The real time of the system plus an offset
// (saved in the town, SvTownData::currentTime) gives the time of the game; on the islands (stage::KIND_TOUR,
// stage::KIND_ISLAND_HUT) the offset is another one (the date of State::islandDate in the current year). The game keeps
// its years in [2012, 2051): later years are moved back by 39 years.
namespace time {
// the first and the end of the years of the game
const s32 FIRST_YEAR = 2012;
const s32 END_YEAR = 2051;
const s32 YEAR_COUNT = END_YEAR - FIRST_YEAR;

// the bits of State::flags
enum Flag : u8
{
    FLAG_BEFORE_FIRST_DAY = 1 << 0, // the time is before 2012-01-01 06:00 or was moved back (also in the save)
    FLAG_FROZEN = 1 << 1,           // Update does nothing
    FLAG_BIT2 = 1 << 2,
    FLAG_BIT3 = 1 << 3,
    FLAG_YEAR_CLAMPED = 1 << 4,     // Start moved the year into [2012, 2051)
};

struct State
{
    // (frozen until Initialize; the saved time is none, the play timer starts with the tick now)
    State() : flags(FLAG_FROZEN), savedTime(0x7FFFFFFFFFFFFFFFLL) {}

    u8 isInitialized;                 // 0x00
    u8 isPlayTimerStarted;            // 0x01
    u8 flags;                         // 0x02 Flag
    SvTime now;                       // 0x08 the time of the game
    SvTime realNow;                   // 0x10 the time of the system
    SvTime offset;                    // 0x18 now - realNow (in the town)
    SvTime pendingOffset;             // 0x20 ApplyPendingOffset makes it the offset
    SvTime islandOffset;              // 0x28 now - realNow on the islands
    s64 pausedSeconds;                // 0x30
    SvTime savedTime;                 // 0x38
    sead::TickTime playTimerStart;    // 0x40
    nn::fnd::DateTime pauseDateTime;  // 0x48
    SvTime yearsSpan;                 // 0x50 from 2012-01-01 to 2051-01-01
    nn::fnd::DateTime baseDateTime;   // 0x58 2000-01-01
    nn::fnd::DateTime firstDateTime;  // 0x60 2012-01-01
    nn::fnd::DateTime endDateTime;    // 0x68 2051-01-01
    nn::fnd::DateTime islandDateTime; // 0x70 2012-08-01
};
ASSERT_SIZE(State, 0x78);

extern State s_State;
extern nn::fnd::DateTimeParameters s_CurrentDate;
extern nn::fnd::DateTimeParameters s_RealDate;
extern nn::fnd::DateTimeParameters s_FirstDate;
extern nn::fnd::DateTimeParameters s_IslandDate;

void Initialize(); // 0x0056D064
// takes the time of the system; Update with the offset of the save, the year clamped
DECOMP_NOINLINE void Start(); // 0x0056C1DC
DECOMP_NOINLINE void Update(); // 0x0056D2B0
void Freeze(); // 0x0056D1C0

// the date of the game
DECOMP_EMIT_COPY inline const nn::fnd::DateTimeParameters* GetCurrentDate() // 0x002FB394
{
    return &s_CurrentDate;
}
const nn::fnd::DateTimeParameters* GetRealDate(); // 0x0056C8FC
SvTime GetNow(); // 0x0056C010
SvTime GetRealNow(); // 0x0056C448
// the time of the date since 2000-01-01
DECOMP_NOINLINE SvTime ToTime(const nn::fnd::DateTimeParameters& date); // 0x0056C908
DECOMP_NOINLINE nn::fnd::DateTimeParameters ToParameters(SvTime time); // 0x0056CD40
SvTime GetSpanFromRealNow(const nn::fnd::DateTimeParameters& date); // 0x0056BF9C
// a - b; wrap: if b is later, a counts in the next round of years (only with FLAG_BEFORE_FIRST_DAY)
SvTime GetDifference(const nn::fnd::DateTimeParameters& a, const nn::fnd::DateTimeParameters& b,
                                bool wrap); // 0x0056C9D4
s64 GetDifferenceInSeconds(const nn::fnd::DateTimeParameters& a, const nn::fnd::DateTimeParameters& b,
                           bool wrap); // 0x0056CDA8

// sets the date of the game: the offset (or the island offset) follows
DECOMP_NOINLINE void SetCurrentDate(const nn::fnd::DateTimeParameters& date); // 0x0056DC84
void SetOffset(const SvTime* offset); // 0x0056DD18
void ResetOffset(); // 0x0056C020
void LoadOffset(); // 0x0056C038
void SaveOffset(); // 0x0056C08C
// the offset that makes the time "time + milliSeconds" now
void SetPendingOffset(SvTime time, s64 milliSeconds); // 0x0056C0E4
void ApplyPendingOffset(); // 0x0056C7EC
// the island date in the year
void SetIslandOffset(s32 year); // 0x0056C520
void ClearBeforeFirstDayFlag(); // 0x0056C4F4
void UpdateBeforeFirstDayFlag(); // 0x0056DA64
bool IsYearClamped(); // 0x0056CEBC
void SetFlagBit2(); // 0x0056D298
bool IsFlagBit2(); // 0x0056D1D8
void ClearFlagBit2(); // 0x0056DA34
void SetFlagBit3(); // 0x0056D280
bool IsFlagBit3(); // 0x0056DA1C
void ClearFlagBit3(); // 0x0056DA4C

// the seconds since StartPlayTimer without the paused ones (0 before)
s64 GetPlayTimeSeconds(); // 0x0056C3D4
void StartPlayTimer(); // 0x0056CCEC
void PausePlayTimer(); // 0x0056C8D4
void ResumePlayTimer(); // 0x0056CC50

// the saved time of the town, else the own one (set from the current date first)
nn::fnd::DateTimeParameters GetSavedTime(); // 0x0056CF44
void ResetSavedTime(); // 0x0056D040

// 2012-01-01 00:00
nn::fnd::DateTimeParameters GetFirstDate(); // 0x0056C670
nn::fnd::DateTimeParameters GetFirstDate2(); // 0x0056DAF4
SvTime GetFirstTime(); // 0x0056C808
// the date of the game (Func308EF0 applied)
nn::fnd::DateTimeParameters GetGameDate(); // 0x0056CED4
// the date (of Func3004A8) is another than the date of the game
bool IsDateChanged(); // 0x0056CBCC

s32 GetDaysInMonth(s32 year, s32 month); // 0x0056BFBC
s32 GetDaysInYear(s32 year); // 0x0056C6C4
nn::fnd::DayOfWeek GetDayOfWeek(s32 year, s32 month, s32 day); // 0x0056D610
nn::fnd::DateTimeParameters MakeDate(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second); // 0x0056D550
SvTime MakeTime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second); // 0x0056D594
// moves the year into [2012, 2051) by 39 years (at most twice, else 2012)
DECOMP_NOINLINE void WrapYear(nn::fnd::DateTimeParameters* date); // 0x0056D664
void SetHour(nn::fnd::DateTimeParameters* date, s32 hour); // 0x0056D7CC
void SetMinute(nn::fnd::DateTimeParameters* date, s32 minute); // 0x0056D96C
void SetSecond(nn::fnd::DateTimeParameters* date, s32 second); // 0x0056D9C4
void SetMilliSecond(nn::fnd::DateTimeParameters* date, s32 milliSecond); // 0x0056D6D0
// (with WrapYear)
void AddHours(nn::fnd::DateTimeParameters* date, s32 hours); // 0x0056D720
void AddMinutes(nn::fnd::DateTimeParameters* date, s32 minutes); // 0x0056D81C
void AddSeconds(nn::fnd::DateTimeParameters* date, s32 seconds); // 0x0056D8C4
SvTime Func56D4F8(SvTime time, s32 a, s32 b, s32 c, f32 d); // 0x0056D4F8

// the part of the day by the hour: 5 (before 5:00), 0 (to 8:00), 1 (to 12:00), 2 (to 16:00, in Spain (event::REGION_EUROPE_SPANISH)
// to 17:00), 3 (to 19:00 or 21:00), 4
u8 GetPartOfDay(const nn::fnd::DateTimeParameters& date); // 0x0056C458
// the age of the moon in days (0..29): a 19-year cycle with a correction per month
s32 GetMoonAge(const nn::fnd::DateTimeParameters& date); // 0x0056C5D4
// the zodiac sign of the date: the index of the first sign whose last day is not before it
u8 GetZodiacSign(s32 month, s32 day); // 0x0056C978
// one of the 23 parts of the year (12 on the islands): the first whose start (s_SeasonStarts) is not before the date
u8 GetSeasonPart(const nn::fnd::DateTimeParameters& date); // 0x0056C710
u8 GetCurrentSeasonPart(); // 0x0056C77C
// the date of the start of the part in the year of the date
void ToSeasonPartStart(const u8* part, nn::fnd::DateTimeParameters* date); // 0x0056C864
// the day after the start of the part before
void ToPreviousSeasonPartEnd(const u8* part, nn::fnd::DateTimeParameters* date); // 0x0056CAC4
u8 Func56CF08(); // 0x0056CF08
u8 Func56D008(); // 0x0056D008
// one of four parts of the year: 0 from 02-25, 1 from 06-01 (and on the islands), 2 from 09-01, 3 from 11-26
u8 GetSeason(const nn::fnd::DateTimeParameters& date); // 0x0056DB48
u8 GetCurrentSeason(); // 0x0056DBE4
// tables by the year
u8 Func56D1F0(const s32* year); // 0x0056D1F0
u8 Func56D224(s32 year); // 0x0056D224
u8 Func56D254(s32 year); // 0x0056D254
} // namespace time
