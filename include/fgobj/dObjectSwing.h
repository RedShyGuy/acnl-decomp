#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj11ObjectSwingE @ 0x008D29A0
// vtable 0x00908BF4 (vptr 0x00908BFC), offset_to_top 0, 10 entries
class ObjectSwing : public ::fgobj::ObjectExe
{
public:
    ObjectSwing(); // ctor candidate(s) 0x00594654 (unverified)
    virtual ~ObjectSwing(); // 0x005948B0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0059485C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x10(); // 0x005936E4 slot 0x10 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x14(); // 0x005947D8 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x0059482C slot 0x20 | virtual slot, introduced by fgobj::ObjectSwing
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
