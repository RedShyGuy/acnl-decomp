#pragma once

// Town wide progress flags.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include <cstring>

// The value IDs of SvTownFlags::GetValue/SetValue/SetFlag (names after the fields; a value of several bits is a
// counter or a small number)
enum SvTownFlagId : u8 {
    TOWN_FLAG_UNK_0x0_0 = 0,
    TOWN_FLAG_EARLY_BIRD_ORDINANCE = 1,
    TOWN_FLAG_NIGHT_OWL_ORDINANCE = 2,
    TOWN_FLAG_BELL_BOOM_ORDINANCE = 3,
    TOWN_FLAG_BEAUTIFUL_ORDINANCE = 4,
    TOWN_FLAG_UNK_0x0_5 = 5, // 3 bits
    TOWN_FLAG_UNK_0x1_0 = 6,
    TOWN_FLAG_UNK_0x1_1 = 7,
    TOWN_FLAG_UNK_0x1_2 = 8,
    TOWN_FLAG_UNK_0x1_3 = 9,
    TOWN_FLAG_UNK_0x1_4 = 10,
    TOWN_FLAG_UNK_0x1_5 = 11,
    TOWN_FLAG_UNK_0x1_6 = 12,
    TOWN_FLAG_UNK_0x1_7 = 13, // 5 bits
    TOWN_FLAG_UNK_0x2_4 = 14,
    TOWN_FLAG_UNK_0x2_5 = 15,
    TOWN_FLAG_UNK_0x2_6 = 16,
    TOWN_FLAG_UNK_0x2_7 = 17,
    TOWN_FLAG_UNK_0x3_0 = 18,
    TOWN_FLAG_UNK_0x3_1 = 19,
    TOWN_FLAG_UNK_0x3_2 = 20,
    TOWN_FLAG_UNK_0x3_3 = 21,
    TOWN_FLAG_UNK_0x3_4 = 22,
    TOWN_FLAG_UNK_0x3_5 = 23,
    TOWN_FLAG_UNK_0x3_6 = 24,
    TOWN_FLAG_UNK_0x3_7 = 25,
    TOWN_FLAG_UNK_0x4_0 = 26,
    TOWN_FLAG_UNK_0x4_1 = 27,
    TOWN_FLAG_UNK_0x4_2 = 28,
    TOWN_FLAG_UNK_0x4_3 = 29,
    TOWN_FLAG_UNK_0x4_4 = 30, // 3 bits
    TOWN_FLAG_UNK_0x4_7 = 31, // 4 bits
    TOWN_FLAG_UNK_0x5_3 = 32, // 6 bits
    TOWN_FLAG_UNK_0x6_1 = 33,
    TOWN_FLAG_UNK_0x6_2 = 34,
    TOWN_FLAG_UNK_0x6_3 = 35, // 4 bits
    TOWN_FLAG_QR_MACHINE_UNLOCKED = 36,
    TOWN_FLAG_UNK_0x7_0 = 37,
    TOWN_FLAG_UNK_0x7_1 = 38,
    TOWN_FLAG_UNK_0x7_2 = 39,
    TOWN_FLAG_UNK_0x7_3 = 40, // 10 bits
    TOWN_FLAG_UNK_0x8_5 = 41,
    TOWN_FLAG_UNK_0x8_6 = 42,
    TOWN_FLAG_UNK_0x8_7 = 43,
    TOWN_FLAG_UNK_0x9_0 = 44, // 3 bits
    TOWN_FLAG_UNK_0x9_3 = 45,
    TOWN_FLAG_UNK_0x9_4 = 46, // 10 bits
    TOWN_FLAG_UNK_0xA_6 = 47,
    TOWN_FLAG_OPEN_PUBLIC_WORK_PROJECT = 48,
    TOWN_FLAG_UNK_0xB_0 = 49,
    TOWN_FLAG_UNK_0xB_1 = 50, // 3 bits
    TOWN_FLAG_UNK_0xB_4 = 51,
    TOWN_FLAG_UNK_0xB_5 = 52,
    TOWN_FLAG_UNK_0xB_6 = 53,
    TOWN_FLAG_UNK_0xB_7 = 54,
    TOWN_FLAG_UNK_0xC_0 = 55,
    TOWN_FLAG_UNK_0xC_1 = 56, // 6 bits
    TOWN_FLAG_UNK_0xC_7 = 57,
    TOWN_FLAG_HHS_STAND_UNLOCKED = 58,
    TOWN_FLAG_UNK_0xD_1 = 59,
    TOWN_FLAG_UNK_0xD_2 = 60,
    TOWN_FLAG_UNK_0xD_3 = 61,
    TOWN_FLAG_UNK_0xD_4 = 62,
    TOWN_FLAG_UNK_0xD_5 = 63,
    TOWN_FLAG_UNK_0xD_6 = 64,
    TOWN_FLAG_UNK_0xD_7 = 65,
    TOWN_FLAG_UNK_0xE_0 = 66,
    TOWN_FLAG_UNK_0xE_1 = 67,
    TOWN_FLAG_UNK_0xE_2 = 68,
    TOWN_FLAG_UNK_0xE_3 = 69,
    TOWN_FLAG_UNK_0xE_4 = 70,
    TOWN_FLAG_UNK_0xE_5 = 71,
    TOWN_FLAG_UNK_0xE_6 = 72,
    TOWN_FLAG_UNK_0xE_7 = 73,
    TOWN_FLAG_UNK_0xF_0 = 74,
    TOWN_FLAG_UNK_0xF_1 = 75,
    TOWN_FLAG_UNK_0xF_2 = 76,
    TOWN_FLAG_UNK_0xF_3 = 77,
    TOWN_FLAG_UNK_0xF_4 = 78,
    TOWN_FLAG_UNK_0xF_5 = 79,
    TOWN_FLAG_UNK_0xF_6 = 80,
    TOWN_FLAG_UNK_0xF_7 = 81,
    TOWN_FLAG_UNK_0x10_0 = 82,
    TOWN_FLAG_UNK_0x10_1 = 83,
    TOWN_FLAG_UNK_0x10_2 = 84,
    TOWN_FLAG_NUM = 85,
};

