#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj11ObjectOtherE @ 0x008D2970
// vtable 0x00908B34 (vptr 0x00908B3C), offset_to_top 0, 10 entries
class ObjectOther : public ::fgobj::ObjectExe
{
public:
    ObjectOther(); // ctor candidate(s) 0x00592AD8 (unverified)
    virtual ~ObjectOther(); // 0x00592C9C slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00592C6C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00592C38 slot 0x20 | virtual slot, introduced by fgobj::ObjectOther
    virtual void vf_0x24(); // 0x00592A70 slot 0x24 | virtual slot, introduced by fgobj::ObjectOther
};
} // namespace fgobj
