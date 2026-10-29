#pragma once

#include "decomp.h"
#include "pead/peadListNode.h"

namespace pead {
class Heap;

// RTTI N4pead9IDisposerE @ 0x008D1324
// vtable 0x00904DC4 (vptr 0x00904DCC), offset_to_top 0, 2 entries
//
// pia's copy of sead::IDisposer: an object that a heap destroys with it. The constructor registers the object
// with the heap that contains it, the destructor takes it out again (pead has only this constructor). The
// member names are ours.
class IDisposer
{
public:
    IDisposer(); // 0x0053DB3C (name after sead)
    virtual ~IDisposer(); // 0x0053DBDC slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053DB90 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)

    // the value of mDisposerHeap once the object is destroyed (1, in .data)
    static Heap* s_pDestructedHeap;

    Heap* mDisposerHeap; // 0x04, the heap that contains the object
    ListNode mListNode;  // 0x08, in the disposer list of that heap
};
ASSERT_SIZE(IDisposer, 0x10);
} // namespace pead
