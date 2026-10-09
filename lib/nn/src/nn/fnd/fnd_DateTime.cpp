#include "nn/fnd/fnd_DateTime.h"
#include "nn/ptm/CTR/detail/detail_Api.h"

namespace nn {
namespace fnd {
namespace {
const s32 MILLISECONDS_PER_SECOND = 1000;
const s32 MILLISECONDS_PER_MINUTE = 60 * MILLISECONDS_PER_SECOND;
const s32 MILLISECONDS_PER_HOUR = 60 * MILLISECONDS_PER_MINUTE;
const s32 MILLISECONDS_PER_DAY = 24 * MILLISECONDS_PER_HOUR;

// the days from 0001-01-01 to 2000-01-01: added before the division by a day so that it rounds
// down for every date after year 1
const s32 DAYS_FROM_YEAR_1 = 730119;

const s32 BASE_YEAR = 2000;
// the day numbers count from 2000-03-01 (day 60), so that the leap day is the last day of a year
const s32 DAYS_BEFORE_MARCH = 60;
const s32 DAYS_PER_400_YEARS = 146097;
const s32 DAYS_PER_100_YEARS = 36524;
const s32 DAYS_PER_4_YEARS = 1461;
const s32 DAYS_PER_YEAR = 365;

// the range of IsValidDate: 1900-01-01 and 2189-12-31 as day numbers
const s32 MIN_DAYS = -36524;
const s32 MAX_DAYS = 69396;

inline bool IsLeapYear(int year)
{
    if (year % 400 == 0) {
        return true;
    }
    if (year % 100 == 0) {
        return false;
    }
    return year % 4 == 0;
}

// the day number of a DateTime
inline s32 GetDays(s64 milliSeconds)
{
    return static_cast<s32>((milliSeconds + static_cast<s64>(DAYS_FROM_YEAR_1) * MILLISECONDS_PER_DAY) / MILLISECONDS_PER_DAY) - DAYS_FROM_YEAR_1;
}

// the milliseconds since the start of the day of a DateTime
inline s32 GetMilliSecondsOfDay(s64 milliSeconds)
{
    return static_cast<s32>((milliSeconds + static_cast<s64>(DAYS_FROM_YEAR_1) * MILLISECONDS_PER_DAY) % MILLISECONDS_PER_DAY);
}

inline DayOfWeek GetDayOfWeekOfDays(s32 days)
{
    // 2000-01-01 is a Saturday; the second % 7 makes it positive
    return static_cast<DayOfWeek>(((days + DAY_OF_WEEK_SATURDAY) % 7 + 7) % 7);
}
} // namespace

// 0x00975EF8
const nn::fnd::DateTime nn::fnd::DateTime::EPOCH(0);

// 0x0012468C | fefates:bytes [tier B]
nn::fnd::DateTime nn::fnd::DateTime::FromParameters(const nn::fnd::DateTimeParameters& parameters)
{
    return FromParameters(parameters.year, parameters.month, parameters.day, parameters.hour, parameters.minute, parameters.second,
                          parameters.milliSecond);
}

// 0x001246C0 | fefates:bytes [tier B]
nn::fnd::DateTime nn::fnd::DateTime::GetNow()
{
    TimeSpan span = TimeSpan::FromMilliSeconds(nn::ptm::CTR::detail::GetSwcMilliSeconds());
    DateTime now = EPOCH;
    return now += span;
}

// 0x00126C04 | nintendogs:bytes [tier A]
nn::fnd::DateTimeParameters nn::fnd::DateTime::GetParameters() const
{
    s32 days = GetDays(mMilliSeconds);
    s32 milliSeconds = GetMilliSecondsOfDay(mMilliSeconds);
    int year;
    int month;
    int day;
    DaysToDate(&year, &month, &day, days);
    DateTimeParameters parameters;
    parameters.year = year;
    parameters.month = month;
    parameters.day = day;
    parameters.dayOfWeek = GetDayOfWeekOfDays(days);
    parameters.hour = milliSeconds / MILLISECONDS_PER_HOUR;
    parameters.minute = milliSeconds / MILLISECONDS_PER_MINUTE % 60;
    parameters.second = milliSeconds / MILLISECONDS_PER_SECOND % 60;
    parameters.milliSecond = milliSeconds % MILLISECONDS_PER_SECOND;
    return parameters;
}

// 0x0012A08C | nintendogs:bytes [tier A]
void nn::fnd::DateTime::DaysToDate(int* pYear, int* pMonth, int* pDay, int days)
{
    days -= DAYS_BEFORE_MARCH;
    int cycles400 = days / DAYS_PER_400_YEARS;
    int rest = days % DAYS_PER_400_YEARS;
    if (rest < 0) {
        cycles400--;
        rest += DAYS_PER_400_YEARS;
    }
    int centuries = rest / DAYS_PER_100_YEARS;
    rest %= DAYS_PER_100_YEARS;
    int cycles4 = rest / DAYS_PER_4_YEARS;
    rest %= DAYS_PER_4_YEARS;
    int years = rest / DAYS_PER_YEAR;
    int dayOfYear = rest % DAYS_PER_YEAR;

    // the month counts from March (0) to February (11)
    int month = (dayOfYear * 5 + 2) / 153;
    int year = cycles400 * 400 + centuries * 100 + cycles4 * 4 + years + BASE_YEAR;
    int day = dayOfYear - (month * 153 + 2) / 5 + 1;
    if (years == 4 || centuries == 4) {
        // the leap day at the end of a 4 (or 400) year cycle
        month = 11;
        day = 29;
        year--;
    }
    if (month <= 9) {
        month += 3;
    } else {
        month -= 9;
        year++;
    }
    if (pYear != 0) {
        *pYear = year;
    }
    if (pMonth != 0) {
        *pMonth = month;
    }
    if (pDay != 0) {
        *pDay = day;
    }
}

// 0x0012A21C | nintendogs:bytes [tier A]
nn::fnd::DateTime nn::fnd::DateTime::FromParameters(int year, int month, int day, int hour, int minute, int second, int milliSecond)
{
    int days = DateToDays(year, month, day);
    return DateTime(static_cast<s64>(days) * MILLISECONDS_PER_DAY + static_cast<s64>(hour) * MILLISECONDS_PER_HOUR +
                    static_cast<s64>(minute) * MILLISECONDS_PER_MINUTE + static_cast<s64>(second) * MILLISECONDS_PER_SECOND + milliSecond);
}

// 0x0012A28C | fefates:bytes [tier B]
nn::fnd::DateTime& nn::fnd::DateTime::operator+=(const nn::fnd::TimeSpan& span)
{
    mMilliSeconds += span.GetMilliSeconds();
    return *this;
}

// 0x00130ED4 | nintendogs:callgraph [tier A]
int nn::fnd::DateTime::DateToDays(int year, int month, int day)
{
    // years and months from March, as in DaysToDate
    year -= BASE_YEAR;
    if (month > 2) {
        month -= 3;
    } else {
        month += 9;
        year--;
    }
    // the divisions below round towards zero: before 2000 a year only gets its leap day here
    int leapDay = 1;
    if (year < 0) {
        leapDay = IsLeapYear(year) ? 1 : 0;
    }
    int centuries = year / 100;
    return centuries * DAYS_PER_400_YEARS / 4 + (year - centuries * 100) * DAYS_PER_4_YEARS / 4 + (month * 153 + 2) / 5 + (day - 1) +
           leapDay + (DAYS_BEFORE_MARCH - 1);
}

// 0x00130FCC | nintendogs:bytes [tier A]
nn::fnd::DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int milliSecond)
{
    *this = FromParameters(year, month, day, hour, minute, second, milliSecond);
}

// 0x00352450 | nintendogs:bytes [tier B]
bool nn::fnd::DateTime::IsValidDate(int year, int month, int day)
{
    const int DAYS_OF_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int minYear;
    DaysToDate(&minYear, 0, 0, MIN_DAYS);
    int maxYear;
    DaysToDate(&maxYear, 0, 0, MAX_DAYS);
    if (year < minYear || year > maxYear) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    int daysOfMonth;
    if (month == 2) {
        daysOfMonth = DAYS_OF_MONTH[1] + (IsLeapYear(year) ? 1 : 0);
    } else {
        daysOfMonth = DAYS_OF_MONTH[month - 1];
    }
    return day >= 1 && day <= daysOfMonth;
}

// 0x00352584 | nintendogs:bytes [tier B]
bool nn::fnd::DateTime::IsValidParameters(int year, int month, int day, int hour, int minute, int second, int milliSecond)
{
    return IsValidDate(year, month, day) && hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60 &&
           milliSecond >= 0 && milliSecond < 1000;
}

// 0x0072975C | fefates:bytes [tier B]
s32 nn::fnd::DateTime::GetMilliSecond() const
{
    return GetMilliSecondsOfDay(mMilliSeconds) % MILLISECONDS_PER_SECOND;
}

// 0x007297B0 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetDay() const
{
    int day;
    DaysToDate(0, 0, &day, GetDays(mMilliSeconds));
    return day;
}

// 0x00729808 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetHour() const
{
    return GetMilliSecondsOfDay(mMilliSeconds) / MILLISECONDS_PER_HOUR % 24;
}

// 0x00729870 (name is ours)
nn::fnd::DayOfWeek nn::fnd::DateTime::GetDayOfWeek() const
{
    return GetDayOfWeekOfDays(GetDays(mMilliSeconds));
}

// 0x00729900 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetYear() const
{
    int year;
    DaysToDate(&year, 0, 0, GetDays(mMilliSeconds));
    return year;
}

// 0x00729958 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetMonth() const
{
    int month;
    DaysToDate(0, &month, 0, GetDays(mMilliSeconds));
    return month;
}

// 0x007299B0 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetMinute() const
{
    return GetMilliSecondsOfDay(mMilliSeconds) / MILLISECONDS_PER_MINUTE % 60;
}

// 0x00729A1C | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetSecond() const
{
    return GetMilliSecondsOfDay(mMilliSeconds) / MILLISECONDS_PER_SECOND % 60;
}

} // namespace fnd
} // namespace nn
