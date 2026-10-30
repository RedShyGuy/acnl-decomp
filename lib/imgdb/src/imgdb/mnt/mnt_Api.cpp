#include "imgdb/SdArchiveMounter.h"
#include "imgdb/mnt/mnt_Api.h"

namespace imgdb {
namespace mnt {

// 0x005ABF80 (name is ours)
void UnmountSdmc()
{
    SdArchiveMounter::GetInstance()->Unmount();
}

// 0x005ABF98 | nintendogs:bytes [tier B]
bool MountSdmc()
{
    return SdArchiveMounter::GetInstance()->Mount();
}

} // namespace mnt
} // namespace imgdb
