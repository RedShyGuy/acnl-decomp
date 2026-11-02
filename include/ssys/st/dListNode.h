#pragma once

#include "decomp.h"
#include "types.h"

namespace ssys {
namespace st {
// a node of a doubly linked list (ssys::st::List)
// RTTI N4ssys2st8ListNodeE @ 0x008D2544
// vtable 0x009073EC (vptr 0x009073F4), offset_to_top 0, 2 entries
class ListNode
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE ListNode() : m_pPrev(NULL), m_pNext(NULL) {} // 0x00133B64
    virtual ~ListNode(); // 0x0056BF98 slot 0x00

    ListNode* GetPrev() const { return m_pPrev; } // (name is ours)
    ListNode* GetNext() const { return m_pNext; } // (name is ours)
    bool IsLinked() const { return m_pPrev != NULL || m_pNext != NULL; } // (name is ours)

    ListNode* m_pPrev;  // 0x4 (names after libgarden)
    ListNode* m_pNext;  // 0x8
};
ASSERT_SIZE(ListNode, 0xC);
} // namespace st
} // namespace ssys