#pragma pack(push, 1)
struct SvTownFlags {
    // (inline in the constructor of the save data) cleared, then Init
    SvTownFlags()
    {
        std::memset(static_cast<void*>(this), 0, sizeof(*this));
        Init();
    }
    // (game code, not decompiled yet; name is ours; also called when the town data is cleared)
    void Init(); // 0x006F485C
    // the flag or the counter of the number (names are ours; index: a value of the enum before the struct, passed as u32
    // like in the original)
    u32 GetValue(u32 index) const; // 0x00771494
    void SetValue(u32 index, u32 value); // 0x006F4E7C
    void SetFlag(u32 index, bool value); // 0x006F51C4
    // (inline) one more in the counter of 10 bits that starts at bit BIT (at most 1000; name is ours)
    template <u32 BIT>
    void AddCounter10()
    {
        u8* bytes = reinterpret_cast<u8*>(this) + BIT / 8;
        const u32 shift = BIT % 8;
        u32 value = (bytes[0] >> shift) + ((bytes[1] & ((1 << (shift + 2)) - 1)) << (8 - shift));
        value = value + 1 > 1000 ? 1000 : value + 1;
        bytes[0] = (bytes[0] & ((1 << shift) - 1)) | (value << shift);
        bytes[1] = (bytes[1] & ~((1 << (shift + 2)) - 1)) | ((value & (0x3FF & ~((1 << (8 - shift)) - 1))) >> (8 - shift));
    }

