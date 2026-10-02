#pragma once

#include "decomp.h"
#include "fgobj/dObjectMove.h"

namespace fgobj {
// RTTI N5fgobj11ObjectThrowE @ 0x008D29AC
// vtable 0x00908C24 (vptr 0x00908C2C), offset_to_top 0, 10 entries
class ObjectThrow : public ::fgobj::ObjectMove
{
public:
    ObjectThrow(); // ctor address unknown
    virtual ~ObjectThrow(); // 0x00594C20 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00594BF0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00594B58 slot 0x20 | virtual slot, introduced by fgobj::ObjectThrow
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
