#pragma once

// One player of the town (4 per save).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include <cstring>

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPlayerInventory.h"
#include "Sv/dSvInitiative.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvMyDesign.h"
#include "Sv/dSvNpcRef.h"
#include "Sv/dSvPersonalId.h"
#include "Sv/dSvPlayerFlags.h"

#pragma pack(push, 1)

struct SvEmoticons {
    /* 0x0000 */ u8 emoticons[0x28];
};
ASSERT_SIZE(SvEmoticons, 0x28);

struct SvMannequin {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvMannequin(); // 0x001DFA80

    /* 0x0000 */ SvFgName hat; // Item ID < 0xXXXX
    /* 0x0004 */ SvFgName accessory; // Item ID < 0xXXXX
    /* 0x0008 */ SvFgName topWear; // Item ID < 0xXXXX
    /* 0x000C */ SvFgName bottomWear; // Item ID < 0xXXXX
    /* 0x0010 */ SvFgName socks; // Item ID < 0xXXXX
    /* 0x0014 */ SvFgName shoes; // Item ID < 0xXXXX
};
ASSERT_SIZE(SvMannequin, 0x18);

struct SvPlayerBadges {
    /* 0x0000 */ SvEncValue badgeValues[0x18]; // 24 badges
    /* 0x00C0 */ u8 badges[0x18]; // 24 badges
    /* 0x00D8 */ SvEncValue unk_0xD8;
    /* 0x00E0 */ SvEncValue unk_0xE0;
};
ASSERT_SIZE(SvPlayerBadges, 0xE8);

struct SvHhaEvaluation {
    /* 0x0000 */ s32 hhaHousePoints;
    /* 0x0004 */ u16 hhaItem1; // Only used if no theme
    /* 0x0006 */ u16 hhaItem2; // Only used if no theme
    /* 0x0008 */ u16 hhaItem3; // Only used if no theme
    /* 0x000A */ u16 hhaItem4; // Only used if no theme
    /* 0x000C */ u16 hhaItem5; // Only used if no theme
    /* 0x000E */ u16 itemExterior; // Only used if you have a theme
    /* 0x0010 */ u16 itemInterior; // Only used if you have a theme
    /* 0x0012 */ u16 hhaItem7; // Only used if you have a theme
    /* 0x0014 */ u8 currentHouseTheme;
    /* 0x0015 */ u8 evaluationType;
    /* 0x0016 */ u8 houseUnk0; // Valid values: 0, 1, 2, 3
    /* 0x0017 */ u8 houseUnk1;
    /* 0x0018 */ u8 houseUnk2; // Read if HouseUnk0==3
    /* 0x0019 */ u8 houseUnk3;
    /* 0x001A */ u8 houseUnk4;
    /* 0x001B */ u8 houseUnk5;
    /* 0x001C */ u8 houseUnk6;
    /* 0x001D */ u8 houseUnk7;
    /* 0x001E */ u8 houseUnk8;
    /* 0x001F */ u8 houseUnk9;
    /* 0x0020 */ u8 houseUnk10;
    /* 0x0021 */ u8 houseExteriorObeyingTheme; // Not Verified; 5 is max, 0 = Not obeying(?); higher better(?)
    /* 0x0022 */ u8 houseInteriorObeyingTheme; // Not Verified; 5 is max, 0 = Not obeying(?); higher better(?)
    /* 0x0023 */ u8 whichFloorWasImpressive; // Not Verified; 5 is max
    /* 0x0024 */ u8 houseUnk14;
    /* 0x0025 */ u8 houseUnk15;
    /* 0x0026 */ u8 futureAdvice; // Values: 0 - 10
    /* 0x0027 */ u8 hhaAwardsUnlocked; // Not Verified; 8 is max
    /* 0x0028 */ u8 hhaAwardsReceived; // Not Verified; 8 is max; 0 = None
    /* 0x0029 */ u8 goldExteriorsUnlocked; // 5 is max
    /* 0x002A */ u8 goldExteriorsApplied; // Not Verified; 5 is max
    /* 0x002B */ u8 houseUnk21;
};
ASSERT_SIZE(SvHhaEvaluation, 0x2C);

struct SvDreamAddress {
    /* 0x0000 */ u32 dcPart1;
    /* 0x0004 */ u32 dcPart2; // Code checks it's less than 1, aka 0
    /* 0x0008 */ bool hasDreamAddress;
    /* 0x0009 */ u8 dcPart3;
    /* 0x000A */ u16 pad_0xA;
};
ASSERT_SIZE(SvDreamAddress, 0xC);

// Starting Point, Top Left
// Naming Convention: Field_X_Y -> X = Row, Y = Column
struct SvSnowmanBingoCard {
    /* 0x0000.0 */ u8 field00 : 1;
    /* 0x0000.1 */ u8 field01 : 1;
    /* 0x0000.2 */ u8 field02 : 1;
    /* 0x0000.3 */ u8 field03 : 1;
    /* 0x0000.4 */ u8 field04 : 1;
    /* 0x0000.5 */ u8 field10 : 1;
    /* 0x0000.6 */ u8 field11 : 1;
    /* 0x0000.7 */ u8 field12 : 1;
    /* 0x0001.0 */ u8 field13 : 1;
    /* 0x0001.1 */ u8 field14 : 1;
    /* 0x0001.2 */ u8 field20 : 1;
    /* 0x0001.3 */ u8 field21 : 1;
    /* 0x0001.4 */ u8 field22 : 1;
    /* 0x0001.5 */ u8 field24 : 1;
    /* 0x0001.6 */ u8 field30 : 1;
    /* 0x0001.7 */ u8 field31 : 1;
    /* 0x0002.0 */ u8 field32 : 1;
    /* 0x0002.1 */ u8 field33 : 1;
    /* 0x0002.2 */ u8 field34 : 1;
    /* 0x0002.3 */ u8 field40 : 1;
    /* 0x0002.4 */ u8 field41 : 1;
    /* 0x0002.5 */ u8 field42 : 1;
    /* 0x0002.6 */ u8 field43 : 1;
    /* 0x0002.7 */ u8 field44 : 1;
    /* 0x0003.0 */ u8 field50 : 1;
    /* 0x0003.1 */ u8 field51 : 1;
    /* 0x0003.2 */ u8 field52 : 1;
    /* 0x0003.3 */ u8 field53 : 1;
    /* 0x0003.4 */ u8 field54 : 1;
    /* 0x0003.5 */ u8 pad_0x3_5 : 3;
};
ASSERT_SIZE(SvSnowmanBingoCard, 0x4);

