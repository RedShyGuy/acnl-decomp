#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
class Heap;

namespace ptcl {
class PtclEditor;
class PtclProjectEmitterSet;
class PtclSystem;

// RTTI N4sead4ptcl11PtclProjectE @ 0x008D2044
// vtable 0x00906AD8 (vptr 0x00906AE0), offset_to_top 0, 2 entries
// The emitter sets the editor sent (the class is from the RTTI, the member names are ours).
class PtclProject
{
public:
    static const s32 NUM_EMITTER_SET_MAX = 64;

    PtclProject(Heap* heap, PtclSystem* system, PtclEditor* editor, s32 resId); // 0x005558C8 (name is ours)
    virtual ~PtclProject(); // 0x00555A54 slot 0x00
    // 0x00555A00 slot 0x04 (deleting dtor)

    // deletes set (name is ours)
    void removeEmitterSet(PtclProjectEmitterSet* set); // 0x00555708
    // the names of the node (name is ours)
    void initName(); // 0x00555754

    Heap* mHeap;                                             // 0x004
    PtclProjectEmitterSet* mEmitterSets[NUM_EMITTER_SET_MAX]; // 0x008
    PtclSystem* mSystem;                                     // 0x108
    PtclEditor* mEditor;                                     // 0x10C
    FixedSafeString<256> mName;                              // 0x110
    FixedSafeString<64> mDispName;                           // 0x21C
    FixedSafeString<128> mMetaName;                          // 0x268
    s32 mResId;                                              // 0x2F4
};
ASSERT_OFFSET(PtclProject, mSystem, 0x108);
ASSERT_OFFSET(PtclProject, mName, 0x110);
ASSERT_OFFSET(PtclProject, mResId, 0x2F4);
ASSERT_SIZE(PtclProject, 0x2F8);
} // namespace ptcl
} // namespace sead
