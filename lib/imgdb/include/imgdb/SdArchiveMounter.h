#pragma once

#include "decomp.h"
#include "imgdb/ArchiveMounter.h"
#include "imgdb/Singleton.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb16SdArchiveMounterE @ 0x008D2B44
// vtable 0x00908F70 (vptr 0x00908F78), offset_to_top 0, 6 entries
// vtable 0x00908F90 (vptr 0x00908F98), offset_to_top -4, 6 entries
class SdArchiveMounter : public ::imgdb::Singleton<imgdb::SdArchiveMounter>, public ::imgdb::ArchiveMounter, public ::nn::util::ADLFireWall::NonCopyable<imgdb::SdArchiveMounter>
{
public:
    virtual void vf_0x00(); // 0x005AAF28 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::SdArchiveMounter>
    virtual void vf_0x04(); // 0x005AAF18 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::SdArchiveMounter>
    virtual void Mount(); // 0x005AAE80 slot 0x08 | nintendogs:bytes
    virtual void Unmount(); // 0x005AAEC8 slot 0x0C | nintendogs:bytes
    virtual void IsMount() const; // 0x00755B54 slot 0x10 | slot vf_0x10 of imgdb::SdArchiveMounter
    virtual void GetFreeSize() const; // 0x00755AF4 slot 0x14 | nintendogs:bytes
    SdArchiveMounter(); // 0x005AAEF8 | nintendogs:bytes [tier B]
};
} // namespace imgdb
