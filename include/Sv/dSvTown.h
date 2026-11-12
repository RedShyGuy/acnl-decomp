#pragma once

// Town data: map, shops, museum, bulletin board, island.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMuseum.h"
#include "Sv/dSvMyDesign.h"
#include "Sv/dSvPersonalId.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvStrc.h"
#include "Sv/dSvTownFlags.h"

#pragma pack(push, 1)

struct SvBulletinBoardMessage {
    /* 0x0000 */ char16 playerName[9]; // Only set when player writes the message
    /* 0x0012 */ char16 townName[9]; // Only set when player from other town writes the message
    /* 0x0024 */ char16 message[0xC1];
    /* 0x01A6 */ SvDate messageDate;
    /* 0x01AA */ u16 usedFlag; // if message is written
};
ASSERT_SIZE(SvBulletinBoardMessage, 0x1AC);

// When a snowmen is built, 0x7FF8 is placed as a town item
struct SvSnowman {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvSnowman(); // 0x006F5E8C

    /* 0x0000 */ SvTime dayOfDeath; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x0008 */ u8 bingoNumberOfTheDay; // Will be set when talking to the snowman
    /* 0x0009 */ u8 unk_0x9; // Seems to change when playing bingo?
    /* 0x000A */ u8 topSnowBallSize;
    /* 0x000B */ u8 bottomSnowBallSize;
};
ASSERT_SIZE(SvSnowman, 0xC);

// The part of the town data from 0x5C on: the map (built by its own constructor; the type and the member name are
// ours, the field names after the research).
struct SvTownDataUnk5C {
    SvTownDataUnk5C(); // 0x0061423C (game code, not decompiled yet)

    /* 0x0000 */ u8 oceanSide; // [0-1]; 0: Left, 1: Right; Museum and Camping Ground and the starting position of the train driving by are all on the opposite side to this.; Ctor sets to 3
    /* 0x0001 */ u8 townGrassType; // [0-2]; 0: Triangle / Square (Winter) | 1: Circle / Star (Winter) | 2: Square / Circle (Winter); Ctor sets to 2
    /* 0x0002 */ u8 cliffType; // [0-2]; This is unused/scrapped. All 3 cliff texture in the ROM are the exact same.; Ctor sets to 2
    /* 0x0003 */ u8 pad_0x3;
    /* 0x0004 */ u16 townAcres[0x2A]; // 42 acres in total; 7 columns, 6 rows. Game reads Acre IDs as u16; ctor sets each to 265 (0x109)
    /* 0x0058 */ SvFgName townItems[0x1400]; // 16*16 items per acre; Items only cover map acres (5*4); 0x1400 of items
    /* 0x5058 */ u8 mapGrassToday[0x1400]; // 16*16 slots per acre; Grass deterioration only affects map acres
    /* 0x6458 */ u8 unk_0x6458[0x28]; // Town data ctor never initializes this, so likely not used.
    /* 0x6480 */ u8 mapGrass[0x3000];
    /* 0x9480 */ u8 unk_0x9480[0x1000]; // Town data actually includes it with MapGrass (0x8000 in total)
};
ASSERT_SIZE(SvTownDataUnk5C, 0xA480);

// (types and member names ours) parts of the town data that are built by their own constructors (game code, not
// decompiled yet)
struct SvTownDataUnkEDBE {
    SvTownDataUnkEDBE(); // 0x001B2968

    /* 0x0000 */ SvFgName lostAndFoundItems[0x10];
};
ASSERT_SIZE(SvTownDataUnkEDBE, 0x40);

struct SvTownDataUnkEE28 {
    // (inline) Init; it is also called when the town data is cleared (game code, not decompiled yet; name is ours)
    SvTownDataUnkEE28() { Init(); }
    void Init(); // 0x001B20E8

    /* 0x0000 */ u8 unk_0x0; // Group 1
    /* 0x0001 */ u8 unk_0x1; // Group 1
    /* 0x0002 */ u8 unk_0x2; // Group 1
    /* 0x0003 */ u8 pad_0x3;
};
ASSERT_SIZE(SvTownDataUnkEE28, 0x4);

struct SvTownDataUnkEF14 {
    SvTownDataUnkEF14(); // 0x001E0738

    /* 0x0000 */ SvMyDesign ableDisplayPattern[8];
};
ASSERT_SIZE(SvTownDataUnkEF14, 0x4380);

struct SvTownDataUnk1AE50 {
    SvTownDataUnk1AE50(); // 0x00614130

    /* 0x0000 */ SvTime unk_0x0;
    /* 0x0008 */ u8 unk_0x8[7];
    /* 0x000F */ SvTime unk_0xF;
    /* 0x0017 */ u8 unk_0x17[7];
};
ASSERT_SIZE(SvTownDataUnk1AE50, 0x1E);

