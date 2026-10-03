#include "nn/fs/fs_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/MPCore/detail/detail_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_ContentRomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_File.h"
#include "nn/fs/CTR/fs_ArchivePaths.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_ExtSaveDataSpecifier.h"
#include "nn/fs/ipc/ipc_FileSystem.h"
#include "nn/fslow/fslow_Api.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

#include <new>
#include <string.h>

namespace nn {
namespace fs {
namespace {

using CTR::MPCore::detail::ArchivePath;
using CTR::MPCore::detail::FileServerArchive;
using CTR::MPCore::detail::IArchive;

// results; the names are ours
const bit32 RESULT_SRV_ALREADY_INITIALIZED = 0x08A067F9;    // info, invalid state, srv, 1017
const bit32 RESULT_NOT_INITIALIZED = 0xE0A046DB;            // usage, invalid state, fs, 731: no Initialize
const bit32 RESULT_ALREADY_MOUNTED = 0xC92044E7;            // status, summary 9, fs, 231: the save data is mounted
const bit32 RESULT_SAVE_DATA_NOT_FOUND = 0xC8804464;        // status, not found, fs, 100
const bit32 RESULT_EXT_SAVE_DATA_NOT_FOUND = 0xC8804482;    // status, not found, fs, 130
const bit32 RESULT_ROM_NOT_READABLE = 0xF96047A2;           // fatal, internal, fs, 930: GetRomRequiredMemorySize failed

const char PORT_NAME[] = "fs:USER";

// archive ids (3dbrew "Filesystem services")
const u32 ARCHIVE_ID_SAVE_DATA = 0x00000004;
const u32 ARCHIVE_ID_SDMC = 0x00000009;

// ArchivePath type (3dbrew "FS Path")
const u32 PATH_TYPE_EMPTY = 1;

// the high word of the save ids of shared extra save data
const u64 SHARED_EXT_SAVE_DATA_ID_HIGH = 0x0004800000000000ULL;

// FormatSaveData: the blocks of a save data
const u32 SAVE_DATA_BLOCKS = 512;
// ControlArchive: commit the save data (3dbrew "FS:ControlArchive" action 0)
const u32 CONTROL_ARCHIVE_COMMIT = 0;
// the secure value slot of the save data
const u32 SECURE_VALUE_SLOT = 0x1000;

// 0x00975F15 (names are ours)
CTR::MPCore::detail::UserFileSystem s_UserFileSystem;
// 0x00975F18
nn::Handle s_Session;
// 0x00975F1C
detail::FileSystemBase s_FileSystemBase;

// errors 220 to 229 of the fs module mean that there is no such extra save data (inline)
nn::Result ConvertExtSaveDataResult(nn::Result result)
{
    if (result.GetModule() == 17 && result.GetDescription() >= 220 && result.GetDescription() <= 229) {
        return nn::Result(RESULT_EXT_SAVE_DATA_NOT_FOUND);
    }
    return result;
}

// the program's own image (3dbrew "Filesystem services")
const u32 ARCHIVE_ID_SELF_NCCH = 0x00000003;
// ArchivePath type (3dbrew "FS Path")
const u32 PATH_TYPE_BINARY = 2;
// the ProgramDataPath type that "patch:" opens
const u32 PROGRAM_DATA_PATH_TYPE_PATCH = 5;

// opens the program's image of path as a RomFs archive (inline; name is ours)
nn::Result OpenRomArchive(IArchive** archive, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache,
                          const nn::fs::CTR::ProgramDataPath& path)
{
    CTR::MPCore::detail::ContentRomFsArchive* created;
    nn::Result result = CTR::MPCore::detail::ContentRomFsArchive::Create(&created, maxFiles, maxDirectories, buffer, bufferSize, useCache, path);
    if (result.IsFailure()) {
        return result;
    }
    *archive = created;
    return nn::Result();
}

// a title id of add-on content (high word 0x0004008C, 3dbrew "Titles"; inline, name is ours)
bool IsAddOnContent(u64 programId)
{
    u32 high = static_cast<u32>(programId >> 32);
    return (high >> 14) == 0x10 && (high & 0xFFFF) == 0x8C;
}

// enters an opened archive under name, destroys it if that fails (inline)
nn::Result RegisterOrDelete(const char* name, IArchive* archive)
{
    nn::Result result = CTR::MPCore::detail::RegisterArchive(name, archive, false, false);
    if (result.IsFailure()) {
        archive->DeleteObject();
    }
    return result;
}

} // namespace

// 0x00123B20 | tier C (confirmed by the code)
nn::Result MountSharedExtSaveData(const char* name, u32 saveId)
{
    nn::fs::ExtSaveDataSpecifier specifier;
    specifier.Make(MEDIA_TYPE_NAND, saveId);
    IArchive* archive;
    nn::Result result = CTR::MPCore::detail::OpenSharedExtSaveData(&archive, specifier);
    if (result.IsFailure()) {
        return result;
    }
    return RegisterOrDelete(name, archive);
}

// 0x0012FDA0 | nintendogs:bytes-fuzzy [tier A]
void Initialize()
{
    if (s_Session.IsValid()) {
        return;
    }
    nn::Result result = nn::srv::Initialize();
    if (result != nn::Result(RESULT_SRV_ALREADY_INITIALIZED)) {
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(nn::srv::GetServiceHandle(&s_Session, PORT_NAME, strlen(PORT_NAME), 0));
    CTR::MPCore::detail::UserFileSystem::Initialize(s_Session);
    s_FileSystemBase.Initialize(&s_UserFileSystem);
    detail::RegisterGlobalFileSystemBase(s_FileSystemBase);
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetIpcFileSystem().SetPriority(0));
}

// 0x0013614C | nintendogs:bytes [tier A]
nn::Result GetPriority(s32* priority)
{
    return detail::GetIpcFileSystem().GetPriority(priority);
}

// 0x0013AF50 | nintendogs:bytes [tier A]
DECOMP_NOINLINE nn::fs::ipc::FileSystem detail::GetIpcFileSystem()
{
    if (!s_Session.IsValid()) {
        nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_NOT_INITIALIZED), nn::err::CTR::GetCurrentAddress());
    }
    return nn::fs::ipc::FileSystem(s_Session);
}

