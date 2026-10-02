#pragma once

#include "decomp.h"
#include "fgobj/dObjectFall.h"

namespace fgobj {
// RTTI N5fgobj11ObjectFruitE @ 0x008D2964
// vtable 0x00908B04 (vptr 0x00908B0C), offset_to_top 0, 10 entries
class ObjectFruit : public ::fgobj::ObjectFall
{
public:
    ObjectFruit(); // ctor candidate(s) 0x00592664 (unverified)
    virtual ~ObjectFruit(); // 0x00592A44 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00592A14 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x18(); // 0x005920B4 slot 0x18 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x1C(); // 0x005920BC slot 0x1C | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x00592860 slot 0x20 | virtual slot, introduced by fgobj::ObjectFruit
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
