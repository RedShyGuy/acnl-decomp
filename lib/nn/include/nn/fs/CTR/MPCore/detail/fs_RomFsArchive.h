#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchiveE @ 0x008CDC90
// vtable 0x008FBE80 (vptr 0x008FBE88), offset_to_top 0, 18 entries
class RomFsArchive : public ::nn::fs::CTR::MPCore::detail::IArchive
{
public:
    class Directory;
    class File;
    class RomFsStorage;
    RomFsArchive(); // ctor address unknown
    virtual nn::Result OpenFile(nn::fs::CTR::MPCore::detail::IFile** file, const ArchivePath& path, u32 mode); // 0x003472E8 slot 0x00
    virtual nn::Result OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory** directory, const ArchivePath& path); // 0x00346DC0 slot 0x04
    virtual nn::Result DeleteFile(const ArchivePath& path); // 0x00346CF4 slot 0x08
    virtual nn::Result RenameFile(const ArchivePath& path, const ArchivePath& newPath); // 0x00346D00 slot 0x0C
    virtual nn::Result DeleteDirectory(const ArchivePath& path); // 0x00346EA4 slot 0x10
    virtual nn::Result DeleteDirectoryRecursively(const ArchivePath& path); // 0x00347058 slot 0x14
    virtual nn::Result CreateFile(const ArchivePath& path, s64 size); // 0x00346CE8 slot 0x18
    virtual nn::Result CreateDirectory(const ArchivePath& path); // 0x00346E98 slot 0x1C
    virtual nn::Result RenameDirectory(const ArchivePath& path, const ArchivePath& newPath); // 0x00346F18 slot 0x20
    virtual nn::Result SetPriority(s32 priority); // 0x00346F34 slot 0x24
    virtual nn::Result GetPriority(s32* priority); // 0x00346F24 slot 0x28
    virtual nn::Result GetFreeBytes(s64* freeBytes); // 0x00348C24 slot 0x2C
    virtual void DeleteObject() = 0;
    virtual ~RomFsArchive(); // 0x003479BC slot 0x34, 0x00347934 slot 0x38 (deleting)
    virtual nn::Result OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle) = 0;
    virtual void vf_0x40(); // 0x00346EB0 slot 0x40 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
    virtual nn::Result OpenLinkHandle(nn::Handle* handle); // 0x00346E88 slot 0x44
    void Initialize(nn::fs::CTR::MPCore::detail::IFile*, unsigned, unsigned, void*, unsigned, bool); // 0x0012FE88 | nintendogs:callseq [tier A]
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
