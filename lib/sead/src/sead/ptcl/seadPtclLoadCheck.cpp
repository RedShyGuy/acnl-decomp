#include "sead/ptcl/seadPtclLoadCheck.h"

#include <string.h>

namespace sead {
namespace ptcl {
// 0x00557EC8 (name is ours)
sead::ptcl::PtclLoadCheck::PtclLoadCheck(PtclProject* project, PtclSystem* system, s32 resId, u8 groupId)
    : hostio::Node(), mProject(project), mSystem(system), mIsRunning(false), mInterval(180), mResId(resId),
      mGroupID(groupId), mDuration(30000)
{
    memset(mEntries, 0, sizeof(mEntries));
    for (s32 i = 0; i < 64; i++) {
        mEntries[i].field_0x08 = 1;
    }
}
} // namespace ptcl
} // namespace sead