// 0x003460CC | tier C (confirmed by the code)
bool IsInitialized()
{
    return s_Session.IsValid();
}

// 0x003460E4 | nintendogs:callseq [tier A]
nn::Result MountSaveData(const char* name)
{
    if (CTR::MPCore::detail::s_pSaveDataArchive != 0) {
        return nn::Result(RESULT_ALREADY_MOUNTED);
    }
    IArchive* archive;
    nn::Result result = CTR::MPCore::detail::OpenSpecialArchiveRaw(&archive, ARCHIVE_ID_SAVE_DATA);
    if (result.IsFailure()) {
        return result;
    }
    result = CTR::MPCore::detail::RegisterArchive(name, archive, false, false);
    if (result.IsSuccess()) {
        CTR::MPCore::detail::s_pSaveDataArchive = archive;
    } else {
        archive->DeleteObject();
    }
    return result;
}

// 0x00346160 (name is ours)
nn::Result MountAccessibleSaveData(const char* name, u32 saveId)
{
    nn::Result result = MountAccessibleSaveData(name, MEDIA_TYPE_GAME_CARD, saveId, 0);
    if (result.IsFailure()) {
        result = MountAccessibleSaveData(name, MEDIA_TYPE_SDMC, saveId, 0);
        if (result.IsFailure()) {
            result = nn::Result(RESULT_SAVE_DATA_NOT_FOUND);
        }
    }
    return result;
}

