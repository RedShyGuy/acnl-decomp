#pragma once

// Villagers (NPCs) living in the town.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvMyDesign.h"
#include "Sv/dSvNpcRef.h"
#include "Sv/dSvPersonalId.h"

#pragma pack(push, 1)
struct SvNpcHomeHistory {
    /* 0x0000 */ SvPersonalId unk_0x0;
    /* 0x002E */ u8 unk_0x2E[0x22]; // research name: Zerod / Related to class script::WordPtrSv, technically same buffer as UnkName
    /* 0x0050 */ char16 unk_0x50[9]; // research name: UnkName / Related to class script::WordPtrSv, technically same buffer as Zerod
    /* 0x0062 */ SvFgName unk_0x62; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0066 */ SvFgName unk_0x66; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x006A */ SvFgName unk_0x6A; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x006E */ SvFgName unk_0x6E; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0072 */ SvFgName unk_0x72; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0076 */ SvFgName unk_0x76; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x007A */ SvFgName unk_0x7A; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x007E */ SvFgName unk_0x7E; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0082 */ SvFgName unk_0x82; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0086 */ SvFgName unk_0x86; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x008A */ SvFgName unk_0x8A; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x008E */ SvFgName unk_0x8E; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0092 */ SvFgName unk_0x92; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x0096 */ SvFgName unk_0x96; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x009A */ SvFgName unk_0x9A; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x009E */ SvFgName unk_0x9E; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00A2 */ SvFgName unk_0xA2; // Group 3; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00A6 */ SvFgName unk_0xA6; // Group 4; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00AA */ SvFgName unk_0xAA; // Group 5; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00AE */ SvFgName unk_0xAE; // Group 6; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00B2 */ SvFgName unk_0xB2; // Group 7; Some Item; Set to 0x00007ffe in player ctor
    /* 0x00B6 */ u8 unk_0xB6[8];
    /* 0x00BE */ u16 unk_0xBE;
    /* 0x00C0 */ SvTownId unk_0xC0;
    /* 0x00D6 */ SvU64 unk_0xD6; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x00DE */ SvDate date1;
    /* 0x00E2 */ SvDate date2;
    /* 0x00E6 */ SvDate date3;
    /* 0x00EA */ SvDate date4;
    /* 0x00EE */ u16 unk_0xEE; // ctor sets to 0xFFFF
    /* 0x00F0 */ u8 unk_0xF0; // ctor sets to 0x31
    /* 0x00F1 */ u8 pad_0xF1;
};
#pragma pack(pop)
ASSERT_SIZE(SvNpcHomeHistory, 0xF2);

struct SvNpcStatus {
    /* 0x0000.0 */ u8 isBoxed : 1;
    /* 0x0000.1 */ u8 hasMoved : 1;
    /* 0x0000.2 */ u8 unk_0x0_2 : 1;
    /* 0x0000.3 */ u8 unk_0x0_3 : 1;
    /* 0x0000.4 */ u8 unk_0x0_4 : 1;
    /* 0x0000.5 */ u8 unk_0x0_5 : 1;
    /* 0x0000.6 */ u8 unk_0x0_6 : 1;
    /* 0x0000.7 */ u8 unk_0x0_7 : 1;
    /* 0x0001.0 */ u8 unk_0x1_0 : 1;
    /* 0x0001.1 */ u8 unk_0x1_1 : 1;
    /* 0x0001.2 */ u8 unk_0x1_2 : 1;
    /* 0x0001.3 */ u8 unk_0x1_3 : 1;
    /* 0x0001.4 */ u8 unk_0x1_4 : 1;
    /* 0x0001.5 */ u8 unk_0x1_5 : 1; // research name: RelatedToBelowButUnk
    /* 0x0001.6 */ u8 movingToAnotherTown : 1;
    /* 0x0001.7 */ u8 unk_0x1_7 : 1;
    /* 0x0002.0 */ u8 unk_0x2_0 : 1;
    /* 0x0002.1 */ u8 unk_0x2_1 : 1;
    /* 0x0002.2 */ u8 unk_0x2_2 : 1;
    /* 0x0002.3 */ u8 unk_0x2_3 : 1;
    /* 0x0002.4 */ u8 unk_0x2_4 : 1;
    /* 0x0002.5 */ u8 unk_0x2_5 : 1;
    /* 0x0002.6 */ u8 unk_0x2_6 : 1;
    /* 0x0002.7 */ u8 unk_0x2_7 : 1;
    /* 0x0003.0 */ u8 unk_0x3_0 : 1;
    /* 0x0003.1 */ u8 unk_0x3_1 : 1;
    /* 0x0003.2 */ u8 unk_0x3_2 : 1;
    /* 0x0003.3 */ u8 unk_0x3_3 : 1;
    /* 0x0003.4 */ u8 unk_0x3_4 : 1;
    /* 0x0003.5 */ u8 unk_0x3_5 : 1;
    /* 0x0003.6 */ u8 unk_0x3_6 : 1;
    /* 0x0003.7 */ u8 unk_0x3_7 : 1;
};
ASSERT_SIZE(SvNpcStatus, 0x4);

