#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/fs/CTR/MPCore/detail/detail_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_ArchiveTableEntry.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/fs/fs_Api.h"
#include "nn/fs/ipc/ipc_FileSystem.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

#include <new>
#include <wchar.h>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
namespace {

const s32 ARCHIVE_NAME_LENGTH = 8;  // the most characters of a mount name (before ':')
const u32 ARCHIVE_TABLE_SIZE = 32;

// ArchivePath types (3dbrew "FS Path")
const u32 PATH_TYPE_EMPTY = 1;
const u32 PATH_TYPE_BINARY = 2;
const u32 PATH_TYPE_UTF16 = 4;

// archive ids (3dbrew "Filesystem services")
const u32 ARCHIVE_ID_EXT_SAVE_DATA = 0x00000006;
const u32 ARCHIVE_ID_SHARED_EXT_SAVE_DATA = 0x00000007;
const u32 ARCHIVE_ID_BOSS_EXT_SAVE_DATA = 0x12345678;
const u32 ARCHIVE_ID_ACCESSIBLE_SAVE_DATA = 0x567890B4;   // save data the exheader allows access to

// the SDK version this library reports to the FS service
const u32 SDK_VERSION = 0x0B0500C8;

// latency emulation: at most this many extra milliseconds for a read / a write
const u32 MAX_RANDOM_READ_LATENCY = 100;
const u32 MAX_RANDOM_WRITE_LATENCY = 380;

// the memory of the object heaps (names are ours)
// 0x0094B6C0
bit32 s_ArchiveHeapBuffer[256 / sizeof(bit32)];
// 0x0094B7C0
bit32 s_FileHeapBuffer[256 / sizeof(bit32)];
// 0x0094B8C0
bit32 s_DirectoryHeapBuffer[128 / sizeof(bit32)];

// 0x00AE1CC8
ArchiveTableEntry s_ArchiveTable[ARCHIVE_TABLE_SIZE];

// the key of a mount name: up to 8 characters before ':' (one byte each, sign extended);
// 0 if there is no ':' within the first 8 characters (inline)
template <typename CharT>
u64 MakeArchiveKey(const CharT* name)
{
    u64 key = 0;
    for (s32 i = 0; i < ARCHIVE_NAME_LENGTH; i++) {
        if (name[i] == ':') {
            return key;
        }
        key = (key << 8) | static_cast<s64>(static_cast<s8>(name[i]));
    }
    return 0;
}

// copies the mount name of path (up to ':' or the end) and appends ':' (inline); a name of more
// than 8 characters stops the program
const char* GetArchiveName(char* buffer, const char* path)
{
    for (s32 i = 0; i < ARCHIVE_NAME_LENGTH; i++) {
        char c = path[i];
        if (c == '\0' || c == ':') {
            buffer[i] = ':';
            return buffer;
        }
        buffer[i] = c;
    }
    nndbgPanic();
    return "";
}

// the part of path behind "name:" as an ArchivePath (inline)
ArchivePath GetPathInArchive(const wchar_t* path)
{
    while (*path++ != L':') {
    }
    ArchivePath archivePath;
    archivePath.type = PATH_TYPE_UTF16;
    archivePath.data = reinterpret_cast<const u8*>(path);
    archivePath.size = (wcslen(path) + 1) * sizeof(wchar_t);
    return archivePath;
}

// opens an archive of the FS service as a FileServerArchive (inline)
DECOMP_ALWAYS_INLINE nn::Result OpenFileServerArchive(IArchive** archive, u32 archiveId, const ArchivePath& path)
{
    u64 handle;
    nn::Result result = nn::fs::ipc::FileSystem(s_Session).OpenArchive(&handle, archiveId, path.type, path.data, path.size);
    if (result.IsFailure()) {
        return result;
    }
    FileServerArchive* opened = new (s_ArchiveHeap.Allocate()) FileServerArchive(s_Session, handle);
    result = (opened == 0) ? nn::Result(RESULT_OUT_OF_OBJECTS) : nn::Result();
    *archive = opened;
    if (result.IsFailure()) {
        nn::fs::ipc::FileSystem(s_Session).CloseArchive(handle);
    }
    return result;
}

} // namespace

