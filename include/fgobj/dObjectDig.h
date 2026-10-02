#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj9ObjectDigE @ 0x008D2A44
// vtable 0x00908E4C (vptr 0x00908E54), offset_to_top 0, 10 entries
class ObjectDig : public ::fgobj::ObjectExe
{
public:
    ObjectDig(); // ctor candidate(s) 0x0059A9D0 (unverified)
    virtual ~ObjectDig(); // 0x005A35F8 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005A35B4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x14(); // 0x005A3544 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x005A3560 slot 0x20 | virtual slot, introduced by fgobj::ObjectDig
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
