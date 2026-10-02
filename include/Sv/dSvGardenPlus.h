#pragma once

// The whole save file garden_plus.dat.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvNpc.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvShared.h"
#include "Sv/dSvStrc.h"
#include "Sv/dSvTown.h"

#pragma pack(push, 1)

struct SvMinigameData {
    /* 0x0000 */ u32 checksum; // Checksum of the 0x28F0 of this data
    /* 0x0004 */ u8 unk_0x4[0x28F0];
};
ASSERT_SIZE(SvMinigameData, 0x28F4);

struct SvGardenDataUnk52BB0 {
    /* 0x0000 */ u32 checksum; // Checksum of the 0x7F0 of this data
    /* 0x0004 */ u8 unk_0x4[0x7F0];
};
ASSERT_SIZE(SvGardenDataUnk52BB0, 0x7F4);

struct SvSecureValueHeader {
    /* 0x0000 */ u64 secureValue; // Unused in ACNL WA
    /* 0x0008 */ u32 saveInitialized; // Has to be exactly 1
    /* 0x000C */ u8 pad_0xC[0x74]; // Always 0
};
ASSERT_SIZE(SvSecureValueHeader, 0x80);

struct SvSaveHeader {
    /* 0x0000 */ u32 headerChecksum; // Checksum of the next 0x1C of header data
    /* 0x0004 */ u16 saveVerifier1; // Always 0x009E; 0x00F8 pre-WA
    /* 0x0006 */ u8 saveVerifier2; // Has to be exactly 0x2; 0x2 pre-WA
    /* 0x0007 */ u8 pad_0x7[0x19]; // Always 0
};
ASSERT_SIZE(SvSaveHeader, 0x20);

struct SvGardenData {
    /* 0x0000 */ SvSaveHeader header; // garden_plus.dat + 0x80
    /* 0x0020 */ SvPlayer players[4]; // garden_plus.dat + 0xA0
    /* 0x29220 */ SvNpcData npcData; // garden_plus.dat + 0x292A0
    /* 0x4BE00 */ SvStrcData strcData; // garden_plus.dat + 0x4BE80
    /* 0x502BC */ SvMinigameData minigameData; // garden_plus.dat + 0x5033C / WA exclusive
    /* 0x52BB0 */ SvGardenDataUnk52BB0 unk_0x52BB0; // garden_plus.dat + 0x52C30 / research name: UnkData / WA exclusive
    /* 0x533A4 */ SvTownData townData; // garden_plus.dat + 0x53424
    /* 0x71880 */ SvSharedData sharedData; // garden_plus.dat + 0x71900
};
ASSERT_SIZE(SvGardenData, 0x89A80);

struct SvGardenPlus {
    /* 0x0000 */ SvSecureValueHeader secureValue;
    /* 0x0080 */ SvGardenData data;
};
ASSERT_SIZE(SvGardenPlus, 0x89B00);

#pragma pack(pop)
