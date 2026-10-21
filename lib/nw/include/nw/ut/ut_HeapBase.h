#pragma once

#include "decomp.h"
#include "nw/ut/ut_HeapNode.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace ut {
// RTTI N2nw2ut8HeapBaseE @ 0x008D0530
// vtable 0x00902344 (vptr 0x0090234C), offset_to_top 0, 2 entries
//
// The base of the heaps: the memory range, a signature and the heaps created inside it. Every
// heap is in the list of the heap that contains its memory, or in the root list. Member names
// are ours.
class HeapBase : public ::nw::ut::HeapNode
{
public:
    // the options of Initialize (names are ours)
    enum Option
    {
        OPTION_CLEAR_ALLOC_MEMORY = 1 << 0, // allocated memory is zero filled
        OPTION_DEBUG_FILL = 1 << 1          // memory is filled with the debug fill values
    };
    // the index of the debug fill values (names are ours)
    enum FillType
    {
        FILL_TYPE_NOT_USED,
        FILL_TYPE_ALLOC,
        FILL_TYPE_FREE,
        FILL_TYPE_COUNT
    };

    typedef LinkList<HeapBase, 4> HeapList;

    virtual ~HeapBase(); // 0x0048BA90 slot 0x00
    // 0x0048BA80 slot 0x04 (deleting dtor)

    // the heap whose memory contains the address, the innermost one (NULL: none in the list)
    static HeapBase* FindContainHeap(nw::ut::LinkList<nw::ut::HeapBase, (long)4>* pList, const void* pMemory); // 0x0014387C | nintendogs:bytes [tier A]

protected:
    // (inline)
    HeapBase() {}

    void Initialize(u32 signature, void* pHeapStart, void* pHeapEnd, u16 option); // 0x0013EE80 | nintendogs:bytes [tier A]
    void Finalize(); // 0x001438D4 | nintendogs:bytes [tier A]
    void FillFreeMemory(void* pAddress, u32 size); // 0x00140E78 | nintendogs:bytes [tier A]
    void FillAllocMemory(void* pAddress, u32 size); // 0x00140EA4 | nintendogs:bytes [tier A]

public:
    LinkListNode m_Link;   // 0x04 (in the list of the containing heap)
    void* m_pHeapStart;    // 0x0C
    void* m_pHeapEnd;      // 0x10
    u32 m_Signature;       // 0x14 (0 once finalized)
    HeapList m_ChildList;  // 0x18 (the heaps inside this one)
    u32 m_Option;          // 0x24
};
ASSERT_OFFSET(HeapBase, m_Link, 0x04);
ASSERT_OFFSET(HeapBase, m_ChildList, 0x18);
ASSERT_SIZE(HeapBase, 0x28);

// the debug fill values of the heaps, by HeapBase::FillType (name is ours)
extern u32 g_HeapFillValues[HeapBase::FILL_TYPE_COUNT];
} // namespace ut
} // namespace nw