// the globals of this file (ARMCC addresses them all from 0x00975F20)
// 0x00975F20
bool s_IsLatencyEmulationEnabled;
// 0x00975F21
bool s_IsLatencyRandomized;
// 0x00975F24
ObjectHeap* s_pContentRomFsArchiveHeap;
// 0x00975F28
IArchive* s_pSaveDataArchive;
// 0x00975F30
nn::Handle s_Session;
// 0x00975F38
s64 s_LatencyMilliSeconds;

// 0x00AE1CB8
nn::os::CriticalSection s_RomFsArchiveLock((nn::os::CriticalSection::InitializeTag()));

// 0x00AE1C04
ObjectHeap s_ArchiveHeap(16, reinterpret_cast<uptr>(s_ArchiveHeapBuffer), sizeof(s_ArchiveHeapBuffer));
// 0x00AE1C40
ObjectHeap s_FileHeap(8, reinterpret_cast<uptr>(s_FileHeapBuffer), sizeof(s_FileHeapBuffer));
// 0x00AE1C7C
ObjectHeap s_DirectoryHeap(8, reinterpret_cast<uptr>(s_DirectoryHeapBuffer), sizeof(s_DirectoryHeapBuffer));

// (the addresses are at the instantiations below)
template <typename CharT>
IArchive* FindArchive(const CharT* path)
{
    u64 key = MakeArchiveKey(path);
    if (key == 0) {
        return 0;
    }
    ArchiveTableEntry* entry = 0;
    for (u32 i = 0; i < ARCHIVE_TABLE_SIZE; i++) {
        if (s_ArchiveTable[i].key == key) {
            entry = &s_ArchiveTable[i];
            break;
        }
    }
    return entry != 0 ? entry->archive : 0;
}

// 0x007D35B8 | tier A
template IArchive* FindArchive<char>(const char* path);
// 0x001359D4 | tier A
template IArchive* FindArchive<wchar_t>(const wchar_t* path);

// 0x001292C8 | nintendogs:callseq-callee [tier A] (with 3 parameters; the binary uses 4)
nn::Result RegisterArchive(const char* name, IArchive* archive, bool unknown, bool isNotOwned)
{
    char buffer[ARCHIVE_NAME_LENGTH];
    u64 key = MakeArchiveKey(GetArchiveName(buffer, name));
    if (key == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    for (s32 i = 0; i < static_cast<s32>(ARCHIVE_TABLE_SIZE); i++) {
        ArchiveTableEntry& entry = s_ArchiveTable[i];
        if (entry.archive == 0) {
            entry.key = key;
            entry.archive = archive;
            entry.unknown0C = unknown;
            entry.isNotOwned = isNotOwned;
            return nn::Result();
        }
        if (entry.key == key) {
            return nn::Result(RESULT_ALREADY_REGISTERED);
        }
    }
    return nn::Result(RESULT_OUT_OF_OBJECTS);
}

// 0x00129584 | nintendogs:bytes [tier B]
nn::Result OpenSharedExtSaveData(IArchive** archive, const nn::fs::ExtSaveDataSpecifier& specifier)
{
    ArchivePath path;
    path.type = PATH_TYPE_BINARY;
    path.data = reinterpret_cast<const u8*>(&specifier);
    path.size = sizeof(specifier);
    return OpenFileServerArchive(archive, ARCHIVE_ID_SHARED_EXT_SAVE_DATA, path);
}

// 0x00142FB0 | nintendogs:bytes [tier A]
// waits like a slow medium (a debug setting of the system)
void LatencyEmulation(bool isRead)
{
    u32 maxRandom = isRead ? MAX_RANDOM_READ_LATENCY : MAX_RANDOM_WRITE_LATENCY;
    u32 random = 0;
    if (!s_IsLatencyEmulationEnabled) {
        return;
    }
    if (s_IsLatencyRandomized) {
        u32 tick = static_cast<u32>(nn::svc::GetSystemTick());
        if (tick & 0x10) {
            random = maxRandom / ((tick & 0xF) + 1);
        }
    }
    u16 milliSeconds = static_cast<u16>(s_LatencyMilliSeconds + random);
    if (milliSeconds != 0) {
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(milliSeconds));
    }
}