#pragma pack(push, 1)
struct SvNpc {
    /* 0x0000 */ SvNpcRef npc; // Which villager
    /* 0x0030 */ SvMyDesign pattern;
    /* 0x08A0 */ SvTownId townId1;
    /* 0x08B6 */ u32 unk_0x8B6[2]; // Game copies value from 0x6E29A, otherwise gens random number, gets u32 value then does something to it (EUR 1.5 IDA: 0x6BBBD4)
    /* 0x08BE */ SvU64 unk_0x8BE; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x08C6 */ SvFgName unk_0x8C6; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08CA */ SvFgName unk_0x8CA; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08CE */ SvFgName unk_0x8CE; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08D2 */ SvFgName unk_0x8D2; // Group 1; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08D6 */ SvFgName unk_0x8D6; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08DA */ SvFgName unk_0x8DA; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08DE */ SvFgName unk_0x8DE; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08E2 */ SvFgName unk_0x8E2; // Group 2; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08E6 */ SvFgName unk_0x8E6; // Group 3; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08EA */ SvFgName unk_0x8EA; // Group 4; Some Item; Set to 0x00007ffe in player ctor
    /* 0x08EE */ SvNpcHomeHistory home[0x10]; // If moving: 0 = next home, 1 = current home | If not moving: 0 = current home, 1 = last home
    /* 0x180E */ u16 pad_0x180E;
    /* 0x1810 */ SvMail letter[5];
    /* 0x2490 */ u16 unk_0x2490; // Single; Set to 2011 in ctor
    /* 0x2492 */ u16 unk_0x2492; // Group 1; Set to 2011 in ctor
    /* 0x2494 */ u16 unk_0x2494; // Group 1; Set to 2011 in ctor
    /* 0x2496 */ u16 unk_0x2496; // Group 1; Set to 2011 in ctor
    /* 0x2498 */ u16 unk_0x2498; // Group 1; Set to 2011 in ctor
    /* 0x249A */ SvFgName shirt; // 246E - 2471
    /* 0x249E */ SvFgName song; // 2472 - 2475
    /* 0x24A2 */ SvFgName wallpaper; // 2476 - 2479
    /* 0x24A6 */ SvFgName carpet; // 247A - 247D
    /* 0x24AA */ SvFgName umbrella; // 247E - 2481
    /* 0x24AE */ SvFgName furniture[0x10]; // 2482 - 24C1
    /* 0x24EE */ SvDate date1; // 24C2 - 24C5
    /* 0x24F2 */ char16 catchphrase[0xB]; // Last character is null terminator | 24C6 - 24DB
    /* 0x2508 */ u8 unk_0x2508; // ctor sets to 7
    /* 0x2509 */ u8 pad_0x2509;
    /* 0x250A */ SvDate date2; // This seems to be a date, maybe date last talked??
    /* 0x250E */ u8 unk_0x250E; // ctor sets to 2 / Also general flags? | 24E4
    /* 0x250F */ u8 unk_0x250F; // ctor sets to 0x18; 0x2 means they're at home
    /* 0x2510 */ SvNpcStatus status; // ctor sets to 0; bit 1 set: moving out, bit 2 set: moving in; bit 3 removed from all villagers when loading game, unk
    /* 0x2514 */ u8 unk_0x2514; // ctor sets to 0xFF
    /* 0x2515 */ u8 unk_0x2515; // ctor sets to 0xFF
    /* 0x2516 */ u8 unk_0x2516; // ctor sets to 0xFF
    /* 0x2517 */ u8 unk_0x2517; // ctor sets to 0
};
#pragma pack(pop)
ASSERT_SIZE(SvNpc, 0x2518);