struct SvTownDataUnk1AE94 {
    SvTownDataUnk1AE94(); // 0x00108430 (Thumb)

    /* 0x0000 */ SvBulletinBoardMessage bBoardMessages[0xF];
};
ASSERT_SIZE(SvTownDataUnk1AE94, 0x1914);

// (game code, not decompiled yet; the constructors of the next two parts end with them, the clear of the town data calls
// them too; names after the addresses)
void Func303000(u8* data, u32 value); // 0x00303000
void Func2EB558(u8* data, u32 value); // 0x002EB558

struct SvTownDataUnk1C7A8 {
    // (inline in the constructor of the save data)
    SvTownDataUnk1C7A8()
    {
        Func303000(&unk_0x1C, 1);
        unk_0x1C = 0;
    }

    /* 0x0000 */ SvTime unk_0x0;
    /* 0x0008 */ u8 unk_0x8[0x14];
    /* 0x001C */ u8 unk_0x1C;
    /* 0x001D */ u8 unk_0x1D;
    /* 0x001E */ SvFgName unk_0x1E;
    /* 0x0022 */ SvTime unk_0x22;
    /* 0x002A */ SvTime unk_0x2A;
    /* 0x0032 */ SvTime unk_0x32;
    /* 0x003A */ SvPersonalId unk_0x3A[3];
    /* 0x00C4 */ u8 unk_0xC4[0x96];
    /* 0x015A */ SvFgName unk_0x15A[3];
};
ASSERT_SIZE(SvTownDataUnk1C7A8, 0x166);

struct SvTownDataUnk1C90E {
    // (inline in the constructor of the save data)
    SvTownDataUnk1C90E()
    {
        Func2EB558(&unk_0x0, 1);
        unk_0x0 = 0;
    }

    /* 0x0000 */ u8 unk_0x0;
    /* 0x0001 */ SvTime unk_0x1;
    /* 0x0009 */ SvTime unk_0x9;
    /* 0x0011 */ SvTime unk_0x11;
    /* 0x0019 */ u8 unk_0x19;
    /* 0x001A */ SvPersonalId unk_0x1A[3];
    /* 0x00A4 */ u8 unk_0xA4[0x96];
    /* 0x013A */ SvFgName unk_0x13A[3];
};
ASSERT_SIZE(SvTownDataUnk1C90E, 0x146);

struct SvTownDataUnk1CA92 {
    SvTownDataUnk1CA92(); // 0x002EC2B0

    /* 0x0000 */ u8 islandGrassType; // [0-2]; 0: Triangle | 1: Circle | 2: Square;
    /* 0x0001 */ u8 pad_0x1;
    /* 0x0002 */ u16 islandAcres[0x10]; // 16 acres in total; 4 columns, 4 rows. Game reads Acre IDs as u16;
    /* 0x0022 */ SvFgName islandItems[0x400]; // 16*16 items per acre; Items only cover map acres (2*2); 0x400 of items
    /* 0x1022 */ SvStrc islandBuildings[2]; // Island Hut and Lloid
};
ASSERT_SIZE(SvTownDataUnk1CA92, 0x102A);

struct SvTownDataUnk1E368 {
    SvTownDataUnk1E368(); // 0x001B6BD4

    /* 0x0000 */ u8 unk_0x0[0xD4];
};
ASSERT_SIZE(SvTownDataUnk1E368, 0xD4);

struct SvTownDataUnk1E448 {
    SvTownDataUnk1E448(); // 0x0024AB04

    /* 0x0000 */ u8 unk_0x0[0x14];
    /* 0x0014 */ u32 unk_0x14[6]; // (cleared with the town data)
    /* 0x002C */ u8 unk_0x2C[0x68];
};
ASSERT_SIZE(SvTownDataUnk1E448, 0x94);

struct SvTownData {
    // (inline in the constructor of the save data: the members and these values)
    DECOMP_ALWAYS_INLINE SvTownData() : unk_0x1DABC(0), unk_0x1DAC6(0), unk_0x1DAC7(0), unk_0x1DAC8(0) {}