// 0x00347E94 | nintendogs:bytes [tier A]
nn::Result OpenExtSaveData(IArchive** archive, const nn::fs::ExtSaveDataSpecifier& specifier, bool isBoss)
{
    ArchivePath path;
    path.type = PATH_TYPE_BINARY;
    path.data = reinterpret_cast<const u8*>(&specifier);
    path.size = sizeof(specifier);
    return OpenFileServerArchive(archive, isBoss ? ARCHIVE_ID_BOSS_EXT_SAVE_DATA : ARCHIVE_ID_EXT_SAVE_DATA, path);
}

// 0x00348B4C | nintendogs:callseq [tier A]
nn::Result OpenSpecialArchiveRaw(IArchive** archive, u32 archiveId)
{
    // an empty path (the service does not look at its byte)
    u8 empty;
    ArchivePath path;
    path.type = PATH_TYPE_EMPTY;
    path.data = &empty;
    path.size = sizeof(empty);
    return OpenFileServerArchive(archive, archiveId, path);
}

// 0x00348B3C (name is ours)
nn::Handle GetSession()
{
    return s_Session;
}

// 0x0013616C | nintendogs:bytes [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::Initialize(nn::Handle session)
{
    s_Session = session;
    nn::fs::ipc::FileSystem(session).InitializeWithSdkVersion(SDK_VERSION);
    return nn::Result();
}

// 0x0012925C | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteFile(const wchar_t* path)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    return archive->DeleteFile(GetPathInArchive(path));
}

// 0x00136198 | nintendogs:bytes-fuzzy [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryCreateFile(const wchar_t* path, s64 size)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    return archive->CreateFile(GetPathInArchive(path), size);
}

// 0x0013AE4C | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryOpenFile(void** file, const wchar_t* path, u32 mode)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    IFile* opened;
    nn::Result result = archive->OpenFile(&opened, GetPathInArchive(path), mode);
    if (result.IsSuccess()) {
        *file = opened;
    }
    return result;
}

// 0x00140544 | nintendogs:bytes [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryReadFile(s32* readSize, void* file, s64 offset, void* buffer,
                                                                    size_t size)
{
    if (file == 0 || buffer == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    LatencyEmulation(true);
    return static_cast<IFile*>(file)->TryRead(readSize, offset, buffer, size);
}

// 0x001405A4 | nintendogs:bytes [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryWriteFile(s32* writtenSize, void* file, s64 offset,
                                                                     const void* buffer, size_t size, bool flush)
{
    if (writtenSize == 0 || file == 0 || buffer == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    LatencyEmulation(false);
    return static_cast<IFile*>(file)->TryWrite(writtenSize, offset, buffer, size, flush);
}

// 0x00140608 | nintendogs:callgraph [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryGetFileSize(s64* size, const void* file)
{
    if (size == 0 || file == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    return static_cast<const IFile*>(file)->TryGetSize(size);
}

// 0x00130268
void nn::fs::CTR::MPCore::detail::UserFileSystem::CloseFile(void* file)
{
    if (file != 0) {
        static_cast<IFile*>(file)->Close();
    }
}

// 0x00347A40 | nintendogs:callgraph [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::CloseDirectory(void* directory)
{
    if (directory != 0) {
        static_cast<IDirectory*>(directory)->Close();
    }
}

// 0x00347A58 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TrySetFileSize(void* file, s64 size)
{
    if (file == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    return static_cast<IFile*>(file)->TrySetSize(size);
}

// 0x00347A78 | nintendogs:callgraph [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryOpenDirectory(void** directory, const wchar_t* path)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    IDirectory* opened;
    nn::Result result = archive->OpenDirectory(&opened, GetPathInArchive(path));
    if (result.IsSuccess()) {
        *directory = opened;
    }
    return result;
}

// 0x00347B04 | nintendogs:bytes [tier A]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryReadDirectory(s32* readCount, void* directory,
                                                                         nn::fs::DirectoryEntry* entries, s32 count)
{
    if (readCount == 0 || directory == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    LatencyEmulation(true);
    return static_cast<IDirectory*>(directory)->TryRead(readCount, entries, count);
}

// 0x00347B54 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryCreateDirectory(const wchar_t* path)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    return archive->CreateDirectory(GetPathInArchive(path));
}

// 0x00347BC0 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteDirectory(const wchar_t* path)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    return archive->DeleteDirectory(GetPathInArchive(path));
}