struct SvNpcDataUnk1744CUnk0Unk9 {
    /* 0x0000.0 */ u8 unk_0x0_0 : 1;
    /* 0x0000.1 */ u8 unk_0x0_1 : 1;
    /* 0x0000.2 */ u8 unk_0x0_2 : 1;
    /* 0x0000.3 */ u8 unk_0x0_3 : 1;
    /* 0x0000.4 */ u8 unk_0x0_4 : 1;
    /* 0x0000.5 */ u8 unk_0x0_5 : 1;
    /* 0x0000.6 */ u8 unk_0x0_6 : 1;
    /* 0x0000.7 */ u8 unk_0x0_7 : 1;
};
ASSERT_SIZE(SvNpcDataUnk1744CUnk0Unk9, 0x1);

struct SvNpcDataUnk1744CUnk0 {
    /* 0x0000 */ SvU64 unk_0x0; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x0008 */ u8 unk_0x8;
    /* 0x0009 */ SvNpcDataUnk1744CUnk0Unk9 unk_0x9; // Time capsule? / 2 = Return Time Capsule | 9 = Time Capsule Returned
    /* 0x000A */ u8 unk_0xA[0x36];
};
ASSERT_SIZE(SvNpcDataUnk1744CUnk0, 0x40);

struct SvNpcDataUnk1744C {
    /* 0x0000 */ SvNpcDataUnk1744CUnk0 unk_0x0[2];
};
ASSERT_SIZE(SvNpcDataUnk1744C, 0x80);

// A time with three state bytes; the constructors set the time to "none" (SvTime()) and the bytes to 33, 0xFF, 9
// (names are ours; the meaning of the bytes is open)
#pragma pack(push, 1)
struct SvNpcDataUnk1764E {
    /* 0x0000 */ SvTime time;
    /* 0x0008 */ u8 unk_0x8; // 33
    /* 0x0009 */ u8 unk_0x9; // 0xFF
    /* 0x000A */ u8 unk_0xA; // 9
};
#pragma pack(pop)
ASSERT_SIZE(SvNpcDataUnk1764E, 0xB);

// One record per town (docs/census.md, value unk_0x550: maybe the sick villager; constructor 0x005BD5EC)
struct SvNpcDataUnk172F8 {
    /* 0x0000 */ SvTime unk_0x0;
    /* 0x0008 */ SvPlayerId unk_0x8[3];
    /* 0x004A */ SvPlayerId unk_0x4A[4];
    /* 0x00A2 */ SvDate unk_0xA2; // set to today (0x5BD354)
    /* 0x00A6 */ u8 unk_0xA6; // a villager index; 0xFF: none
    /* 0x00A7 */ u8 unk_0xA7;
    /* 0x00A8 */ s8 unk_0xA8; // -4 at the start; 0x5BD578 adds to it (range -4..40 or 60)
    /* 0x00A9 */ u8 unk_0xA9; // counted up to 3 by 0x5BD5C0
    /* 0x00AA */ u8 unk_0xAA; // flags (bit 4+i: entry i handled); 2 at the start
    /* 0x00AB */ u8 unk_0xAB; // state; 3 at the start
};
ASSERT_SIZE(SvNpcDataUnk172F8, 0xAC);

// constructor 0x00311418
struct SvNpcDataUnk173A4 {
    /* 0x0000 */ SvPlayerId unk_0x0;
    /* 0x0016 */ SvNpcDataUnk1764E unk_0x16;
    /* 0x0021 */ u8 unk_0x21; // 0xFF
    /* 0x0022 */ u8 unk_0x22;
    /* 0x0023 */ u8 unk_0x23;
    /* 0x0024 */ u8 unk_0x24;
    /* 0x0025 */ u8 unk_0x25;
};
ASSERT_SIZE(SvNpcDataUnk173A4, 0x26);

