#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

class nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage
{
public:
    void ReadBytes(long long, void*, unsigned); // 0x00346D0C | nintendogs:bytes [tier A]
};