// 24CA98 31F2FA5C, 0
struct SvPlayerSpecialMail {
    // (game code, not decompiled yet)
    SvPlayerSpecialMail(); // 0x0024DEB8

    /* 0x0000 */ u32 unk_0x0; // Set to 0 in Player ctor
    /* 0x0004 */ SvFgName gulliverGift;
    /* 0x0008 */ u8 gulliverLetter; // SvGulliverMail::Name / received the next day
    /* 0x0009 */ u8 unk_0x9; // Set to 0x80 in Player ctor
    /* 0x000A */ u16 unk_0xA; // Set to 0 in Player ctor
    /* 0x000C */ u8 unk_0xC; // Set to 0xFF in Player ctor
    /* 0x000D */ u32 unk_0xD; // Set to 0 in Player ctor (Set later, before 1st loop)
    /* 0x0011 */ u8 unk_0x11; // Set to 0 in Player ctor
    /* 0x0012 */ u8 unk_0x12; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0013 */ u8 pad_0x13; // Padding: Not set in Player ctor
    /* 0x0014 */ u16 unk_0x14; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0016 */ u8 unk_0x16; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0017 */ u8 pad_0x17; // Padding: Not set in Player ctor
    /* 0x0018 */ u16 unk_0x18; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x001A */ u8 unk_0x1A; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x001B */ u8 pad_0x1B; // Padding: Not set in Player ctor
    /* 0x001C */ u16 unk_0x1C; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x001E */ u8 unk_0x1E; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x001F */ u8 pad_0x1F; // Padding: Not set in Player ctor
    /* 0x0020 */ u16 unk_0x20; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0022 */ u8 unk_0x22; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0023 */ u8 pad_0x23; // Padding: Not set in Player ctor
    /* 0x0024 */ u16 unk_0x24; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0026 */ u8 unk_0x26; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x0027 */ u8 pad_0x27; // Padding: Not set in Player ctor
    /* 0x0028 */ u16 unk_0x28; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x002A */ u8 unk_0x2A; // Set to 0 in Player ctor
    /* 0x002B */ u8 pad_0x2B; // Padding: Not set in Player ctor
    /* 0x002C */ u16 unk_0x2C; // Set to 0 in Player ctor (Set in 1st loop)
    /* 0x002E */ u8 unk_0x2E; // Set to 0 in Player ctor
    /* 0x002F */ u8 unk_0x2F; // Set to 0 in Player ctor
    /* 0x0030 */ u16 unk_0x30; // research name: UnknownCurrentYear / not sure for what
    /* 0x0032 */ u16 unk_0x32; // research name: UnknownYear2 / not sure for what
    /* 0x0034 */ u32 unk_0x34; // Set to 0 in Player ctor
    /* 0x0038 */ u8 jingleLetter; // received the next day / 1 for receiving, 0 for not receiving
    /* 0x0039 */ u8 unk_0x39; // Set to 0 in Player ctor
    /* 0x003A */ u16 unk_0x3A; // Set to 0 in Player ctor
    /* 0x003C */ u8 unk_0x3C; // Set to 0 in Player ctor
    /* 0x003D */ u8 unk_0x3D; // Set to 0 in Player ctor
    /* 0x003E */ u16 unk_0x3E; // Set to 0 in Player ctor
    /* 0x0040 */ SvPrizeMailFlags chipLetter; // received the next day
    /* 0x0041 */ u8 unk_0x41; // Set to 1 in Player ctor
    /* 0x0042 */ u16 unk_0x42; // Set to 0 in Player ctor
    /* 0x0044 */ SvPrizeMailFlags natLetter; // received the next day
    /* 0x0045 */ SvSnowmanMailFlags snowmanLetter; // received the next day
    /* 0x0046 */ SvFgName snowmanGift; // only used if the item is dynamic (like snowboy gifts)
    /* 0x004A */ u16 unk_0x4A; // research name: UnknownYear3 / Set to 0 in Player ctor
    /* 0x004C */ SvSnowmanBingoCard bingoCard;
    /* 0x0050 */ u8 currentBingoCard; // BingoCards are not random, they are calculated based on this number (0 - 255)
    /* 0x0051 */ u8 unk_0x51; // Set to 0 in Player ctor
    /* 0x0052 */ u16 unk_0x52; // Set to 0 in Player ctor
    /* 0x0054 */ u8 unk_0x54; // Set to 0 in Player ctor
    /* 0x0055 */ u8 unk_0x55; // Set to 0 in Player ctor
    /* 0x0056 */ u8 unk_0x56; // Set to 0 in Player ctor
    /* 0x0057 */ u8 unk_0x57; // Set to 0 in Player ctor
    /* 0x0058 */ u8 unk_0x58; // Set to 5 in Player ctor
    /* 0x0059 */ u8 unk_0x59; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005A */ u8 unk_0x5A; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005B */ u8 unk_0x5B; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005C */ u8 unk_0x5C; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005D */ u8 unk_0x5D; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005E */ u8 unk_0x5E; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x005F */ u8 unk_0x5F; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0060 */ u8 unk_0x60; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0061 */ u8 unk_0x61; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0062 */ u8 unk_0x62; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0063 */ u8 unk_0x63; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0064 */ u8 unk_0x64; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0065 */ u8 unk_0x65; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0066 */ u8 unk_0x66; // Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
    /* 0x0067 */ u8 katieLetter; // if 9 letter will be received the next day
    /* 0x0068 */ u8 unk_0x68; // Set to 0 in Player ctor
    /* 0x0069 */ u8 unk_0x69; // Set to 0 in Player ctor
    /* 0x006A */ SvTownId katieTraveledToTownData; // Will be shown in katies letter
};
ASSERT_SIZE(SvPlayerSpecialMail, 0x80);

