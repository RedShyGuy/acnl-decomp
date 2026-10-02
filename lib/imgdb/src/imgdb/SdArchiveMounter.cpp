#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/Singleton.h"
#include "imgdb/ArchiveMounter.h"
#include "imgdb/SdArchiveMounter.h"

namespace imgdb {
// 0x005AAF28 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::SdArchiveMounter>
void imgdb::SdArchiveMounter::vf_0x00()
{
}

// 0x005AAF18 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::SdArchiveMounter>
void imgdb::SdArchiveMounter::vf_0x04()
{
}

// 0x005AAE80 slot 0x08 | nintendogs:bytes
void imgdb::SdArchiveMounter::Mount()
{
}

// 0x005AAEC8 slot 0x0C | nintendogs:bytes
void imgdb::SdArchiveMounter::Unmount()
{
}

// 0x00755B54 slot 0x10 | slot vf_0x10 of imgdb::SdArchiveMounter
void imgdb::SdArchiveMounter::IsMount() const
{
}

// 0x00755AF4 slot 0x14 | nintendogs:bytes
void imgdb::SdArchiveMounter::GetFreeSize() const
{
}

// 0x005AAEF8 | nintendogs:bytes [tier B]
imgdb::SdArchiveMounter::SdArchiveMounter()
{
}

} // namespace imgdb
