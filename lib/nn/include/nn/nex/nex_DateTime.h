#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace nex {
class StringStream;

// A date and time packed in 64 bits: second (bits 0-5), minute (6-11), hour (12-16), day
// (17-21), month (22-25), year (26-39) and a state (40-41, 1: invalid). The names of the fields,
// of the members and of the functions without a reference name are ours.
class DateTime
{
public:
    static const u64 SECOND_MASK = 0x3F;
    static const u64 MINUTE_MASK = 0xFC0;
    static const u64 HOUR_MASK = 0x1F000;
    static const u64 DAY_MASK = 0x3E0000;
    static const u64 MONTH_MASK = 0x3C00000;
    static const u64 YEAR_MASK = 0xFFFC000000ULL;
    static const u64 STATE_MASK = 0x30000000000ULL;
    static const u32 MINUTE_SHIFT = 6;
    static const u32 HOUR_SHIFT = 12;
    static const u32 DAY_SHIFT = 17;
    static const u32 MONTH_SHIFT = 22;
    static const u32 YEAR_SHIFT = 26;
    static const u32 STATE_SHIFT = 40;
    static const u8 STATE_INVALID = 1;

    DateTime(); // 0x003D4C3C | fefates:callgraph [tier C]
    // (inline) the packed value
    explicit DateTime(u64 value) : m_Value(value) {}
    DateTime(const nn::nex::DateTime& other); // 0x003D4B40 | fefates:callgraph [tier C]
    // (year 1900-9999; an invalid date is s_pNever)
    DateTime(unsigned short year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute,
             unsigned char second); // 0x003D4B4C | fefates:bytes-fuzzy [tier B]
    nn::nex::DateTime& operator=(const nn::nex::DateTime& other); // 0x003D4C4C | fefates:callgraph [tier C]

    // the local time of the system
    static void GetSystemTime(nn::nex::DateTime& dateTime); // 0x003D494C | fefates:bytes [tier B]
    // the seconds since 2000-03-01 minus 59 days (with the leap day rule only before 2000)
    u64 ToEpochTime() const; // 0x0072E644 | fefates:bytes [tier B]
    operator unsigned long long() const; // 0x0072E99C | fefates:callseq-callee [tier B]
    // the packed value (a second copy of the conversion; the name is ours)
    u64 GetValue() const; // 0x003D4C58
    // the milliseconds between the times
    s64 operator-(const nn::nex::DateTime& other) const; // 0x0072E9AC | fefates:bytes [tier B]

    // the date of a day number (days since 2000-01-01)
    void SetDate(s32 days); // 0x003D47F8
    // the date and time of seconds since the day dayOffset (days since 2000-01-01)
    void SetTime(s64 seconds, s32 dayOffset); // 0x003D4A7C
    // the date and time of seconds since 1970-01-01
    void SetUnixTime(s64 seconds); // 0x003D4A64
    // the date as an nn::fnd::DateTime (the epoch of fnd if it is invalid)
    nn::fnd::DateTime ToFndDateTime() const; // 0x0072E7D8

    // the fields (inline in the original; armlink placed copies at the end of the code)
    u8 GetDay() const; // 0x0072E904
    u8 GetHour() const; // 0x0072E920
    u16 GetYear() const; // 0x0072E93C
    u8 GetMonth() const; // 0x0072E958
    u8 GetMinute() const; // 0x0072E974
    u8 GetSecond() const; // 0x0072E990

    // (inline)
    void SetField(u64 mask, u32 shift, u64 value) { m_Value = (m_Value & ~mask) | ((value << shift) & mask); }

    u64 m_Value; // 0x0

    // (references in the original: the year 0 and 9999-12-31 23:59:59)
    static const DateTime* s_pNever;
    static const DateTime* s_pForever;
};
ASSERT_SIZE(DateTime, 0x8);

// the date as text "YYYY-MM-DD hh:mm:ss" (the name is ours)
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::DateTime& dateTime); // 0x003DA0A8
} // namespace nex
} // namespace nn
