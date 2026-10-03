#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_Directory.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_File.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/svc/svc_Api.h"

#include <new>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x00347F68 slot 0x18 | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::CreateFile(const ArchivePath& path, s64 size)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    nn::fs::Attributes attributes = {};
    return GetIpcFileSystem().CreateFile(TRANSACTION_NONE, mArchive, path.type, path.data, path.size, attributes, size);
}

// 0x00347FD8 slot 0x08 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteFile(const ArchivePath& path)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    return GetIpcFileSystem().DeleteFile(TRANSACTION_NONE, mArchive, path.type, path.data, path.size);
}

// 0x00348034 slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::RenameFile(const ArchivePath& path, const ArchivePath& newPath)
{
    if (path.size > MAX_PATH_SIZE || newPath.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    return GetIpcFileSystem().RenameFile(TRANSACTION_NONE, mArchive, path.type, path.data, path.size, mArchive,
                                         newPath.type, newPath.data, newPath.size);
}

// 0x00348098 slot 0x30 | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteObject()
{
    this->~FileServerArchive();
    s_ArchiveHeap.Free(this);
}

// 0x003480E8 slot 0x2C (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::GetFreeBytes(s64* freeBytes)
{
    return GetIpcFileSystem().GetFreeBytes(freeBytes, mArchive);
}

// 0x0034811C slot 0x04 | nintendogs:callseq
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::OpenDirectory(IDirectory** directory, const ArchivePath& path)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    nn::Handle handle;
    nn::Result result = GetIpcFileSystem().OpenDirectory(&handle, mArchive, path.type, path.data, path.size);
    if (result.IsFailure()) {
        return result;
    }
    Directory* opened = new (s_DirectoryHeap.Allocate()) Directory(handle);
    *directory = opened;
    if (opened == 0) {
        nn::svc::CloseHandle(handle);
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }
    return nn::Result();
}

// 0x003481C8 slot 0x1C | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::CreateDirectory(const ArchivePath& path)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    nn::fs::Attributes attributes = {};
    return GetIpcFileSystem().CreateDirectory(TRANSACTION_NONE, mArchive, path.type, path.data, path.size, attributes);
}

// 0x00348224 slot 0x10
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteDirectory(const ArchivePath& path)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    return GetIpcFileSystem().DeleteDirectory(TRANSACTION_NONE, mArchive, path.type, path.data, path.size);
}

// 0x00348280 slot 0x20 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::RenameDirectory(const ArchivePath& path,
                                                                           const ArchivePath& newPath)
{
    if (path.size > MAX_PATH_SIZE || newPath.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    return GetIpcFileSystem().RenameDirectory(TRANSACTION_NONE, mArchive, path.type, path.data, path.size, mArchive,
                                              newPath.type, newPath.data, newPath.size);
}

// 0x003482E4 slot 0x28 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::GetPriority(s32* priority)
{
    return GetIpcFileSystem().GetArchivePriority(priority, mArchive);
}

// 0x00348318 slot 0x24 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::SetPriority(s32 priority)
{
    return GetIpcFileSystem().SetArchivePriority(mArchive, priority);
}

// 0x00348354 slot 0x14 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteDirectoryRecursively(const ArchivePath& path)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    return GetIpcFileSystem().DeleteDirectoryRecursively(TRANSACTION_NONE, mArchive, path.type, path.data, path.size);
}

// 0x003486D4 slot 0x00 | nintendogs:callseq
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::OpenFile(IFile** file, const ArchivePath& path, u32 mode)
{
    if (path.size > MAX_PATH_SIZE) {
        return nn::Result(RESULT_PATH_TOO_LONG);
    }
    nn::Handle handle;
    nn::fs::Attributes attributes = {};
    nn::Result result = GetIpcFileSystem().OpenFile(&handle, TRANSACTION_NONE, mArchive, path.type, path.data, path.size,
                                                    mode, attributes);
    if (result.IsFailure()) {
        return result;
    }
    File* opened = new (s_FileHeap.Allocate()) File(handle);
    *file = opened;
    if (opened == 0) {
        nn::svc::CloseHandle(handle);
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }
    return nn::Result();
}

// 0x00348938 slot 0x34 | nintendogs:bytes
// 0x003488DC slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::FileServerArchive::~FileServerArchive()
{
    if (mArchive != 0) {
        GetIpcFileSystem().CloseArchive(mArchive);
        mArchive = 0;
    }
    // the session belongs to the file system, ~HandleObject must not close it
    mHandle = nn::Handle();
}

// 0x00726978 (name is ours)
DECOMP_NOINLINE nn::fs::ipc::FileSystem nn::fs::CTR::MPCore::detail::FileServerArchive::GetIpcFileSystem() const
{
    if (!GetHandle().IsValid()) {
        nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_NOT_OPENED), nn::err::CTR::GetCurrentAddress());
    }
    return nn::fs::ipc::FileSystem(GetHandle());
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
