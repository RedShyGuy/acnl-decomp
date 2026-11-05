#include "time/time_Api.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nn/svc/svc_Api.h"
#include "sead/seadTickSpan.h"
#include "stage/stage_Api.h"
#include "time/dTimeHioNode.h"
#include "time/time_Extern.h"

namespace time {
// 0x0095D4F0
State s_State;
// 0x00AD66B0
nn::fnd::DateTimeParameters s_CurrentDate;
// 0x00AD66BC
nn::fnd::DateTimeParameters s_RealDate;
// 0x00AD66C8
nn::fnd::DateTimeParameters s_FirstDate;
// 0x00AD66D4
nn::fnd::DateTimeParameters s_IslandDate;
// 0x0095D568
TimeHioNode s_HioNode;

// the state (the saved time is none, the play timer starts with the tick now) and the node
// 0x007A0B00 (static initializer)

namespace {
// the days of the months
// 0x00891278
extern const u8 s_DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
} // namespace

// the starts of the 23 parts of the year (month << 8 | day)
// 0x00891286
extern const u16 s_SeasonPartStarts[23] = {
    0x010F, 0x0212, 0x0218, 0x031F, 0x0405, 0x040A, 0x040E, 0x0607, 0x060F, 0x061B, 0x0705, 0x0717,
    0x0907, 0x090F, 0x0A10, 0x0A18, 0x0B01, 0x0B09, 0x0B11, 0x0B19, 0x0B1E, 0x0C0A, 0x0C1F,
};
// a value for each last digit of the year
// 0x008912B4
extern const u8 s_YearDigitTable[10] = {1, 2, 3, 2, 0, 1, 2, 3, 2, 0};
// the last day (month, day) of each zodiac sign
// 0x008912BE
extern const u8 s_ZodiacSignEnds[12][2] = {
    {1, 19}, {2, 18}, {3, 20}, {4, 19}, {5, 20}, {6, 21}, {7, 22}, {8, 22}, {9, 22}, {10, 23}, {11, 22}, {12, 21},
};
// the correction of the moon age for each month
// 0x008912D8
extern const s32 s_MoonAgeMonthCorrections[12] = {0, 2, 0, 2, 2, 4, 5, 6, 7, 8, 9, 10};

namespace {
// the stages on the islands use the island offset
inline bool IsIslandStage()
{
    return stage::IsKind(stage::KIND_TOUR, stage::GetCurrent()) || stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent());
}

inline SvTownData& GetTownData()
{
    return GetGardenData()->townData;
}
} // namespace

// 0x0056BF9C (name is ours)
SvTime GetSpanFromRealNow(const nn::fnd::DateTimeParameters& date)
{
    return ToTime(date) - s_State.realNow;
}

// 0x0056BFBC (name is ours)
s32 GetDaysInMonth(s32 year, s32 month)
{
    if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2) {
        return 29;
    }
    return s_DaysInMonth[month - 1];
}

// 0x0056C010 (name is ours)
SvTime GetNow()
{
    return s_State.now;
}

// 0x0056C020 (name is ours)
void ResetOffset()
{
    s_State.offset = 0;
    Update();
}

// 0x0056C038 (name is ours)
void LoadOffset()
{
    s_State.offset = GetTownData().currentTime;
    if (IsTimeFlagSet(&GetTownData().currentTime)) {
        s_State.flags |= FLAG_BEFORE_FIRST_DAY;
    } else {
        s_State.flags &= ~FLAG_BEFORE_FIRST_DAY;
    }
    Start();
}

// 0x0056C08C (name is ours)
void SaveOffset()
{
    GetTownData().currentTime = s_State.offset;
    if ((s_State.flags & FLAG_BEFORE_FIRST_DAY) != 0) {
        SetTimeFlag(&GetTownData().currentTime);
    }
    s_State.flags &= ~FLAG_YEAR_CLAMPED;
}

// 0x0056C0E4 (name is ours)
void SetPendingOffset(SvTime time, s64 milliSeconds)
{
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    SvTime target = time;
    if (milliSeconds != 0) {
        nn::fnd::DateTime dateTime = s_State.baseDateTime;
        dateTime += nn::fnd::TimeSpan::FromNanoSeconds(time);
        dateTime += nn::fnd::TimeSpan::FromMilliSeconds(milliSeconds);
        target = (dateTime - s_State.baseDateTime).GetNanoSeconds();
    }
    s_State.pendingOffset = target - (now - s_State.baseDateTime).GetNanoSeconds();
}