struct SvPlayerUnk8D4C {
    // (game code, not decompiled yet)
    SvPlayerUnk8D4C(); // 0x005BDD04

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 pad_0xB; // Padding: Not set in ctor
    /* 0x000C */ u16 unk_0xC; // ???; Set to 0x0000 in player ctor
    /* 0x000E */ u8 unk_0xE; // ???; Set to 0x6 in player ctor
    /* 0x000F */ u8 unk_0xF; // ???; Set to 0xFF in player ctor
};
ASSERT_SIZE(SvPlayerUnk8D4C, 0x10);

// Probably exact same struct as UnknownStruct3 but is set differently
struct SvPlayerUnk8D5C {
    // (game code, not decompiled yet)
    SvPlayerUnk8D5C(); // 0x005BDECC

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 pad_0xB; // Padding: Not set in ctor
    /* 0x000C */ u16 unk_0xC; // ???; Set to 0x7ffe in player ctor
    /* 0x000E */ u8 unk_0xE; // ???; Set to 0x4 in player ctor
    /* 0x000F */ u8 unk_0xF; // ???; Set to 0xFF in player ctor
};
ASSERT_SIZE(SvPlayerUnk8D5C, 0x10);

// Similar to UnknownStruct3
struct SvPlayerUnk8D6C {
    // (game code, not decompiled yet)
    SvPlayerUnk8D6C(); // 0x005BE570

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 pad_0xB; // Padding: Not set in ctor
    /* 0x000C */ u32 unk_0xC; // ???; Set to 0x00007ffe in player ctor
    /* 0x0010 */ u32 unk_0x10; // ???; Set to 0x00007ffe in player ctor
    /* 0x0014 */ u8 unk_0x14; // ???; Set to 0xFF in player ctor
    /* 0x0015 */ u8 unk_0x15; // ???; Set to 0x7 in player ctor
    /* 0x0016 */ u8 unk_0x16; // ???; Set to 0xFF in player ctor
    /* 0x0017 */ u8 pad_0x17; // Padding: Not set in ctor;
};
ASSERT_SIZE(SvPlayerUnk8D6C, 0x18);

struct SvPlayerUnk8D84 {
    // (game code, not decompiled yet)
    SvPlayerUnk8D84(); // 0x005BEC6C

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 pad_0xB; // Padding: Not set in ctor
    /* 0x000C */ SvNpcRef mini1;
    /* 0x003C */ SvNpcRef mini2;
    /* 0x006C */ u32 unk_0x6C; // ???; Set to 0x00007ffe in player ctor
    /* 0x0070 */ u32 unk_0x70; // ???; Set to 0x00007ffe in player ctor
    /* 0x0074 */ u8 unk_0x74; // ???; Set to 0 in player ctor
    /* 0x0075 */ u8 unk_0x75; // ???; Set to 0 in player ctor
};
ASSERT_SIZE(SvPlayerUnk8D84, 0x76);

// Probably exact same struct as UnknownStruct4 but is set differently
struct SvPlayerUnk8DFA {
    // (game code, not decompiled yet)
    SvPlayerUnk8DFA(); // 0x005BC4C0

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 unk_0xB; // ???; Set to 0xFF in player ctor
    /* 0x000C */ u8 unk_0xC; // ???; Set to 0xFF in player ctor
    /* 0x000D */ u8 pad_0xD; // Padding: Not set in ctor
};
ASSERT_SIZE(SvPlayerUnk8DFA, 0xE);

// Similar to UnknownStruct3
struct SvPlayerUnk8E08 {
    // (game code, not decompiled yet)
    SvPlayerUnk8E08(); // 0x005BDAEC

    /* 0x0000 */ s64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 unk_0x8; // ???; Set to 0x21 in player ctor
    /* 0x0009 */ u8 unk_0x9; // ???; Set to 0xFF in player ctor
    /* 0x000A */ u8 unk_0xA; // ???; Set to 0x9 in player ctor
    /* 0x000B */ u8 unk_0xB; // Padding: Not set in ctor
};
ASSERT_SIZE(SvPlayerUnk8E08, 0xC);

struct SvPlayerUnk8EE0 {
    // (game code, not decompiled yet)
    SvPlayerUnk8EE0(); // 0x0020C484

    /* 0x0000 */ u16 unk_0x0; // ???; Set to 0 in player ctor
    /* 0x0002 */ u8 unk_0x2; // ???; Set to 0 in player ctor
    /* 0x0003 */ u8 unk_0x3; // ???; Set to 0 in player ctor
    /* 0x0004 */ u32 unk_0x4; // ???; Set to 0 in player ctor
    /* 0x0008 */ u32 unk_0x8; // ???; Set to 0 in player ctor
    /* 0x000C */ u32 unk_0xC; // ???; Set to 0 in player ctor
    /* 0x0010 */ u32 unk_0x10; // ???; Set to 0 in player ctor
    /* 0x0014 */ u32 unk_0x14; // ???; Set to 0 in player ctor
    /* 0x0018 */ u16 unk_0x18; // ???; Set to 0 in player ctor
    /* 0x001A */ u32 unk_0x1A; // ???; Set to 0 in player ctor
    /* 0x001E */ u32 unk_0x1E; // ???; Set to 0 in player ctor
    /* 0x0022 */ u32 unk_0x22; // ???; Set to 0 in player ctor
    /* 0x0026 */ u16 unk_0x26; // ???; Set to 0 in player ctor
    /* 0x0028 */ u8 unk_0x28; // ???; Set to 0 in player ctor
    /* 0x0029 */ u8 pad_0x29; // Padding: Not set in ctor
};
ASSERT_SIZE(SvPlayerUnk8EE0, 0x2A);

