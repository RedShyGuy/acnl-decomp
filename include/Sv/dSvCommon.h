#pragma once

// Basic value types of the save data (dates, encrypted values, time).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"

#pragma pack(push, 1)

// Values that the game stores obfuscated (bells, medals, ...): see SvEncValue tools.
typedef u64 SvEncValue;

// Nanoseconds since 2000-01-01 00:00:00 (not since 1970).
typedef s64 SvTime;

struct SvDate {
    /* 0x0000 */ u16 year;
    /* 0x0002 */ u8 month;
    /* 0x0003 */ u8 day;
};
ASSERT_SIZE(SvDate, 0x4);

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
