#pragma once

#include "decomp.h"

namespace sead {
class AudioFxMgr;
class Heap;

// RTTI N4sead16AudioFxMemoryMgrE @ 0x008D1AF4
// vtable 0x00905E04 (vptr 0x00905E0C), offset_to_top 0, 2 entries
// The memory of the work buffers of the effects (from a heap; member names and setup are ours).
class AudioFxMemoryMgr
{
public:
    // called by AudioFxMemoryMgrCtr (out of line in the original)
    DECOMP_NOINLINE AudioFxMemoryMgr(); // 0x0013C69C (name is ours)
    virtual ~AudioFxMemoryMgr(); // 0x00546628 slot 0x00
    // 0x00546620 slot 0x04 (deleting dtor)

    void* alloc(u32 size, s32 alignment); // 0x005465E8
    void setup(AudioFxMgr* fxMgr, Heap* heap); // 0x005465F8
    // takes the effects of the memory out of the effect manager
    void dispose(); // 0x00546604 | nintendogs:bytes [tier B]

    AudioFxMgr* mFxMgr; // 0x4
    Heap* mHeap;        // 0x8
};
ASSERT_SIZE(AudioFxMemoryMgr, 0xC);
} // namespace sead
