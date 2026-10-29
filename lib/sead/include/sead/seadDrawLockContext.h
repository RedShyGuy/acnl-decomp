#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadCriticalSection.h"

namespace sead {
class Thread;

// RTTI N4sead13DrawLockContextE
//
// A lock of the drawing that the thread holding it may take again (it counts). symbols.json had the
// names Graphics::lockDrawContext/unlockDrawContext on lock/unlock; those are the Graphics functions
// right in front of them. Names are ours.
class DrawLockContext : public ::sead::hostio::Node
{
public:
    DrawLockContext(); // 0x001208F4

    void lock(); // 0x00133148
    void unlock(); // 0x001331B8

    u32 field_0x04;              // 0x04 (0)
    CriticalSection mCriticalSection; // 0x08
    Thread* mOwner;              // 0x24
    s32 mLockCount;              // 0x28
};
ASSERT_SIZE(DrawLockContext, 0x2C);
} // namespace sead