// 0x003461B0 | fefates:bytes [tier B]
nn::Result CommitSaveData(const char* name)
{
    IArchive* archive = CTR::MPCore::detail::FindArchive(name);
    if (archive == 0) {
        return nn::Result(CTR::MPCore::detail::RESULT_ARCHIVE_NOT_FOUND);
    }
    u8 input;
    u8 output;
    return nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session)
        .ControlArchive(static_cast<FileServerArchive*>(archive)->GetArchiveHandle(), CONTROL_ARCHIVE_COMMIT, &input,
                        sizeof(input), &output, sizeof(output));
}

// 0x0034620C | nintendogs:callseq [tier A]
nn::Result FormatSaveData(u32 maxFiles, u32 maxDirectories, bool duplicateData)
{
    u32 directoryBuckets = nn::fslow::QueryOptimalBucketCount(maxDirectories);
    u32 fileBuckets = nn::fslow::QueryOptimalBucketCount(maxFiles);
    // an empty path (the service does not look at its byte)
    u8 empty;
    ArchivePath path;
    path.type = PATH_TYPE_EMPTY;
    path.data = &empty;
    path.size = sizeof(empty);
    return nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session)
        .FormatSaveData(ARCHIVE_ID_SAVE_DATA, path.type, path.data, path.size, SAVE_DATA_BLOCKS, maxDirectories, maxFiles,
                        directoryBuckets, fileBuckets, duplicateData);
}

// 0x00346288 | nintendogs:callgraph [tier A]
bool IsSdmcInserted()
{
    bool detected;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetIpcFileSystem().IsSdmcDetected(&detected));
    return detected;
}

// 0x003462C0 (name is ours)
bool IsSdmcWritable()
{
    bool writable;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetIpcFileSystem().IsSdmcWritable(&writable));
    return writable;
}

// 0x003463AC | tier C (confirmed by the code)
nn::Result GetTwlPhotoSize(s64* totalSize, s64* freeSize)
{
    return GetFileSystemSizeCore(totalSize, freeSize, SYSTEM_MEDIA_TYPE_TWL_PHOTO);
}

// 0x0034654C | nintendogs:callseq [tier A]
nn::Result MountExtSaveData(const char* name, u64 saveId)
{
    nn::fs::ExtSaveDataSpecifier specifier;
    specifier.Make(MEDIA_TYPE_SDMC, saveId);
    IArchive* archive;
    nn::Result result = ConvertExtSaveDataResult(CTR::MPCore::detail::OpenExtSaveData(&archive, specifier, false));
    if (result.IsFailure()) {
        return result;
    }
    return RegisterOrDelete(name, archive);
}

// 0x003465E8 | nintendogs:bytes-fuzzy [tier A]
nn::Result CreateExtSaveData(u64 saveId, const void* smdh, size_t smdhSize, u32 maxDirectories, u32 maxFiles)
{
    nn::fs::ExtSaveDataSpecifier specifier;
    specifier.Make(MEDIA_TYPE_SDMC, saveId);
    // no size limit
    return ConvertExtSaveDataResult(nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session)
                                        .CreateExtSaveData(specifier, maxDirectories, maxFiles, -1, smdh, smdhSize));
}

// 0x0034667C | nintendogs:bytes [tier A]
nn::Result DeleteExtSaveData(u64 saveId)
{
    nn::fs::ExtSaveDataSpecifier specifier;
    specifier.Make(MEDIA_TYPE_SDMC, saveId);
    nn::Result result = ConvertExtSaveDataResult(detail::GetIpcFileSystem().DeleteExtSaveData(specifier));
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x003466EC | tier C (confirmed by the code)
nn::Result MountSpecialArchive(const char* name, u32 archiveId)
{
    IArchive* archive;
    nn::Result result = CTR::MPCore::detail::OpenSpecialArchiveRaw(&archive, archiveId);
    if (result.IsFailure()) {
        return result;
    }
    return RegisterOrDelete(name, archive);
}

// 0x0034673C | tier C (confirmed by the code); armlink replaced the tail call by a nop, so this
// falls through
nn::Result GetSdmcSize(s64* totalSize, s64* freeSize)
{
    return GetFileSystemSizeCore(totalSize, freeSize, SYSTEM_MEDIA_TYPE_SDMC);
}

// 0x00346744 | nintendogs:bytes [tier A]
nn::Result GetFileSystemSizeCore(s64* totalSize, s64* freeSize, nn::fs::SystemMediaType mediaType)
{
    *freeSize = 0;
    *totalSize = 0;
    nn::fs::ArchiveResource resource;
    nn::Result result = nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session).GetArchiveResource(&resource, mediaType);
    if (result.IsFailure()) {
        return result;
    }
    *freeSize = static_cast<u64>(resource.freeClusters) * resource.clusterSize;
    *totalSize = static_cast<u64>(resource.partitionClusters) * resource.clusterSize;
    return nn::Result();
}

