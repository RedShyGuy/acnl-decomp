#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
// --- fs_Api.cpp ---
// connects to FS:USER (once) and sets up the user file system
void Initialize(); // 0x0012FDA0 | nintendogs:bytes-fuzzy [tier A]
bool IsInitialized(); // 0x003460CC | tier C (confirmed by the code)
nn::Result GetPriority(s32* priority); // 0x0013614C | nintendogs:bytes [tier A]
bool IsSdmcInserted(); // 0x00346288 | nintendogs:callgraph [tier A]
bool IsSdmcWritable(); // 0x003462C0 (name is ours, after the FS command)
nn::Result DeleteExtSaveData(u64 saveId); // 0x0034667C | nintendogs:bytes [tier A]
nn::Result GetSharedExtSaveDataBlockSize(s64* totalBlocks, s64* freeBlocks, s32* blockSize, u32 saveId); // 0x00346934 | nintendogs:bytes [tier B]

// --- the functions that use the globals of fs_UserFileSystem.cpp (0x00975F20) ---
void InitializeLatencyEmulation(); // 0x0011D228 | fefates:bytes [tier B]
nn::Result Unmount(const char* path); // 0x00136254 | nintendogs:callgraph [tier A]
nn::Result MountSharedExtSaveData(const char* name, u32 saveId); // 0x00123B20 | tier C (confirmed by the code)
nn::Result MountSaveData(const char* name); // 0x003460E4 | nintendogs:callseq [tier A]
nn::Result CommitSaveData(const char* name); // 0x003461B0 | fefates:bytes [tier B]
nn::Result FormatSaveData(u32 maxFiles, u32 maxDirectories, bool duplicateData); // 0x0034620C | nintendogs:callseq [tier A]
nn::Result MountExtSaveData(const char* name, u64 saveId); // 0x0034654C | nintendogs:callseq [tier A]
nn::Result CreateExtSaveData(u64 saveId, const void* smdh, size_t smdhSize, u32 maxDirectories, u32 maxFiles); // 0x003465E8 | nintendogs:bytes-fuzzy [tier A]
nn::Result MountSpecialArchive(const char* name, u32 archiveId); // 0x003466EC | tier C (confirmed by the code)
nn::Result MountSdmc(const char* name); // 0x0034992C | tier C (confirmed by the code)
nn::Result GetFileSystemSizeCore(s64* totalSize, s64* freeSize, nn::fs::SystemMediaType mediaType); // 0x00346744 | nintendogs:bytes [tier A]
nn::Result GetSdmcSize(s64* totalSize, s64* freeSize); // 0x0034673C | tier C (confirmed by the code)
nn::Result GetTwlPhotoSize(s64* totalSize, s64* freeSize); // 0x003463AC | tier C (confirmed by the code)
// the buffer MountContent needs without the cache
size_t GetContentRequiredMemorySize(nn::fs::MediaType mediaType, u64 programId, u32 contentIndex, u32 maxFiles, u32 maxDirectories); // 0x0034691C | fefates:bytes [tier B]

// the save data of another title via archive 0x567890B4 (3dbrew); tries the game card, then the
// SD card (names are ours)
nn::Result MountAccessibleSaveData(const char* name, u32 saveId); // 0x00346160 (name is ours)
nn::Result MountAccessibleSaveData(const char* name, nn::fs::MediaType mediaType, u32 saveId, u8 unknown); // 0x003467C0 (name is ours)
// the secure value of a mounted save data (FS:SetSaveArchiveSecureValue; names are ours)
nn::Result SetSaveArchiveSecureValue(const char* name, u32 slot, u64 value, bool unknown); // 0x003468C4 (name is ours)
nn::Result SetSaveDataSecureValue(const char* name, u64 value); // 0x00346990 (name is ours)
// whether value is the secure value of this title's save data (true if there is none or it is on a
// game card; FS:GetThisSaveDataSecureValue; name is ours)
bool CheckSaveDataSecureValue(u64 value); // 0x003469A8 (name is ours)

// --- RomFs ---
// the buffer MountRom needs (path: which image of the program)
s32 GetRomRequiredMemorySizeImpl(u32 maxFiles, u32 maxDirectories, bool useCache, const nn::fs::CTR::ProgramDataPath& path); // 0x001290D8 | fefates:bytes [tier B]
s32 GetRomRequiredMemorySize(u32 maxFiles, u32 maxDirectories, bool useCache); // 0x0012FE60 | fefates:bytes [tier B]
s32 GetRomPatchRequiredMemorySize(u32 maxFiles, u32 maxDirectories, bool useCache); // 0x0011D27C (name is ours)
// mounts the program's RomFS ("rom:" or name); errors are fatal
nn::Result MountRom(u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x0013059C | fefates:bytes [tier B]
nn::Result MountRom(const char* name, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x001363B4 | fefates:bytes [tier B]
// the same for the patch image (ACNL: "patch:")
nn::Result MountRomPatch(const char* name, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x0011D184 (name is ours)
// mounts a content of a title (e.g. add-on content)
nn::Result MountContent(const char* name, nn::fs::MediaType mediaType, u64 programId, u32 contentIndex, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x0034601C | fefates:bytes [tier B]
} // namespace fs
} // namespace nn
