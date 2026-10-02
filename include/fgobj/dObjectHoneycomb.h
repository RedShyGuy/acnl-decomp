#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"

namespace fgobj {
// RTTI N5fgobj15ObjectHoneycombE @ 0x008D2A00
// vtable 0x00908D54 (vptr 0x00908D5C), offset_to_top 0, 10 entries
class ObjectHoneycomb : public ::fgobj::ObjectFall
{
public:
    ObjectHoneycomb(); // ctor candidate(s) 0x00595C7C (unverified)
    virtual ~ObjectHoneycomb(); // 0x00595FE4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00595FB4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x18(); // 0x005920B4 slot 0x18 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x1C(); // 0x005920BC slot 0x1C | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x00595DDC slot 0x20 | virtual slot, introduced by fgobj::ObjectHoneycomb
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
