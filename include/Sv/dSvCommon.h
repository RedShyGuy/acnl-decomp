#pragma once

// Basic value types of the save data (dates, encrypted values, time).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"

#pragma pack(push, 1)

// Values that the game stores obfuscated (bells, medals, ...): see SvEncValue tools.
struct SvEncValue {
    // (game code, not decompiled yet; names are ours) the value 0 / a value is stored obfuscated
    SvEncValue() { Set(0); } // 0x00614898
    void Set(u32 value); // 0x003036A4
    // (game code, not decompiled yet) the value goes up, at most to max
    void Add(u32 value, u32 max); // 0x00614764
    // (game code, not decompiled yet; names are ours) the value / the value goes down
    u32 Get() const; // 0x003037DC
    void Sub(u32 value); // 0x00614810

    /* 0x0000 */ u32 data[2];
};
ASSERT_SIZE(SvEncValue, 0x8);

// A time in the save data: nanoseconds since 2000-01-01 00:00:00 (not since 1970); 0x7FFFFFFFFFFFFFFF is no time.
// The constructor clears it and copies the value in (the struct is packed). The clock itself calculates with s64.
struct SvTime {
    static const s64 NONE = 0x7FFFFFFFFFFFFFFFll;

    SvTime(s64 time = NONE); // 0x005CEA18 (name is ours)

    /* 0x0000 */ s64 value;
};
ASSERT_SIZE(SvTime, 0x8);

struct SvDate {
    // (names are ours) cleared / cleared and set (SetSvDate)
    constexpr SvDate() : year(0), month(0), day(0) {} // 0x003020B8
    SvDate(u16 year, u8 month, u8 day); // 0x005CBA08

    /* 0x0000 */ u16 year;
    /* 0x0002 */ u8 month;
    /* 0x0003 */ u8 day;
};
ASSERT_SIZE(SvDate, 0x4);

// a month and a day (name is ours; the constructor takes them from time::GetFirstDate). Only user: a local array of 18
// of them in the game function 0x104B54 (Thumb; filled with dates through 0x301B70, read with a bounds check 0x301B64);
// not the birthday of the player (SvPlayer::birthMonth/birthDay are two single bytes)
struct SvMonthDay {
    SvMonthDay(); // 0x006BA2A8

    /* 0x0000 */ u8 month;
    /* 0x0001 */ s8 day;
};
ASSERT_SIZE(SvMonthDay, 0x2);

struct SvDateTime {
    /* 0x0000 */ u16 year;
    /* 0x0002 */ u16 unk_0x2; // Maybe padding
    /* 0x0004 */ u8 month;
    /* 0x0005 */ u8 day;
    /* 0x0006 */ u8 unk_0x6; // some sort of flag probably
    /* 0x0007 */ u8 hour;
    /* 0x0008 */ u8 minute;
    /* 0x0009 */ u8 second;
    /* 0x000A */ u16 millisecond;
};
ASSERT_SIZE(SvDateTime, 0xC);

#pragma pack(pop)
