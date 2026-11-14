#pragma once

// Progress / tutorial flags of a player (0x34 bytes of bits).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

// The value IDs of SvPlayerFlags::GetValue/SetValue/SetFlag (names after the fields; a value of several bits is a
// counter or a small number)
enum SvPlayerFlagId : u16 {
    PLAYER_FLAG_UNK_0x0_0 = 0, // 4 bits
    PLAYER_FLAG_PLAYER_SET_NAME_AND_TOWN_NAME = 1,
    PLAYER_FLAG_REDD_INTRODUCED = 2,
    PLAYER_FLAG_UNK_0x0_6 = 3,
    PLAYER_FLAG_UNK_0x0_7 = 4,
    PLAYER_FLAG_UNK_0x1_0 = 5,
    PLAYER_FLAG_FINISHED_FIRST_DAY = 6,
    PLAYER_FLAG_UNK_0x1_2 = 7,
    PLAYER_FLAG_UNK_0x1_3 = 8, // 5 bits
    PLAYER_FLAG_UNK_0x2_0 = 9,
    PLAYER_FLAG_UNK_0x2_1 = 10,
    PLAYER_FLAG_UNK_0x2_2 = 11,
    PLAYER_FLAG_UNK_0x2_3 = 12,
    PLAYER_FLAG_UNK_0x2_4 = 13,
    PLAYER_FLAG_UNK_0x2_5 = 14,
    PLAYER_FLAG_UNK_0x2_6 = 15,
    PLAYER_FLAG_UNK_0x2_7 = 16,
    PLAYER_FLAG_UNK_0x3_0 = 17,
    PLAYER_FLAG_UNK_0x3_1 = 18,
    PLAYER_FLAG_UNK_0x3_2 = 19,
    PLAYER_FLAG_TOM_NOOK_INTRODUCED = 20,
    PLAYER_FLAG_BLATHERS_INTRODUCED = 21,
    PLAYER_FLAG_UNK_0x3_5 = 22,
    PLAYER_FLAG_CELESTE_INTRODUCED = 23,
    PLAYER_FLAG_CYRUS_INTRODUCED = 24,
    PLAYER_FLAG_UNK_0x4_0 = 25,
    PLAYER_FLAG_UNK_0x4_1 = 26,
    PLAYER_FLAG_UNK_0x4_2 = 27,
    PLAYER_FLAG_UNK_0x4_3 = 28,
    PLAYER_FLAG_TOM_NOOK_DOWN_FIRST_PAYMENT = 29,
    PLAYER_FLAG_CHOOSE_HOUSE_LOCATION = 30,
    PLAYER_FLAG_MAIN_STREET_UNLOCKED = 31,
    PLAYER_FLAG_HOUSE_LOCATION_CHOSEN = 32,
    PLAYER_FLAG_KNOW_ISABELLE_NAME = 33,
    PLAYER_FLAG_KNOW_KAPPN = 34,
    PLAYER_FLAG_UNK_0x5_2 = 35,
    PLAYER_FLAG_UNK_0x5_3 = 36,
    PLAYER_FLAG_UNK_0x5_4 = 37,
    PLAYER_FLAG_UNK_0x5_5 = 38,
    PLAYER_FLAG_UNK_0x5_6 = 39,
    PLAYER_FLAG_UNK_0x5_7 = 40,
    PLAYER_FLAG_UNK_0x6_0 = 41,
    PLAYER_FLAG_UNK_0x6_1 = 42, // 4 bits
    PLAYER_FLAG_UNK_0x6_5 = 43,
    PLAYER_FLAG_UNK_0x6_6 = 44,
    PLAYER_FLAG_UNK_0x6_7 = 45,
    PLAYER_FLAG_UNK_0x7_0 = 46,
    PLAYER_FLAG_UNK_0x7_1 = 47,
    PLAYER_FLAG_UNK_0x7_2 = 48,
    PLAYER_FLAG_UNK_0x7_3 = 49,
    PLAYER_FLAG_UNK_0x7_4 = 50,
    PLAYER_FLAG_UNK_0x7_5 = 51,
    PLAYER_FLAG_UNK_0x7_6 = 52,
    PLAYER_FLAG_UNK_0x7_7 = 53,
    PLAYER_FLAG_UNK_0x8_0 = 54,
    PLAYER_FLAG_UNK_0x8_1 = 55,
    PLAYER_FLAG_UNK_0x8_2 = 56,
    PLAYER_FLAG_UNK_0x8_3 = 57,
    PLAYER_FLAG_UNK_0x8_4 = 58,
    PLAYER_FLAG_UNK_0x8_5 = 59,
    PLAYER_FLAG_UNK_0x8_6 = 60,
    PLAYER_FLAG_UNK_0x8_7 = 61,
    PLAYER_FLAG_UNK_0x9_0 = 62,
    PLAYER_FLAG_UNK_0x9_1 = 63,
    PLAYER_FLAG_UNK_0x9_2 = 64,
    PLAYER_FLAG_UNK_0x9_3 = 65,
    PLAYER_FLAG_UNK_0x9_4 = 66,
    PLAYER_FLAG_UNK_0x9_5 = 67, // 4 bits
    PLAYER_FLAG_RESET_PENDING = 68,
    PLAYER_FLAG_UNK_0xA_2 = 69, // 4 bits
    PLAYER_FLAG_UNK_0xA_6 = 70,
    PLAYER_FLAG_PETE_INTRODUCTION = 71,
    PLAYER_FLAG_UNK_0xB_0 = 72,
    PLAYER_FLAG_UNK_0xB_1 = 73,
    PLAYER_FLAG_UNK_0xB_2 = 74,
    PLAYER_FLAG_EXTERIOR_RENOVATIONS_UNLOCKED = 75,
    PLAYER_FLAG_HOUSE_LOAN_REPAID = 76,
    PLAYER_FLAG_UNK_0xB_5 = 77,
    PLAYER_FLAG_UNK_0xB_6 = 78,
    PLAYER_FLAG_HOUSE_UPGRADE_FINISHED = 79,
    PLAYER_FLAG_UNK_0xC_0 = 80, // 5 bits
    PLAYER_FLAG_MUSEUM_EXHIBIT_EXPLAINED = 81,
    PLAYER_FLAG_UNK_0xC_6 = 82,
    PLAYER_FLAG_UNK_0xC_7 = 83,
    PLAYER_FLAG_PERMIT_APPROVAL_ARRIVED = 84,
    PLAYER_FLAG_MAYOR_JOB_INTRODUCTION = 85,
    PLAYER_FLAG_PERMIT_APPROVAL = 86,
    PLAYER_FLAG_PERMIT_INTRODUCTION = 87,
    PLAYER_FLAG_CITIZEN_SATISFACTION_EXPLAINED = 88,
    PLAYER_FLAG_TOWN_FLAG_EXPLAINED = 89,
    PLAYER_FLAG_TOWN_TUNE_EXPLAINED = 90,
    PLAYER_FLAG_PWP_EXPLAINED = 91,
    PLAYER_FLAG_ORDINANCE_EXPLAINED = 92,
    PLAYER_FLAG_UNK_0xE_1 = 93,
    PLAYER_FLAG_UNK_0xE_2 = 94,
    PLAYER_FLAG_PERMIT_FINISHED = 95,
    PLAYER_FLAG_UNK_0xE_4 = 96,
    PLAYER_FLAG_UNK_0xE_5 = 97,
    PLAYER_FLAG_HAS_BEE_STING = 98,
    PLAYER_FLAG_UNK_0xE_7 = 99,
    PLAYER_FLAG_EDIT_DESIGN_INTRO = 100,
    PLAYER_FLAG_UNK_0xF_1 = 101,
    PLAYER_FLAG_UNK_0xF_2 = 102,
    PLAYER_FLAG_CAN_TRAVEL = 103,
    PLAYER_FLAG_HAS_TPC_PICTURE = 104,
    PLAYER_FLAG_UNK_0xF_5 = 105,
    PLAYER_FLAG_UNK_0xF_6 = 106,
    PLAYER_FLAG_UNK_0xF_7 = 107,
    PLAYER_FLAG_UNK_0x10_0 = 108,
    PLAYER_FLAG_UNK_0x10_1 = 109,
    PLAYER_FLAG_UNK_0x10_2 = 110,
    PLAYER_FLAG_BEFRIEND_SABLE = 111, // 4 bits
    PLAYER_FLAG_UNK_0x10_7 = 112,
    PLAYER_FLAG_BEFRIEND_SABLE3 = 113,
    PLAYER_FLAG_UNK_0x11_1 = 114,
    PLAYER_FLAG_UNK_0x11_2 = 115,
    PLAYER_FLAG_UNK_0x11_3 = 116,
    PLAYER_FLAG_UNK_0x11_4 = 117, // 4 bits
    PLAYER_FLAG_UNK_0x12_0 = 118,
    PLAYER_FLAG_UNK_0x12_1 = 119,
    PLAYER_FLAG_TALK_TO_LYLE_FOR_THE_FIRST_TIME = 120,
    PLAYER_FLAG_UNK_0x12_3 = 121,
    PLAYER_FLAG_UNK_0x12_4 = 122,
    PLAYER_FLAG_UNK_0x12_5 = 123,
    PLAYER_FLAG_UNK_0x12_6 = 124,
    PLAYER_FLAG_UNLOCKED_KAPPN = 125,
    PLAYER_FLAG_LYLE_INTRODUCTION = 126,
    PLAYER_FLAG_UNK_0x13_1 = 127,
    PLAYER_FLAG_RECEIVED_HHS_INTRO = 128,
    PLAYER_FLAG_UNK_0x13_3 = 129,
    PLAYER_FLAG_UNK_0x13_4 = 130,
    PLAYER_FLAG_UNK_0x13_5 = 131,
    PLAYER_FLAG_UNK_0x13_6 = 132,
    PLAYER_FLAG_UNK_0x13_7 = 133,
    PLAYER_FLAG_UNK_0x14_0 = 134,
    PLAYER_FLAG_UNK_0x14_1 = 135,
    PLAYER_FLAG_FINISHED_SHRUNK_SIGNATURES = 136,
    PLAYER_FLAG_UNK_0x14_3 = 137,
    PLAYER_FLAG_UNK_0x14_4 = 138,
    PLAYER_FLAG_UNK_0x14_5 = 139,
    PLAYER_FLAG_UNK_0x14_6 = 140,
    PLAYER_FLAG_UNK_0x14_7 = 141,
    PLAYER_FLAG_UNK_0x15_0 = 142,
    PLAYER_FLAG_UNK_0x15_1 = 143,
    PLAYER_FLAG_UNK_0x15_2 = 144,
    PLAYER_FLAG_UNK_0x15_3 = 145,
    PLAYER_FLAG_ISABELLE_WALLPAPER_GIFT_FOR_GETTING_HOUSE = 146,
    PLAYER_FLAG_KICK_INTRODUCED = 147,
    PLAYER_FLAG_UNK_0x15_6 = 148,
    PLAYER_FLAG_UNK_0x15_7 = 149,
    PLAYER_FLAG_UNK_0x16_0 = 150,
    PLAYER_FLAG_UNK_0x16_1 = 151,
    PLAYER_FLAG_UNK_0x16_2 = 152,
    PLAYER_FLAG_UNK_0x16_3 = 153,
    PLAYER_FLAG_UNK_0x16_4 = 154, // 7 bits
    PLAYER_FLAG_UNK_0x17_3 = 155, // 3 bits
    PLAYER_FLAG_UNK_0x17_6 = 156,
    PLAYER_FLAG_UNK_0x17_7 = 157, // 4 bits
    PLAYER_FLAG_UNK_0x18_3 = 158,
    PLAYER_FLAG_UNK_0x18_4 = 159,
    PLAYER_FLAG_UNK_0x18_5 = 160, // 4 bits
    PLAYER_FLAG_UNK_0x19_1 = 161,
    PLAYER_FLAG_UNK_0x19_2 = 162,
    PLAYER_FLAG_UNK_0x19_3 = 163,
    PLAYER_FLAG_UNK_0x19_4 = 164,
    PLAYER_FLAG_UNK_0x19_5 = 165,
    PLAYER_FLAG_UNK_0x19_6 = 166,
    PLAYER_FLAG_KNOWS_PERMIT_REQUIREMENTS = 167,
    PLAYER_FLAG_TOM_NOOK_TOLD_YOU_FIRST_PAYMENT = 168,
    PLAYER_FLAG_PERMIT_POINTS = 169, // 10 bits
    PLAYER_FLAG_UNK_0x1B_3 = 170,
    PLAYER_FLAG_UNK_0x1B_4 = 171, // 7 bits
    PLAYER_FLAG_UNK_0x1C_3 = 172, // 7 bits
    PLAYER_FLAG_UNK_0x1D_2 = 173, // 7 bits
    PLAYER_FLAG_UNK_0x1E_1 = 174, // 4 bits
    PLAYER_FLAG_UNK_0x1E_5 = 175, // 4 bits
    PLAYER_FLAG_UNK_0x1F_1 = 176,
    PLAYER_FLAG_HAS_CLUB_TORTIMER_MEMBERSHIP = 177,
    PLAYER_FLAG_CLUB_TORTIMER_FIRST_ASKED = 178,
    PLAYER_FLAG_CLUB_TORTIMER_RULES_EXPLAINED = 179,
    PLAYER_FLAG_UNK_0x1F_5 = 180,
    PLAYER_FLAG_UNK_0x1F_6 = 181,
    PLAYER_FLAG_UNK_0x1F_7 = 182,
    PLAYER_FLAG_UNK_0x20_0 = 183,
    PLAYER_FLAG_UNK_0x20_1 = 184,
    PLAYER_FLAG_UNK_0x20_2 = 185,
    PLAYER_FLAG_UNK_0x20_3 = 186,
    PLAYER_FLAG_UNK_0x20_4 = 187,
    PLAYER_FLAG_UNK_0x20_5 = 188,
    PLAYER_FLAG_UNK_0x20_6 = 189,
    PLAYER_FLAG_UNK_0x20_7 = 190,
    PLAYER_FLAG_UNK_0x21_0 = 191,
    PLAYER_FLAG_UNK_0x21_1 = 192,
    PLAYER_FLAG_UNK_0x21_2 = 193,
    PLAYER_FLAG_UNK_0x21_3 = 194,
    PLAYER_FLAG_UNK_0x21_4 = 195,
    PLAYER_FLAG_UNK_0x21_5 = 196, // 3 bits
    PLAYER_FLAG_UNK_0x22_0 = 197,
    PLAYER_FLAG_UNK_0x22_1 = 198, // 7 bits
    PLAYER_FLAG_UNK_0x23_0 = 199,
    PLAYER_FLAG_UNK_0x23_1 = 200,
    PLAYER_FLAG_UNK_0x23_2 = 201,
    PLAYER_FLAG_UNK_0x23_3 = 202,
    PLAYER_FLAG_UNK_0x23_4 = 203,
    PLAYER_FLAG_UNK_0x23_5 = 204,
    PLAYER_FLAG_UNK_0x23_6 = 205,
    PLAYER_FLAG_UNK_0x23_7 = 206,
    PLAYER_FLAG_UNK_0x24_0 = 207,
    PLAYER_FLAG_UNK_0x24_1 = 208,
    PLAYER_FLAG_UNK_0x24_2 = 209,
    PLAYER_FLAG_UNK_0x24_3 = 210,
    PLAYER_FLAG_UNK_0x24_4 = 211,
    PLAYER_FLAG_UNK_0x24_5 = 212,
    PLAYER_FLAG_UNK_0x24_6 = 213,
    PLAYER_FLAG_UNK_0x24_7 = 214,
    PLAYER_FLAG_UNK_0x25_0 = 215,
    PLAYER_FLAG_UNK_0x25_1 = 216,
    PLAYER_FLAG_UNK_0x25_2 = 217,
    PLAYER_FLAG_UNK_0x25_3 = 218,
    PLAYER_FLAG_UNK_0x25_4 = 219,
    PLAYER_FLAG_UNK_0x25_5 = 220, // 3 bits
    PLAYER_FLAG_UNK_0x26_0 = 221,
    PLAYER_FLAG_UNK_0x26_1 = 222,
    PLAYER_FLAG_UNK_0x26_2 = 223,
    PLAYER_FLAG_UNK_0x26_3 = 224,
    PLAYER_FLAG_UNK_0x26_4 = 225,
    PLAYER_FLAG_UNK_0x26_5 = 226,
    PLAYER_FLAG_UNK_0x26_6 = 227,
    PLAYER_FLAG_UNK_0x26_7 = 228,
    PLAYER_FLAG_UNK_0x27_0 = 229,
    PLAYER_FLAG_UNK_0x27_1 = 230,
    PLAYER_FLAG_SECRET_STORAGE_EXPLAINED = 231,
    PLAYER_FLAG_UNK_0x27_3 = 232,
    PLAYER_FLAG_UNK_0x27_4 = 233,
    PLAYER_FLAG_UNK_0x27_5 = 234,
    PLAYER_FLAG_BUILDING_SECRET_STORAGE = 235,
    PLAYER_FLAG_UNLOCKED_SECRET_STORAGE = 236,
    PLAYER_FLAG_UNK_0x28_0 = 237,
    PLAYER_FLAG_UNK_0x28_1 = 238,
    PLAYER_FLAG_UNK_0x28_2 = 239, // 10 bits
    PLAYER_FLAG_UNK_0x29_4 = 240, // 10 bits
    PLAYER_FLAG_UNK_0x2A_6 = 241,
    PLAYER_FLAG_UNK_0x2A_7 = 242,
    PLAYER_FLAG_UNK_0x2B_0 = 243,
    PLAYER_FLAG_UNK_0x2B_1 = 244,
    PLAYER_FLAG_UNK_0x2B_2 = 245,
    PLAYER_FLAG_UNK_0x2B_3 = 246,
    PLAYER_FLAG_UNK_0x2B_4 = 247,
    PLAYER_FLAG_UNK_0x2B_5 = 248,
    PLAYER_FLAG_UNK_0x2B_6 = 249,
    PLAYER_FLAG_UNK_0x2B_7 = 250,
    PLAYER_FLAG_UNK_0x2C_0 = 251,
    PLAYER_FLAG_UNK_0x2C_1 = 252, // 2 bits
    PLAYER_FLAG_UNLOCKED_HOUSE_EDITOR = 253,
    PLAYER_FLAG_UNK_0x2C_4 = 254,
    PLAYER_FLAG_UNK_0x2C_5 = 255,
    PLAYER_FLAG_UNK_0x2C_6 = 256,
    PLAYER_FLAG_RECEIVED_CAT_INTRO = 257,
    PLAYER_FLAG_UNK_0x2D_0 = 258,
    PLAYER_FLAG_UNK_0x2D_1 = 259,
    PLAYER_FLAG_UNK_0x2D_2 = 260,
    PLAYER_FLAG_UNK_0x2D_3 = 261,
    PLAYER_FLAG_UNK_0x2D_4 = 262,
    PLAYER_FLAG_UNK_0x2D_5 = 263,
    PLAYER_FLAG_UNK_0x2D_6 = 264,
    PLAYER_FLAG_UNK_0x2D_7 = 265,
    PLAYER_FLAG_UNK_0x2E_0 = 266,
    PLAYER_FLAG_UNK_0x2E_1 = 267,
    PLAYER_FLAG_UNK_0x2E_2 = 268,
    PLAYER_FLAG_UNK_0x2E_3 = 269,
    PLAYER_FLAG_UNK_0x2E_4 = 270,
    PLAYER_FLAG_UNK_0x2E_5 = 271,
    PLAYER_FLAG_UNK_0x2E_6 = 272,
    PLAYER_FLAG_UNK_0x2E_7 = 273,
    PLAYER_FLAG_UNK_0x2F_0 = 274,
    PLAYER_FLAG_UNK_0x2F_1 = 275,
    PLAYER_FLAG_UNK_0x2F_2 = 276,
    PLAYER_FLAG_UNK_0x2F_3 = 277,
    PLAYER_FLAG_UNK_0x2F_4 = 278,
    PLAYER_FLAG_UNK_0x2F_5 = 279,
    PLAYER_FLAG_CAN_USE_CENSUS_MENU = 280,
    PLAYER_FLAG_UNK_0x2F_7 = 281,
    PLAYER_FLAG_UNK_0x30_0 = 282,
    PLAYER_FLAG_UNK_0x30_1 = 283,
    PLAYER_FLAG_UNK_0x30_2 = 284,
    PLAYER_FLAG_UNK_0x30_3 = 285,
    PLAYER_FLAG_UNK_0x30_4 = 286,
    PLAYER_FLAG_UNK_0x30_5 = 287,
    PLAYER_FLAG_UNK_0x30_6 = 288,
    PLAYER_FLAG_UNK_0x30_7 = 289,
    PLAYER_FLAG_UNK_0x31_0 = 290,
    PLAYER_FLAG_UNK_0x31_1 = 291,
    PLAYER_FLAG_UNK_0x31_2 = 292,
    PLAYER_FLAG_UNK_0x31_3 = 293,
    PLAYER_FLAG_UNK_0x31_4 = 294,
    PLAYER_FLAG_UNK_0x31_5 = 295,
    PLAYER_FLAG_UNK_0x31_6 = 296,
    PLAYER_FLAG_UNK_0x31_7 = 297,
    PLAYER_FLAG_UNK_0x32_0 = 298,
    PLAYER_FLAG_UNK_0x32_1 = 299,
    PLAYER_FLAG_NUM = 300,
};

