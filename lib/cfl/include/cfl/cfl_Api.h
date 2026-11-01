#pragma once

// CFL, Nintendo's face library: the Mii database (CFL_DB.dat in the shared extdata 0xF000000B), the resource
// archive (CFL_Res.dat in the system content 0x0004009B00010202) and the drawing of Mii models and icons.
// Function names come from the reference symbols (nintendogs, mk7dlp); names without a parameter list there have C
// linkage. All other names are ours and marked so.

#include "cfl/cfl_Types.h"
#include "nn/fnd/fnd_ExpHeapTemplate.h"
#include "nn/os/os_LockPolicy_NoLock_LockObject.h"

typedef nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::NoLock> CFLiHeap;

// the sections of the resource archive (u16 count, u16 size, offset into the archive; name is ours)
struct CFLiResSection
{
    u16 num;     // 0x0
    u16 size;    // 0x2
    u32 offset;  // 0x4
};
ASSERT_SIZE(CFLiResSection, 0x8);

// the loaded resource archive (name is ours)
struct CFLiResLoader
{
    CFLiResSection sections[20];  // 0x00
    const u8* data;               // 0xA0
    u32 size;                     // 0xA4
    bool cached;                  // 0xA8
    u8 _A9[3];                    // 0xA9
    u16 version;                  // 0xAC
    u32 _B0;                      // 0xB0
    u32 _B4;                      // 0xB4
};
ASSERT_SIZE(CFLiResLoader, 0xB8);

typedef void (*CFLiDrawDoneCallback)();

// the state of CFL at the start of the work buffer (name is ours)
struct CFLiManager
{
    CFLiHeap* heap;             // 0x00 the rest of the work buffer
    CFLiResLoader resLoader;      // 0x04
    u32 _BC;                      // 0xBC
    bool working;                 // 0xC0 an asynchronous operation runs
    bool _C1;                     // 0xC1 (argument of CFL_InitRes)
    u32 flags;                    // 0xC4 bit n: file n broken
    s32 status;                   // 0xC8 result of the asynchronous operation
    s32 reason;                   // 0xCC
    s32 savedStatus;              // 0xD0
    s32 savedReason;              // 0xD4
    CFLiDrawDoneCallback modelDrawDoneCallback;  // 0xD8
    CFLiDrawDoneCallback iconDrawDoneCallback;   // 0xDC
};
ASSERT_SIZE(CFLiManager, 0xE0);

// the random generator of CFL (a linear congruential generator; name is ours)
struct CFLiRandom
{
    u32 value;       // 0x0
    u32 multiplier;  // 0x4
    u32 increment;   // 0x8
};

// globals (names are ours)
extern bool s_HDMode;
extern s32 s_AsyncStatus;
extern s32 s_LastReason;
extern u32 s_Flags;
extern CFLiManager* s_pManager;
extern u64 s_UserID;
extern CFLiRandom s_Random;
extern bool s_EnableSpecialMii;

// the six default Miis (the official data of CFL; name is ours)
struct CFLiOfficialData
{
    u32 num;                       // 0x000
    CFLiMiiDataOfficial data[6];   // 0x004
};
ASSERT_SIZE(CFLiOfficialData, 0x22C);

// how the default Miis are read (a table of functions; names are ours)
struct CFLiOfficialDataInterface
{
    u32 (*GetNum)(const CFLiOfficialData* official);
    const CFLiMiiDataOfficial* (*GetData)(const CFLiOfficialData* official, int index);
};
extern CFLiOfficialData s_OfficialData;
extern const CFLiOfficialDataInterface s_OfficialDataInterface;

