#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace fnd {
// the day of the week (values from GetParameters: 2000-01-01, day 0, is 6; names are ours)
enum DayOfWeek : u8
{
    DAY_OF_WEEK_SUNDAY = 0,
    DAY_OF_WEEK_MONDAY,
    DAY_OF_WEEK_TUESDAY,
    DAY_OF_WEEK_WEDNESDAY,
    DAY_OF_WEEK_THURSDAY,
    DAY_OF_WEEK_FRIDAY,
    DAY_OF_WEEK_SATURDAY,
};

// a date and time in parts (the type is from the symbols; layout from GetParameters and
// FromParameters, member names are ours)
struct DateTimeParameters
{
    s32 year;          // 0x0
    s8 month;          // 0x4, 1..12
    s8 day;            // 0x5, 1..31
    DayOfWeek dayOfWeek; // 0x6
    s8 hour;           // 0x7
    s8 minute;         // 0x8
    s8 second;         // 0x9
    s16 milliSecond;   // 0xA
};
ASSERT_SIZE(DateTimeParameters, 0xC);

// A point in time: milliseconds since 2000-01-01 00:00 (EPOCH). The dates are proleptic
// Gregorian; the valid ones are 1900-01-01 to 2189-12-31 (IsValidDate).
class DateTime
{
public:
    // (no out-of-line default constructor exists; inline so that stubs compile, value unknown)
    DateTime() {}
    DateTime(int year, int month, int day, int hour, int minute, int second, int milliSecond); // 0x00130FCC | nintendogs:bytes [tier A]

    static DateTime FromParameters(const nn::fnd::DateTimeParameters& parameters); // 0x0012468C | fefates:bytes [tier B]
    DECOMP_NOINLINE static DateTime FromParameters(int year, int month, int day, int hour, int minute, int second, int milliSecond); // 0x0012A21C | nintendogs:bytes [tier A]
    // the time of the clock of the system
    static DateTime GetNow(); // 0x001246C0 | fefates:bytes [tier B]
    DateTimeParameters GetParameters() const; // 0x00126C04 | nintendogs:bytes [tier A]
    DECOMP_NOINLINE DateTime& operator+=(const nn::fnd::TimeSpan& span); // 0x0012A28C | fefates:bytes [tier B]

    // the date of a day number (0: 2000-01-01); null pointers are skipped
    DECOMP_NOINLINE static void DaysToDate(int* pYear, int* pMonth, int* pDay, int days); // 0x0012A08C | nintendogs:bytes [tier A]
    // the day number of a date
    DECOMP_NOINLINE static int DateToDays(int year, int month, int day); // 0x00130ED4 | nintendogs:callgraph [tier A]
    DECOMP_NOINLINE static bool IsValidDate(int year, int month, int day); // 0x00352450 | nintendogs:bytes [tier B]
    static bool IsValidParameters(int year, int month, int day, int hour, int minute, int second, int milliSecond); // 0x00352584 | nintendogs:bytes [tier B]

    s32 GetMilliSecond() const; // 0x0072975C | fefates:bytes [tier B]
    s32 GetDay() const; // 0x007297B0 | nintendogs:bytes [tier A]
    s32 GetHour() const; // 0x00729808 | nintendogs:bytes [tier A]
    DayOfWeek GetDayOfWeek() const; // 0x00729870 (name is ours)
    s32 GetYear() const; // 0x00729900 | nintendogs:bytes [tier A]
    s32 GetMonth() const; // 0x00729958 | nintendogs:bytes [tier A]
    s32 GetMinute() const; // 0x007299B0 | nintendogs:bytes [tier A]
    s32 GetSecond() const; // 0x00729A1C | nintendogs:bytes [tier A]

    // 2000-01-01 00:00, the origin of mMilliSeconds (name is ours)
    static const DateTime EPOCH;

    // inline (name is ours)
    friend TimeSpan operator-(const DateTime& lhs, const DateTime& rhs)
    {
        return TimeSpan::FromMilliSeconds(lhs.mMilliSeconds - rhs.mMilliSeconds);
    }

private:
    explicit DateTime(s64 milliSeconds) : mMilliSeconds(milliSeconds) {}

    s64 mMilliSeconds;      // 0x0 (the member name is ours)
};
ASSERT_SIZE(DateTime, 8);
} // namespace fnd
} // namespace nn