struct SvPlayerUnkA468 {
    // (the empty item and 0)
    DECOMP_NOINLINE SvPlayerUnkA468(); // 0x006BDBB0

    /* 0x0000 */ SvFgName unk_0x0; // Some Item; Set to 0x00007ffe in player ctor
    /* 0x0004 */ u16 unk_0x4; // ???; Set to 0 in player ctor
};
ASSERT_SIZE(SvPlayerUnkA468, 0x6);

struct SvHhaLyleFlags {
    /* 0x0000.0 */ u8 hasHeardEvaluation : 1; // Set when asking about home evaluation
    /* 0x0000.1 */ u8 hasAskedWhatsNew : 1;
    /* 0x0000.2 */ u8 hasHeardFirstEvaluation : 1; // Set when asking about home evaluation
    /* 0x0000.3 */ u8 lyleWhatsNew : 5;
};
ASSERT_SIZE(SvHhaLyleFlags, 0x1);

struct SvMiiData {
    /* 0x0000 */ u8 miiFace[0x5C]; // Based on https: / 3dbrew.org/wiki/Mii#Mii_format
    /* 0x005C */ u16 pad_0x5C; // U16 Zero Padding; Always 0x0000
    /* 0x005E */ u16 miiCrc16;
    /* 0x0060 */ u32 aesCcmMac[4];
    /* 0x0070 */ u8 unk_0x70[0x18]; // Gets written to when getting a Mii form Harriet. Never read(?)
    /* 0x0088 */ u8 pad_0x88[0x1E];
    /* 0x00A6 */ u16 pad_0xA6;
};
ASSERT_SIZE(SvMiiData, 0xA8);

struct SvEncyclopediaSizes {
    /* 0x0000 */ u16 insects[0x48]; // range 1 to 0x3FFF
    /* 0x0090 */ u16 fish[0x48]; // range 1 to 0x3FFF
    /* 0x0120 */ u16 seaCreatures[0x1E]; // range 1 to 0x3FFF
};
ASSERT_SIZE(SvEncyclopediaSizes, 0x15C);

struct SvPlayerFeatures {
    /* 0x0000 */ u8 hairStyle;
    /* 0x0001 */ u8 hairColor; // Values: 0x0 -> 0xF
    /* 0x0002 */ u8 face; // Values: 0x0 -> 0xB
    /* 0x0003 */ u8 eyeColor; // Values: 0x0 -> 0x5
    /* 0x0004 */ u16 tan; // Values: 0x0 -> 0xF
};
ASSERT_SIZE(SvPlayerFeatures, 0x6);

struct SvPlayerOutfit {
    /* 0x0000 */ SvFgName hat; // Item ID < 0xXXXX
    /* 0x0004 */ SvFgName accessory; // Item ID < 0xXXXX
    /* 0x0008 */ SvFgName topWear; // Item ID < 0xXXXX
    /* 0x000C */ SvFgName underTopWear; // Item ID < 0xXXXX
    /* 0x0010 */ SvFgName bottomWear; // Item ID < 0xXXXX
    /* 0x0014 */ SvFgName socks; // Item ID < 0xXXXX
    /* 0x0018 */ SvFgName shoes; // Item ID < 0xXXXX
    /* 0x001C */ SvFgName heldItem; // Item ID < 0xXXXX
    /* 0x0020 */ u8 unk_0x20; // Inverted gender(?): 1 for male, 0 for female. Default = 1 in PlayerConstructor (EUR 1.5 0x20D27C)
};
ASSERT_SIZE(SvPlayerOutfit, 0x21);

struct SvPlayerAppearance {
    /* 0x0000 */ SvPlayerFeatures playerFeatures;
    /* 0x0006 */ SvPlayerOutfit playerOutfit;
};
ASSERT_SIZE(SvPlayerAppearance, 0x27);

// UnknownNotSetYetX = Not set by player ctor
// (the parts of the player with their own constructors; names are ours)
// the bit field of the unlocked items (one bit per item index)
struct SvPlayerUnlockedItems {
    SvPlayerUnlockedItems() { Clear(); }
    // (game code, not decompiled yet) all bits are cleared / the bit of an item is set
    void Clear(); // 0x0024ABA8
    bool Func7711D4(SvFgName item) const; // 0x007711D4

    /* 0x0000 */ u32 bits[0xBA];
};
ASSERT_SIZE(SvPlayerUnlockedItems, 0x2E8);

// the texts of the letters of the player (cleared, both indents 0xFF)
struct SvPlayerUnk8908 {
    // (game code, not decompiled yet) the constructor / the same as a function
    SvPlayerUnk8908(); // 0x001B2C28
    void Clear(); // 0x001B2C08

    /* 0x0000 */ char16 letterHeader[0x20];
    /* 0x0040 */ u16 pad_0x8948;
    /* 0x0042 */ char16 futureLetterHeader[0x20];
    /* 0x0082 */ u16 pad_0x898A;
    /* 0x0084 */ char16 letterSignature[0x20];
    /* 0x00C4 */ u16 pad_0x89CC;
    /* 0x00C6 */ u8 defaultLtrReceiverNameIndent;
    /* 0x00C7 */ u8 defaultFutureLtrReceiverNameIndent; // Cannot be >= 0x20
};
ASSERT_SIZE(SvPlayerUnk8908, 0xC8);

