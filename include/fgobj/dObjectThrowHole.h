#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

namespace fgobj {
// RTTI N5fgobj15ObjectThrowHoleE @ 0x008D2A18
// vtable 0x00908DB4 (vptr 0x00908DBC), offset_to_top 0, 10 entries
class ObjectThrowHole : public ::fgobj::ObjectMove
{
public:
    ObjectThrowHole(); // ctor candidate(s) 0x00596B0C (unverified)
    virtual ~ObjectThrowHole(); // 0x00596D54 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00596D24 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00596C58 slot 0x20 | virtual slot, introduced by fgobj::ObjectThrowHole
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
