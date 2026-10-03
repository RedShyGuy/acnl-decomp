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
    virtual nn::Result TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count); // 0x00347618 slot 0x00
    virtual void Close(); // 0x003475C8 slot 0x04
    virtual nn::Result TrySetPriority(s32 priority); // 0x003475BC slot 0x08
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x0072696C slot 0x0C
    virtual ~Directory(); // 0x00347930 slot 0x10, 0x0034792C slot 0x14 (deleting)
};
