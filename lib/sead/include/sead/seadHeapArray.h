#pragma once

#include "decomp.h"
#include "sead/seadHeapMgr.h"

namespace sead {
class Heap;

// The heaps of a task, one in each root heap (the class name is from the reference symbols, the
// member names are ours).
class HeapArray
{
public:
    // (the heaps are set by whoever makes them)
    HeapArray() : mPrimaryIndex(0)
    {
        for (s32 i = 0; i < 4; i++) {
            mAdjusted[i] = false;
        }
    }

    // the heap in the root heap index (the first one for an index out of range)
    Heap* getHeap(s32 index) const
    {
        if (static_cast<u32>(index) < static_cast<u32>(HeapMgr::s_RootHeaps.size())) {
            return mHeaps[index];
        }
        return mHeaps[0];
    }
    // the primary heap (no range check; name is ours)
    Heap* getPrimaryHeap() const { return mHeaps[mPrimaryIndex]; }

    Heap* mHeaps[4];    // 0x00
    bool mAdjusted[4];  // 0x10
    s32 mPrimaryIndex;  // 0x14
};
ASSERT_SIZE(HeapArray, 0x18);
} // namespace sead