// 0x0056C1DC (name is ours)
void Start()
{
    s_State.realNow = (nn::fnd::DateTime::GetNow() - s_State.baseDateTime).GetNanoSeconds();
    s_RealDate = ToParameters(s_State.realNow);
    s_State.now = s_State.offset + s_State.realNow;
    s_CurrentDate = ToParameters(s_State.now);
    if (s_CurrentDate.year < FIRST_YEAR) {
        s_CurrentDate = s_FirstDate;
        s_State.now = ToTime(s_CurrentDate);
        s_State.flags |= FLAG_YEAR_CLAMPED;
    } else if (s_CurrentDate.year >= END_YEAR) {
        WrapYear(&s_CurrentDate);
        s_State.now = ToTime(s_CurrentDate);
        s_State.flags |= FLAG_YEAR_CLAMPED | FLAG_BEFORE_FIRST_DAY;
    }
    SetCurrentDate(s_CurrentDate);
    if (!s_State.isInitialized) {
        s_State.isInitialized = true;
    }
    GetSavedTime();
}

// 0x0056C3D4 (name is ours)
s64 GetPlayTimeSeconds()
{
    if (s_State.isPlayTimerStarted) {
        s64 seconds = (nn::svc::GetSystemTick() - static_cast<s64>(s_State.playTimerStart.mTick)) / sead::TickSpan::sFrequency;
        if (s_State.pausedSeconds > 0) {
            seconds -= s_State.pausedSeconds;
        }
        if (seconds >= 0) {
            return seconds;
        }
    }
    return 0;
}

// 0x0056C448 (name is ours)
SvTime GetRealNow()
{
    return s_State.realNow;
}

// 0x0056C458 (name is ours)
u8 GetPartOfDay(const nn::fnd::DateTimeParameters& date)
{
    if (Func5D51AC() == 5) {
        if (date.hour < 5) {
            return 5;
        }
        if (date.hour < 8) {
            return 0;
        }
        if (date.hour < 12) {
            return 1;
        }
        if (date.hour < 17) {
            return 2;
        }
        if (date.hour < 21) {
            return 3;
        }
        return 4;
    }
    if (date.hour < 5) {
        return 5;
    }
    if (date.hour < 8) {
        return 0;
    }
    if (date.hour < 12) {
        return 1;
    }
    if (date.hour < 16) {
        return 2;
    }
    if (date.hour < 19) {
        return 3;
    }
    return 4;
}

// 0x0056C4F4 (name is ours)
void ClearBeforeFirstDayFlag()
{
    ClearTimeFlag(&GetTownData().currentTime);
    s_State.flags &= ~FLAG_BEFORE_FIRST_DAY;
}

// 0x0056C520 (name is ours)
void SetIslandOffset(s32 year)
{
    nn::fnd::DateTimeParameters island = s_IslandDate;
    island = nn::fnd::DateTime(year, island.month, island.day, island.hour, island.minute, island.second,
                               island.milliSecond)
                 .GetParameters();
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    s_State.islandOffset = ToTime(island) - (now - s_State.baseDateTime).GetNanoSeconds();
}

// 0x0056C5D4 (name is ours)
s32 GetMoonAge(const nn::fnd::DateTimeParameters& date)
{
    nn::fnd::DateTimeParameters gameDate = date;
    Func308EF0(&gameDate);
    s32 age = ((gameDate.year - 11) % 19) * 11 + s_MoonAgeMonthCorrections[gameDate.month - 1] + gameDate.day + 2;
    return age % 30;
}

// 0x0056C670 (name is ours)
nn::fnd::DateTimeParameters GetFirstDate()
{
    return nn::fnd::DateTime(FIRST_YEAR, 1, 1, 0, 0, 0, 0).GetParameters();
}

// 0x0056C6C4 (name is ours)
s32 GetDaysInYear(s32 year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return 366;
    }
    return 365;
}

// 0x0056C710 (name is ours)
u8 GetSeasonPart(const nn::fnd::DateTimeParameters& date)
{
    if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
        return 12;
    }
    u16 monthDay = date.month << 8 | date.day;
    for (s32 i = 0; i < 23; i++) {
        if (s_SeasonPartStarts[i] >= monthDay) {
            return i;
        }
    }
    return 0;
}

