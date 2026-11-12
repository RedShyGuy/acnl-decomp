#pragma once

#include "decomp.h"

struct SvGardenPlus;

// The save data in memory and the parts of it the save code writes (game code; all names are ours). The functions
// here are declared for the save I/O; their files are not decompiled yet.

// the save data (g_SvSaveMemory.pData; another copy of GetGardenPlus)
SvGardenPlus* SvGetSaveMemory(); // 0x005CB7DC
// the save data, after the check of the save I/O on the whole file (another copy)
SvGardenPlus* SvGetSaveMemoryChecked(); // 0x00300E20
// the part of size bytes at offset of the save file is copied into the copy of the save data (g_SvSaveMemory.pCopy),
// whose checksums are calculated again; the part in the copy (it is written from there; data itself if there is no
// copy or the part lies outside the file)
void* SvPrepareSaveWrite(const void* data, u32 size, u32 offset); // 0x005CB158
// the secure value at the start of the save data goes up by one (skipping 0)
void SvIncrementSecureValue(); // 0x005CB118
// the secure value of the save data is the one of the save data archive (nn::fs::CheckSaveDataSecureValue)
bool SvCheckSecureValue(); // 0x005CB0FC
// the copy of the save data that the writes are made from is made / freed (in the stage heap)
void SvCreateSaveCopy(); // 0x005CB1D8
void SvDestroySaveCopy(); // 0x005CB238
// the CRC-32C of the block from data up to base + size is the one in *checksum (which counts as 0 for it; isLogged:
// passed, not used)
DECOMP_NOINLINE bool SvIsCrcValid(const void* base, const void* data, u32* checksum, u32 size, bool isLogged); // 0x005CB0A8
// ... is written into *checksum
DECOMP_NOINLINE void SvUpdateCrc(const void* base, const void* data, u32* checksum, u32 size); // 0x005CB9C4
// the save data is cleared
void SvClearSaveData(SvGardenPlus* data); // 0x005CB7EC
// (game code of this file, not decompiled yet; called with the save data)
// a new secure value and everything cleared
void Func5CB7F8(SvGardenPlus* data);
// the save data is allocated in the heap of g_SvSaveMemory and built, then reset as above (nothing if it is there)
void SvAllocateSaveMemory(); // 0x005CB84C
void Func5CB274(SvGardenPlus* data);
void Func5CBAA0(SvGardenPlus* data);
void Func5CB4B4(SvGardenPlus* data);
// (after loads: the parts of the game that depend on the town data; names after the addresses)
void Func5CB2EC();
void Func5CB49C();
void Func5CB690();
void Func5CB7A4();
void Func5CB7CC();
struct SvSaveMemoryUnk8;
SvSaveMemoryUnk8* SvGetSaveMemoryUnk8(); // 0x005CB0EC
// 158 (SvSaveHeader::saveVerifier1)
u16 SvGetSaveVerifier(); // 0x005CB110

// the offsets of the parts in the save file
u32 SvGetGardenDataOffset(); // 0x00607C70
u32 SvGetNpcDataOffset(); // 0x00607EA4
u32 SvGetPlayerOffset(u32 saveNo); // 0x00608060
u32 SvGetSharedDataOffset(); // 0x006080C4
// (the same offset, calculated the other way round)
u32 SvGetSharedDataOffset2(); // 0x00608168
u32 SvGetMinigameDataOffset(); // 0x005CB490
u32 SvGetUnk52BB0Offset(); // 0x005CB798
// the size of the part of the save data that is written (0x89A60: SvGardenData and the shared data without its last 0x20 bytes)
u32 SvGetGardenDataSaveSize(); // 0x00758C0C

// the part of the shared data at +0x24 (game code 0x2D4xxx): its address and offset in the save file
void* SvGetSharedUnk24(); // 0x002D43E4
u32 SvGetSharedUnk24Offset(); // 0x002D4518

// the checksums of the parts before they are saved (the prepare functions of the save kinds of SaveProcess; names are
// ours; dSvChecksum.cpp)
// saveInitialized = 1 and all checksums
void SvPrepareSaveAll(); // 0x00758C18
void SvUpdateNpcDataChecksum(); // 0x00119850
void SvUpdateSharedUnk0Checksum(); // 0x0071015C
void SvUpdateCensusChecksum(); // 0x0071CD68
void SvUpdateMinigameDataChecksum(); // 0x0071E660
void SvUpdateSharedUnk24Checksum(); // 0x00720894
void SvUpdateUnk52BB0Checksum(); // 0x00720AE4
void SvUpdateSharedDataChecksums(); // 0x0070FA8C
