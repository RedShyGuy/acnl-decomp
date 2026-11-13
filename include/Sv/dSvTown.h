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
#include "Sv/dSvNpcRef.h"
#include "Sv/dSvPersonalId.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvStrc.h"
#include "Sv/dSvTownFlags.h"
#include "script/dWordPtrSv.h"

namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

#pragma pack(push, 1)

struct SvBulletinBoardMessage {
    // (game code of the bulletin board, not decompiled yet) cleared
    SvBulletinBoardMessage(); // 0x003011F4 (Thumb)
    // (game code of the bulletin board, not decompiled yet; names are ours) the town name / the player name from a word
    // (NULL: cleared) / cleared, then used with a date / cleared / the bit 0x20 of usedFlag
    void Func1186E4(const script::IWord* town); // 0x001186E4 (Thumb)
    void Func118714(const script::IWord* name); // 0x00118714 (Thumb)
    void Func118750(const nn::fnd::DateTimeParameters* date); // 0x00118750 (Thumb)
    void Func118774(); // 0x00118774 (Thumb)
    void Func1187BC(); // 0x001187BC (Thumb)
    // the text as a word of the save data
    script::WordPtrSv GetMessage(); // 0x001187AA (Thumb)

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
    // (game code of the bulletin board, not decompiled yet; name is ours) a message is posted
    void Func302120(const SvBulletinBoardMessage* message); // 0x00302120 (Thumb)

    /* 0x0000 */ SvBulletinBoardMessage bBoardMessages[0xF];
};
ASSERT_SIZE(SvTownDataUnk1AE94, 0x1914);

// The Fishing Tourney of the town (the counterpart of SvInsectConvention; the binary has the namespace svfscnve for its
// classes, the class name is ours): the fish of the tourney, the three best places with a player or a villager each,
// the size points and the fish.
struct SvFishingConvention {
    // (functions in the shared area 0x2FF000-0x305000; names are ours, see the SvInsectConvention counterparts) the
    // daily update / a place is removed / a villager / a player enters / a villager catches a fish at random (two ways)
    // / the time passes / cleared / the tourney starts / a villager may take part / the prizes / the result notice
    void Func2FFF68(); // 0x002FFF68
    void Func300658(u32 place); // 0x00300658
    bool Func30096C(const SvNpcRef* npc, const SvFgName* fish, u16 points); // 0x0030096C
    void Func300CAC(const SvPersonalId* player, const SvFgName* fish, u16 points); // 0x00300CAC
    bool Func300E90(const SvNpcRef* npc); // 0x00300E90
    void Func301BDC(const SvNpcRef* npc, const nn::fnd::DateTimeParameters* date, bool isLimited); // 0x00301BDC
    void Func3022F8(bool a, bool b, bool c); // 0x003022F8
    void Func303000(bool isReset); // 0x00303000
    void Func30329C(); // 0x0030329C
    bool Func3039D0(s32 index, bool a, bool b, bool c); // 0x003039D0
    void Func303C6C(s32 value); // 0x00303C6C
    void Func30454C(); // 0x0030454C

    /* 0x0000 */ u8 unk_0x0; // bit 0: on; bit 3: a villager entered; bits 4-6: a random value 1..2 of the start
    /* 0x0001 */ u8 unk_0x1;
    /* 0x0002 */ SvFgName unk_0x2;   // the fish of the tourney (empty: any fish)
    /* 0x0006 */ SvTime unk_0x6;
    /* 0x000E */ SvTime unk_0xE;     // the start
    /* 0x0016 */ SvTime unk_0x16;    // the end
    /* 0x001E */ SvPersonalId unk_0x1E[3]; // the players of the places (or none)
    /* 0x00A8 */ SvNpcRef unk_0xA8[3];     // the villagers of the places (or none)
    /* 0x0138 */ u16 unk_0x138[3];         // the points of the places
    /* 0x013E */ SvFgName unk_0x13E[3];    // the fish of the places
};
ASSERT_SIZE(SvFishingConvention, 0x14A);