    /* 0x0000 */ u32 checksum; // Checksum of the 0x1E4D8 of this data
    /* 0x0004 */ u8 unk_0x4[0x58];
    /* 0x005C */ SvTownDataUnk5C unk_0x5C;
    /* 0xA4DC */ SvPlayerHouseList playerHouses;
    /* 0xED7C */ s64 currentTime;
    /* 0xED84 */ s64 unk_0xED84;
    /* 0xED8C */ s64 playtime;
    /* 0xED94 */ SvTownId townData1;
    /* 0xEDAA */ u8 unk_0xEDAA; // low four bits: the region of the game (event::Region), from the console
    /* 0xEDAB */ SvTownFlags townFlags;
    /* 0xEDBD */ u8 pad_0xEDBD;
    /* 0xEDBE */ SvTownDataUnkEDBE unk_0xEDBE;
    /* 0xEDFE */ u8 unk_0xEDFE[0x10]; // likely padding
    /* 0xEE0E */ SvTime unk_0xEE0E; // ctor sets 0x7FFFFFFFFFFFFFFF (none); time::GetSavedTime prefers it
    /* 0xEE16 */ SvFgName townFruit;
    /* 0xEE1A */ u16 daysPlayed;
    /* 0xEE1C */ u8 unk_0xEE1C[0xC];
    /* 0xEE28 */ SvTownDataUnkEE28 unk_0xEE28;
    /* 0xEE2C */ SvTime unk_0xEE2C; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0xEE34 */ SvTime unk_0xEE34; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0xEE3C */ u8 unk_0xEE3C[4];
    /* 0xEE40 */ u8 nooklingState; // level of nookling shop
    /* 0xEE41 */ u8 nooklingStateUnknown;
    /* 0xEE42 */ u8 unk_0xEE42[2];
    /* 0xEE44 */ SvEncValue nooklingBellsSpent;
    /* 0xEE4C */ SvFgName nooklingsItems[0x18];
    /* 0xEEAC */ u8 unk_0xEEAC[0x1C]; // likely padding
    /* 0xEEC8 */ SvFgName ablesItems[5];
    /* 0xEEDC */ SvFgName ablesPatternItems[8];
    /* 0xEEFC */ SvEncValue unk_0xEEFC;
    /* 0xEF04 */ u8 unk_0xEF04[0x10]; // likely padding
    /* 0xEF14 */ SvTownDataUnkEF14 unk_0xEF14;
    /* 0x13294 */ SvFgName labellesItems[7]; // Accessories in right of shop
    /* 0x132B0 */ u8 unk_0x132B0[8]; // likely padding
    /* 0x132B8 */ SvEncValue unk_0x132B8;
    /* 0x132C0 */ char16 scrappedString[4];
    /* 0x132C8 */ SvEncValue unk_0x132C8;
    /* 0x132D0 */ u8 leifUnlockStatus; // 0 = locked; 1 = Being Built; X = Levels
    /* 0x132D1 */ u8 unk_0x132D1;
    /* 0x132D2 */ SvFgName leifItems[0xB];
    /* 0x132FE */ u8 unk_0x132FE[0xE]; // likely padding
    /* 0x1330C */ SvFgName reddItems[4];
    /* 0x1331C */ u8 unk_0x1331C[4]; // likely padding
    /* 0x13320 */ SvPersonalId unk_0x13320[4]; // unused?
    /* 0x133D8 */ SvEncValue unk_0x133D8;
    /* 0x133E0 */ SvEncValue unk_0x133E0;
    /* 0x133E8 */ SvEncValue unk_0x133E8;
    /* 0x133F0 */ SvEncValue unk_0x133F0;
    /* 0x133F8 */ u8 unk_0x133F8[8];
    /* 0x13400 */ SvEncValue unk_0x13400;
    /* 0x13408 */ u8 kickUnlockStatus; // 0 = locked; 1 = Being Built; 2 = Built/Unlocked
    /* 0x13409 */ u8 pad_0x13409;
    /* 0x1340A */ SvFgName kicksItems[6];
    /* 0x13422 */ u8 unk_0x13422[6];
    /* 0x13428 */ SvFgName unk_0x13428[4]; // ctor does this and below separately, 4 at a time
    /* 0x13438 */ SvFgName unk_0x13438[4];
    /* 0x13448 */ u32 pad_0x13448;
    /* 0x1344C */ SvTime unk_0x1344C; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x13454 */ SvTime unk_0x13454; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x1345C */ SvTime unk_0x1345C; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x13464 */ SvTime unk_0x13464; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x1346C */ u8 unk_0x1346C[0x10]; // likely padding
    /* 0x1347C */ SvFgName reTailPremiumItems[5]; // if wealthy town, 2 items are shown, if people on streetpass are seen, even more
    /* 0x13490 */ SvFgName reTailItems[8];
    /* 0x134B0 */ u8 reTailItemsPlayerIndex[8]; // Which player put the item up for sale, unsure what the values are for villagers
    /* 0x134B8 */ SvEncValue reTailItemPrices[8];
    /* 0x134F8 */ SvMyDesign reTailItemDesign[8]; // if furniture was customized with a design, the design will be stored here if the item is put for sale
    /* 0x17878 */ SvEncValue unk_0x17878;
    /* 0x17880 */ SvEncValue unk_0x17880;
    /* 0x17888 */ SvEncValue unk_0x17888;
    /* 0x17890 */ SvEncValue unk_0x17890;
    /* 0x17898 */ u8 museumShopUnlockState;
    /* 0x17899 */ u8 unk_0x17899;
    /* 0x1789A */ SvFgName museumItems[3];
    /* 0x178A6 */ u8 unk_0x178A6[4]; // likely padding
    /* 0x178AA */ SvFgName nooksHomeItems[8];
    /* 0x178CA */ u8 unk_0x178CA[8]; // likely padding
    /* 0x178D2 */ u8 gracieUnlockStatus; // 0 = locked??; 1 = Unlocked??
    /* 0x178D3 */ u8 pad_0x178D3;
    /* 0x178D4 */ SvFgName gracieItems[0x12];
    /* 0x1791C */ u8 unk_0x1791C[0x12]; // likely padding
    /* 0x1792E */ SvMannequin gracieMannequin1;
    /* 0x17946 */ SvMannequin gracieMannequin2;
    /* 0x1795E */ u8 clubLolUnlockState;
    /* 0x1795F */ u8 unk_0x1795F[0xF];
    /* 0x1796E */ SvFgName clubLolGyroids[4];
    /* 0x1797E */ u8 dreamSuiteUnlockStatus;
    /* 0x1797F */ u8 unk_0x1797F;
    /* 0x17980 */ u8 fortuneTellerUnlockStatus;
    /* 0x17981 */ u8 unk_0x17981[0xF];
    /* 0x17990 */ u8 shampoodleUnlockStatus;
    /* 0x17991 */ u8 unk_0x17991[3];
    /* 0x17994 */ SvFgName islandShopItems[4];
    /* 0x179A4 */ SvEncValue unk_0x179A4;
    /* 0x179AC */ SvEncValue unk_0x179AC[2];
    /* 0x179BC */ SvEncValue turnipPrices[0xC]; // first 6 are AM, second 6 are PM
    /* 0x17A1C */ u8 unk_0x17A1C[8];
    /* 0x17A24 */ SvFgName unk_0x17A24;
    /* 0x17A28 */ SvFgName unk_0x17A28[2]; // new year hats?
    /* 0x17A30 */ SvFgName campgroundShopItems[2];
    /* 0x17A38 */ u8 unk_0x17A38[4];
    /* 0x17A3C */ u16 campgroundCaravan[2]; // left and right caravan (uses NPC VID); 0xFFFF if empty
    /* 0x17A40 */ u8 unk_0x17A40[8];
    /* 0x17A48 */ SvDate unk_0x17A48;
    /* 0x17A4C */ SvDate unk_0x17A4C;
    /* 0x17A50 */ u8 unk_0x17A50[0x44];
    /* 0x17A94 */ SvMuseum museum;
    /* 0x1AE50 */ SvTownDataUnk1AE50 unk_0x1AE50;
    /* 0x1AE6E */ SvTime unk_0x1AE6E; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x1AE76 */ u8 unk_0x1AE76[0xA];
    /* 0x1AE80 */ SvPwpProject nextPwpToBuild;
    /* 0x1AE90 */ SvStrc unk_0x1AE90;
    /* 0x1AE94 */ SvTownDataUnk1AE94 unk_0x1AE94;
    /* 0x1C7A8 */ SvTownDataUnk1C7A8 unk_0x1C7A8;
    /* 0x1C90E */ SvTownDataUnk1C90E unk_0x1C90E;
    /* 0x1CA54 */ SvSnowman snowmen[4];
    /* 0x1CA84 */ SvTime unk_0x1CA84;
    /* 0x1CA8C */ u8 unk_0x1CA8C[6];
    /* 0x1CA92 */ SvTownDataUnk1CA92 unk_0x1CA92;
    /* 0x1DABC */ u16 unk_0x1DABC;
    /* 0x1DABE */ SvTime unk_0x1DABE; // set to the current time when the town data is cleared
    /* 0x1DAC6 */ u8 unk_0x1DAC6; // high four bits: a counter up to 10 (counted up after a load)
    /* 0x1DAC7 */ u8 unk_0x1DAC7; // bits 0 and 2 are read by netgame::JoinMgr
    /* 0x1DAC8 */ u8 unk_0x1DAC8;
    /* 0x1DAC9 */ u8 pad_0x1DAC9;
    /* 0x1DACA */ SvPersonalId unk_0x1DACA;
    /* 0x1DAF8 */ SvMyDesign townFlag; // the design of the town flag (cleared with SvMyDesign's clear 0x00305B40)
    /* 0x1E368 */ SvTownDataUnk1E368 unk_0x1E368;
    /* 0x1E43C */ SvTime unk_0x1E43C; // (read after a load: set as the current date when valid)
    /* 0x1E444 */ SvDate unk_0x1E444;
    /* 0x1E448 */ SvTownDataUnk1E448 unk_0x1E448;
};
ASSERT_SIZE(SvTownData, 0x1E4DC);

#pragma pack(pop)