struct SvPlayerUnk89FA {
    SvPlayerUnk89FA()
    {
        for (s32 i = 0; i < 0x20; i++) {
            ids[i] = 0xFFFF;
        }
    }

    /* 0x0000 */ u16 ids[0x20]; // Not Verified; 0xFFFF is default, then 0xXXXX is ID of DLC recieved
};
ASSERT_SIZE(SvPlayerUnk89FA, 0x40);

// the meow coupons and five values that 0x002BF528 adds to them
struct SvPlayerUnk8D1C {
    SvPlayerUnk8D1C() { Clear(); }
    // (game code, not decompiled yet) all six values become 0 / the five values are added to the meow coupons (at most
    // 99999)
    void Clear(); // 0x002BF5BC
    void Func2BF528(); // 0x002BF528

    /* 0x0000 */ SvEncValue meowCoupons;
    // (research: the flows read the second word of [2] as a count)
    /* 0x0008 */ SvEncValue unk_0x8[5];
};
ASSERT_SIZE(SvPlayerUnk8D1C, 0x30);

struct SvPlayerUnk8EBE {
    // (game code, not decompiled yet) all 11 halfwords become 0xFFFF / a function on it (0x005BF34C)
    SvPlayerUnk8EBE(); // 0x005BF390
    void Func5BF34C(); // 0x005BF34C

    /* 0x0000 */ u8 data[0x16]; // Always contains 0x16 0xFF bytes. ctor sets this.
};
ASSERT_SIZE(SvPlayerUnk8EBE, 0x16);

struct SvPlayerUnk8F77 {
    SvPlayerUnk8F77()
    {
        std::memset(data, 0, sizeof(data));
        Init();
    }
    // (game code, not decompiled yet)
    void Init(); // 0x002C0614

    /* 0x0000 */ u8 data[0x22];
};
ASSERT_SIZE(SvPlayerUnk8F77, 0x22);

struct SvPlayerUnk8F99 {
    // (the byte is cleared, then the bits 0..2 one by one)
    SvPlayerUnk8F99()
    {
        value = 0;
        for (s32 i = 0; i < 3; i++) {
            switch (i) {
            case 0:
                value &= ~1;
                break;
            case 1:
                value &= ~2;
                break;
            case 2:
                value &= ~4;
                break;
            }
        }
    }

    /* 0x0000 */ u8 value;
};
ASSERT_SIZE(SvPlayerUnk8F99, 0x1);

struct SvPlayerUnk8FD0 {
    // (game code, not decompiled yet) cleared
    SvPlayerUnk8FD0(); // 0x0024AB28

    /* 0x0000 */ u8 data[0x320]; // Always 0?? Game just memclr's in player ctor
};
ASSERT_SIZE(SvPlayerUnk8FD0, 0x320);

struct SvPlayerUnk9604 {
    // (ARMCC's out-of-line implicit constructor: the letters are built)
    DECOMP_NOINLINE SvPlayerUnk9604() = default; // 0x00332ED8

    /* 0x0000 */ SvMail unk_0x0[5];
};
ASSERT_SIZE(SvPlayerUnk9604, 0xC80);

struct SvPlayerUnkA47A {
    // (a copy of the empty item)
    SvPlayerUnkA47A(); // 0x00304CF8

    /* 0x0000 */ SvFgName item;
};
ASSERT_SIZE(SvPlayerUnkA47A, 0x4);

// the first part of the player (checksum1 and the data it covers); the player constructor builds it first
// (name is ours)
struct SvPlayerUnk0 {
    // (game code, not decompiled yet)
    SvPlayerUnk0(); // 0x0020D1AC

    /* 0x0000 */ u32 checksum1; // Checksum of the first 0x6b84 of player data
    /* 0x0004 */ SvPlayerAppearance playerAppearance;
    /* 0x002B */ u8 pad_0x2B;
    /* 0x002C */ SvMyDesign patterns[0xA]; // 10 Patterns
    /* 0x548C */ u8 patternOrder[0xA]; // Order of patterns from 0x0 - 0x9
    /* 0x5496 */ u16 pad_0x5496; // U16 Zero Padding; Always 0x0000
    /* 0x5498 */ SvMiiData playerMii;
    /* 0x5540 */ u8 hasMii; // Values: 0 = No Mii, 1 = Has Mii, <1 = Has Mii, face doesn't show
    /* 0x5541 */ u8 pad_0x5541; // Not Verified: U8 Zero Padding; Always 0x00
    /* 0x5542 */ u16 pad_0x5542; // Not Verified: U16 Zero Padding; Always 0x0000
    /* 0x5544 */ SvMannequin mannequin1;
    /* 0x555C */ SvMannequin mannequin2;
    /* 0x5574 */ SvMannequin mannequin3;
    /* 0x558C */ SvMannequin mannequin4;
    /* 0x55A4 */ u16 pad_0x55A4;
    /* 0x55A6 */ SvPersonalId playerInfo;
    /* 0x55D4 */ u8 birthMonth;
    /* 0x55D5 */ u8 birthDay;
    /* 0x55D6 */ u16 yearRegistered;
    /* 0x55D8 */ u8 monthRegistered;
    /* 0x55D9 */ u8 dayRegistered;
    /* 0x55DA */ u16 pad_0x55DA; // Zero Padding; Always 0x0000
    /* 0x55DC */ SvPlayerBadges badges;
    /* 0x56C4 */ SvHhaEvaluation hhaHouse;
    /* 0x56F0 */ SvDreamAddress dreamCode;
    /* 0x56FC */ u32 pad_0x56FC;
    /* 0x5700 */ SvPlayerFlags playerFlags;
    /* 0x5734 */ u32 hasTpcPic;
    /* 0x5738 */ u8 tpcPic[0x1400];
    /* 0x6B38 */ char16 tpcText[0x21];
    /* 0x6B7A */ u8 townSlot; // bits 0-1: the slot of the player in its own town (0 = first player), stored so that copies (players of other towns in a network game) keep it; the player creation writes it, the census arrays are indexed with it (0x005C7ED8)
    /* 0x6B7B */ u8 unk_0x6B7B;
    /* 0x6B7C */ u32 unk_0x6B7C;
    /* 0x6B80 */ u32 unk_0x6B80;
    /* 0x6B84 */ u32 pad_0x6B84;
};
ASSERT_SIZE(SvPlayerUnk0, 0x6B88);