// 0x0056C77C (name is ours)
u8 GetCurrentSeasonPart()
{
    return GetSeasonPart(s_CurrentDate);
}

// 0x0056C7EC (name is ours)
void ApplyPendingOffset()
{
    s_State.offset = s_State.pendingOffset;
    Update();
}

// 0x0056C808 (name is ours)
SvTime GetFirstTime()
{
    nn::fnd::DateTimeParameters first = nn::fnd::DateTime(FIRST_YEAR, 1, 1, 0, 0, 0, 0).GetParameters();
    return ToTime(first);
}

// 0x0056C864 (name is ours)
void ToSeasonPartStart(const u8* part, nn::fnd::DateTimeParameters* date)
{
    u16 start = s_SeasonPartStarts[*part];
    *date = nn::fnd::DateTime(date->year, start >> 8, start & 0xFF, date->hour, date->minute, date->second,
                              date->milliSecond)
                .GetParameters();
}

// 0x0056C8D4 (name is ours)
void PausePlayTimer()
{
    s_State.pauseDateTime = nn::fnd::DateTime::GetNow();
}

// 0x0056C8FC (name is ours)
const nn::fnd::DateTimeParameters* GetRealDate()
{
    return &s_RealDate;
}

// 0x0056C908 (name is ours)
SvTime ToTime(const nn::fnd::DateTimeParameters& date)
{
    return (nn::fnd::DateTime::FromParameters(date) - s_State.baseDateTime).GetNanoSeconds();
}

// 0x0056C978 (name is ours)
u8 GetZodiacSign(s32 month, s32 day)
{
    u8 sign = 0;
    for (u8 i = 0; i < 12; i++) {
        if (s_ZodiacSignEnds[i][0] > month || (s_ZodiacSignEnds[i][0] == month && s_ZodiacSignEnds[i][1] >= day)) {
            sign = i;
            break;
        }
    }
    return sign;
}

// 0x0056C9D4 (name is ours)
SvTime GetDifference(const nn::fnd::DateTimeParameters& a, const nn::fnd::DateTimeParameters& b, bool wrap)
{
    nn::fnd::DateTime dateTimeA = nn::fnd::DateTime::FromParameters(a);
    nn::fnd::DateTime dateTimeB = nn::fnd::DateTime::FromParameters(b);
    if (wrap && (s_State.flags & FLAG_BEFORE_FIRST_DAY) != 0 && dateTimeA < dateTimeB) {
        dateTimeA += nn::fnd::TimeSpan::FromNanoSeconds(s_State.yearsSpan);
    }
    return (dateTimeA - dateTimeB).GetNanoSeconds();
}

// 0x0056CAC4 (name is ours)
void ToPreviousSeasonPartEnd(const u8* part, nn::fnd::DateTimeParameters* date)
{
    u8 previous = *part != 0 ? *part - 1 : 22;
    u16 start = s_SeasonPartStarts[previous];
    *date = nn::fnd::DateTime(date->year, start >> 8, start & 0xFF, date->hour, date->minute, date->second,
                              date->milliSecond)
                .GetParameters();
    nn::fnd::DateTime dateTime = nn::fnd::DateTime::FromParameters(*date);
    dateTime += nn::fnd::TimeSpan::FromNanoSeconds(1LL * 24 * 60 * 60 * 1000000000);
    *date = dateTime.GetParameters();
    WrapYear(date);
}

// 0x0056CBCC (name is ours)
bool IsDateChanged()
{
    nn::fnd::DateTimeParameters other;
    Func3004A8(&other);
    nn::fnd::DateTimeParameters gameDate = s_CurrentDate;
    Func308EF0(&gameDate);
    bool isSame = other.year == gameDate.year && other.month == gameDate.month && other.day == gameDate.day;
    return !isSame;
}

// 0x0056CC50 (name is ours)
void ResumePlayTimer()
{
    s64 seconds = (nn::fnd::DateTime::GetNow() - s_State.pauseDateTime).GetSeconds();
    if (seconds < 0) {
        seconds = 0;
    }
    s_State.pausedSeconds += seconds;
}

