#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_File.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/CTR/fs_ArchivePaths.h"
#include "nn/svc/svc_Api.h"

#include <new>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail19ContentRomFsArchiveE @ 0x008CDCFC
// vtable 0x008FBF78 (vptr 0x008FBF80), offset_to_top 0, 18 entries
//
// A RomFsArchive on a file the FS service opened directly (the program's RomFS or a content of a
// title). The objects come from a heap of 16 (AllocateBuffer).
class ContentRomFsArchive : public ::nn::fs::CTR::MPCore::detail::RomFsArchive
{
public:
    // not user-provided: "new (p) ContentRomFsArchive()" clears the object first
    DECOMP_NOINLINE ContentRomFsArchive() = default; // 0x00130368 | nintendogs:bytes [tier A]

    virtual void DeleteObject(); // 0x003489DC slot 0x30 | fefates:bytes
    virtual ~ContentRomFsArchive(); // 0x00348AB8 slot 0x34, 0x00348A30 slot 0x38 (deleting) | fefates:bytes
    virtual nn::Result OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle); // 0x00348990 slot 0x3C | fefates:bytes

    // memory for one object, 0 if all are used
    static void* AllocateBuffer(); // 0x00130280 | fefates:bytes [tier B]

    // opens the image of path (archive 3) as a new archive
    static nn::Result Create(ContentRomFsArchive** archive, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache, const nn::fs::CTR::ProgramDataPath& path); // 0x00129424 | fefates:bytes [tier B]
};
ASSERT_SIZE(ContentRomFsArchive, 0x130);

// a FileServerArchive::File on handle; the handle is closed if no object is left (inline, in
// ContentRomFsArchive::OpenDirect and OpenDataContent; name is ours)
inline nn::Result OpenFileServerFile(IFile** file, nn::Handle handle)
{
    FileServerArchive::File* opened = new (s_FileHeap.Allocate()) FileServerArchive::File(handle);
    if (opened == 0) {
        nn::svc::CloseHandle(handle);
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }
    *file = opened;
    return nn::Result();
}
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
