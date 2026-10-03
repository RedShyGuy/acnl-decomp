#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive_Directory.h"

// ctor address unknown
nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::Directory()
{
}

// 0x00347618 slot 0x00
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count)
{
}

// 0x003475C8 slot 0x04
void nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::Close()
{
}

// 0x003475BC slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TrySetPriority(s32 priority)
{
}

// 0x0072696C slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TryGetPriority(s32* priority) const
{
}

// 0x00347930 slot 0x10
// 0x0034792C slot 0x14 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::~Directory()
{
}