// 0x0056CCEC (name is ours)
void StartPlayTimer()
{
    s_State.playTimerStart.setNow();
    s_State.pauseDateTime = nn::fnd::DateTime::GetNow();
    s_State.isPlayTimerStarted = true;
    s_State.pausedSeconds = 0;
}

// 0x0056CD40 (name is ours)
nn::fnd::DateTimeParameters ToParameters(SvTime time)
{
    nn::fnd::DateTime dateTime = s_State.baseDateTime;
    dateTime += nn::fnd::TimeSpan::FromNanoSeconds(time);
    return dateTime.GetParameters();
}

// 0x0056CDA8 (name is ours)
s64 GetDifferenceInSeconds(const nn::fnd::DateTimeParameters& a, const nn::fnd::DateTimeParameters& b,
                                 bool wrap)
{
    return nn::fnd::TimeSpan::FromNanoSeconds(GetDifference(a, b, wrap)).GetSeconds();
}

// 0x0056CEBC (name is ours)
bool IsYearClamped()
{
    return (s_State.flags & FLAG_YEAR_CLAMPED) != 0;
}

// 0x0056CED4 (name is ours)
nn::fnd::DateTimeParameters GetGameDate()
{
    nn::fnd::DateTimeParameters date = s_CurrentDate;
    Func308EF0(&date);
    return date;
}

// 0x0056CF08 (name is ours)
u8 Func56CF08()
{
    for (s32 i = 0; i < 23; i++) {
        if (!Func630200(i)) {
            return i - 1;
        }
    }
    return 0;
}

// 0x0056CF44 (name is ours)
nn::fnd::DateTimeParameters GetSavedTime()
{
    if (g_SvIsSaveMemoryValid && GetGardenPlus()->data.townData.townData1.IsValid() &&
        SvTimeIsValid(&GetGardenPlus()->data.townData.unk_0xEE0E)) {
        SvTimeClear(&s_State.savedTime);
        return SvTimeToParameters(&GetGardenPlus()->data.townData.unk_0xEE0E);
    }
    if (!SvTimeIsValid(&s_State.savedTime)) {
        ResetSavedTime();
    }
    return SvTimeToParameters(&s_State.savedTime);
}

// 0x0056D008 (name is ours)
u8 Func56D008()
{
    for (s32 i = 22; i >= 0; i--) {
        if (!Func630200(i)) {
            return i + 1;
        }
    }
    return 0;
}

// 0x0056D040 (name is ours)
void ResetSavedTime()
{
    SvTimeSet(&s_State.savedTime, s_CurrentDate);
    SvTimeNormalize(&s_State.savedTime);
}

// 0x0056D064 (name is ours)
void Initialize()
{
    s_State.isPlayTimerStarted = false;
    s_State.flags = 0;
    s_State.baseDateTime = nn::fnd::DateTime(2000, 1, 1, 0, 0, 0, 0);
    s_State.firstDateTime = nn::fnd::DateTime(FIRST_YEAR, 1, 1, 0, 0, 0, 0);
    s_State.endDateTime = nn::fnd::DateTime(END_YEAR, 1, 1, 0, 0, 0, 0);
    s_State.islandDateTime = nn::fnd::DateTime(FIRST_YEAR, 8, 1, 0, 0, 0, 0);
    s_State.yearsSpan = (s_State.endDateTime - s_State.firstDateTime).GetNanoSeconds();
    s_FirstDate = s_State.firstDateTime.GetParameters();
    s_IslandDate = s_State.islandDateTime.GetParameters();
    SvTimeClear(&s_State.savedTime);
    Update();
}

// 0x0056D1C0 (name is ours)
void Freeze()
{
    s_State.flags |= FLAG_FROZEN;
}

// 0x0056D1D8 (name is ours)
bool IsFlagBit2()
{
    return (s_State.flags & FLAG_BIT2) != 0;
}

// 0x0056D1F0 (name is ours)
u8 Func56D1F0(const s32* year)
{
    return s_YearDigitTable[*year % 10];
}

// 0x0056D224 (name is ours)
u8 Func56D224(s32 year)
{
    return s_YearDigitTable[year % 10];
}

// 0x0056D254 (name is ours)
u8 Func56D254(s32 year)
{
    return (year - 2008) % 12;
}

// 0x0056D280 (name is ours)
void SetFlagBit3()
{
    s_State.flags |= FLAG_BIT3;
}

