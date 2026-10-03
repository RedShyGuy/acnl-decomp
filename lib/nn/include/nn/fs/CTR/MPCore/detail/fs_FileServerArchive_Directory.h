#pragma once

#include "decomp.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/ipc/ipc_Directory.h"
#include "nn/os/os_HandleObject.h"

// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchive9DirectoryE @ 0x008CDCBC
// vtable 0x008FBF14 (vptr 0x008FBF1C), offset_to_top 0, 6 entries
//
// A directory of a FileServerArchive: the HandleObject holds the directory session (closed by
// ~HandleObject). Allocated from s_DirectoryHeap.
class nn::fs::CTR::MPCore::detail::FileServerArchive::Directory : public ::nn::fs::CTR::MPCore::detail::IDirectory, public ::nn::os::HandleObject
{
public:
    // inline (FileServerArchive::OpenDirectory)
    explicit Directory(nn::Handle handle) { mHandle = handle; }

    virtual nn::Result TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count); // 0x00348838 slot 0x00 | nintendogs:bytes
    virtual void Close(); // 0x003487D0 slot 0x04 | nintendogs:bytes
    virtual nn::Result TrySetPriority(s32 priority); // 0x00348794 slot 0x08
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x00726A08 slot 0x0C
    virtual ~Directory(); // 0x003488B4 slot 0x10

private:
    // the directory session; a fatal error if there is none (inline everywhere; name is ours)
    nn::fs::ipc::Directory GetIpcDirectory() const
    {
        if (!GetHandle().IsValid()) {
            nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_NOT_OPENED), nn::err::CTR::GetCurrentAddress());
        }
        return nn::fs::ipc::Directory(GetHandle());
    }
};
