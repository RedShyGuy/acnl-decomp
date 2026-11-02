#pragma once

#include "decomp.h"
#include "ssys/st/dListNodePriorityBase.h"

namespace ssys {
namespace ma {
// a free or used block of a VramExpHeap, sorted by address (member names are ours)
// RTTI N4ssys2ma12VramMemBlockE @ 0x008D2428
// vtable 0x0090722C (vptr 0x00907234), offset_to_top 0, 4 entries
class VramMemBlock : public ::ssys::st::ListNodePriorityBase
{
public:
    virtual ~VramMemBlock(); // 0x00567DC4 slot 0x00
    bool IsBefore(const ::ssys::st::ListNode* other) const override; // 0x00567D7C slot 0x08
    ::ssys::st::ListNode* FindInsertPosition(::ssys::st::ListNode* start) const override; // 0x00567D94 slot 0x0C

    u32 m_Size = 0;     // 0x0C including the alignment offset
    u32 m_Offset = 0;   // 0x10 the alignment of the allocation at the start
    uptr m_Address = 0; // 0x14
};
ASSERT_SIZE(VramMemBlock, 0x18);
} // namespace ma
} // namespace ssys
