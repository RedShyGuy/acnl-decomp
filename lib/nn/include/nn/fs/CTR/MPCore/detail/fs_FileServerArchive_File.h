#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/os/os_HandleObject.h"

// RTTI N2nn2fs3CTR6MPCore6detail17FileServerArchive4FileE @ 0x008CDC9C
// vtable 0x008FBED0 (vptr 0x008FBED8), offset_to_top 0, 15 entries
class nn::fs::CTR::MPCore::detail::FileServerArchive::File : public ::nn::fs::CTR::MPCore::detail::IFile, public ::nn::os::HandleObject
{
public:
    File(); // ctor candidate(s) 0x001290D8, 0x00347CD8 (unverified)
    virtual void TryRead(int*, long long, void*, unsigned); // 0x003484E0 slot 0x00 | nintendogs:bytes
    virtual void TryWrite(int*, long long, const void*, unsigned, bool); // 0x003485A8 slot 0x04 | nintendogs:bytes
    virtual void vf_0x08(); // 0x00348638 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void TryGetSize(long long*) const; // 0x007269A0 slot 0x0C | slot vf_0x0C of nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void TrySetSize(long long); // 0x003483B0 slot 0x10 | fefates:bytes
    virtual void TryFlush(); // 0x00348520 slot 0x14 | nintendogs:bytes
    virtual void TrySetPriority(int); // 0x00348400 slot 0x18 | slot vf_0x18 of nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void vf_0x1C(); // 0x007269E8 slot 0x1C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void vf_0x20(); // 0x00348420 slot 0x20 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void OpenLinkHandle(nn::Handle*); // 0x003483E0 slot 0x24 | slot vf_0x24 of nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void GetFileHandle(); // 0x003483D8 slot 0x28 | slot vf_0x28 of nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void vf_0x2C(); // 0x00348464 slot 0x2C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void Close(); // 0x00348470 slot 0x30 | nintendogs:bytes-fuzzy
    virtual ~File(); // 0x003486AC slot 0x34 | slot vf_0x34 of nn::fs::CTR::MPCore::detail::FileServerArchive::File
    virtual void vf_0x38(); // 0x0034867C slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive::File
};