struct SvTownDataUnk1C7A8 {
    // (inline in the constructor of the save data)
    SvTownDataUnk1C7A8()
    {
        unk_0x1C.Func303000(true);
        unk_0x1C.unk_0x0 = 0;
    }

    /* 0x0000 */ SvTime unk_0x0;
    /* 0x0008 */ u8 unk_0x8[0x14];
    /* 0x001C */ SvFishingConvention unk_0x1C;
};
ASSERT_SIZE(SvTownDataUnk1C7A8, 0x166);

// The Bug-Off of the town (the class name is from the original file dSvInsectConvention.cpp; all other names are ours):
// the three best places are a player or a villager each, with the points and the insect.
struct SvInsectConvention {
    // (inline in the constructor of the save data)
    SvInsectConvention()
    {
        Func2EB558(true);
        unk_0x0 = 0;
    }

    // (game code of dSvInsectConvention.cpp; names are ours) a place is removed (the places below move up) / the
    // update with a value / a villager enters with points and an insect (false: not entered) / a player enters /
    // a villager catches an insect at random (two ways)
    // the daily update (before: from the start of the day of the Bug-Off on a reset of the Bug-Off of the last day)
    void Func2EA430(); // 0x002EA430
    void Func2EA5B0(u32 place); // 0x002EA5B0
    void Func2EA68C(u32 value); // 0x002EA68C
    bool Func2EA6D8(const SvNpcRef* npc, const SvFgName* item, u16 points); // 0x002EA6D8
    void Func2EA888(const SvPersonalId* player, const SvFgName* item, u16 points); // 0x002EA888
    bool Func2EAA1C(const SvNpcRef* npc); // 0x002EAA1C
    void Func2EACF4(const SvNpcRef* npc, const nn::fnd::DateTimeParameters* date, bool isLimited); // 0x002EACF4
    // the villager / the insect / the player of a place (place 0 for a place from 3)
    SvNpcRef* Func2EA860(s32 place); // 0x002EA860
    SvFgName* Func2EA874(s32 place); // 0x002EA874
    SvPersonalId* Func2EA9FC(s32 place); // 0x002EA9FC
    // the time passes: in steps of 3 minutes from unk_0x1 to now (until the end unk_0x11) villagers that may take part
    // catch insects (a: also before the first 30 minutes, b: the villager check of the first step, c: the points are
    // limited) / cleared (with the prize flags of the players) / the Bug-Off starts (on its day: start and end times,
    // a villager takes the places until all three are set) / a villager may take part (not for index >= 10; a: it must
    // have the state 1, b: unused, c: it must be an NPC of the stage that the KameyamaForeachFunction marks) / the
    // prizes: the players of the places get the prize flag, then (value > 0) the letters "Mail_SP_Kameyama" with the
    // prizes / the result notice on the bulletin board after the end
    void Func2EB0CC(bool a, bool b, bool c); // 0x002EB0CC
    void Func2EB558(bool isReset); // 0x002EB558
    void Func2EB680(); // 0x002EB680
    bool Func2EB97C(s32 index, bool a, bool b, bool c); // 0x002EB97C
    void Func2EBB9C(s32 value); // 0x002EBB9C
    void Func2EBDFC(); // 0x002EBDFC

    /* 0x0000 */ u8 unk_0x0; // bit 0: on; bit 3: a villager entered (Func2EACF4)
    /* 0x0001 */ SvTime unk_0x1;
    /* 0x0009 */ SvTime unk_0x9;
    /* 0x0011 */ SvTime unk_0x11;
    /* 0x0019 */ u8 unk_0x19;
    /* 0x001A */ SvPersonalId unk_0x1A[3]; // the players of the places (or none)
    /* 0x00A4 */ SvNpcRef unk_0xA4[3];     // the villagers of the places (or none)
    /* 0x0134 */ u16 unk_0x134[3];         // the points of the places
    /* 0x013A */ SvFgName unk_0x13A[3];    // the insects of the places
};
ASSERT_SIZE(SvInsectConvention, 0x146);

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