struct SvPlayer : SvPlayerUnk0 {
    // (the members are built one after the other; the constructor of the save data builds it)
    SvPlayer(); // 0x006BEBB0
    // (game code of dSvPlayer.cpp; names are ours) the debt from 0x0076E7CC / a value as the debt, the bit 0x10 of
    // unk_0x8F77[12] each / the Mii part (0x0058381C) / the bit of an item of the kind 44 / bells from Reese are
    // added / the bits of the kind 44 are cleared / the meow coupons get the value of the initiative / a bit below
    // 0x18F
    // the play time goes up (at most to the largest time); every full minute goes to Func6F0C80(16, minutes), the
    // seconds are collected over the calls (name is ours, after SvGardenData::AddPlaytime)
    void AddPlaytime(s64 time); // 0x006BD504
    void Func6BD90C(); // 0x006BD90C
    void Func6BDB84(u32 value); // 0x006BDB84
    void Func6BD940(); // 0x006BD940
    void Func6BD9A0(const SvFgName& item); // 0x006BD9A0
    void Func6BD9FC(u32 value); // 0x006BD9FC
    void Func6BDA0C(); // 0x006BDA0C
    void Func6BDB48(); // 0x006BDB48
    void Func6BDBCC(u32 index); // 0x006BDBCC
    // the value of the held item (if Func769644 does not hold for it) and of the inventory items goes to the bank, the
    // items are removed; returns the sum
    s32 Func6BDA24(); // 0x006BDA24
    // each usual item of unk_0xA468 is sent in a letter "Mail_SP_Autocamp" into the letter pool of the shared data, then
    // the remaining entries are moved to the front
    void Func6BDC0C(); // 0x006BDC0C
    // the size in the encyclopedia of an insect, fish or sea creature (0: none) / unk_0x8F28 goes up by a positive value
    // / the item and the value go into the first entry of unk_0xA468 without a usual item / the first such entry is
    // cleared (false: none) / a bit of unk_0xA3E4 / of unk_0xA424 (not for 0xFFFF) / the bits of unk_0x8F3C are cleared
    // / unk_0x8D84 if Func76EE94 holds for the villager, else NULL
    u16 Func6BDE18(const SvFgName& item) const; // 0x006BDE18
    void Func6BDF08(s32 value); // 0x006BDF08
    void Func6BDF34(const SvFgName& item, u16 value); // 0x006BDF34
    bool Func6BDFA0(); // 0x006BDFA0
    void Func6BE028(u32 index); // 0x006BE028
    void Func6BE05C(u32 index); // 0x006BE05C
    void Func6BE090(); // 0x006BE090
    SvPlayerUnk8D84* Func6BE0A0(const SvNpcRef* npc, s32 index); // 0x006BE0A0
    // the size of an insect, fish or sea creature becomes the one of the item (Func76981C) if it is larger / a new
    // random unk_0xA464 / the sizes of the encyclopedia are cleared / the player is created again (Func6BE460 first,
    // then Func20C8AC with the arguments, the town, the start values; the census of the town slot goes up by 10)
    void Func6BE0BC(const SvFgName& item); // 0x006BE0BC
    void Func6BE210(); // 0x006BE210
    void Func6BE254(); // 0x006BE254
    void Func6BE2C4(u32 a, u32 b, u32 c, const SvTownId* town, u32 townSlot); // 0x006BE2C4
    // the player is cleared and the parts get their start values (unk_0x8F2A becomes 2011, unk_0x8F2C too unless
    // keepUnk8F2C)
    void Func6BE460(bool keepUnk8F2C); // 0x006BE460
    // the debt goes down by a value (to 0 if it is smaller) / medals are added (up to 99999) / hasBeeSting and three
    // other flags are cleared (two of them only if Func5C304C/Func5C6AF4 hold) / medals go down / the player is
    // taken over (Func20CBD8 and the parts with the arguments; letters with bit 7 of letterType are cleared, the
    // others go through Func6BC774)
    void Func6BE8E8(u32 value); // 0x006BE8E8
    void Func6BE928(u32 value); // 0x006BE928
    void Func6BE93C(); // 0x006BE93C
    void Func6BE9E8(u32 value); // 0x006BE9E8
    void Func6BE9F4(u32 a, u32 b, u8 c); // 0x006BE9F4

    // (inline in dSvPlayer.cpp; name is ours) the size of an entry of the encyclopedia (0: none)
    u16 GetEncyclopediaSize(item::Kind kind, u32 index) const;

    // the checksum of the first part of the player (checksum1; names are ours, index and isLogged are not used)
    DECOMP_NOINLINE void UpdateChecksum1(u32 index); // 0x007136FC
    DECOMP_NOINLINE bool IsChecksum1Valid(u32 index, bool isLogged); // 0x00713724
    // both checksums (a copy in the shared area; name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void UpdateChecksums(u32 index); // 0x0076F0B4