// 0x003468C4 (name is ours)
nn::Result SetSaveArchiveSecureValue(const char* name, u32 slot, u64 value, bool unknown)
{
    IArchive* archive = CTR::MPCore::detail::FindArchive(name);
    if (archive == 0) {
        return nn::Result(CTR::MPCore::detail::RESULT_ARCHIVE_NOT_FOUND);
    }
    return nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session)
        .SetSaveArchiveSecureValue(static_cast<FileServerArchive*>(archive)->GetArchiveHandle(), slot, value, unknown);
}

// 0x0034691C | fefates:bytes [tier B]
// the buffer MountContent needs without the cache (the content is not opened)
size_t GetContentRequiredMemorySize(nn::fs::MediaType mediaType, u64 programId, u32 contentIndex, u32 maxFiles, u32 maxDirectories)
{
    return CTR::MPCore::detail::RomFsArchive::GetRequiredMemorySize(0, maxFiles, maxDirectories, false);
}

// 0x00346934 | nintendogs:bytes [tier B]
nn::Result GetSharedExtSaveDataBlockSize(s64* totalBlocks, s64* freeBlocks, s32* blockSize, u32 saveId)
{
    nn::fs::ExtSaveDataSpecifier specifier;
    specifier.Make(MEDIA_TYPE_NAND, SHARED_EXT_SAVE_DATA_ID_HIGH | saveId);
    return detail::GetIpcFileSystem().GetExtDataBlockSize(totalBlocks, freeBlocks, blockSize, specifier);
}

// 0x00346990 (name is ours)
nn::Result SetSaveDataSecureValue(const char* name, u64 value)
{
    return SetSaveArchiveSecureValue(name, SECURE_VALUE_SLOT, value, true);
}

// 0x003469A8 (name is ours)
bool CheckSaveDataSecureValue(u64 value)
{
    bool exists;
    bool isGameCard;
    u64 secureValue;
    if (nn::fs::ipc::FileSystem(CTR::MPCore::detail::GetSession())
            .GetThisSaveDataSecureValue(&exists, &isGameCard, &secureValue, SECURE_VALUE_SLOT)
            .IsFailure()) {
        nndbgPanic();
    }
    if (isGameCard || !exists) {
        return true;
    }
    return secureValue == value;
}

// 0x0034992C | tier C (confirmed by the code)
nn::Result MountSdmc(const char* name)
{
    IArchive* archive;
    nn::Result result = CTR::MPCore::detail::OpenSpecialArchiveRaw(&archive, ARCHIVE_ID_SDMC);
    if (result.IsFailure()) {
        return result;
    }
    return RegisterOrDelete(name, archive);
}

// --- RomFs ---

