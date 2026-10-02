#pragma once

#include "decomp.h"
#include "fgobj/dObjectThrow.h"

namespace fgobj {
// RTTI N5fgobj12ObjectHanabiE @ 0x008D29D0
// vtable 0x00908C94 (vptr 0x00908C9C), offset_to_top 0, 10 entries
class ObjectHanabi : public ::fgobj::ObjectThrow
{
public:
    ObjectHanabi(); // ctor candidate(s) 0x0059BB68 (unverified)
    virtual ~ObjectHanabi(); // 0x00595284 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00595240 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x14(); // 0x00595010 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x20(); // 0x00595068 slot 0x20 | virtual slot, introduced by fgobj::ObjectThrow
};
} // namespace fgobj