// 0x0056D298 (name is ours)
void SetFlagBit2()
{
    s_State.flags |= FLAG_BIT2;
}

// 0x0056D2B0 (name is ours)
void Update()
{
    if ((s_State.flags & FLAG_FROZEN) != 0) {
        return;
    }
    s_State.realNow = (nn::fnd::DateTime::GetNow() - s_State.baseDateTime).GetNanoSeconds();
    s_RealDate = ToParameters(s_State.realNow);
    if (IsIslandStage()) {
        s_State.now = s_State.realNow + s_State.islandOffset;
    } else {
        s_State.now = s_State.offset + s_State.realNow;
    }
    s_CurrentDate = ToParameters(s_State.now);
    if (s_CurrentDate.year < FIRST_YEAR) {
        s_CurrentDate = s_FirstDate;
        s_State.now = ToTime(s_CurrentDate);
    } else if (s_CurrentDate.year >= END_YEAR) {
        WrapYear(&s_CurrentDate);
        s_State.now = ToTime(s_CurrentDate);
        s_State.flags |= FLAG_BEFORE_FIRST_DAY;
    }
    SetCurrentDate(s_CurrentDate);
    if (!s_State.isInitialized) {
        s_State.isInitialized = true;
    }
    GetSavedTime();
}

// 0x0056D4F8 (name is ours)
SvTime Func56D4F8(SvTime time, s32 a, s32 b, s32 c, f32 d)
{
    nn::fnd::DateTimeParameters date = ToParameters(time);
    Func30A5AC(&date, a, b, c, d);
    return ToTime(date);
}

// 0x0056D550 (name is ours)
nn::fnd::DateTimeParameters MakeDate(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second)
{
    return nn::fnd::DateTime(year, month, day, hour, minute, second, 0).GetParameters();
}

// 0x0056D594 (name is ours)
SvTime MakeTime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second)
{
    return (nn::fnd::DateTime(year, month, day, hour, minute, second, 0) - s_State.baseDateTime).GetNanoSeconds();
}

// 0x0056D610 (name is ours)
nn::fnd::DayOfWeek GetDayOfWeek(s32 year, s32 month, s32 day)
{
    return nn::fnd::DateTime(year, month, day, 0, 0, 0, 0).GetParameters().dayOfWeek;
}

// 0x0056D664 (name is ours)
void WrapYear(nn::fnd::DateTimeParameters* date)
{
    if (date->year >= END_YEAR) {
        date->year -= YEAR_COUNT;
        if (date->year >= END_YEAR) {
            date->year -= YEAR_COUNT;
            if (date->year >= END_YEAR) {
                date->year = FIRST_YEAR;
            }
        }
    } else if (date->year < FIRST_YEAR) {
        date->year += YEAR_COUNT;
        if (date->year < FIRST_YEAR) {
            date->year += YEAR_COUNT;
            if (date->year < FIRST_YEAR) {
                date->year = FIRST_YEAR;
            }
        }
    }
}

// 0x0056D6D0 (name is ours)
void SetMilliSecond(nn::fnd::DateTimeParameters* date, s32 milliSecond)
{
    *date = nn::fnd::DateTime(date->year, date->month, date->day, date->hour, date->minute, date->second, milliSecond)
                .GetParameters();
}

// 0x0056D720 (name is ours)
void AddHours(nn::fnd::DateTimeParameters* date, s32 hours)
{
    if (hours == 0) {
        return;
    }
    nn::fnd::DateTime dateTime = nn::fnd::DateTime::FromParameters(*date);
    dateTime += nn::fnd::TimeSpan::FromNanoSeconds(hours * 3600000000000LL);
    *date = dateTime.GetParameters();
    WrapYear(date);
}

// 0x0056D7CC (name is ours)
void SetHour(nn::fnd::DateTimeParameters* date, s32 hour)
{
    *date = nn::fnd::DateTime(date->year, date->month, date->day, hour, date->minute, date->second, date->milliSecond)
                .GetParameters();
}

// 0x0056D81C (name is ours)
void AddMinutes(nn::fnd::DateTimeParameters* date, s32 minutes)
{
    if (minutes == 0) {
        return;
    }
    nn::fnd::DateTime dateTime = nn::fnd::DateTime::FromParameters(*date);
    dateTime += nn::fnd::TimeSpan::FromNanoSeconds(minutes * 60000000000LL);
    *date = dateTime.GetParameters();
    WrapYear(date);
}

