#pragma once

#include "decomp.h"
#include "fgobj/dObjectStump.h"

namespace fgobj {
// RTTI N5fgobj15ObjectStumpAnimE @ 0x008D2A0C
// vtable 0x00908D84 (vptr 0x00908D8C), offset_to_top 0, 10 entries
class ObjectStumpAnim : public ::fgobj::ObjectStump
{
public:
    ObjectStumpAnim(); // ctor candidate(s) 0x005967D0 (unverified)
    virtual ~ObjectStumpAnim(); // 0x00596AB4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00596A58 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x14(); // 0x00596908 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x0059697C slot 0x20 | virtual slot, introduced by fgobj::ObjectStump
};
} // namespace fgobj
