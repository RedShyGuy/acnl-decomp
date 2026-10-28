#pragma once

#include "decomp.h"
#include "sead/seadListNode.h"

namespace sead {
class Heap;

// RTTI N4sead9IDisposerE @ 0x008D2390
// vtable 0x00907110 (vptr 0x00907118), offset_to_top 0, 2 entries
//
// An object that a heap destroys with it: the constructor registers the object with a heap (by
// HeapNullOption the heap that contains the object or the current one), the destructor takes it out
// again. The member and enum names are ours.
class IDisposer
{
public:
    // which heap the object registers with when none is given
    enum HeapNullOption
    {
        HEAP_NULL_OPTION_DEFAULT = 0,            // the same as FIND_CONTAIN_HEAP in this build
        HEAP_NULL_OPTION_FIND_CONTAIN_HEAP = 1, // the heap that contains the object
        HEAP_NULL_OPTION_NONE = 2,              // no heap
        HEAP_NULL_OPTION_CURRENT_HEAP = 3,      // the current heap of the thread
    };

    IDisposer(); // 0x00133834 (name is ours)
    IDisposer(Heap* disposerHeap, HeapNullOption option); // 0x0013378C (name is ours)
    virtual ~IDisposer(); // 0x0013F6E0 slot 0x00 | nintendogs:callseq-callee
    // 0x005629A8 slot 0x04 (deleting dtor)

    // the value of mDisposerHeap once the object is destroyed (1, in .data)
    static Heap* s_pDestructedHeap;

    Heap* mDisposerHeap;      // 0x04
    ListNode mListNode;       // 0x08, in the disposer list of the heap
};
ASSERT_SIZE(IDisposer, 0x10);
} // namespace sead
