#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
namespace ptcl {
class PtclProject;
class PtclSystem;

// RTTI N4sead4ptcl13PtclLoadCheckE @ 0x008D206C
// vtable 0x00906B20 (vptr 0x00906B28), offset_to_top 0, 1 entries
// The load test of the editor: emits the emitter sets of the project one after the other (the class is from
// the RTTI, the member names are ours).
class PtclLoadCheck : public ::sead::hostio::Node
{
public:
    // an emitter set of the test
    struct Entry
    {
        u32 field_0x00;
        u32 field_0x04;
        s32 field_0x08; // 1
        u32 field_0x0C;
        u32 field_0x10;
    };

    PtclLoadCheck(PtclProject* project, PtclSystem* system, s32 resId, u8 groupId); // 0x00557EC8 (name is ours)

    PtclProject* mProject;  // 0x04
    PtclSystem* mSystem;    // 0x08
    bool mIsRunning;        // 0x0C
    s32 mInterval;          // 0x10, frames (180)
    u32 field_0x14;         // 0x14
    s32 mResId;             // 0x18
    u32 mGroupID;           // 0x1C
    s32 mDuration;          // 0x20, frames (30000)
    Entry mEntries[64];     // 0x24
};
ASSERT_OFFSET(PtclLoadCheck, mEntries, 0x24);
ASSERT_SIZE(PtclLoadCheck, 0x524);
} // namespace ptcl
} // namespace sead
