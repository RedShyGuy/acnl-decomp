#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive_File.h"

// ctor address unknown
nn::fs::CTR::MPCore::detail::RomFsArchive::File::File()
{
}

// 0x00347230 slot 0x00
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryRead(s32* readSize, s64 offset, void* buffer, size_t size)
{
}

// 0x003472D4 slot 0x04
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush)
{
}

// 0x00348C18 slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetAvailable(s64* available, s64 offset, s64 size)
{
}

// 0x00726930 slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetSize(s64* size) const
{
}

// 0x00347064 slot 0x10
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TrySetSize(s64 size)
{
}

// 0x003472C8 slot 0x14
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryFlush()
{
}

// 0x00347080 slot 0x18
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TrySetPriority(s32 priority)
{
}

// 0x00726958 slot 0x1C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetPriority(s32* priority) const
{
}

// 0x003471A8 slot 0x20
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::OpenSubFile(nn::Handle* file, s64 offset, s64 size)
{
}

// 0x00347070 slot 0x24
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::OpenLinkHandle(nn::Handle* handle)
{
}

// 0x00348C0C slot 0x28
nn::Handle nn::fs::CTR::MPCore::detail::RomFsArchive::File::GetFileHandle()
{
}

// 0x00348C14 slot 0x2C
void nn::fs::CTR::MPCore::detail::RomFsArchive::File::DetachFileHandle()
{
}

// 0x003471E0 slot 0x30
void nn::fs::CTR::MPCore::detail::RomFsArchive::File::Close()
{
}

// 0x003472E4 slot 0x34
// 0x003472E0 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::File::~File()
{
}

