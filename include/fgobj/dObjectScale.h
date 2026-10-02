#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj11ObjectScaleE @ 0x008D2988
// vtable 0x00908B94 (vptr 0x00908B9C), offset_to_top 0, 10 entries
class ObjectScale : public ::fgobj::ObjectExe
{
public:
    ObjectScale(); // ctor candidate(s) 0x00592EE4 (unverified)
    virtual ~ObjectScale(); // 0x00593138 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005930F4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x14(); // 0x00591594 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x0059302C slot 0x20 | virtual slot, introduced by fgobj::ObjectScale
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
