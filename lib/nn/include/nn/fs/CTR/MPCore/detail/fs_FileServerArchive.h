#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/ipc/ipc_FileSystem.h"
#include "nn/os/os_HandleObject.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchiveE @ 0x008CDCDC
// vtable 0x008FBF34 (vptr 0x008FBF3C), offset_to_top 0, 15 entries
//
// An archive of the FS service: every function sends the FS:USER command of the same name with
// the archive handle. The HandleObject holds the FS:USER session (shared, so it is not closed).
// Allocated from s_ArchiveHeap. Member names are ours.
class FileServerArchive : public ::nn::fs::CTR::MPCore::detail::IArchive, public ::nn::os::HandleObject
{
public:
    class Directory;
    class File;

    // inline (e.g. in the mount function at 0x003467C0)
    FileServerArchive(nn::Handle session, u64 archive) : mArchive(archive) { mHandle = session; }

    virtual nn::Result OpenFile(IFile** file, const ArchivePath& path, u32 mode); // 0x003486D4 slot 0x00 | nintendogs:callseq
    virtual nn::Result OpenDirectory(IDirectory** directory, const ArchivePath& path); // 0x0034811C slot 0x04 | nintendogs:callseq
    virtual nn::Result DeleteFile(const ArchivePath& path); // 0x00347FD8 slot 0x08
    virtual nn::Result RenameFile(const ArchivePath& path, const ArchivePath& newPath); // 0x00348034 slot 0x0C
    virtual nn::Result DeleteDirectory(const ArchivePath& path); // 0x00348224 slot 0x10
    virtual nn::Result DeleteDirectoryRecursively(const ArchivePath& path); // 0x00348354 slot 0x14
    virtual nn::Result CreateFile(const ArchivePath& path, s64 size); // 0x00347F68 slot 0x18 | nintendogs:bytes
    virtual nn::Result CreateDirectory(const ArchivePath& path); // 0x003481C8 slot 0x1C | nintendogs:bytes
    virtual nn::Result RenameDirectory(const ArchivePath& path, const ArchivePath& newPath); // 0x00348280 slot 0x20
    virtual nn::Result SetPriority(s32 priority); // 0x00348318 slot 0x24
    virtual nn::Result GetPriority(s32* priority); // 0x003482E4 slot 0x28
    virtual nn::Result GetFreeBytes(s64* freeBytes); // 0x003480E8 slot 0x2C
    virtual void DeleteObject(); // 0x00348098 slot 0x30 | nintendogs:bytes
    virtual ~FileServerArchive(); // 0x00348938 slot 0x34 | nintendogs:bytes

    // the archive handle of the FS service (inline; name is ours)
    u64 GetArchiveHandle() const { return mArchive; }

private:
    // the FS:USER session; a fatal error if there is none (inline, out of line at 0x00726978)
    nn::fs::ipc::FileSystem GetIpcFileSystem() const;

    u64 mArchive;   // 0x08, 0 once closed

    static void CheckLayout()
    {
        ASSERT_OFFSET(FileServerArchive, mArchive, 0x8);
    }
};
ASSERT_SIZE(FileServerArchive, 0x10);
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