    /* 0x6B88 */ u32 checksum2;
    /* 0x6B8C */ SvEncValue bankAmount;
    /* 0x6B94 */ SvEncValue debtAmount;
    /* 0x6B9C */ SvEncValue medalAmount;
    /* 0x6BA4 */ SvEncValue bellsFromReeseAmount;
    /* 0x6BAC */ u32 pad_0x6BAC;
    /* 0x6BB0 */ s64 playtime;
    /* 0x6BB8 */ SvTownId townData2;
    /* 0x6BCE */ u16 pad_0x6BCE;
    /* 0x6BD0 */ SvPlayerInventory inventory;
    /* 0x6C20 */ SvPlayerUnlockedItems unlockedItems; // Game uses one big bitfield for items 'unlocked'. Bits correspond to item ids. Used for catalog, encyclopedia, etc
    /* 0x6F08 */ SvEncValue pocketMoney;
    /* 0x6F10 */ SvFgName islandBox[0x28];
    /* 0x6FB0 */ SvFgName islandInventory[0x10];
    /* 0x6FF0 */ u8 islandInventoryItemLocks[0x10]; // may be Padding also, needs testing
    /* 0x7000 */ SvFgName unk_0x7000;
    /* 0x7004 */ SvFgName unk_0x7004;
    /* 0x7008 */ SvMail letters[0xA];
    /* 0x8908 */ SvPlayerUnk8908 unk_0x8908;
    /* 0x89D0 */ SvEmoticons emotes; // Players Emotes (40 slots)
    /* 0x89F8 */ s8 emotePage; // 0xFF = Page 1, 0x00 = Page 2
    /* 0x89F9 */ u8 pad_0x89F9;
    /* 0x89FA */ SvPlayerUnk89FA spotpassDlcReceivedIds; // Not Verified; 0xFFFF is default, then 0xXXXX is ID of DLC recieved
    /* 0x8A3A */ u16 pad_0x8A3A;
    /* 0x8A3C */ SvPlayerSpecialMail unk_0x8A3C;
    /* 0x8ABC */ u16 pad_0x8ABC;
    /* 0x8ABE */ SvHhaLyleFlags lyleFlag;
    /* 0x8ABF */ u8 hasDeductions; // Not 100% sure; something to do with items facing a wall, therefore deductions; Reads HouseUnk9 later in code
    /* 0x8AC0 */ u8 hhaAwardsUnlockedDupe;
    /* 0x8AC1 */ u8 goldExteriorsUnlockedDupe;
    /* 0x8AC2 */ u8 hhaUnk1;
    /* 0x8AC3 */ u8 hhaUnk2; // READU8(CurrentHouseTheme+1) | 0x80; Only When Exterior/Theme???
    /* 0x8AC4 */ u8 hhaUnk3[0x1C]; // Come back to later
    /* 0x8AE0 */ SvPlayerInitiative initiative;
    /* 0x8D1C */ SvPlayerUnk8D1C unk_0x8D1C;
    /* 0x8D4C */ SvPlayerUnk8D4C unk_0x8D4C;
    /* 0x8D5C */ SvPlayerUnk8D5C unk_0x8D5C;
    /* 0x8D6C */ SvPlayerUnk8D6C unk_0x8D6C;
    /* 0x8D84 */ SvPlayerUnk8D84 unk_0x8D84;
    /* 0x8DFA */ SvPlayerUnk8DFA unk_0x8DFA;
    /* 0x8E08 */ SvPlayerUnk8E08 unk_0x8E08;
    /* 0x8E14 */ u8 unk_0x8E14[0x56];
    // (research: an item; the constructor builds no SvFgName here)
    /* 0x8E6A */ u32 requestExteriorToRealtor; // when in a different town, go to tom nook and request the exterior
    /* 0x8E6E */ u32 unk_0x8E6E;
    /* 0x8E72 */ u8 unk_0x8E72[0x4C];
    /* 0x8EBE */ SvPlayerUnk8EBE filledWithFF; // Always contains 0x16 0xFF bytes. ctor sets this.
    /* 0x8ED4 */ u8 unk_0x8ED4[0xC];
    /* 0x8EE0 */ SvPlayerUnk8EE0 unk_0x8EE0;
    /* 0x8F0A */ SvFgName unk_0x8F0A;
    /* 0x8F0E */ SvFgName unk_0x8F0E;
    /* 0x8F12 */ u8 unk_0x8F12[0xE];
    /* 0x8F20 */ u8 unk_0x8F20[8];
    /* 0x8F28 */ u16 unk_0x8F28; // a counter up to 0xFFFF (Func6BDF08)
    /* 0x8F2A */ u16 unk_0x8F2A = 2011; // Set to 0x7DB in player ctor
    /* 0x8F2C */ u16 unk_0x8F2C = 2011; // Set to 0x7DB in player ctor
    /* 0x8F2E */ SvDate unk_0x8F2E; // research name: UnknownDate / Does get set when Katrina tells lucky item
    /* 0x8F32 */ u8 unk_0x8F32[2];
    /* 0x8F34 */ u32 unk_0x8F34; // set to 0 by Func6BE460
    /* 0x8F38 */ u32 unk_0x8F38; // set to 0 after a load (0x005CB274)
    /* 0x8F3C */ u8 unk_0x8F3C[0x32]; // a bit for each index below 0x18F (Func6BDBCC)
    /* 0x8F6E */ u8 unk_0x8F6E[9]; // a bit for each index below 67 of the items of the kind 44 (Func6BD9A0)
    /* 0x8F77 */ SvPlayerUnk8F77 unk_0x8F77; // ctor unsets every bit except in last byte in buffer, it only unsets bits 0 to 5
    /* 0x8F99 */ SvPlayerUnk8F99 unk_0x8F99; // Set to 0 in player ctor
    /* 0x8F9A */ u16 pad_0x8F9A;
    /* 0x8F9C */ u32 addedSongs[3]; // Bitfield for added songs
    /* 0x8FA8 */ SvFgName santaBagInv[0xA];
    /* 0x8FD0 */ SvPlayerUnk8FD0 filledWithZero; // Always 0?? Game just memclr's in player ctor
    /* 0x92F0 */ SvFgName dressers[0xB4]; // Each dresser is 60 long
    /* 0x95C0 */ char16 bDayWish[0x22];
    /* 0x9604 */ SvPlayerUnk9604 unk_0x9604;
    /* 0xA284 */ u8 unk_0xA284[4];
    /* 0xA288 */ SvEncyclopediaSizes encyclopediaSizes;
    /* 0xA3E4 */ u8 unk_0xA3E4[0x40] = {}; // a bit for each index below 512 (Func6BE028)
    /* 0xA424 */ u8 unk_0xA424[0x40] = {}; // a bit for each index below 512 (Func6BE05C)
    /* 0xA464 */ s32 unk_0xA464; // a random value 0..4, another one than before (Func6BE210)
    /* 0xA468 */ SvPlayerUnkA468 unk_0xA468[3];
    /* 0xA47A */ SvPlayerUnkA47A unk_0xA47A; // Some Item; Set to 0x00007ffe in player ctor
    /* 0xA47E */ u16 pad_0xA47E;
};
ASSERT_SIZE(SvPlayer, 0xA480);

