#pragma once

#include "decomp.h"
#include "imgdb/ArchiveMounter.h"
#include "imgdb/Singleton.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb21TwlNandArchiveMounterE @ 0x008D2BC8
// vtable 0x00909030 (vptr 0x00909038), offset_to_top 0, 6 entries
// vtable 0x00909050 (vptr 0x00909058), offset_to_top -4, 6 entries
class TwlNandArchiveMounter : public ::imgdb::Singleton<imgdb::TwlNandArchiveMounter>, public ::imgdb::ArchiveMounter, public ::nn::util::ADLFireWall::NonCopyable<imgdb::TwlNandArchiveMounter>
{
public:
    TwlNandArchiveMounter(); // ctor candidate(s) 0x005AB81C (unverified)
    virtual void vf_0x00(); // 0x005AB848 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::TwlNandArchiveMounter>
    virtual void vf_0x04(); // 0x005AB838 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::TwlNandArchiveMounter>
    virtual void Mount(); // 0x005AB798 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x005AB7D8 slot 0x0C | virtual slot, introduced by imgdb::TwlNandArchiveMounter
    virtual void IsMount() const; // 0x00755ECC slot 0x10 | slot vf_0x10 of imgdb::TwlNandArchiveMounter
    virtual void GetFreeSize() const; // 0x00755E78 slot 0x14 | nintendogs:bytes
};
} // namespace imgdb