// The shop of the Nooklings (the class name is from the original file dSvShopZakka.cpp; "zakka" = general goods). All
// names but the research names are ours.
struct SvShopZakka {
    // (game code of dSvShopZakka.cpp; names are ours) the bells spent go up (at most to the value of the level) / an
    // item is bought: it becomes a sold-out sign on the field / the shop is renewed to a level (times, values, notices)
    // / one day passes (the values go down, a renewal is started) / the daily update with a flag / the notices are
    // posted on the bulletin board
    void Func1E5238(u32 value); // 0x001E5238
    void Func1E5280(u32 stage, u32 index, s32 x, s32 z, s32 unknown); // 0x001E5280
    void Func1E545C(u8 level); // 0x001E545C
    void Func1E57AC(s32 days); // 0x001E57AC
    void Func1E5A28(s32 days); // 0x001E5A28
    void Func1E5BBC(); // 0x001E5BBC
    // (Thumb part) the shop is cleared / it gets new items (with the notice of a featured item) / a seasonal rug in the
    // place 6 or wallpaper in the place 4: harvest (Harvest Festival), Jingle (Toy Day), spooky (Halloween)
    void Func109610(); // 0x00109610 (Thumb)
    void Func108C40(); // 0x00108C40 (Thumb)
    void Func1094FC(); // 0x001094FC (Thumb)
    void Func109558(); // 0x00109558 (Thumb)
    void Func1095B4(); // 0x001095B4 (Thumb)

    // (copies of the inline functions of the shop in the shared area; names are ours)
    // the night owl ordinance (a bit of the town flags; the shop is not read)
    bool Func759E24() const; // 0x00759E24
    // the hour of the time service (unk_0x8; 0: none)
    u32 Func71224C() const; // 0x0071224C
    // the days from unk_0x0 to today are 1 / -1 / 0 (false without a time)
    bool Func711C14() const; // 0x00711C14
    bool Func711C7C() const; // 0x00711C7C
    bool Func711D04() const; // 0x00711D04
    // the time service is on (unk_0x8 is valid and Func75A6C8 holds)
    bool Func711DE0() const; // 0x00711DE0
    // the first item that is not bought and has the value 9 of Func769264 (empty: none)
    SvFgName Func71228C() const; // 0x0071228C
    // the number of items of a group of places at the level (the row of s_NumItems; 0 from the level 5) / of the group of
    // the place 8 (always 1)
    template <u32 ROW>
    DECOMP_NOINLINE u8 GetNumItems() const
    {
        return nooklingState < 5 ? s_NumItems[ROW][nooklingState] : 0;
    }
    u8 GetNumItems8() const; // 0x00712284

    // the numbers of items per group of places and level (rows: the places 0, 2, 4, 6, 9, 11, 12, 13, 15, 17, 19, 21, 23
    // and the featured item)
    static const u8 s_NumItems[14][5];

    /* 0x0000 */ SvTime unk_0x0; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x0008 */ SvTime unk_0x8; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x0010 */ s32 unk_0x10; // a value per level (0x00890774), goes down (Func1E57AC)
    /* 0x0014 */ u8 nooklingState; // level of nookling shop
    /* 0x0015 */ u8 nooklingStateUnknown;
    /* 0x0016 */ u8 unk_0x16[2];
    /* 0x0018 */ SvEncValue nooklingBellsSpent;
    /* 0x0020 */ SvFgName nooklingsItems[0x18];
    /* 0x0080 */ u8 unk_0x80[0x18]; // per item: set to 1 when the item is bought (Func1E5280)
    /* 0x0098 */ s32 unk_0x98; // 15 on a renewal to level 0, one less per bought item
};
ASSERT_SIZE(SvShopZakka, 0x9C);