    /* 0x0000.0 */ u8 unk_0x0_0 : 1;
    /* 0x0000.1 */ u8 earlyBirdOrdinance : 1;
    /* 0x0000.2 */ u8 nightOwlOrdinance : 1;
    /* 0x0000.3 */ u8 bellBoomOrdinance : 1;
    /* 0x0000.4 */ u8 beautifulOrdinance : 1;
    /* 0x0000.5 */ u8 unk_0x0_5 : 3;
    /* 0x0001.0 */ u8 unk_0x1_0 : 1;
    /* 0x0001.1 */ u8 unk_0x1_1 : 1;
    /* 0x0001.2 */ u8 unk_0x1_2 : 1; // gulliver woken up?
    /* 0x0001.3 */ u8 unk_0x1_3 : 1; // second player created?
    /* 0x0001.4 */ u8 unk_0x1_4 : 1;
    /* 0x0001.5 */ u8 unk_0x1_5 : 1;
    /* 0x0001.6 */ u8 unk_0x1_6 : 1;
    /* 0x0001.7 */ u16 unk_0x1_7 : 5;
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
    /* 0x0004.0 */ u8 unk_0x4_0 : 1;
    /* 0x0004.1 */ u8 unk_0x4_1 : 1;
    /* 0x0004.2 */ u8 unk_0x4_2 : 1;
    /* 0x0004.3 */ u8 unk_0x4_3 : 1;
    /* 0x0004.4 */ u8 unk_0x4_4 : 3; // ctor sets this bit
    /* 0x0004.7 */ u16 unk_0x4_7 : 4;
    /* 0x0005.3 */ u16 unk_0x5_3 : 6;
    /* 0x0006.1 */ u8 unk_0x6_1 : 1;
    /* 0x0006.2 */ u8 unk_0x6_2 : 1;
    /* 0x0006.3 */ u8 unk_0x6_3 : 4;
    /* 0x0006.7 */ u8 qrMachineUnlocked : 1;
    /* 0x0007.0 */ u8 unk_0x7_0 : 1;
    /* 0x0007.1 */ u8 unk_0x7_1 : 1;
    /* 0x0007.2 */ u8 unk_0x7_2 : 1;
    /* 0x0007.3 */ u16 unk_0x7_3 : 10;
    /* 0x0008.5 */ u8 unk_0x8_5 : 1;
    /* 0x0008.6 */ u8 unk_0x8_6 : 1;
    /* 0x0008.7 */ u8 unk_0x8_7 : 1; // gets set when you talked to isabelle after she wants to build the dream suite
    /* 0x0009.0 */ u8 unk_0x9_0 : 3; // ctor sets this bit
    /* 0x0009.3 */ u8 unk_0x9_3 : 1;
    /* 0x0009.4 */ u16 unk_0x9_4 : 10;
    /* 0x000A.6 */ u8 unk_0xA_6 : 1; // someone new moved in, isabelle mentions it, then its set to 0 again
    /* 0x000A.7 */ u8 openPublicWorkProject : 1;
    /* 0x000B.0 */ u8 unk_0xB_0 : 1;
    /* 0x000B.1 */ u8 unk_0xB_1 : 3;
    /* 0x000B.4 */ u8 unk_0xB_4 : 1;
    /* 0x000B.5 */ u8 unk_0xB_5 : 1;
    /* 0x000B.6 */ u8 unk_0xB_6 : 1;
    /* 0x000B.7 */ u8 unk_0xB_7 : 1;
    /* 0x000C.0 */ u8 unk_0xC_0 : 1;
    /* 0x000C.1 */ u8 unk_0xC_1 : 6;
    /* 0x000C.7 */ u8 unk_0xC_7 : 1;
    /* 0x000D.0 */ u8 hhsStandUnlocked : 1; // lyles stand in tom nooks shop
    /* 0x000D.1 */ u8 unk_0xD_1 : 1;
    /* 0x000D.2 */ u8 unk_0xD_2 : 1;
    /* 0x000D.3 */ u8 unk_0xD_3 : 1;
    /* 0x000D.4 */ u8 unk_0xD_4 : 1;
    /* 0x000D.5 */ u8 unk_0xD_5 : 1;
    /* 0x000D.6 */ u8 unk_0xD_6 : 1; // found magic lamp?
    /* 0x000D.7 */ u8 unk_0xD_7 : 1;
    /* 0x000E.0 */ u8 unk_0xE_0 : 1;
    /* 0x000E.1 */ u8 unk_0xE_1 : 1;
    /* 0x000E.2 */ u8 unk_0xE_2 : 1;
    /* 0x000E.3 */ u8 unk_0xE_3 : 1;
    /* 0x000E.4 */ u8 unk_0xE_4 : 1;
    /* 0x000E.5 */ u8 unk_0xE_5 : 1;
    /* 0x000E.6 */ u8 unk_0xE_6 : 1;
    /* 0x000E.7 */ u8 unk_0xE_7 : 1; // this might be related to the last census menu entry
    /* 0x000F.0 */ u8 unk_0xF_0 : 1; // this might be related to the last census menu entry
    /* 0x000F.1 */ u8 unk_0xF_1 : 1;
    /* 0x000F.2 */ u8 unk_0xF_2 : 1;
    /* 0x000F.3 */ u8 unk_0xF_3 : 1;
    /* 0x000F.4 */ u8 unk_0xF_4 : 1; // its snowing/raining?
    /* 0x000F.5 */ u8 unk_0xF_5 : 1;
    /* 0x000F.6 */ u8 unk_0xF_6 : 1;
    /* 0x000F.7 */ u8 unk_0xF_7 : 1;
    /* 0x0010.0 */ u8 unk_0x10_0 : 1;
    /* 0x0010.1 */ u8 unk_0x10_1 : 1;
    /* 0x0010.2 */ u8 unk_0x10_2 : 1;
    /* 0x0010.3 */ u8 unk_0x10_3 : 1;
    /* 0x0010.4 */ u8 unk_0x10_4 : 1;
    /* 0x0010.5 */ u8 unk_0x10_5 : 1;
    /* 0x0010.6 */ u8 unk_0x10_6 : 1;
    /* 0x0010.7 */ u8 unk_0x10_7 : 1;
    /* 0x0011 */ u8 unk_0x11; // (cleared with the flags by the constructor)
};
#pragma pack(pop)
ASSERT_SIZE(SvTownFlags, 0x12);