extern "C" {
// manager
bool CFL_IsAvailable();
s32 CFL_InitRes(void* work, const void* res, u32 resSize, bool hd);
u32 CFL_GetWorkSize(bool hd);
void CFL_Exit();
void CFLi_Exit(bool keepDatabase);  // 0x0012F738 (name is ours)
bool CFL_FreeCachedResource();
bool CFL_IsResourceCached();
bool CFLi_GetFileBroken(int file);
void CFLi_SetFileBroken(int file);
bool CFLi_GetWorking();
void CFLi_SetWorking(bool working);
void CFLi_EndWorkingReason(s32 status, s32 reason);
void CFLi_SetIconDrawDoneCallback(CFLiDrawDoneCallback callback);
void CFLi_SetModelDrawDoneCallback(CFLiDrawDoneCallback callback);
bool CFLi_IsHDModeEnabled();

// Mii data
bool CFLi_IsHomeAuthorID(const CFLiAuthorID* id);
bool CFLi_IsNTRMiiID(const CFLiCreateID* id);
bool CFLi_IsNormalMiiID(const CFLiCreateID* id);  // 0x00155658 (name is ours) bit 31 of the ID: not special
bool CFLi_IsNullAuthorID(const CFLiAuthorID* id);
bool CFLi_IsNullMiiID(const CFLiCreateID* id);
bool CFLi_IsTemporaryMiiID(const CFLiCreateID* id);
bool CFLi_IsValidMiiID(const CFLiCreateID* id);
bool CFLi_IsSameMiiID(const CFLiCreateID* a, const CFLiCreateID* b);
bool CFLi_IsValidStoreDataInner(const CFLStoreData* data);
void CFLi_GetDefaultCreateIDonCTR(CFLiCreateID* id, int index);
void CFLi_GetDefaultMiiDataCore(CFLiMiiDataCore* data, int index);
void CFLi_ClearCreatorName(CFLiMiiDataOfficial* data);
void CFLi_GetStoreDataFromPackedData(CFLStoreData* store, const CFLiMiiDataCore* core, bool withBirthday,
                                     const u16* creatorName);
void CFLi_SwapRFLMiiDataEndian(void* dst, const void* src, u32 size);
void CFLi_UnpackMiiDataCore(CFLiCharInfo* info, const CFLiMiiDataCore* data, bool withBirthday,
                            const u16* creatorName);
void CFLi_PackMiiDataCore(CFLiMiiDataCore* data, u16* creatorName, const CFLiCharInfo* info, bool withBirthday);
bool CFLi_UnpackRFLMiiDataCore(CFLiCharInfo* info, const CFLiRFLMiiData* data, const u16* creatorName);
bool CFLi_VerifyCharInfo(const CFLiCharInfo* info, bool checkName);
bool CFLi_ReplaceProhibitedCharInfo(CFLiCharInfo* info);
void CFLi_EnableSpecialMii(u32 key);
u32 CFLi_GetFacelineRGBA8(int color);
void CFLi_GetAdditionalInfo(CFLAdditionalInfo* out, const CFLiCharInfo* info, bool checkFont);
s32 CFL_GetAdditionalInfo(CFLAdditionalInfo* out, int source, const void* data, u16 index, bool checkFont);
} // extern "C"

// (C++ linkage in the reference symbols)
CFLiManager* CFLi_GetManager();
void* CFLi_AllocateMemory(u32 size, s32 alignment);  // 0x0013DEEC (name is ours)
void CFLi_FreeMemory(void* ptr);
s32 CFL_GetAsyncStatus();
s32 CFL_WaitAsync();
s32 CFL_GetLastReason();
void CFLi_Initialize(void* work, u32 size);  // 0x0017D234 (name is ours)
u32 CFLi_GetWorkSizeWithoutRes();           // 0x00175A9C (name is ours)
bool CFLi_Get_C1();                         // 0x001726D4 (name is ours)

u16 CFLi_GetCRC16(int crc, const void* data, unsigned size);
void CFLi_PutCRC16(int crc, void* data, unsigned size);
void CFLi_CopyString16(unsigned short* dst, const unsigned short* src, int length);
int CFLi_GetStringLength8(const char* str);
int CFLi_SearchCharacter8(const char* str, char c);
bool CFLi_IsNumberCharCode(unsigned short c);
int CFLi_GetMonthOfDay(int month);
void CFLi_GetNowDateTime(int* year, int* month, int* day, int* hour, int* minute, int* second);
const CFLiAuthorID* CFLi_GetUserID();
void* CFLi_AllocateAlignedBuffer(void** buffer, unsigned* bufferSize, unsigned size);
