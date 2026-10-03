#pragma once

// nn::fs::ipc::FileSystem - the commands of the FS:USER session (3dbrew "Filesystem services").
// Paths are passed as (type, data, size); archives are named by the u64 handle from OpenArchive.
// The member name is ours.

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/fs/fs_ExtSaveDataSpecifier.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace ipc {
class FileSystem
{
public:
    explicit FileSystem(nn::Handle session) : mSession(session) {}

    nn::Result OpenArchive(u64* archive, u32 archiveId, u32 pathType, const u8* path, size_t pathSize); // 0x0013044C | nintendogs:bytes [tier A]
    nn::Result CloseArchive(u64 archive); // 0x001304AC | nintendogs:callgraph [tier A]
    nn::Result OpenFileDirectly(nn::Handle* file, nn::fs::Transaction transaction, u32 archiveId, u32 archivePathType,
                                const u8* archivePath, size_t archivePathSize, u32 pathType, const u8* path, size_t pathSize,
                                u32 openFlags, nn::fs::Attributes attributes); // 0x001304DC | nintendogs:bytes [tier A]
    nn::Result SetPriority(s32 priority); // 0x00136214 | nintendogs:bytes [tier A]
    nn::Result GetPriority(s32* priority); // 0x0013AEE0 | nintendogs:bytes [tier A]
    nn::Result InitializeWithSdkVersion(u32 version); // 0x0013AF18 | nintendogs:bytes [tier A]
    nn::Result CreateFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize,
                          nn::fs::Attributes attributes, s64 size); // 0x00348C30 | nintendogs:bytes [tier A]
    nn::Result DeleteFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize); // 0x00348C8C | nintendogs:bytes [tier A]
    nn::Result RenameFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize,
                          u64 newArchive, u32 newPathType, const u8* newPath, size_t newPathSize); // 0x00348CDC | fefates:bytes [tier B]
    nn::Result GetFreeBytes(s64* freeBytes, u64 archive); // 0x00348D6C (name after 3dbrew)
    nn::Result OpenDirectory(nn::Handle* directory, u64 archive, u32 pathType, const u8* path, size_t pathSize); // 0x00348DA8 | nintendogs:bytes [tier A]
    nn::Result ControlArchive(u64 archive, u32 action, const void* input, size_t inputSize, void* output, size_t outputSize); // 0x00348E0C | nintendogs:bytes [tier A]
    nn::Result FormatSaveData(u32 archiveId, u32 pathType, const u8* path, size_t pathSize, u32 blocks, u32 directories,
                              u32 files, u32 directoryBuckets, u32 fileBuckets, bool duplicateData); // 0x00348E78 | nintendogs:bytes [tier A]
    nn::Result IsSdmcDetected(bool* detected); // 0x00348ED0 | nintendogs:bytes [tier A]
    nn::Result IsSdmcWritable(bool* writable); // 0x00348F08 | nintendogs:bytes [tier A]
    nn::Result CreateDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize,
                               nn::fs::Attributes attributes); // 0x00348F40 | nintendogs:bytes [tier B]
    nn::Result DeleteDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize); // 0x00348F94 | nintendogs:bytes [tier B]
    nn::Result RenameDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path, size_t pathSize,
                               u64 newArchive, u32 newPathType, const u8* newPath, size_t newPathSize); // 0x00348FE4 | manual:3dbrew [tier B]
    nn::Result CreateExtSaveData(const nn::fs::ExtSaveDataSpecifier& specifier, u32 directories, u32 files, s64 sizeLimit,
                                 const void* smdh, size_t smdhSize); // 0x00349074 | nintendogs:bytes [tier A]
    nn::Result DeleteExtSaveData(const nn::fs::ExtSaveDataSpecifier& specifier); // 0x003490D0 | nintendogs:bytes [tier A]
    nn::Result GetArchivePriority(s32* priority, u64 archive); // 0x0034910C (name after 3dbrew)
    nn::Result GetArchiveResource(nn::fs::ArchiveResource* resource, nn::fs::SystemMediaType mediaType); // 0x00349148 | nintendogs:bytes [tier A]
    nn::Result SetArchivePriority(u64 archive, s32 priority); // 0x00349198 | manual:3dbrew [tier B]
    nn::Result GetExtDataBlockSize(s64* totalBlocks, s64* freeBlocks, s32* blockSize,
                                   const nn::fs::ExtSaveDataSpecifier& specifier); // 0x003491D0 | nintendogs:bytes [tier B]
    // 3dbrew shows 5 parameter words, the binary sends a 6th (a byte; meaning unknown)
    nn::Result SetSaveArchiveSecureValue(u64 archive, u32 slot, u64 value, bool unknown); // 0x0034923C | manual:3dbrew [tier B]
    nn::Result DeleteDirectoryRecursively(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path,
                                          size_t pathSize); // 0x0034928C | manual:3dbrew [tier B]
    nn::Result GetThisSaveDataSecureValue(bool* exists, bool* isGameCard, u64* value, u32 slot); // 0x003492DC (name after 3dbrew)
    nn::Result OpenFile(nn::Handle* file, nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path,
                        size_t pathSize, u32 openFlags, nn::fs::Attributes attributes); // 0x00349334 | nintendogs:bytes [tier A]

private:
    nn::Handle mSession;
};
} // namespace ipc
} // namespace fs
} // namespace nn
