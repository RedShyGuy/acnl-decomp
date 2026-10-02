#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

namespace fgobj {
// RTTI N5fgobj13ObjectPresentE @ 0x008D29DC
// vtable 0x00908CC4 (vptr 0x00908CCC), offset_to_top 0, 10 entries
class ObjectPresent : public ::fgobj::ObjectMove
{
public:
    ObjectPresent(); // ctor candidate(s) 0x005953D4 (unverified)
    virtual ~ObjectPresent(); // 0x00595710 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005956E0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00595530 slot 0x20 | virtual slot, introduced by fgobj::ObjectPresent
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
