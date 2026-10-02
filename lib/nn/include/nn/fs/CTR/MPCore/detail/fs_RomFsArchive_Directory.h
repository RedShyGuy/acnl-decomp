#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchive9DirectoryE @ 0x008CDC84
// vtable 0x008FBE60 (vptr 0x008FBE68), offset_to_top 0, 6 entries
class nn::fs::CTR::MPCore::detail::RomFsArchive::Directory : public ::nn::fs::CTR::MPCore::detail::IDirectory
{
public:
    Directory(); // ctor address unknown
    virtual void TryRead(int*, nn::fs::DirectoryEntry*, int); // 0x00347618 slot 0x00 | nintendogs:bytes
    virtual void Close(); // 0x003475C8 slot 0x04 | nintendogs:bytes
    virtual void vf_0x08(); // 0x003475BC slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::Directory
    virtual void vf_0x0C(); // 0x0072696C slot 0x0C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::Directory
    virtual void vf_0x10(); // 0x00347930 slot 0x10 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::Directory
    virtual void vf_0x14(); // 0x0034792C slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::Directory
};