// constructor 0x005BC85C, cleared by 0x0030B260
struct SvNpcDataUnk173F0 {
    /* 0x0000 */ SvPlayerId unk_0x0;
    /* 0x0016 */ SvNpcRef unk_0x16;
    /* 0x0046 */ SvNpcDataUnk1764E unk_0x46; // unk_0x9 is 5 after the constructor
    /* 0x0051 */ u8 unk_0x51;
    /* 0x0052 */ SvFgName unk_0x52;
    /* 0x0056 */ u8 unk_0x56[3];
    /* 0x0059 */ u8 unk_0x59; // 0xFE
    /* 0x005A */ u8 unk_0x5A[2];
};
ASSERT_SIZE(SvNpcDataUnk173F0, 0x5C);

// A villager with 16 homes (like SvNpc::home) and a player (two of them in SvNpcData; the first villager id selects the
// music of the pavilion, see dStageOpusPavilion.cpp)
struct SvNpcDataUnk1767A {
    /* 0x0000 */ SvNpcRef unk_0x0;
    /* 0x0030 */ SvNpcHomeHistory unk_0x30[0x10];
    /* 0x0F50 */ u8 unk_0xF50[0x16];
    /* 0x0F66 */ SvPlayerId unk_0xF66;
};
ASSERT_SIZE(SvNpcDataUnk1767A, 0xF7C);

// constructor 0x001141E8 (Thumb)
struct SvNpcDataUnk19574 {
    /* 0x0000 */ u8 unk_0x0[8];
    /* 0x0008 */ SvDate unk_0x8;
};
ASSERT_SIZE(SvNpcDataUnk19574, 0xC);

struct SvNpcData {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvNpcData(); // 0x00112598 (Thumb)

    // the checksum of the data behind it (names are ours; isLogged: passed, not used)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x00119870 (Thumb)
    DECOMP_NOINLINE bool IsChecksumValid(bool isLogged); // 0x0011988C (Thumb)

    /* 0x0000 */ u32 checksum; // Checksum of the 0x22BC8 of this data
    /* 0x0004 */ SvNpc npcs[0xA];
    /* 0x172F4 */ u32 unk_0x172F4;
    // (the constructor 0x00111D5C builds npcs .. unk_0x196FE as one part: the original has a type for 0x0004..0x1976C)
    /* 0x172F8 */ SvNpcDataUnk172F8 unk_0x172F8;
    /* 0x173A4 */ SvNpcDataUnk173A4 unk_0x173A4;
    /* 0x173CA */ SvNpcDataUnk173A4 unk_0x173CA;
    /* 0x173F0 */ SvNpcDataUnk173F0 unk_0x173F0;
    /* 0x1744C */ SvNpcDataUnk1744C unk_0x1744C[4]; // for each player one struct
    /* 0x1764C */ u8 unk_0x1764C[2];
    /* 0x1764E */ SvNpcDataUnk1764E unk_0x1764E;
    /* 0x17659 */ u8 unk_0x17659[0x21];
    /* 0x1767A */ SvNpcDataUnk1767A unk_0x1767A; // (the research named its SvNpcRef parts townId1/townId2: campsite?)
    /* 0x185F6 */ SvNpcDataUnk1767A unk_0x185F6;
    /* 0x19572 */ u8 unk_0x19572[2];
    /* 0x19574 */ SvNpcDataUnk19574 unk_0x19574[0x10];
    /* 0x19634 */ SvNpcDataUnk19574 unk_0x19634[0x10];
    /* 0x196F4 */ u8 unk_0x196F4[2];
    /* 0x196F6 */ u16 unk_0x196F6[4]; // a year each, 2011 after the constructor (an element type with the constructor
                                     // 0x006137E4, like SvPlayer::unk_0x8F2A)
    /* 0x196FE */ u8 unk_0x196FE[0x6E];
    /* 0x1976C */ SvNpc unk_0x1976C[4];
    /* 0x22BCC */ u8 unk_0x22BCC[0x14];
};
ASSERT_SIZE(SvNpcData, 0x22BE0);

