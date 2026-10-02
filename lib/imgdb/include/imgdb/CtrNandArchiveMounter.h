#pragma once

#include "decomp.h"
#include "imgdb/ArchiveMounter.h"
#include "imgdb/Singleton.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb21CtrNandArchiveMounterE @ 0x008D2B94
// vtable 0x00908FD8 (vptr 0x00908FE0), offset_to_top 0, 6 entries
// vtable 0x00908FF8 (vptr 0x00909000), offset_to_top -4, 6 entries
class CtrNandArchiveMounter : public ::imgdb::Singleton<imgdb::CtrNandArchiveMounter>, public ::imgdb::ArchiveMounter, public ::nn::util::ADLFireWall::NonCopyable<imgdb::CtrNandArchiveMounter>
{
public:
    CtrNandArchiveMounter(); // ctor candidate(s) 0x005AB5EC (unverified)
    virtual void vf_0x00(); // 0x005AB618 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::CtrNandArchiveMounter>
    virtual void vf_0x04(); // 0x005AB608 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::CtrNandArchiveMounter>
    virtual void vf_0x08(); // 0x005AB558 slot 0x08 | virtual slot, introduced by imgdb::CtrNandArchiveMounter
    virtual void vf_0x0C(); // 0x005AB5A8 slot 0x0C | virtual slot, introduced by imgdb::CtrNandArchiveMounter
    virtual void vf_0x10(); // 0x00755E70 slot 0x10 | virtual slot, introduced by imgdb::CtrNandArchiveMounter
    virtual void vf_0x14(); // 0x00755D70 slot 0x14 | virtual slot, introduced by imgdb::CtrNandArchiveMounter
    void GetSharedExtSaveDataBlockSize(long long&, long long&, int&) const; // 0x00755E14 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace imgdb