// 0x57A0 -> 0x57D3
#pragma pack(push, 1)
struct SvPlayerFlags {
    // the flag or the counter of the number (names are ours; index: a value of the enum before the struct, passed as u32
    // like in the original)
    u32 GetValue(u32 index) const; // 0x007101D4
    void SetValue(u32 index, u32 value); // 0x001E2258
    void SetFlag(u32 index, bool value); // 0x001E29C4

    /* 0x0000.0 */ u8 unk_0x0_0 : 4;
    /* 0x0000.4 */ u8 playerSetNameAndTownName : 1; // most likely player set their name and townname
    /* 0x0000.5 */ u8 reddIntroduced : 1;
    /* 0x0000.6 */ u8 unk_0x0_6 : 1;
    /* 0x0000.7 */ u8 unk_0x0_7 : 1;
    /* 0x0001.0 */ u8 unk_0x1_0 : 1;
    /* 0x0001.1 */ u8 finishedFirstDay : 1; // Introduction day finished
    /* 0x0001.2 */ u8 unk_0x1_2 : 1; // Meet Tommy or Timmy in shop for first time
    /* 0x0001.3 */ u8 unk_0x1_3 : 5;
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
    /* 0x0003.3 */ u8 tomNookIntroduced : 1;
    /* 0x0003.4 */ u8 blathersIntroduced : 1; // somehow also set tom nook introduced to 1???
    /* 0x0003.5 */ u8 unk_0x3_5 : 1;
    /* 0x0003.6 */ u8 celesteIntroduced : 1;
    /* 0x0003.7 */ u8 cyrusIntroduced : 1;
    /* 0x0004.0 */ u8 unk_0x4_0 : 1;
    /* 0x0004.1 */ u8 unk_0x4_1 : 1; // Reese and Tommy conversation (ReTail)
    /* 0x0004.2 */ u8 unk_0x4_2 : 1;
    /* 0x0004.3 */ u8 unk_0x4_3 : 1;
    /* 0x0004.4 */ u8 tomNookDownFirstPayment : 1;
    /* 0x0004.5 */ u8 chooseHouseLocation : 1; // Tom nook first time meeting -> finding place to build house
    /* 0x0004.6 */ u8 mainStreetUnlocked : 1; // go to tom nook after talking to isabelle in town hall at start of game
    /* 0x0004.7 */ u8 houseLocationChosen : 1; // Build House (Tom Nook placed down tent)
    /* 0x0005.0 */ u8 knowIsabelleName : 1;
    /* 0x0005.1 */ u8 knowKappn : 1;
    /* 0x0005.2 */ u8 unk_0x5_2 : 1; // after repaying loan in post office
    /* 0x0005.3 */ u8 unk_0x5_3 : 1; // set birthday in town hall / received TPC
    /* 0x0005.4 */ u8 unk_0x5_4 : 1;
    /* 0x0005.5 */ u8 unk_0x5_5 : 1;
    /* 0x0005.6 */ u8 unk_0x5_6 : 1;
    /* 0x0005.7 */ u8 unk_0x5_7 : 1;
    /* 0x0006.0 */ u8 unk_0x6_0 : 1; // Met Resetti for the first time
    /* 0x0006.1 */ u8 unk_0x6_1 : 4;
    /* 0x0006.5 */ u8 unk_0x6_5 : 1;
    /* 0x0006.6 */ u8 unk_0x6_6 : 1;
    /* 0x0006.7 */ u8 unk_0x6_7 : 1;
    /* 0x0007.0 */ u8 unk_0x7_0 : 1;
    /* 0x0007.1 */ u8 unk_0x7_1 : 1;
    /* 0x0007.2 */ u8 unk_0x7_2 : 1;
    /* 0x0007.3 */ u8 unk_0x7_3 : 1;
    /* 0x0007.4 */ u8 unk_0x7_4 : 1; // Meet Tommy or Timmy in shop for first time
    /* 0x0007.5 */ u8 unk_0x7_5 : 1;
    /* 0x0007.6 */ u8 unk_0x7_6 : 1;
    /* 0x0007.7 */ u8 unk_0x7_7 : 1;
    /* 0x0008.0 */ u8 unk_0x8_0 : 1;
    /* 0x0008.1 */ u8 unk_0x8_1 : 1;
    /* 0x0008.2 */ u8 unk_0x8_2 : 1;
    /* 0x0008.3 */ u8 unk_0x8_3 : 1; // Porter Introduction
    /* 0x0008.4 */ u8 unk_0x8_4 : 1; // i think got map from isabelle at start of game
    /* 0x0008.5 */ u8 unk_0x8_5 : 1;
    /* 0x0008.6 */ u8 unk_0x8_6 : 1;
    /* 0x0008.7 */ u8 unk_0x8_7 : 1;
    /* 0x0009.0 */ u8 unk_0x9_0 : 1;
    /* 0x0009.1 */ u8 unk_0x9_1 : 1;
    /* 0x0009.2 */ u8 unk_0x9_2 : 1;
    /* 0x0009.3 */ u8 unk_0x9_3 : 1;
    /* 0x0009.4 */ u8 unk_0x9_4 : 1;
    /* 0x0009.5 */ u16 unk_0x9_5 : 4;
    /* 0x000A.1 */ u8 resetPending : 1;
    /* 0x000A.2 */ u8 unk_0xA_2 : 4; // Villager related
    /* 0x000A.6 */ u8 unk_0xA_6 : 1; // isabelle welcomed you in your tent (actually also gets set when placing any item in house for first time, if never been in tent)
    /* 0x000A.7 */ u8 peteIntroduction : 1;
    /* 0x000B.0 */ u8 unk_0xB_0 : 1; // Reese and Tommy conversation (ReTail)
    /* 0x000B.1 */ u8 unk_0xB_1 : 1;
    /* 0x000B.2 */ u8 unk_0xB_2 : 1;
    /* 0x000B.3 */ u8 exteriorRenovationsUnlocked : 1; // If house is max size
    /* 0x000B.4 */ u8 houseLoanRepaid : 1; // after repaying loan in post office = 1 | / Talking to tom nook to let him know you payed your loan = 0
    /* 0x000B.5 */ u8 unk_0xB_5 : 1;
    /* 0x000B.6 */ u8 unk_0xB_6 : 1; // Gets set when you talk to tom nook after your house was built = 0
    /* 0x000B.7 */ u8 houseUpgradeFinished : 1; // next day when house upgrade is finished = 1 | when you talk to tom nook after your house was built = 0
    /* 0x000C.0 */ u8 unk_0xC_0 : 5;
    /* 0x000C.5 */ u8 museumExhibitExplained : 1;
    /* 0x000C.6 */ u8 unk_0xC_6 : 1;
    /* 0x000C.7 */ u8 unk_0xC_7 : 1;
    /* 0x000D.0 */ u8 permitApprovalArrived : 1; // day of arrival of permit
    /* 0x000D.1 */ u8 mayorJobIntroduction : 1; // after permit
    /* 0x000D.2 */ u8 permitApproval : 1; // if approval arrived
    /* 0x000D.3 */ u8 permitIntroduction : 1; // Isabelle told you about the mayor permit
    /* 0x000D.4 */ u8 citizenSatisfactionExplained : 1;
    /* 0x000D.5 */ u8 townFlagExplained : 1;
    /* 0x000D.6 */ u8 townTuneExplained : 1;
    /* 0x000D.7 */ u8 pwpExplained : 1;
    /* 0x000E.0 */ u8 ordinanceExplained : 1;
    /* 0x000E.1 */ u8 unk_0xE_1 : 1;
    /* 0x000E.2 */ u8 unk_0xE_2 : 1;
    /* 0x000E.3 */ u8 permitFinished : 1;
    /* 0x000E.4 */ u8 unk_0xE_4 : 1;
    /* 0x000E.5 */ u8 unk_0xE_5 : 1;
    /* 0x000E.6 */ u8 hasBeeSting : 1;
    /* 0x000E.7 */ u8 unk_0xE_7 : 1;
    /* 0x000F.0 */ u8 editDesignIntro : 1;
    /* 0x000F.1 */ u8 unk_0xF_1 : 1;
    /* 0x000F.2 */ u8 unk_0xF_2 : 1;
    /* 0x000F.3 */ u8 canTravel : 1; // i.e. can use train
    /* 0x000F.4 */ u8 hasTpcPicture : 1;
    /* 0x000F.5 */ u8 unk_0xF_5 : 1; // after first time asking porter to travel with no TPC
    /* 0x000F.6 */ u8 unk_0xF_6 : 1; // First time going in able sisters (Sable and mable)
    /* 0x000F.7 */ u8 unk_0xF_7 : 1;
    /* 0x0010.0 */ u8 unk_0x10_0 : 1;
    /* 0x0010.1 */ u8 unk_0x10_1 : 1;
    /* 0x0010.2 */ u8 unk_0x10_2 : 1;
    /* 0x0010.3 */ u8 befriendSable : 4; // 4-bit value (the research read bit 1 and bit 3 of it as befriendSable1/2)
    /* 0x0010.7 */ u8 unk_0x10_7 : 1;
    /* 0x0011.0 */ u8 befriendSable3 : 1;
    /* 0x0011.1 */ u8 unk_0x11_1 : 1;
    /* 0x0011.2 */ u8 unk_0x11_2 : 1;
    /* 0x0011.3 */ u8 unk_0x11_3 : 1; // Gets set when you talk to tom nook after your house was built
    /* 0x0011.4 */ u8 unk_0x11_4 : 4;
    /* 0x0012.0 */ u8 unk_0x12_0 : 1;
    /* 0x0012.1 */ u8 unk_0x12_1 : 1;
    /* 0x0012.2 */ u8 talkToLyleForTheFirstTime : 1;
    /* 0x0012.3 */ u8 unk_0x12_3 : 1;
    /* 0x0012.4 */ u8 unk_0x12_4 : 1; // Gets set when you talk to tom nook after your house was built
    /* 0x0012.5 */ u8 unk_0x12_5 : 1;
    /* 0x0012.6 */ u8 unk_0x12_6 : 1;
    /* 0x0012.7 */ u8 unlockedKappn : 1; // unlocked kappn boat
    /* 0x0013.0 */ u8 lyleIntroduction : 1;
    /* 0x0013.1 */ u8 unk_0x13_1 : 1;
    /* 0x0013.2 */ u8 receivedHhsIntro : 1;
    /* 0x0013.3 */ u8 unk_0x13_3 : 1; // after repaying loan in post office
    /* 0x0013.4 */ u8 unk_0x13_4 : 1;
    /* 0x0013.5 */ u8 unk_0x13_5 : 1; // Talked to tortimer
    /* 0x0013.6 */ u8 unk_0x13_6 : 1;
    /* 0x0013.7 */ u8 unk_0x13_7 : 1;
    /* 0x0014.0 */ u8 unk_0x14_0 : 1;
    /* 0x0014.1 */ u8 unk_0x14_1 : 1;
    /* 0x0014.2 */ u8 finishedShrunkSignatures : 1;
    /* 0x0014.3 */ u8 unk_0x14_3 : 1;
    /* 0x0014.4 */ u8 unk_0x14_4 : 1;
    /* 0x0014.5 */ u8 unk_0x14_5 : 1;
    /* 0x0014.6 */ u8 unk_0x14_6 : 1;
    /* 0x0014.7 */ u8 unk_0x14_7 : 1;
    /* 0x0015.0 */ u8 unk_0x15_0 : 1;
    /* 0x0015.1 */ u8 unk_0x15_1 : 1;
    /* 0x0015.2 */ u8 unk_0x15_2 : 1;
    /* 0x0015.3 */ u8 unk_0x15_3 : 1;
    /* 0x0015.4 */ u8 isabelleWallpaperGiftForGettingHouse : 1;
    /* 0x0015.5 */ u8 kickIntroduced : 1;
    /* 0x0015.6 */ u8 unk_0x15_6 : 1;
    /* 0x0015.7 */ u8 unk_0x15_7 : 1;
    /* 0x0016.0 */ u8 unk_0x16_0 : 1;
    /* 0x0016.1 */ u8 unk_0x16_1 : 1;
    /* 0x0016.2 */ u8 unk_0x16_2 : 1;
    /* 0x0016.3 */ u8 unk_0x16_3 : 1;
    /* 0x0016.4 */ u16 unk_0x16_4 : 7;
    /* 0x0017.3 */ u8 unk_0x17_3 : 3;
    /* 0x0017.6 */ u8 unk_0x17_6 : 1;
    /* 0x0017.7 */ u16 unk_0x17_7 : 4; // Gets set and unset at a new day
    /* 0x0018.3 */ u8 unk_0x18_3 : 1;
    /* 0x0018.4 */ u8 unk_0x18_4 : 1;
    /* 0x0018.5 */ u16 unk_0x18_5 : 4;
    /* 0x0019.1 */ u8 unk_0x19_1 : 1;
    /* 0x0019.2 */ u8 unk_0x19_2 : 1;
    /* 0x0019.3 */ u8 unk_0x19_3 : 1;
    /* 0x0019.4 */ u8 unk_0x19_4 : 1;
    /* 0x0019.5 */ u8 unk_0x19_5 : 1;
    /* 0x0019.6 */ u8 unk_0x19_6 : 1;
    /* 0x0019.7 */ u8 knowsPermitRequirements : 1; // Isabelle explained how to obtain mayor permit
    /* 0x001A.0 */ u8 tomNookToldYouFirstPayment : 1;
    /* 0x001A.1 */ u16 permitPoints : 10; // counter (the research read its bits as 1, 2, 4, ..., 512 points)
    /* 0x001B.3 */ u8 unk_0x1B_3 : 1;
    /* 0x001B.4 */ u16 unk_0x1B_4 : 7;
    /* 0x001C.3 */ u16 unk_0x1C_3 : 7;
    /* 0x001D.2 */ u16 unk_0x1D_2 : 7;
    /* 0x001E.1 */ u8 unk_0x1E_1 : 4; // Gets set and unset at a new day
    /* 0x001E.5 */ u16 unk_0x1E_5 : 4; // Gets set and unset at a new day
    /* 0x001F.1 */ u8 unk_0x1F_1 : 1;
    /* 0x001F.2 */ u8 hasClubTortimerMembership : 1;
    /* 0x001F.3 */ u8 clubTortimerFirstAsked : 1; // Kappn asks user for the first time
    /* 0x001F.4 */ u8 clubTortimerRulesExplained : 1;
    /* 0x001F.5 */ u8 unk_0x1F_5 : 1;
    /* 0x001F.6 */ u8 unk_0x1F_6 : 1;
    /* 0x001F.7 */ u8 unk_0x1F_7 : 1;
    /* 0x0020.0 */ u8 unk_0x20_0 : 1;
    /* 0x0020.1 */ u8 unk_0x20_1 : 1; // lyle talked to you at your house for becoming VIP
    /* 0x0020.2 */ u8 unk_0x20_2 : 1; // maybe after first day??
    /* 0x0020.3 */ u8 unk_0x20_3 : 1;
    /* 0x0020.4 */ u8 unk_0x20_4 : 1;
    /* 0x0020.5 */ u8 unk_0x20_5 : 1;
    /* 0x0020.6 */ u8 unk_0x20_6 : 1;
    /* 0x0020.7 */ u8 unk_0x20_7 : 1;
    /* 0x0021.0 */ u8 unk_0x21_0 : 1;
    /* 0x0021.1 */ u8 unk_0x21_1 : 1;
    /* 0x0021.2 */ u8 unk_0x21_2 : 1; // maybe donated fossil?
    /* 0x0021.3 */ u8 unk_0x21_3 : 1;
    /* 0x0021.4 */ u8 unk_0x21_4 : 1;
    /* 0x0021.5 */ u8 unk_0x21_5 : 3;
    /* 0x0022.0 */ u8 unk_0x22_0 : 1; // Island People Here Tutorial
    /* 0x0022.1 */ u8 unk_0x22_1 : 7;
    /* 0x0023.0 */ u8 unk_0x23_0 : 1; // Tommy/Timmy first time selling item
    /* 0x0023.1 */ u8 unk_0x23_1 : 1;
    /* 0x0023.2 */ u8 unk_0x23_2 : 1;
    /* 0x0023.3 */ u8 unk_0x23_3 : 1; // Isabelle second advice? (Letters)
    /* 0x0023.4 */ u8 unk_0x23_4 : 1; // Isabelle second advice? (Letters)
    /* 0x0023.5 */ u8 unk_0x23_5 : 1; // Isabelle third advice (Beach)
    /* 0x0023.6 */ u8 unk_0x23_6 : 1; // Isabelle third advice (Beach)
    /* 0x0023.7 */ u8 unk_0x23_7 : 1; // Isabelle fourth advice (Timmy and Tommy)
    /* 0x0024.0 */ u8 unk_0x24_0 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.1 */ u8 unk_0x24_1 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.2 */ u8 unk_0x24_2 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.3 */ u8 unk_0x24_3 : 1;
    /* 0x0024.4 */ u8 unk_0x24_4 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.5 */ u8 unk_0x24_5 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.6 */ u8 unk_0x24_6 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.7 */ u8 unk_0x24_7 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0025.0 */ u8 unk_0x25_0 : 1;
    /* 0x0025.1 */ u8 unk_0x25_1 : 1; // Isabelle fourth advice? (Timmy and Tommy)
    /* 0x0025.2 */ u8 unk_0x25_2 : 1;
    /* 0x0025.3 */ u8 unk_0x25_3 : 1; // Isabelle ask first advice (Talk to new villager)
    /* 0x0025.4 */ u8 unk_0x25_4 : 1;
    /* 0x0025.5 */ u8 unk_0x25_5 : 3;
    /* 0x0026.0 */ u8 unk_0x26_0 : 1;
    /* 0x0026.1 */ u8 unk_0x26_1 : 1;
    /* 0x0026.2 */ u8 unk_0x26_2 : 1;
    /* 0x0026.3 */ u8 unk_0x26_3 : 1;
    /* 0x0026.4 */ u8 unk_0x26_4 : 1;
    /* 0x0026.5 */ u8 unk_0x26_5 : 1;
    /* 0x0026.6 */ u8 unk_0x26_6 : 1;
    /* 0x0026.7 */ u8 unk_0x26_7 : 1;
    /* 0x0027.0 */ u8 unk_0x27_0 : 1;
    /* 0x0027.1 */ u8 unk_0x27_1 : 1; // Nintendo 3DS Image Share used for the first time
    /* 0x0027.2 */ u8 secretStorageExplained : 1; // Tom nook tells you what the secret storage is
    /* 0x0027.3 */ u8 unk_0x27_3 : 1; // Found magic lamp for the first time
    /* 0x0027.4 */ u8 unk_0x27_4 : 1; // Talked to wisp for the first time
    /* 0x0027.5 */ u8 unk_0x27_5 : 1; // isabelle wants to hold ceremony for becoming major | received tpc?
    /* 0x0027.6 */ u8 buildingSecretStorage : 1; // Next day secret storage will be built (1 = will be built -> afterwards 0 when built)
    /* 0x0027.7 */ u8 unlockedSecretStorage : 1;
    /* 0x0028.0 */ u8 unk_0x28_0 : 1;
    /* 0x0028.1 */ u8 unk_0x28_1 : 1; // Talking to harvey for the first time
    /* 0x0028.2 */ u16 unk_0x28_2 : 10;
    /* 0x0029.4 */ u16 unk_0x29_4 : 10;
    /* 0x002A.6 */ u8 unk_0x2A_6 : 1;
    /* 0x002A.7 */ u8 unk_0x2A_7 : 1;
    /* 0x002B.0 */ u8 unk_0x2B_0 : 1;
    /* 0x002B.1 */ u8 unk_0x2B_1 : 1;
    /* 0x002B.2 */ u8 unk_0x2B_2 : 1;
    /* 0x002B.3 */ u8 unk_0x2B_3 : 1; // Related to scanning amiibo at wisp lamp
    /* 0x002B.4 */ u8 unk_0x2B_4 : 1;
    /* 0x002B.5 */ u8 unk_0x2B_5 : 1; // Related to finishing first day
    /* 0x002B.6 */ u8 unk_0x2B_6 : 1; // after talking to cat in train at beginning of game
    /* 0x002B.7 */ u8 unk_0x2B_7 : 1;
    /* 0x002C.0 */ u8 unk_0x2C_0 : 1;
    /* 0x002C.1 */ u8 unk_0x2C_1 : 2;
    /* 0x002C.3 */ u8 unlockedHouseEditor : 1;
    /* 0x002C.4 */ u8 unk_0x2C_4 : 1;
    /* 0x002C.5 */ u8 unk_0x2C_5 : 1; // talking to harvey for the first time
    /* 0x002C.6 */ u8 unk_0x2C_6 : 1;
    /* 0x002C.7 */ u8 receivedCatIntro : 1; // at the townhall station
    /* 0x002D.0 */ u8 unk_0x2D_0 : 1;
    /* 0x002D.1 */ u8 unk_0x2D_1 : 1;
    /* 0x002D.2 */ u8 unk_0x2D_2 : 1;
    /* 0x002D.3 */ u8 unk_0x2D_3 : 1;
    /* 0x002D.4 */ u8 unk_0x2D_4 : 1;
    /* 0x002D.5 */ u8 unk_0x2D_5 : 1;
    /* 0x002D.6 */ u8 unk_0x2D_6 : 1;
    /* 0x002D.7 */ u8 unk_0x2D_7 : 1;
    /* 0x002E.0 */ u8 unk_0x2E_0 : 1;
    /* 0x002E.1 */ u8 unk_0x2E_1 : 1;
    /* 0x002E.2 */ u8 unk_0x2E_2 : 1;
    /* 0x002E.3 */ u8 unk_0x2E_3 : 1;
    /* 0x002E.4 */ u8 unk_0x2E_4 : 1;
    /* 0x002E.5 */ u8 unk_0x2E_5 : 1;
    /* 0x002E.6 */ u8 unk_0x2E_6 : 1;
    /* 0x002E.7 */ u8 unk_0x2E_7 : 1;
    /* 0x002F.0 */ u8 unk_0x2F_0 : 1;
    /* 0x002F.1 */ u8 unk_0x2F_1 : 1;
    /* 0x002F.2 */ u8 unk_0x2F_2 : 1;
    /* 0x002F.3 */ u8 unk_0x2F_3 : 1;
    /* 0x002F.4 */ u8 unk_0x2F_4 : 1;
    /* 0x002F.5 */ u8 unk_0x2F_5 : 1;
    /* 0x002F.6 */ u8 canUseCensusMenu : 1;
    /* 0x002F.7 */ u8 unk_0x2F_7 : 1;
    /* 0x0030.0 */ u8 unk_0x30_0 : 1; // finished cat entry (First intro one?)
    /* 0x0030.1 */ u8 unk_0x30_1 : 1; // finished cat entry (Second Intro one)
    /* 0x0030.2 */ u8 unk_0x30_2 : 1; // finished all intro cat entries?
    /* 0x0030.3 */ u8 unk_0x30_3 : 1;
    /* 0x0030.4 */ u8 unk_0x30_4 : 1; // TPC explained (Intro)
    /* 0x0030.5 */ u8 unk_0x30_5 : 1;
    /* 0x0030.6 */ u8 unk_0x30_6 : 1;
    /* 0x0030.7 */ u8 unk_0x30_7 : 1;
    /* 0x0031.0 */ u8 unk_0x31_0 : 1;
    /* 0x0031.1 */ u8 unk_0x31_1 : 1;
    /* 0x0031.2 */ u8 unk_0x31_2 : 1;
    /* 0x0031.3 */ u8 unk_0x31_3 : 1;
    /* 0x0031.4 */ u8 unk_0x31_4 : 1;
    /* 0x0031.5 */ u8 unk_0x31_5 : 1; // Related to finishing first day
    /* 0x0031.6 */ u8 unk_0x31_6 : 1;
    /* 0x0031.7 */ u8 unk_0x31_7 : 1;
    /* 0x0032.0 */ u8 unk_0x32_0 : 1;
    /* 0x0032.1 */ u8 unk_0x32_1 : 1; // CAT explained in TPC (Intro)
    /* 0x0032.2 */ u8 unk_0x32_2 : 1;
    /* 0x0032.3 */ u8 unk_0x32_3 : 1;
    /* 0x0032.4 */ u8 unk_0x32_4 : 1;
    /* 0x0032.5 */ u8 unk_0x32_5 : 1;
    /* 0x0032.6 */ u8 unk_0x32_6 : 1;
    /* 0x0032.7 */ u8 unk_0x32_7 : 1;
    /* 0x0033.0 */ u8 unk_0x33_0 : 1;
    /* 0x0033.1 */ u8 unk_0x33_1 : 1;
    /* 0x0033.2 */ u8 unk_0x33_2 : 1;
    /* 0x0033.3 */ u8 unk_0x33_3 : 1;
    /* 0x0033.4 */ u8 unk_0x33_4 : 1;
    /* 0x0033.5 */ u8 unk_0x33_5 : 1;
    /* 0x0033.6 */ u8 unk_0x33_6 : 1;
    /* 0x0033.7 */ u8 unk_0x33_7 : 1;
};
#pragma pack(pop)
ASSERT_SIZE(SvPlayerFlags, 0x34);

