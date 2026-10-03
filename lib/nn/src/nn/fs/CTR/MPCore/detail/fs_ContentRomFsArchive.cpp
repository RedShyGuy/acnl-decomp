#include "nn/fs/CTR/MPCore/detail/fs_ContentRomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_File.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/ipc/ipc_FileSystem.h"
#include "nn/svc/svc_Api.h"

#include <new>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
namespace {

// the program's own image (3dbrew "Filesystem services")
const u32 ARCHIVE_ID_SELF_NCCH = 0x00000003;

// ArchivePath types (3dbrew "FS Path")
const u32 PATH_TYPE_EMPTY = 1;
const u32 PATH_TYPE_BINARY = 2;

const s32 CONTENT_ROM_FS_ARCHIVE_COUNT = 16;

// the memory of the archive heap (name is ours)
// 0x0094B940
bit32 s_ContentRomFsArchiveHeapBuffer[CONTENT_ROM_FS_ARCHIVE_COUNT * sizeof(ContentRomFsArchive) / sizeof(bit32)];

} // namespace

// 0x00129424 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::ContentRomFsArchive::Create(ContentRomFsArchive** archive, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache, const nn::fs::CTR::ProgramDataPath& path)
{
    ContentRomFsArchive* created = new (AllocateBuffer()) ContentRomFsArchive();
    if (created == 0) {
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }
    // an empty path (the service does not look at its byte)
    u8 empty;
    nn::fs::Attributes attributes = {};
    nn::Handle handle;
    nn::Result result = nn::fs::ipc::FileSystem(s_Session).OpenFileDirectly(
        &handle, TRANSACTION_NONE, ARCHIVE_ID_SELF_NCCH, PATH_TYPE_EMPTY, &empty, sizeof(empty), PATH_TYPE_BINARY,
        reinterpret_cast<const u8*>(&path), sizeof(path), OPEN_MODE_READ, attributes);
    if (result.IsSuccess()) {
        IFile* file = 0;
        result = created->OpenDirect(&file, handle);
        if (result.IsSuccess()) {
            result = created->Initialize(file, maxFiles, maxDirectories, buffer, bufferSize, useCache);
            if (result.IsFailure()) {
                file->Close();
            }
        }
    }
    if (result.IsSuccess()) {
        *archive = created;
        return nn::Result();
    }
    created->DeleteObject();
    return result;
}

// 0x00130280 | fefates:bytes [tier B]
void* nn::fs::CTR::MPCore::detail::ContentRomFsArchive::AllocateBuffer()
{
    nn::os::CriticalSection::ScopedLock lock(s_RomFsArchiveLock);
    // 0x00AE1BC8 (name is ours; guard 0x00975F2C)
    static ObjectHeap s_Heap(sizeof(ContentRomFsArchive), reinterpret_cast<uptr>(s_ContentRomFsArchiveHeapBuffer),
                             sizeof(s_ContentRomFsArchiveHeapBuffer));
    s_pContentRomFsArchiveHeap = &s_Heap;
    return s_Heap.Allocate();
}

// 0x00348990 slot 0x3C | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::ContentRomFsArchive::OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle)
{
    return OpenFileServerFile(file, handle);
}

// 0x003489DC slot 0x30 | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::DeleteObject()
{
    this->~ContentRomFsArchive();
    s_pContentRomFsArchiveHeap->Free(this);
}

// 0x00348AB8 slot 0x34 | fefates:bytes [tier B]
// 0x00348A30 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::ContentRomFsArchive::~ContentRomFsArchive()
{
    // nothing to do (the RomFsArchive part closes the files)
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
