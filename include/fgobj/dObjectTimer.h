#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj11ObjectTimerE @ 0x008D29B8
// vtable 0x00908C54 (vptr 0x00908C5C), offset_to_top 0, 10 entries
class ObjectTimer : public ::fgobj::ObjectExe
{
public:
    ObjectTimer(); // ctor candidate(s) 0x0059B8D8 (unverified)
    virtual ~ObjectTimer(); // 0x00594D5C slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00594D2C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x10(); // 0x00594C4C slot 0x10 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x00594C54 slot 0x20 | virtual slot, introduced by fgobj::ObjectTimer
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