// 0x0056D8C4 (name is ours)
void AddSeconds(nn::fnd::DateTimeParameters* date, s32 seconds)
{
    if (seconds == 0) {
        return;
    }
    nn::fnd::DateTime dateTime = nn::fnd::DateTime::FromParameters(*date);
    dateTime += nn::fnd::TimeSpan::FromNanoSeconds(seconds * 1000000000LL);
    *date = dateTime.GetParameters();
    WrapYear(date);
}

// 0x0056D96C (name is ours)
void SetMinute(nn::fnd::DateTimeParameters* date, s32 minute)
{
    *date = nn::fnd::DateTime(date->year, date->month, date->day, date->hour, minute, date->second, date->milliSecond)
                .GetParameters();
}

// 0x0056D9C4 (name is ours)
void SetSecond(nn::fnd::DateTimeParameters* date, s32 second)
{
    *date = nn::fnd::DateTime(date->year, date->month, date->day, date->hour, date->minute, second, date->milliSecond)
                .GetParameters();
}

// 0x0056DA1C (name is ours)
bool IsFlagBit3()
{
    return (s_State.flags & FLAG_BIT3) != 0;
}

// 0x0056DA34 (name is ours)
void ClearFlagBit2()
{
    s_State.flags &= ~FLAG_BIT2;
}

// 0x0056DA4C (name is ours)
void ClearFlagBit3()
{
    s_State.flags &= ~FLAG_BIT3;
}

// 0x0056DA64 (name is ours)
void UpdateBeforeFirstDayFlag()
{
    // before the first day of the game starts (2012-01-01 06:00)
    if (s_CurrentDate.year <= FIRST_YEAR && s_CurrentDate.month <= 1 && s_CurrentDate.day <= 1 &&
        s_CurrentDate.hour < 6) {
        s_State.flags |= FLAG_BEFORE_FIRST_DAY;
        SetTimeFlag(&GetTownData().currentTime);
    } else {
        s_State.flags &= ~FLAG_BEFORE_FIRST_DAY;
        ClearTimeFlag2(&GetTownData().currentTime);
    }
}

// 0x0056DAF4 (name is ours)
nn::fnd::DateTimeParameters GetFirstDate2()
{
    return nn::fnd::DateTime(FIRST_YEAR, 1, 1, 0, 0, 0, 0).GetParameters();
}

// 0x0056DB48 (name is ours)
u8 GetSeason(const nn::fnd::DateTimeParameters& date)
{
    if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
        return 1;
    }
    s8 month = date.month;
    if (month == 2) {
        if (date.day < 25) {
            return 3;
        }
        return 0;
    }
    if (month == 3 || month == 4 || month == 5) {
        return 0;
    }
    if (month == 6 || month == 7 || month == 8) {
        return 1;
    }
    if (month == 9 || month == 10) {
        return 2;
    }
    if (month == 11) {
        if (date.day > 25) {
            return 3;
        }
        return 2;
    }
    return 3;
}

// 0x0056DBE4 (name is ours)
u8 GetCurrentSeason()
{
    return GetSeason(s_CurrentDate);
}

// 0x0056DC84 (name is ours)
void SetCurrentDate(const nn::fnd::DateTimeParameters& date)
{
    s_CurrentDate = date;
    s_State.now = ToTime(date);
    if (IsIslandStage()) {
        s_State.islandOffset = s_State.now - s_State.realNow;
    } else {
        s_State.offset = s_State.now - s_State.realNow;
    }
}

// 0x0056DD18 (name is ours)
void SetOffset(const SvTime* offset)
{
    s_State.offset = *offset;
    s_State.now = s_State.realNow + s_State.offset;
    s_CurrentDate = ToParameters(s_State.now);
    if (s_CurrentDate.year < FIRST_YEAR) {
        s_CurrentDate = s_FirstDate;
        s_State.now = ToTime(s_CurrentDate);
    } else if (s_CurrentDate.year >= END_YEAR) {
        WrapYear(&s_CurrentDate);
        s_State.now = ToTime(s_CurrentDate);
        s_State.flags |= FLAG_BEFORE_FIRST_DAY;
    }
    SetCurrentDate(s_CurrentDate);
}
} // namespace time
