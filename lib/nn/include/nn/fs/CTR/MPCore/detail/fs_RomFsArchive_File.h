#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchive4FileE @ 0x008CDC78
// vtable 0x008FBE1C (vptr 0x008FBE24), offset_to_top 0, 15 entries
class nn::fs::CTR::MPCore::detail::RomFsArchive::File : public ::nn::fs::CTR::MPCore::detail::IFile
{
public:
    File(); // ctor address unknown
    virtual nn::Result TryRead(s32* readSize, s64 offset, void* buffer, size_t size); // 0x00347230 slot 0x00
    virtual nn::Result TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush); // 0x003472D4 slot 0x04
    virtual nn::Result TryGetAvailable(s64* available, s64 offset, s64 size); // 0x00348C18 slot 0x08
    virtual nn::Result TryGetSize(s64* size) const; // 0x00726930 slot 0x0C
    virtual nn::Result TrySetSize(s64 size); // 0x00347064 slot 0x10
    virtual nn::Result TryFlush(); // 0x003472C8 slot 0x14
    virtual nn::Result TrySetPriority(s32 priority); // 0x00347080 slot 0x18
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x00726958 slot 0x1C
    virtual nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size); // 0x003471A8 slot 0x20
    virtual nn::Result OpenLinkHandle(nn::Handle* handle); // 0x00347070 slot 0x24
    virtual nn::Handle GetFileHandle(); // 0x00348C0C slot 0x28
    virtual void DetachFileHandle(); // 0x00348C14 slot 0x2C
    virtual void Close(); // 0x003471E0 slot 0x30
    virtual ~File(); // 0x003472E4 slot 0x34, 0x003472E0 slot 0x38 (deleting)
};
