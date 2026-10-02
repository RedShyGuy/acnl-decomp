#pragma once

#include "decomp.h"
#include "ssys/st/dListNodePriorityBase.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma12VramMemBlockE @ 0x008D2428
// vtable 0x0090722C (vptr 0x00907234), offset_to_top 0, 4 entries
class VramMemBlock : public ::ssys::st::ListNodePriorityBase
{
public:
    VramMemBlock(); // ctor candidate(s) 0x00120BF4, 0x0056788C (unverified)
    virtual ~VramMemBlock(); // 0x00567DC4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00567DC0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x00567D7C slot 0x08 | virtual slot, introduced by ssys::ma::VramMemBlock
    virtual void vf_0x0C(); // 0x00567D94 slot 0x0C | virtual slot, introduced by ssys::ma::VramMemBlock
};
} // namespace ma
} // namespace ssys
