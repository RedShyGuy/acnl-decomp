#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"

namespace fgobj {
// RTTI N5fgobj10ObjectLeafE @ 0x008D2940
// vtable 0x00908AC4 (vptr 0x00908ACC), offset_to_top 0, 10 entries
class ObjectLeaf : public ::fgobj::ObjectFall
{
public:
    ObjectLeaf(); // ctor candidate(s) 0x00592238 (unverified)
    virtual ~ObjectLeaf(); // 0x00592534 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00592504 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x18(); // 0x005920B4 slot 0x18 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x1C(); // 0x005920BC slot 0x1C | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x00592410 slot 0x20 | virtual slot, introduced by fgobj::ObjectLeaf
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
