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
    /* 0x0000 */ SvTime dayOfDeath; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x0008 */ u8 bingoNumberOfTheDay; // Will be set when talking to the snowman
    /* 0x0009 */ u8 unk_0x9; // Seems to change when playing bingo?
    /* 0x000A */ u8 topSnowBallSize;
    /* 0x000B */ u8 bottomSnowBallSize;
};
ASSERT_SIZE(SvSnowman, 0xC);

struct SvTownData {
    /* 0x0000 */ u32 checksum; // Checksum of the 0x1E4D8 of this data
    /* 0x0004 */ u8 unk_0x4[0x58];
    /* 0x005C */ u8 oceanSide; // [0-1]; 0: Left, 1: Right; Museum and Camping Ground and the starting position of the train driving by are all on the opposite side to this.; Ctor sets to 3
    /* 0x005D */ u8 townGrassType; // [0-2]; 0: Triangle / Square (Winter) | 1: Circle / Star (Winter) | 2: Square / Circle (Winter); Ctor sets to 2
    /* 0x005E */ u8 cliffType; // [0-2]; This is unused/scrapped. All 3 cliff texture in the ROM are the exact same.; Ctor sets to 2
    /* 0x005F */ u8 pad_0x5F;
    /* 0x0060 */ u16 townAcres[0x2A]; // 42 acres in total; 7 columns, 6 rows. Game reads Acre IDs as u16; ctor sets each to 265 (0x109)
    /* 0x00B4 */ SvFgName townItems[0x1400]; // 16*16 items per acre; Items only cover map acres (5*4); 0x1400 of items
    /* 0x50B4 */ u8 mapGrassToday[0x1400]; // 16*16 slots per acre; Grass deterioration only affects map acres (5*4); 0x1400 of grass
    /* 0x64B4 */ u8 unk_0x64B4[0x28]; // Town data ctor never initializes this, so likely not used. Where the code would branch to do so, there is a NOP (both in WA and Orig).
    /* 0x64DC */ u8 mapGrass[0x3000];
    /* 0x94DC */ u8 unk_0x94DC[0x1000]; // Town data actually includes it with MapGrass (0x8000 in total), despite this portion not being used
    /* 0xA4DC */ SvPlayerHouse playerHouse[4];
    /* 0xED7C */ s64 currentTime;
    /* 0xED84 */ s64 unk_0xED84;
    /* 0xED8C */ s64 playtime;
    /* 0xED94 */ SvTownId townData1;
    /* 0xEDAA */ u8 unk_0xEDAA;
    /* 0xEDAB */ SvTownFlags townFlags;
    /* 0xEDBC */ u16 unk_0xEDBC; // Likely padding
    /* 0xEDBE */ SvFgName lostAndFoundItems[0x10];
    /* 0xEDFE */ u8 unk_0xEDFE[0x10]; // likely padding
    /* 0xEE0E */ u64 unk_0xEE0E; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0xEE16 */ SvFgName townFruit;
    /* 0xEE1A */ u16 daysPlayed;
    /* 0xEE1C */ u8 unk_0xEE1C[0xC];
    /* 0xEE28 */ u8 unk_0xEE28; // Group 1
    /* 0xEE29 */ u8 unk_0xEE29; // Group 1
    /* 0xEE2A */ u8 unk_0xEE2A; // Group 1
    /* 0xEE2B */ u8 pad_0xEE2B;
    /* 0xEE2C */ u64 unk_0xEE2C; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0xEE34 */ u64 unk_0xEE34; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
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
    /* 0xEF14 */ SvMyDesign ableDisplayPattern[8];
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
    /* 0x1344C */ u64 unk_0x1344C; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x13454 */ u64 unk_0x13454; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1345C */ u64 unk_0x1345C; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x13464 */ u64 unk_0x13464; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
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
    /* 0x17A94 */ SvDate museumDonationDates[0x112];
    /* 0x17EDC */ u8 museumDonations[0x112];
    /* 0x17FEE */ u8 pad_0x17FEE[2];
    /* 0x17FF0 */ SvMuseumExhibit exhibit[4];
    /* 0x1AE50 */ u64 unk_0x1AE50; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1AE58 */ u8 unk_0x1AE58[7];
    /* 0x1AE5F */ u64 unk_0x1AE5F; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1AE67 */ u8 unk_0x1AE67[7];
    /* 0x1AE6E */ u64 unk_0x1AE6E; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1AE76 */ u8 unk_0x1AE76[0xA];
    /* 0x1AE80 */ SvPwpProject nextPwpToBuild;
    /* 0x1AE90 */ SvStrc unk_0x1AE90;
    /* 0x1AE94 */ SvBulletinBoardMessage bBoardMessages[0xF];
    /* 0x1C7A8 */ u64 unk_0x1C7A8; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1C7B0 */ u8 unk_0x1C7B0[0x14];
    /* 0x1C7C4 */ u16 unk_0x1C7C4;
    /* 0x1C7C6 */ SvFgName unk_0x1C7C6;
    /* 0x1C7CA */ u64 unk_0x1C7CA; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1C7D2 */ u64 unk_0x1C7D2; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1C7DA */ u64 unk_0x1C7DA; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    /* 0x1C7E2 */ SvPersonalId unk_0x1C7E2[3];
    /* 0x1C86C */ u8 unk_0x1C86C[0x96];
    /* 0x1C902 */ SvFgName unk_0x1C902[3];
    /* 0x1C90E */ u8 unk_0x1C90E[0x146];
    /* 0x1CA54 */ SvSnowman snowmen[4];
    /* 0x1CA84 */ u8 unk_0x1CA84[0xE];
    /* 0x1CA92 */ u8 islandGrassType; // [0-2]; 0: Triangle | 1: Circle | 2: Square;
    /* 0x1CA93 */ u8 pad_0x1CA93;
    /* 0x1CA94 */ u16 islandAcres[0x10]; // 16 acres in total; 4 columns, 4 rows. Game reads Acre IDs as u16;
    /* 0x1CAB4 */ SvFgName islandItems[0x400]; // 16*16 items per acre; Items only cover map acres (2*2); 0x400 of items
    /* 0x1DAB4 */ SvStrc islandBuildings[2]; // Island Hut and Lloid
    /* 0x1DABC */ SvMyDesign townFlag;
    /* 0x1E32C */ u8 unk_0x1E32C[0x1B0];
};
ASSERT_SIZE(SvTownData, 0x1E4DC);

#pragma pack(pop)
