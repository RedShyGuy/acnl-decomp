#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

namespace fgobj {
// RTTI N5fgobj11ObjectStumpE @ 0x008D2994
// vtable 0x00908BC4 (vptr 0x00908BCC), offset_to_top 0, 10 entries
class ObjectStump : public ::fgobj::ObjectMove
{
public:
    ObjectStump(); // ctor candidate(s) 0x00593328 (unverified)
    virtual ~ObjectStump(); // 0x0059347C slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0059344C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00593438 slot 0x20 | virtual slot, introduced by fgobj::ObjectStump
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
