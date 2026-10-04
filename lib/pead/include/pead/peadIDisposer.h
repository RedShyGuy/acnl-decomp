#pragma once

#include "decomp.h"

namespace pead {
class Heap;

// RTTI N4pead9IDisposerE @ 0x008D1324
// vtable 0x00904DC4 (vptr 0x00904DCC), offset_to_top 0, 2 entries
//
// The constructor registers the object with the heap that contains it. Layout from the
// constructor (0x0053DB3C); the member names are ours.
class IDisposer
{
public:
    IDisposer(); // ctor candidate(s) 0x0053DB3C (unverified)
    virtual ~IDisposer(); // 0x0053DBDC slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053DB90 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)

    Heap* mDisposerHeap; // 0x04, the heap that contains the object
    void* mListNode[2];  // 0x08, node in the disposer list of that heap
};
ASSERT_SIZE(IDisposer, 0x10);
} // namespace pead
