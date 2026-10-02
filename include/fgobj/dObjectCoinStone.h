#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj15ObjectCoinStoneE @ 0x008D29E8
// vtable 0x00908CF4 (vptr 0x00908CFC), offset_to_top 0, 10 entries
class ObjectCoinStone : public ::fgobj::ObjectExe
{
public:
    ObjectCoinStone(); // ctor candidate(s) 0x0059D888 (unverified)
    virtual ~ObjectCoinStone(); // 0x005959F8 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005959C8 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00595930 slot 0x20 | virtual slot, introduced by fgobj::ObjectCoinStone
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
