#include <cstdio>
#include <cstring>

#include "imgdb/DateTimeSeconds.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

// (the check of the parameters is an assertion; only the call is left)
inline s64 ComputeSeconds(int year, int month, int day, int hour, int minute, int second)
{
    nn::fnd::DateTime::IsValidParameters(year, month, day, hour, minute, second, 0);
    return static_cast<s64>(nn::fnd::DateTime::DateToDays(year, month, day)) * DateTimeSeconds::SECONDS_PER_DAY +
           (hour * 60 + minute) * 60 + second;
}

} // namespace

// 0x005A95FC | nintendogs:callgraph [tier C]
void imgdb::DateTimeSeconds::Initialize()
{
    m_Seconds = 0;
}

// (clamped to the valid range)
// 0x005A960C | nintendogs:bytes [tier B]
void imgdb::DateTimeSeconds::SetSeconds(s64 seconds)
{
    if (seconds < GetMinSeconds().m_Seconds)
    {
        *this = GetMinSeconds();
    }
    else if (seconds > GetMaxSeconds().m_Seconds)
    {
        *this = GetMaxSeconds();
    }
    else
    {
        m_Seconds = seconds;
    }
}

// 2150-12-31 23:59:59
// 0x005A9684 | nintendogs:bytes [tier B]
imgdb::DateTimeSeconds imgdb::DateTimeSeconds::GetMaxSeconds()
{
    // 0x0097F0F0 (guard 0x0097F0E1)
    static DateTimeSeconds s_MaxSeconds = {ComputeSeconds(2150, 12, 31, 23, 59, 59)};
    return s_MaxSeconds;
}

// 1900-01-01 00:00:00
// 0x005A971C | nintendogs:bytes [tier B]
imgdb::DateTimeSeconds imgdb::DateTimeSeconds::GetMinSeconds()
{
    // 0x0097F0E8 (guard 0x0097F0E0)
    static DateTimeSeconds s_MinSeconds = {ComputeSeconds(1900, 1, 1, 0, 0, 0)};
    return s_MinSeconds;
}

// 0x005A97A8 | nintendogs:bytes [tier B]
bool imgdb::DateTimeSeconds::ConvertFromParam(int year, int month, int day, int hour, int minute, int second)
{
    if (!nn::fnd::DateTime::IsValidParameters(year, month, day, hour, minute, second, 0))
    {
        return false;
    }
    SetSeconds(static_cast<s64>(nn::fnd::DateTime::DateToDays(year, month, day)) * SECONDS_PER_DAY +
               (hour * 60 + minute) * 60 + second);
    return true;
}

// 0x005A988C | nintendogs:bytes [tier B]
imgdb::DateTimeSeconds imgdb::DateTimeSeconds::GetCurrentDateTime()
{
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    nn::fnd::DateTimeParameters param = now.GetParameters();
    DateTimeSeconds dateTime;
    dateTime.ConvertFromParam(param.year, param.month, param.day, param.hour, param.minute, param.second);
    return dateTime;
}

// 0x005A98F0 | nintendogs:bytes [tier B]
void imgdb::DateTimeSeconds::SetCurrentDateTime()
{
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    nn::fnd::DateTimeParameters param = now.GetParameters();
    ConvertFromParam(param.year, param.month, param.day, param.hour, param.minute, param.second);
}

// 0x005A9948 | nintendogs:bytes [tier B]
bool imgdb::DateTimeSeconds::ConvertFromNNDateTime(const nn::fnd::DateTime& dateTime)
{
    nn::fnd::DateTimeParameters param = dateTime.GetParameters();
    return ConvertFromParam(param.year, param.month, param.day, param.hour, param.minute, param.second);
}

// the date format of EXIF: "YYYY:MM:DD HH:MM:SS"
// 0x005A9988 | nintendogs:bytes [tier B]
bool imgdb::DateTimeSeconds::ConvertFromDateTimeString(const char* string)
{
    if (std::strlen(string) != 19 || string[4] != ':' || string[7] != ':' || string[10] != ' ' ||
        string[13] != ':' || string[16] != ':')
    {
        return false;
    }

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    if (!util::AToI(year, string, 4) || !util::AToI(month, string + 5, 2) || !util::AToI(day, string + 8, 2) ||
        !util::AToI(hour, string + 11, 2) || !util::AToI(minute, string + 14, 2) ||
        !util::AToI(second, string + 17, 2))
    {
        return false;
    }
    return ConvertFromParam(year, month, day, hour, minute, second);
}

// (rounded down for times before 2000)
// 0x007556D0 | nintendogs:bytes [tier B]
s32 imgdb::DateTimeSeconds::GetDateDays() const
{
    s64 seconds = m_Seconds;
    if (seconds < 0)
    {
        seconds -= SECONDS_PER_DAY - 1;
    }
    return static_cast<s32>(seconds / SECONDS_PER_DAY);
}

// 0x0075570C | nintendogs:bytes [tier B]
DECOMP_NOINLINE void imgdb::DateTimeSeconds::ConvertToParam(int* year, int* month, int* day, int* hour, int* minute,
                                           int* second) const
{
    int y;
    int mo;
    int d;
    nn::fnd::DateTime::DaysToDate(&y, &mo, &d, GetDateDays());

    s32 seconds = static_cast<s32>(m_Seconds % SECONDS_PER_DAY);
    if (m_Seconds < 0)
    {
        seconds = static_cast<s32>((seconds + static_cast<s64>(SECONDS_PER_DAY)) % SECONDS_PER_DAY);
    }
    int h = seconds / 3600;
    seconds -= h * 3600;
    int mi = seconds / 60;
    int s = seconds - mi * 60;

    if (year != NULL)
    {
        *year = y;
    }
    if (month != NULL)
    {
        *month = mo;
    }
    if (day != NULL)
    {
        *day = d;
    }
    if (hour != NULL)
    {
        *hour = h;
    }
    if (minute != NULL)
    {
        *minute = mi;
    }
    if (second != NULL)
    {
        *second = s;
    }
}

// 0x0075584C | nintendogs:bytes [tier B]
nn::fnd::DateTime imgdb::DateTimeSeconds::ConvertToNNDateTime() const
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    ConvertToParam(&year, &month, &day, &hour, &minute, &second);
    return nn::fnd::DateTime::FromParameters(year, month, day, hour, minute, second, 0);
}

// (the size of the buffer is not checked)
// 0x007558AC | nintendogs:bytes-fuzzy [tier B]
void imgdb::DateTimeSeconds::ConvertToDateTimeString(char* buffer, int size) const
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    ConvertToParam(&year, &month, &day, &hour, &minute, &second);
    std::sprintf(buffer, "%04d:%02d:%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
}

// 0x00755924 | nintendogs:bytes [tier B]
bool imgdb::DateTimeSeconds::IsInvalid() const
{
    return m_Seconds < GetMinSeconds().m_Seconds || m_Seconds > GetMaxSeconds().m_Seconds;
}

} // namespace imgdb