// 0x001290D8 | fefates:bytes [tier B]
s32 GetRomRequiredMemorySizeImpl(u32 maxFiles, u32 maxDirectories, bool useCache, const nn::fs::CTR::ProgramDataPath& path)
{
    // an empty path (the service does not look at its byte)
    u8 empty;
    nn::fs::Attributes attributes = {};
    nn::Handle handle;
    nn::Result result = nn::fs::ipc::FileSystem(CTR::MPCore::detail::s_Session).OpenFileDirectly(
        &handle, TRANSACTION_NONE, ARCHIVE_ID_SELF_NCCH, PATH_TYPE_EMPTY, &empty, sizeof(empty), PATH_TYPE_BINARY,
        reinterpret_cast<const u8*>(&path), sizeof(path), OPEN_MODE_READ, attributes);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    FileServerArchive::File* file = new (CTR::MPCore::detail::s_FileHeap.Allocate()) FileServerArchive::File(handle);
    if (file == 0) {
        nn::svc::CloseHandle(handle);
        nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_ROM_NOT_READABLE), nn::err::CTR::GetCurrentAddress());
    }
    s32 size = CTR::MPCore::detail::RomFsArchive::GetRequiredMemorySize(file, maxFiles, maxDirectories, useCache);
    if (size <= 0) {
        nn::svc::CloseHandle(handle);
        nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_ROM_NOT_READABLE), nn::err::CTR::GetCurrentAddress());
    }
    file->Close();
    return size;
}

// 0x0012FE60 | fefates:bytes [tier B]
s32 GetRomRequiredMemorySize(u32 maxFiles, u32 maxDirectories, bool useCache)
{
    nn::fs::CTR::ProgramDataPath path = {};
    return GetRomRequiredMemorySizeImpl(maxFiles, maxDirectories, useCache, path);
}

// 0x0011D27C (name is ours)
s32 GetRomPatchRequiredMemorySize(u32 maxFiles, u32 maxDirectories, bool useCache)
{
    nn::fs::CTR::ProgramDataPath path = {PROGRAM_DATA_PATH_TYPE_PATCH};
    return GetRomRequiredMemorySizeImpl(maxFiles, maxDirectories, useCache, path);
}

// 0x001363B4 | fefates:bytes [tier B]
nn::Result MountRom(const char* name, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    nn::fs::CTR::ProgramDataPath path = {};
    IArchive* archive;
    nn::err::CTR::ThrowFatalErrAllIfFailure(OpenRomArchive(&archive, maxFiles, maxDirectories, buffer, bufferSize, useCache, path));
    nn::Result result = CTR::MPCore::detail::RegisterArchive(name, archive, false, false);
    if (result.IsFailure()) {
        archive->DeleteObject();
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    return nn::Result();
}

// 0x0011D184 (name is ours; symbols.json: MountRom, tier C)
nn::Result MountRomPatch(const char* name, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    nn::fs::CTR::ProgramDataPath path = {PROGRAM_DATA_PATH_TYPE_PATCH};
    IArchive* archive;
    nn::err::CTR::ThrowFatalErrAllIfFailure(OpenRomArchive(&archive, maxFiles, maxDirectories, buffer, bufferSize, useCache, path));
    nn::Result result = CTR::MPCore::detail::RegisterArchive(name, archive, false, false);
    if (result.IsFailure()) {
        archive->DeleteObject();
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    return nn::Result();
}

// 0x0013059C | fefates:bytes [tier B]
nn::Result MountRom(u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    return MountRom("rom:", maxFiles, maxDirectories, buffer, bufferSize, useCache);
}

// 0x0034601C | fefates:bytes [tier B]
nn::Result MountContent(const char* name, nn::fs::MediaType mediaType, u64 programId, u32 contentIndex, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    nn::fs::CTR::DataContentArchivePath path;
    path.programId = programId;
    path.mediaType = mediaType;
    path.contentIndex = contentIndex;
    IArchive* archive;
    nn::Result result = CTR::MPCore::detail::OpenDataContent(&archive, path, maxFiles, maxDirectories, buffer, bufferSize, useCache);
    if (result.IsFailure()) {
        return result;
    }
    result = CTR::MPCore::detail::RegisterArchive(name, archive, IsAddOnContent(programId), false);
    if (result.IsFailure()) {
        archive->DeleteObject();
        return result;
    }
    return result;
}

} // namespace fs
} // namespace nn