// the four players of the town (name after libgarden)
struct SvPlayerData {
    // the player (NULL for an index of 4 or more)
    SvPlayer* Get(u32 index); // 0x002FB9EC (name is ours)
    // the checksums of the players / all of them are right (names are ours)
    DECOMP_NOINLINE void UpdateChecksums(); // 0x0071AE00
    DECOMP_NOINLINE bool AreChecksumsValid(bool isLogged); // 0x0071AE60

    /* 0x0000 */ SvPlayer players[4];
};
ASSERT_SIZE(SvPlayerData, 0x29200);

// (game code that the functions of the player call, not decompiled yet; names after the addresses)
u32 Func76E7CC(SvPlayer* player); // 0x0076E7CC
void Func58381C(SvPlayer* player); // 0x0058381C
u32 Func24AC8C(SvPlayerInitiative* initiative); // 0x0024AC8C
s32 Func76A018(const SvFgName* item); // 0x0076A018
bool Func769644(const SvFgName* item); // 0x00769644
u8 Func70162C(const SvFgName& item); // 0x0070162C
u8 Func61643C(const SvFgName& item); // 0x0061643C
void Func617740(s32* index, item::Kind* kind, s32 id); // 0x00617740
bool Func76EE94(SvPlayer* player, const SvNpcRef* npc, s32 index); // 0x0076EE94
u16 Func76981C(const SvFgName& item); // 0x0076981C
void Func20C8AC(SvPlayer* player, u32 a, u32 b, u32 c, const SvTownId* town, u32 townSlot); // 0x0020C8AC
void Func24E2B4(SvEmoticons* emoticons); // 0x0024E2B4
void Func6F0B74(u8* data); // 0x006F0B74
void Func24CE14(SvPlayerSpecialMail* mail); // 0x0024CE14
void Func24AD90(SvPlayerInitiative* initiative); // 0x0024AD90
// (the parts of the player when it is cleared, Func6BE460)
void Func20CABC(SvPlayer* player); // 0x0020CABC
void Func6F0B80(u8* data); // 0x006F0B80
void Func5C7230(); // 0x005C7230
void Func24E2D0(SvEmoticons* emoticons); // 0x0024E2D0
void Func24CE90(SvPlayerSpecialMail* mail); // 0x0024CE90
void Func305A94(u8* data); // 0x00305A94
void Func5BDB2C(SvPlayerUnk8D4C* data); // 0x005BDB2C
void Func5BDD70(SvPlayerUnk8D5C* data); // 0x005BDD70
void Func5BE334(SvPlayerUnk8D6C* data); // 0x005BE334
void Func5BEBCC(SvPlayerUnk8D84* data); // 0x005BEBCC
void Func30A834(SvPlayerUnk8DFA* data); // 0x0030A834
void Func5BD6CC(SvPlayerUnk8E08* data, SvPlayer* player); // 0x005BD6CC
void Func20C3D8(SvPlayerUnk8EE0* data); // 0x0020C3D8
void Func6CD8A4(SvFgName* dressers); // 0x006CD8A4
void Func1B5184(SvFgName* santaBagInv); // 0x001B5184
void Func24AE04(SvPlayerInitiative* initiative); // 0x0024AE04
// (the flags of Func6BE93C and the parts of Func6BE9F4)
bool Func5C304C(SvPlayer* player); // 0x005C304C
bool Func5C6AF4(SvPlayer* player); // 0x005C6AF4
void Func20CBD8(SvPlayer* player, u32 a, u32 b, u8 c); // 0x0020CBD8
void Func3238AC(SvPlayerInventory* inventory, u32 b); // 0x003238AC
void Func6CD8D0(SvFgName* dressers, u32 b); // 0x006CD8D0
void Func2D5004(SvFgName* islandBox, u32 b); // 0x002D5004
struct SvSharedData;
void Func6CDB2C(void* data, u32 b); // 0x006CDB2C
void Func332E88(SvPlayerUnk9604* data, u32 a, SvPersonalId* player, u32 b); // 0x00332E88
void Func6BC774(SvMail* mail, u32 a, SvPersonalId* player, u32 b); // 0x006BC774
void Func24DD34(SvPlayerSpecialMail* mail); // 0x0024DD34
void Func5BD8D4(SvPlayerUnk8E08* data, SvPlayer* player); // 0x005BD8D4

#pragma pack(pop)
