#pragma once

#include "decomp.h"
#include "fgobj/dObjectDeco.h"

namespace fgobj {
// RTTI N5fgobj15ObjectDecoSwingE @ 0x008D29F4
// vtable 0x00908D24 (vptr 0x00908D2C), offset_to_top 0, 10 entries
class ObjectDecoSwing : public ::fgobj::ObjectDeco
{
public:
    ObjectDecoSwing(); // ctor candidate(s) 0x00595A24 (unverified)
    virtual ~ObjectDecoSwing(); // 0x00595C28 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00595BD0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00595B48 slot 0x20 | virtual slot, introduced by fgobj::ObjectDeco
};
} // namespace fgobj
