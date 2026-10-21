#pragma once

#include "decomp.h"
#include "nw/ut/ut_HeapBase.h"

namespace nw {
namespace ut {
// RTTI N2nw2ut9FrameHeapE @ 0x008D0544
// vtable 0x00902354 (vptr 0x0090235C), offset_to_top 0, 2 entries
//
// A heap that allocates from both ends of its memory and frees only everything (or back to a
// recorded state). The object itself lies at the start of the memory. Member names are ours.
class FrameHeap : public ::nw::ut::HeapBase
{
public:
    // the signature of a frame heap
    static const u32 SIGNATURE = 0x46524D48; // 'FRMH'
    // the parts Free releases (names are ours)
    enum FreeMode
    {
        FREE_HEAD = 1 << 0,
        FREE_TAIL = 1 << 1,
        FREE_ALL = FREE_HEAD | FREE_TAIL
    };

    // the heap in the memory (NULL: the memory is too small)
    static FrameHeap* Create(void* pStartAddress, u32 size, u16 option); // 0x0013B76C | nintendogs:bytes [tier A]
    // the memory of the heap
    void* Destroy(); // 0x00140F6C | nintendogs:callgraph [tier A]

    // alignment >= 0: from the head; < 0: from the tail, aligned to -alignment
    void* Alloc(u32 size, s32 alignment); // 0x0013F050 | nintendogs:bytes [tier A]
    void Free(s32 mode); // 0x00140DE8 | nintendogs:bytes [tier A]
    // the current ends are saved under the tag
    bool RecordState(u32 tagName); // 0x0013EFC4 | nintendogs:bytes [tier A]
    // back to the state of the tag (0: the last one)
    bool FreeByState(u32 tagName); // 0x0013EF20 | nintendogs:bytes [tier A]

    virtual ~FrameHeap(); // 0x0048BB54 slot 0x00
    // 0x0048BB44 slot 0x04 (deleting dtor)

private:
    // a recorded state, allocated from the head (names are ours)
    struct HeapState
    {
        HeapState(u32 tag, void* pHead) : tagName(tag), pHeadAllocator(pHead), pTailAllocator(nullptr), pNext(nullptr) {}

        u32 tagName;          // 0x0
        void* pHeadAllocator; // 0x4
        void* pTailAllocator; // 0x8
        HeapState* pNext;     // 0xC
    };

    // (inline)
    FrameHeap() {}

    // (inline; names are ours)
    void* AllocFromHead(u32 size, s32 alignment);
    void* AllocFromTail(u32 size, s32 alignment);

public:
    void* m_pHeadAllocator; // 0x28
    void* m_pTailAllocator; // 0x2C
    HeapState* m_pState;    // 0x30
};
ASSERT_SIZE(FrameHeap, 0x34);
} // namespace ut
} // namespace nw
