#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/Singleton.h"
#include "imgdb/ArchiveMounter.h"
#include "imgdb/TwlNandArchiveMounter.h"

namespace imgdb {
// ctor candidate(s) 0x005AB81C (unverified)
imgdb::TwlNandArchiveMounter::TwlNandArchiveMounter()
{
}

// 0x005AB848 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::TwlNandArchiveMounter>
void imgdb::TwlNandArchiveMounter::vf_0x00()
{
}

// 0x005AB838 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::TwlNandArchiveMounter>
void imgdb::TwlNandArchiveMounter::vf_0x04()
{
}

// 0x005AB798 slot 0x08 | nintendogs:bytes
void imgdb::TwlNandArchiveMounter::Mount()
{
}

// 0x005AB7D8 slot 0x0C | virtual slot, introduced by imgdb::TwlNandArchiveMounter
void imgdb::TwlNandArchiveMounter::vf_0x0C()
{
}

// 0x00755ECC slot 0x10 | slot vf_0x10 of imgdb::TwlNandArchiveMounter
void imgdb::TwlNandArchiveMounter::IsMount() const
{
}

// 0x00755E78 slot 0x14 | nintendogs:bytes
void imgdb::TwlNandArchiveMounter::GetFreeSize() const
{
}

} // namespace imgdb
