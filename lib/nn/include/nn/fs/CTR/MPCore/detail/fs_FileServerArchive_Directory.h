#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/os/os_HandleObject.h"

// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchive9DirectoryE @ 0x008CDCBC
// vtable 0x008FBF14 (vptr 0x008FBF1C), offset_to_top 0, 6 entries
class nn::fs::CTR::MPCore::detail::FileServerArchive::Directory : public ::nn::fs::CTR::MPCore::detail::IDirectory, public ::nn::os::HandleObject
{
public:
    Directory(); // ctor address unknown
    virtual void TryRead(int*, nn::fs::DirectoryEntry*, int); // 0x00348838 slot 0x00 | nintendogs:bytes
    virtual void Close(); // 0x003487D0 slot 0x04 | nintendogs:bytes
    virtual void vf_0x08(); // 0x00348794 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::Directory
    virtual void vf_0x0C(); // 0x00726A08 slot 0x0C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::Directory
    virtual void vf_0x10(); // 0x003488B4 slot 0x10 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::Directory
    virtual void vf_0x14(); // 0x00348884 slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::Directory
};
