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
    virtual void TryRead(int*, long long, void*, unsigned int); // 0x00347230 slot 0x00 | fefates:bytes
    virtual void TryWrite(int*, long long, const void*, unsigned int, bool); // 0x003472D4 slot 0x04 | slot vf_0x04 of nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void vf_0x08(); // 0x00348C18 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void TryGetSize(long long*) const; // 0x00726930 slot 0x0C | fefates:bytes
    virtual void TrySetSize(long long); // 0x00347064 slot 0x10 | slot vf_0x10 of nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void vf_0x14(); // 0x003472C8 slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void TrySetPriority(int); // 0x00347080 slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x00726958 slot 0x1C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void vf_0x20(); // 0x003471A8 slot 0x20 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void OpenLinkHandle(nn::Handle*); // 0x00347070 slot 0x24 | slot vf_0x24 of nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void GetFileHandle(); // 0x00348C0C slot 0x28 | slot vf_0x28 of nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void vf_0x2C(); // 0x00348C14 slot 0x2C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void Close(); // 0x003471E0 slot 0x30 | fefates:bytes
    virtual ~File(); // 0x003472E4 slot 0x34 | slot vf_0x34 of nn::fs::CTR::MPCore::detail::RomFsArchive::File
    virtual void vf_0x38(); // 0x003472E0 slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive::File
};
