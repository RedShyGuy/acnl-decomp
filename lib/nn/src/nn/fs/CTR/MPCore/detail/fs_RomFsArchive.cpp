#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// ctor address unknown
nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsArchive()
{
}

// 0x003472E8 slot 0x00
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenFile(nn::fs::CTR::MPCore::detail::IFile** file, const ArchivePath& path, u32 mode)
{
}

// 0x00346DC0 slot 0x04
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory** directory, const ArchivePath& path)
{
}

// 0x00346CF4 slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteFile(const ArchivePath& path)
{
}

// 0x00346D00 slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::RenameFile(const ArchivePath& path, const ArchivePath& newPath)
{
}

// 0x00346EA4 slot 0x10
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteDirectory(const ArchivePath& path)
{
}

// 0x00347058 slot 0x14
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteDirectoryRecursively(const ArchivePath& path)
{
}

// 0x00346CE8 slot 0x18
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::CreateFile(const ArchivePath& path, s64 size)
{
}

// 0x00346E98 slot 0x1C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::CreateDirectory(const ArchivePath& path)
{
}

// 0x00346F18 slot 0x20
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::RenameDirectory(const ArchivePath& path, const ArchivePath& newPath)
{
}

// 0x00346F34 slot 0x24
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::SetPriority(s32 priority)
{
}

// 0x00346F24 slot 0x28
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::GetPriority(s32* priority)
{
}

// 0x00348C24 slot 0x2C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::GetFreeBytes(s64* freeBytes)
{
}

// 0x003479BC slot 0x34
// 0x00347934 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::~RomFsArchive()
{
}

// 0x00346EB0 slot 0x40 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x40()
{
}

// 0x00346E88 slot 0x44
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenLinkHandle(nn::Handle* handle)
{
}

// 0x0012FE88 | nintendogs:callseq [tier A]
void nn::fs::CTR::MPCore::detail::RomFsArchive::Initialize(nn::fs::CTR::MPCore::detail::IFile*, unsigned, unsigned, void*, unsigned, bool)
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