// The shops of the town (one object that the daily update and the loading get; names are ours). The Nooklings are the
// first one.
struct SvTownDataUnkEE2C {
    // (game code of the shops, not decompiled yet; names are ours) the daily update of all shops / after a load with
    // the save number / (Thumb) / (Thumb) with a value
    void Func5CE224(); // 0x005CE224
    void Func20D640(u32 saveNo); // 0x0020D640
    void Func10A2C8(); // 0x0010A2C8
    void Func1144A0(u32 value); // 0x001144A0 (Thumb)

    /* 0x0000 */ SvShopZakka shopZakka;
    /* 0x009C */ SvFgName ablesItems[5];
    /* 0x00B0 */ SvFgName ablesPatternItems[8];
    /* 0x00D0 */ SvEncValue unk_0xEEFC;
    /* 0x00D8 */ u8 unk_0xEF04[0x10]; // likely padding
    /* 0x00E8 */ SvTownDataUnkEF14 unk_0xEF14;
    /* 0x4468 */ SvFgName labellesItems[7]; // Accessories in right of shop
    /* 0x4484 */ u8 unk_0x132B0[8]; // likely padding
    /* 0x448C */ SvEncValue unk_0x132B8;
    // (research: scrappedString[4]; the shop reads the first word as a number, Func1E57AC)
    /* 0x4494 */ s32 unk_0x132C0;
    /* 0x4498 */ u8 unk_0x132C4[4];
    /* 0x449C */ SvEncValue unk_0x132C8;
    /* 0x44A4 */ u8 leifUnlockStatus; // 0 = locked; 1 = Being Built; X = Levels
    /* 0x44A5 */ u8 unk_0x132D1;
    /* 0x44A6 */ SvFgName leifItems[0xB];
    /* 0x44D2 */ u8 unk_0x132FE[0xE]; // likely padding
    /* 0x44E0 */ SvFgName reddItems[4];
    /* 0x44F0 */ u8 unk_0x1331C[4]; // likely padding
    /* 0x44F4 */ SvPersonalId unk_0x13320[4]; // unused?
    /* 0x45AC */ SvEncValue unk_0x133D8;
    /* 0x45B4 */ SvEncValue unk_0x133E0;
    /* 0x45BC */ SvEncValue unk_0x133E8;
    /* 0x45C4 */ SvEncValue unk_0x133F0;
    /* 0x45CC */ u8 unk_0x133F8[8];
    /* 0x45D4 */ SvEncValue unk_0x13400;
    /* 0x45DC */ u8 kickUnlockStatus; // 0 = locked; 1 = Being Built; 2 = Built/Unlocked
    /* 0x45DD */ u8 pad_0x13409;
    /* 0x45DE */ SvFgName kicksItems[6];
    /* 0x45F6 */ u8 unk_0x13422[6];
    /* 0x45FC */ SvFgName unk_0x13428[4]; // ctor does this and below separately, 4 at a time
    /* 0x460C */ SvFgName unk_0x13438[4];
    /* 0x461C */ u32 pad_0x13448;
    /* 0x4620 */ SvTime unk_0x1344C; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x4628 */ SvTime unk_0x13454; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x4630 */ SvTime unk_0x1345C; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x4638 */ SvTime unk_0x13464; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x4640 */ u8 unk_0x1346C[0x10]; // likely padding
    /* 0x4650 */ SvFgName reTailPremiumItems[5]; // if wealthy town, 2 items are shown, if people on streetpass are seen, even more
    /* 0x4664 */ SvFgName reTailItems[8];
    /* 0x4684 */ u8 reTailItemsPlayerIndex[8]; // Which player put the item up for sale, unsure what the values are for villagers
    /* 0x468C */ SvEncValue reTailItemPrices[8];
    /* 0x46CC */ SvMyDesign reTailItemDesign[8]; // if furniture was customized with a design, the design will be stored here if the item is put for sale
    /* 0x8A4C */ SvEncValue unk_0x17878;
    /* 0x8A54 */ SvEncValue unk_0x17880;
    /* 0x8A5C */ SvEncValue unk_0x17888;
    /* 0x8A64 */ SvEncValue unk_0x17890;
    /* 0x8A6C */ u8 museumShopUnlockState;
    /* 0x8A6D */ u8 unk_0x17899;
    /* 0x8A6E */ SvFgName museumItems[3];
    /* 0x8A7A */ u8 unk_0x178A6[4]; // likely padding
    /* 0x8A7E */ SvFgName nooksHomeItems[8];
    /* 0x8A9E */ u8 unk_0x178CA[8]; // likely padding
    /* 0x8AA6 */ u8 gracieUnlockStatus; // 0 = locked??; 1 = Unlocked??
    /* 0x8AA7 */ u8 pad_0x178D3;
    /* 0x8AA8 */ SvFgName gracieItems[0x12];
    /* 0x8AF0 */ u8 unk_0x1791C[0x12]; // likely padding
    /* 0x8B02 */ SvMannequin gracieMannequin1;
    /* 0x8B1A */ SvMannequin gracieMannequin2;
    /* 0x8B32 */ u8 clubLolUnlockState;
    /* 0x8B33 */ u8 unk_0x1795F[0xF];
    /* 0x8B42 */ SvFgName clubLolGyroids[4];
    /* 0x8B52 */ u8 dreamSuiteUnlockStatus;
    /* 0x8B53 */ u8 unk_0x1797F;
    /* 0x8B54 */ u8 fortuneTellerUnlockStatus;
    /* 0x8B55 */ u8 unk_0x17981[0xF];
    /* 0x8B64 */ u8 shampoodleUnlockStatus;
    /* 0x8B65 */ u8 unk_0x17991[3];
    /* 0x8B68 */ SvFgName islandShopItems[4];
    /* 0x8B78 */ SvEncValue unk_0x179A4;
    /* 0x8B80 */ SvEncValue unk_0x179AC[2];
    /* 0x8B90 */ SvEncValue turnipPrices[0xC]; // first 6 are AM, second 6 are PM
    /* 0x8BF0 */ u8 unk_0x17A1C[8];
    /* 0x8BF8 */ SvFgName unk_0x17A24;
    /* 0x8BFC */ SvFgName unk_0x17A28[2]; // new year hats?
    /* 0x8C04 */ SvFgName campgroundShopItems[2];
    /* 0x8C0C */ u8 unk_0x17A38[4];
    /* 0x8C10 */ u16 campgroundCaravan[2]; // left and right caravan (uses NPC VID); 0xFFFF if empty
    /* 0x8C14 */ u8 unk_0x17A40[8];
    /* 0x8C1C */ SvDate unk_0x17A48;
    /* 0x8C20 */ SvDate unk_0x17A4C;
    /* 0x8C24 */ u8 unk_0x17A50[0x44];
};
ASSERT_SIZE(SvTownDataUnkEE2C, 0x8C68);

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
    /* 0xEE2C */ SvTownDataUnkEE2C unk_0xEE2C;
    /* 0x17A94 */ SvMuseum museum;
    /* 0x1AE50 */ SvTownDataUnk1AE50 unk_0x1AE50;
    /* 0x1AE6E */ SvTime unk_0x1AE6E; // ctor sets 0x7FFFFFFFFFFFFFFF (none; the constructor sets it like a time, 0x005CEA18)
    /* 0x1AE76 */ u8 unk_0x1AE76[0xA];
    /* 0x1AE80 */ SvPwpProject nextPwpToBuild;
    /* 0x1AE90 */ SvStrc unk_0x1AE90;
    /* 0x1AE94 */ SvTownDataUnk1AE94 unk_0x1AE94;
    /* 0x1C7A8 */ SvTownDataUnk1C7A8 unk_0x1C7A8;
    /* 0x1C90E */ SvInsectConvention unk_0x1C90E;
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
