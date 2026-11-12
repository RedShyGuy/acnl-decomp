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
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvMinigameData(); // 0x002BFB40

    // the checksum of the data behind it (names are ours; isLogged: passed, not used)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x0071E67C
    DECOMP_NOINLINE bool IsChecksumValid(); // 0x0071E6A0

    /* 0x0000 */ u32 checksum; // Checksum of the 0x28F0 of this data
    /* 0x0004 */ u8 unk_0x4[0x28F0];
};
ASSERT_SIZE(SvMinigameData, 0x28F4);

struct SvGardenDataUnk52BB0 {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvGardenDataUnk52BB0(); // 0x002D4B60

    // the checksum of the data behind it (names are ours; isLogged: passed, not used)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x00720B00
    DECOMP_NOINLINE bool IsChecksumValid(); // 0x00720B20

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
    // the verifiers of this version (name is ours)
    DECOMP_NOINLINE void Initialize(); // 0x005CBA34
    // the verifiers have the values of this version (names are ours)
    DECOMP_NOINLINE bool IsSaveVerifier2Valid(); // 0x00758CDC
    DECOMP_NOINLINE bool IsSaveVerifier1Valid(); // 0x00758CF4
    // the checksum of the data behind it (names are ours; isLogged: passed, not used)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x00758D18
    DECOMP_NOINLINE bool IsChecksumValid(); // 0x00758D3C

    /* 0x0000 */ u32 headerChecksum; // Checksum of the next 0x1C of header data
    /* 0x0004 */ u16 saveVerifier1; // Always 0x009E; 0x00F8 pre-WA
    /* 0x0006 */ u8 saveVerifier2; // Has to be exactly 0x2; 0x2 pre-WA
    /* 0x0007 */ u8 pad_0x7[0x19]; // Always 0
};
ASSERT_SIZE(SvSaveHeader, 0x20);

struct SvGardenData {
    // the parts are built (the shared data is built by the caller, SvAllocateSaveMemory)
    SvGardenData(); // 0x005CBF6C
    // the checksums of all parts; all checksums are right and the region of the town is the one of the console (names
    // are ours)
    DECOMP_NOINLINE void UpdateChecksums(); // 0x00758F24
    DECOMP_NOINLINE bool IsValid(bool isLogged); // 0x00758F94
    // the villagers
    SvNpcData* GetNpcData(); // 0x002FB32C (name is ours)
    // (game code of the file of the save data in memory; names are ours) everything is cleared / set up again after
    // a load / the play time goes up (at most to the largest time) / the time 0x1AE6E becomes the current date
    void Clear(); // 0x005CBD3C
    void Func5CBCAC(); // 0x005CBCAC
    void AddPlaytime(s64 time); // 0x005CBA50
    DECOMP_NOINLINE void Func5CBAB8(); // 0x005CBAB8
    // the parts of a player in the town are removed (saveNo 0..3)
    void Func5CBADC(u32 saveNo); // 0x005CBADC
    void Func5CBB30(u32 saveNo); // 0x005CBB30

    /* 0x0000 */ SvSaveHeader header; // garden_plus.dat + 0x80
    /* 0x0020 */ SvPlayerData players; // garden_plus.dat + 0xA0
    /* 0x29220 */ SvNpcData npcData; // garden_plus.dat + 0x292A0
    /* 0x4BE00 */ SvStrcData strcData; // garden_plus.dat + 0x4BE80
    /* 0x502BC */ SvMinigameData minigameData; // garden_plus.dat + 0x5033C / WA exclusive
    /* 0x52BB0 */ SvGardenDataUnk52BB0 unk_0x52BB0; // garden_plus.dat + 0x52C30 / research name: UnkData / WA exclusive
    /* 0x533A4 */ SvTownData townData; // garden_plus.dat + 0x53424
};
ASSERT_SIZE(SvGardenData, 0x71880);

struct SvGardenPlus {
    // the checksums of the garden data and the shared data / all of them are right (names are ours)
    DECOMP_NOINLINE void UpdateChecksums(); // 0x00758C38
    DECOMP_NOINLINE bool IsValid(bool isLogged); // 0x00758C58
    // the header has the verifiers of this version (name is ours)
    DECOMP_NOINLINE bool IsVersionValid(); // 0x00758BE0

    /* 0x0000 */ SvSecureValueHeader secureValue;
    /* 0x0080 */ SvGardenData data;
    /* 0x71900 */ SvSharedData sharedData; // (not a part of SvGardenData: its constructor and checksums stop before it)
};
ASSERT_SIZE(SvGardenPlus, 0x89B00);

#pragma pack(pop)
