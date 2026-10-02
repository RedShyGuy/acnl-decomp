#pragma once

#include "decomp.h"
#include "ssys/st/dListNodePriorityBase.h"

namespace fgobj {
// RTTI N5fgobj10ObjectBaseE @ 0x008D2910
// vtable 0x00908A3C (vptr 0x00908A44), offset_to_top 0, 8 entries
class ObjectBase : public ::ssys::st::ListNodePriorityBase
{
public:
    ObjectBase(); // ctor address unknown
    virtual ~ObjectBase(); // 0x00591BA0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00591B70 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x005908B0 slot 0x08 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x0C(); // 0x0056B88C slot 0x0C | virtual slot, introduced by demo::Order
    virtual void vf_0x10(); // 0x00590428 slot 0x10 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x14(); // 0x005915C0 slot 0x14 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x18(); // 0x005902F8 slot 0x18 | virtual slot, introduced by fgobj::ObjectBase
    virtual void vf_0x1C(); // 0x00590300 slot 0x1C | virtual slot, introduced by fgobj::ObjectBase
};
} // namespace fgobj