// 0x00347C2C | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TrySetPriorityForFile(void* file, s32 priority)
{
    if (file == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    return static_cast<IFile*>(file)->TrySetPriority(priority);
}

// 0x00347C4C (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteDirectoryRecursively(const wchar_t* path)
{
    IArchive* archive = FindArchive(path);
    if (archive == 0) {
        return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
    }
    return archive->DeleteDirectoryRecursively(GetPathInArchive(path));
}

// 0x00347CB8 (the name symbols.json gives 0x00347A58)
nn::Result nn::fs::CTR::MPCore::detail::UserFileSystem::TryFlush(void* file)
{
    if (file == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    return static_cast<IFile*>(file)->TryFlush();
}

} // namespace detail
} // namespace MPCore
} // namespace CTR

// 0x0011D228 | fefates:bytes [tier B]
void InitializeLatencyEmulation()
{
    using namespace CTR::MPCore::detail;
    s_LatencyMilliSeconds = nn::cfg::CTR::GetFsLatencyEmulationParam() * 10;
    if (nn::cfg::CTR::IsDebugMode()) {
        s_IsLatencyRandomized = true;
    }
    if (s_IsLatencyRandomized || s_LatencyMilliSeconds != 0) {
        s_IsLatencyEmulationEnabled = true;
    }
}

// 0x003467C0 (name is ours)
nn::Result MountAccessibleSaveData(const char* name, nn::fs::MediaType mediaType, u32 saveId, u8 unknown)
{
    using namespace CTR::MPCore::detail;
    // the path of archive 0x567890B4 (3dbrew): media type, save id, a byte
    struct {
        u32 mediaType;
        u32 saveId;
        u8 unknown;
    } binary;
    binary.mediaType = mediaType;
    binary.saveId = saveId;
    binary.unknown = unknown;
    ArchivePath path;
    path.type = PATH_TYPE_BINARY;
    path.data = reinterpret_cast<const u8*>(&binary);
    path.size = sizeof(binary);
    IArchive* archive;
    nn::Result result = OpenFileServerArchive(&archive, ARCHIVE_ID_ACCESSIBLE_SAVE_DATA, path);
    if (result.IsFailure()) {
        return result;
    }
    result = RegisterArchive(name, archive, false, false);
    if (result.IsFailure()) {
        archive->DeleteObject();
    }
    return result;
}

// 0x00136254 | nintendogs:callgraph [tier A]
nn::Result Unmount(const char* path)
{
    using namespace CTR::MPCore::detail;
    char buffer[ARCHIVE_NAME_LENGTH];
    u64 key = MakeArchiveKey(GetArchiveName(buffer, path));
    for (u32 i = 0; i < ARCHIVE_TABLE_SIZE; i++) {
        ArchiveTableEntry& entry = s_ArchiveTable[i];
        if (entry.key == key) {
            IArchive* archive = entry.archive;
            bool isNotOwned = entry.isNotOwned;
            entry.archive = 0;
            entry.key = 0;
            if (archive != 0 && !isNotOwned) {
                archive->DeleteObject();
                if (archive == s_pSaveDataArchive) {
                    s_pSaveDataArchive = 0;
                }
            }
            return nn::Result();
        }
    }
    return nn::Result(RESULT_ARCHIVE_NOT_FOUND);
}

} // namespace fs
} // namespace nn
