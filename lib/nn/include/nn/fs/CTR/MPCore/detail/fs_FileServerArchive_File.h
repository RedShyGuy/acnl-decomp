#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/fs/ipc/ipc_File.h"
#include "nn/os/os_HandleObject.h"

// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchive4FileE @ 0x008CDC9C
// vtable 0x008FBED0 (vptr 0x008FBED8), offset_to_top 0, 15 entries
//
// A file of a FileServerArchive: the HandleObject holds the file session (closed by
// ~HandleObject). Allocated from s_FileHeap.
class nn::fs::CTR::MPCore::detail::FileServerArchive::File : public ::nn::fs::CTR::MPCore::detail::IFile, public ::nn::os::HandleObject
{
public:
    // inline (FileServerArchive::OpenFile)
    explicit File(nn::Handle handle) { mHandle = handle; }

    virtual nn::Result TryRead(s32* readSize, s64 offset, void* buffer, size_t size); // 0x003484E0 slot 0x00 | nintendogs:bytes
    virtual nn::Result TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush); // 0x003485A8 slot 0x04 | nintendogs:bytes
    virtual nn::Result TryGetAvailable(s64* available, s64 offset, s64 size); // 0x00348638 slot 0x08
    virtual nn::Result TryGetSize(s64* size) const; // 0x007269A0 slot 0x0C
    virtual nn::Result TrySetSize(s64 size); // 0x003483B0 slot 0x10 | fefates:bytes
    virtual nn::Result TryFlush(); // 0x00348520 slot 0x14 | nintendogs:bytes
    virtual nn::Result TrySetPriority(s32 priority); // 0x00348400 slot 0x18
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x007269E8 slot 0x1C
    virtual nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size); // 0x00348420 slot 0x20
    virtual nn::Result OpenLinkHandle(nn::Handle* handle); // 0x003483E0 slot 0x24
    virtual nn::Handle GetFileHandle(); // 0x003483D8 slot 0x28
    virtual void DetachFileHandle(); // 0x00348464 slot 0x2C
    virtual void Close(); // 0x00348470 slot 0x30 | nintendogs:bytes-fuzzy
    virtual ~File(); // 0x003486AC slot 0x34

private:
    // the file session; a fatal error if there is none (inline, out of line at 0x007269C0)
    nn::fs::ipc::File GetIpcFile() const;
};
