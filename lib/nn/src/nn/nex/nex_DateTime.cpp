#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_StringStream.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// 0x0096CAE0 (name is ours)
DateTime s_Never;
// 0x0096CAE8 (name is ours)
DateTime s_Forever(9999, 12, 31, 23, 59, 59);
} // namespace

// 0x0096CA90 (name is ours)
const DateTime* DateTime::s_pNever = &s_Never;
// 0x0096CA94 (name is ours)
const DateTime* DateTime::s_pForever = &s_Forever;

// 0x003D47F8 (name is ours)
void nn::nex::DateTime::SetDate(s32 days)
{
    // (the Gregorian date of the Julian day number days + 2451545)
    s32 l = days + 2520114;
    s32 n = 4 * l / 146097;
    l = l - (146097 * n + 3) / 4;
    s32 i = 4000 * (l + 1) / 1461001;
    l = l - 1461 * i / 4 + 31;
    s32 j = 80 * l / 2447;
    s32 day = l - 2447 * j / 80;
    l = j / 11;
    s32 month = j + 2 - 12 * l;
    s32 year = 100 * (n - 49) + i + l;
    SetField(YEAR_MASK, YEAR_SHIFT, year);
    SetField(MONTH_MASK, MONTH_SHIFT, month);
    SetField(DAY_MASK, DAY_SHIFT, day);
}

// 0x003D494C | fefates:bytes [tier B]
void nn::nex::DateTime::GetSystemTime(nn::nex::DateTime& dateTime)
{
    nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    dateTime.SetField(YEAR_MASK, YEAR_SHIFT, now.GetYear());
    dateTime.SetField(MONTH_MASK, MONTH_SHIFT, now.GetMonth());
    dateTime.SetField(DAY_MASK, DAY_SHIFT, now.GetDay());
    dateTime.SetField(HOUR_MASK, HOUR_SHIFT, now.GetHour());
    dateTime.SetField(MINUTE_MASK, MINUTE_SHIFT, now.GetMinute());
    dateTime.SetField(SECOND_MASK, 0, now.GetSecond());
}

// 0x003D4A64 (name is ours)
void nn::nex::DateTime::SetUnixTime(s64 seconds)
{
    // (the days from 1970-01-01 to 2000-01-01)
    SetTime(seconds, -10957);
}

// 0x003D4A7C (name is ours)
void nn::nex::DateTime::SetTime(s64 seconds, s32 dayOffset)
{
    m_Value = 0;
    SetDate(static_cast<s32>(seconds / 86400) + dayOffset);
    SetField(HOUR_MASK, HOUR_SHIFT, static_cast<u64>(seconds) / 3600 % 24);
    SetField(MINUTE_MASK, MINUTE_SHIFT, static_cast<u64>(seconds) / 60 % 60);
    SetField(SECOND_MASK, 0, static_cast<u64>(seconds) % 60);
}

// 0x003D4B40 | fefates:callgraph [tier C]
nn::nex::DateTime::DateTime(const nn::nex::DateTime& other) : m_Value(other.m_Value)
{
}

// 0x003D4B4C | fefates:bytes-fuzzy [tier B]
nn::nex::DateTime::DateTime(unsigned short year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute,
                            unsigned char second)
    : m_Value(0)
{
    SetField(YEAR_MASK, YEAR_SHIFT, year);
    SetField(MONTH_MASK, MONTH_SHIFT, month);
    SetField(DAY_MASK, DAY_SHIFT, day);
    SetField(HOUR_MASK, HOUR_SHIFT, hour);
    SetField(MINUTE_MASK, MINUTE_SHIFT, minute);
    SetField(SECOND_MASK, 0, second);
    if (year < 1900) {
        SetField(STATE_MASK, STATE_SHIFT, STATE_INVALID);
    }
    if (year > 9999 || month == 0 || month > 12 || day == 0 || day > 31) {
        SetField(STATE_MASK, STATE_SHIFT, STATE_INVALID);
        *this = *s_pNever;
    }
}

// 0x003D4C3C | fefates:callgraph [tier C]
nn::nex::DateTime::DateTime() : m_Value(0)
{
}

// 0x003D4C4C | fefates:callgraph [tier C]
nn::nex::DateTime& nn::nex::DateTime::operator=(const nn::nex::DateTime& other)
{
    m_Value = other.m_Value;
    return *this;
}

// 0x0072E644 | fefates:bytes [tier B]
u64 nn::nex::DateTime::ToEpochTime() const
{
    s32 year = GetYear() - 2000;
    s32 month = GetMonth();
    s32 shiftedMonth;
    if (month <= 2) {
        shiftedMonth = month + 9;
        year--;
    } else {
        shiftedMonth = month - 3;
    }
    s32 leapDay = 1;
    if (year < 0 && year % 400 != 0) {
        if (year % 100 == 0) {
            leapDay = 0;
        } else if ((year & 3) != 0) {
            leapDay = 0;
        }
    }
    s32 days = (year / 100) * 146097 / 4 + (year % 100) * 1461 / 4 + (153 * shiftedMonth + 2) / 5 + (GetDay() - 1) + leapDay + 59;
    return static_cast<s64>(days) * 86400 + GetSecond() + (static_cast<u64>(GetMinute()) * 60 + static_cast<u64>(GetHour()) * 3600);
}

// 0x0072E7D8 (name is ours)
nn::fnd::DateTime nn::nex::DateTime::ToFndDateTime() const
{
    nn::fnd::DateTime dateTime = nn::fnd::DateTime::MIN_DATETIME;
    if (nn::fnd::DateTime::IsValidParameters(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond(), 0)) {
        dateTime = nn::fnd::DateTime(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond(), 0);
    }
    return dateTime;
}

// 0x0072E904
u8 nn::nex::DateTime::GetDay() const
{
    return static_cast<u8>((m_Value & DAY_MASK) >> DAY_SHIFT);
}

// 0x0072E920
u8 nn::nex::DateTime::GetHour() const
{
    return static_cast<u8>((m_Value & HOUR_MASK) >> HOUR_SHIFT);
}

// 0x0072E93C
u16 nn::nex::DateTime::GetYear() const
{
    return static_cast<u16>((m_Value & YEAR_MASK) >> YEAR_SHIFT);
}

// 0x0072E958
u8 nn::nex::DateTime::GetMonth() const
{
    return static_cast<u8>((m_Value & MONTH_MASK) >> MONTH_SHIFT);
}

// 0x0072E974
u8 nn::nex::DateTime::GetMinute() const
{
    return static_cast<u8>((m_Value & MINUTE_MASK) >> MINUTE_SHIFT);
}

// 0x0072E990
u8 nn::nex::DateTime::GetSecond() const
{
    return static_cast<u8>(m_Value & SECOND_MASK);
}

// 0x003D4C58
u64 nn::nex::DateTime::GetValue() const
{
    return m_Value;
}

// 0x0072E99C | fefates:callseq-callee [tier B]
nn::nex::DateTime::operator unsigned long long() const
{
    return m_Value;
}

// 0x0072E9AC | fefates:bytes [tier B]
s64 nn::nex::DateTime::operator-(const nn::nex::DateTime& other) const
{
    return (ToEpochTime() - other.ToEpochTime()) * 1000;
}

// 0x003DA0A8
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::DateTime& dateTime)
{
    wchar_t text[24];
    swprintf(text, sizeof(text) / sizeof(text[0]), L"%04d-%02d-%02d %02d:%02d:%02d", dateTime.GetYear(), dateTime.GetMonth(),
             dateTime.GetDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
    return stream << text;
}

} // namespace nex
} // namespace nn
