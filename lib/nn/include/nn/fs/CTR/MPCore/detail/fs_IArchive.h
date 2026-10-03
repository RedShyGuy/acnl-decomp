#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fslow/fslow_LowPath.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class IDirectory;
class IFile;

typedef nn::fslow::LowPath<const char*, const wchar_t*> ArchivePath; // (name is ours)

// RTTI N2nn2fs3CTR6MPCore6detail8IArchiveE @ 0x008CDD10
// A mounted archive (FileServerArchive: an archive of the FS service; RomFsArchive). The names of
// the slots without a symbol (0x08, 0x14, 0x20 to 0x2C) are ours, after the FS command each
// FileServerArchive slot sends.
class IArchive
{
public:
    virtual nn::Result OpenFile(IFile** file, const ArchivePath& path, u32 mode) = 0; // slot 0x00
    virtual nn::Result OpenDirectory(IDirectory** directory, const ArchivePath& path) = 0; // slot 0x04
    virtual nn::Result DeleteFile(const ArchivePath& path) = 0; // slot 0x08
    virtual nn::Result RenameFile(const ArchivePath& path, const ArchivePath& newPath) = 0; // slot 0x0C
    virtual nn::Result DeleteDirectory(const ArchivePath& path) = 0; // slot 0x10
    virtual nn::Result DeleteDirectoryRecursively(const ArchivePath& path) = 0; // slot 0x14
    virtual nn::Result CreateFile(const ArchivePath& path, s64 size) = 0; // slot 0x18
    virtual nn::Result CreateDirectory(const ArchivePath& path) = 0; // slot 0x1C
    virtual nn::Result RenameDirectory(const ArchivePath& path, const ArchivePath& newPath) = 0; // slot 0x20
    virtual nn::Result SetPriority(s32 priority) = 0; // slot 0x24
    virtual nn::Result GetPriority(s32* priority) = 0; // slot 0x28
    virtual nn::Result GetFreeBytes(s64* freeBytes) = 0; // slot 0x2C
    // destroys the archive and gives its memory back (the archives come from unit heaps)
    virtual void DeleteObject() = 0; // slot 0x30
    virtual ~IArchive() {} // slots 0x34, 0x38
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
