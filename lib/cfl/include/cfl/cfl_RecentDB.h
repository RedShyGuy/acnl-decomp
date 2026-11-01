#pragma once

// The recently used Miis ("CFRA" in CFL_DB.dat; the class name is from the reference symbols, members are ours) and
// the list of received Miis that go into the hidden Miis of the database (names are ours).

#include "cfl/cfl_Types.h"

#define CFL_RECENT_NUM 100

class CFLiRecentDBFile
{
public:
    s32 DeleteIndex(int index); // 0x002B7530 | nintendogs:bytes [tier B]

    u32 magic;                              // 0x0000 "CFRA"
    s32 num;                                // 0x0004
    u8 order[CFL_RECENT_NUM];               // 0x0008 the slots, oldest first
    CFLiMiiDataCore entries[CFL_RECENT_NUM];  // 0x006C
    bool dirty;                             // 0x1C8C
    u8 _1C8D[0x11];                         // 0x1C8D
    u16 crc;                                // 0x1C9E
};
ASSERT_SIZE(CFLiRecentDBFile, 0x1CA0);

// a hidden Mii in the file after the database (3dbrew: the 3000 Miis with a timestamp): bits 25-31 year - 2000,
// 21-24 month, 16-20 day, 11-15 hour, 5-10 minute, 0-4 second / 2
struct CFLiHiddenEntry
{
    CFLiMiiDataCore core;  // 0x00
    u32 timestamp;         // 0x48
    u32 _4C;               // 0x4C
    u32 _50;               // 0x50
};
ASSERT_SIZE(CFLiHiddenEntry, 0x54);

struct CFLiDevice;

struct CFLiReceivedList
{
    CFLiHiddenEntry entries[128];  // 0x0000
    u16 num;                       // 0x2A00
    bool moveToEnd;                // 0x2A02 Miis that are hidden already move to the end of the list
    CFLiHiddenEntry* mirror;       // 0x2A04 a copy of the hidden Miis of the program
    bool hasMirror;                // 0x2A08
    void (*callback)(CFLiDevice* device, s32 value);  // 0x2A0C
    CFLiHiddenEntry* buffer;       // 0x2A10
    u32 _2A14;                     // 0x2A14
    s32 written;                   // 0x2A18 hidden Miis cleared so far, -1 before the first
    s32 _2A1C;                     // 0x2A1C
    u32 _2A20;                     // 0x2A20
    u32 _2A24;                     // 0x2A24
    u32 _2A28;                     // 0x2A28
};
ASSERT_SIZE(CFLiReceivedList, 0x2A2C);
