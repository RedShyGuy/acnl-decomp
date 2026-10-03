#pragma once

// The file system of the user API (nn::fs::FileInputStream etc.): paths "name:/..." are looked up
// in the archive table (RegisterArchive), files and directories are IFile / IDirectory objects
// passed as void*. All functions are static. Original file: fs_UserFileSystem.cpp (its static
// initializer __sti___21_fs_UserFileSystem_cpp also constructs the object heaps below).

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/fnd/fnd_UnitHeapTemplate.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class UserFileSystem
{
public:
    static nn::Result Initialize(nn::Handle session); // 0x0013616C | nintendogs:bytes [tier A]

    static nn::Result TryOpenFile(void** file, const wchar_t* path, u32 mode); // 0x0013AE4C | fefates:bytes [tier B]
    static nn::Result TryCreateFile(const wchar_t* path, s64 size); // 0x00136198 | nintendogs:bytes-fuzzy [tier A]
    static nn::Result TryDeleteFile(const wchar_t* path); // 0x0012925C | fefates:bytes [tier B]
    static nn::Result TryOpenDirectory(void** directory, const wchar_t* path); // 0x00347A78 | nintendogs:callgraph [tier A]
    static nn::Result TryCreateDirectory(const wchar_t* path); // 0x00347B54 (name is ours)
    static nn::Result TryDeleteDirectory(const wchar_t* path); // 0x00347BC0 (name is ours)
    static nn::Result TryDeleteDirectoryRecursively(const wchar_t* path); // 0x00347C4C (name is ours)

    static nn::Result TryReadFile(s32* readSize, void* file, s64 offset, void* buffer, size_t size); // 0x00140544 | nintendogs:bytes [tier A]
    static nn::Result TryWriteFile(s32* writtenSize, void* file, s64 offset, const void* buffer, size_t size, bool flush); // 0x001405A4 | nintendogs:bytes [tier A]
    static nn::Result TryGetFileSize(s64* size, const void* file); // 0x00140608 | nintendogs:callgraph [tier A]
    // symbols.json names 0x00347A58 "TryFlush(void*)", but it calls IFile::TrySetSize
    static nn::Result TrySetFileSize(void* file, s64 size); // 0x00347A58 (name is ours)
    static nn::Result TryFlush(void* file); // 0x00347CB8 (the name symbols.json gives 0x00347A58)
    static nn::Result TrySetPriorityForFile(void* file, s32 priority); // 0x00347C2C | fefates:bytes [tier B]
    // symbols.json also names 0x00373B68 CloseFile (nintendogs:bytes); that one calls slot 0x2C
    // and lies among nn::nex code
    static void CloseFile(void* file); // 0x00130268

    static nn::Result TryReadDirectory(s32* readCount, void* directory, nn::fs::DirectoryEntry* entries, s32 count); // 0x00347B04 | nintendogs:bytes [tier A]
    static void CloseDirectory(void* directory); // 0x00347A40 | nintendogs:callgraph [tier A]
};

// results of the archive classes (module fs); the names are ours
const bit32 RESULT_INVALID_ARGUMENT = 0xE0E046BC;    // usage, invalid argument, 700: a null pointer / bad name
const bit32 RESULT_PATH_TOO_LONG = 0xE0E046BF;       // usage, invalid argument, 703: path over MAX_PATH_SIZE
const bit32 RESULT_ARCHIVE_NOT_FOUND = 0xC8804465;   // status, not found, 101: no archive of that name
const bit32 RESULT_ALREADY_REGISTERED = 0xC82044B4;  // status, nothing happened, 180: the name is taken
const bit32 RESULT_OUT_OF_OBJECTS = 0xD8604659;      // permanent, out of resource, 601: a heap or the table is full
const bit32 RESULT_NOT_OPENED = 0xE0A046DA;          // usage, invalid state, 730: no session / handle

const size_t MAX_PATH_SIZE = 512;   // bytes of an ArchivePath

class IArchive;

// the FS:USER session (out of line at 0x00348B3C; name is ours)
nn::Handle GetSession();

// the globals of fs_UserFileSystem.cpp at 0x00975F20 (names are ours)
extern bool s_IsLatencyEmulationEnabled;    // 0x00975F20
extern bool s_IsLatencyRandomized;          // 0x00975F21, debug mode: a random extra latency
extern IArchive* s_pSaveDataArchive;        // 0x00975F28, set by nn::fs::MountSaveData
extern nn::Handle s_Session;                // 0x00975F30, the FS:USER session
extern s64 s_LatencyMilliSeconds;           // 0x00975F38

// The archive, file and directory objects come from these unit heaps (names are ours).
typedef nn::fnd::UnitHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> > ObjectHeap;
extern ObjectHeap s_ArchiveHeap;    // 0x00AE1C04, 16 archives of up to 16 bytes
extern ObjectHeap s_FileHeap;       // 0x00AE1C40, 32 files of up to 8 bytes
extern ObjectHeap s_DirectoryHeap;  // 0x00AE1C7C, 16 directories of up to 8 bytes
extern ObjectHeap* s_pContentRomFsArchiveHeap;      // 0x00975F24, set by ContentRomFsArchive::AllocateBuffer
extern nn::os::CriticalSection s_RomFsArchiveLock;  // 0x00AE1CB8, guards ContentRomFsArchive::AllocateBuffer
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
