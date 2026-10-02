#pragma once

#include "decomp.h"
#include "Other/dListNodePriority.h"

namespace demo {
// RTTI N4demo5OrderE @ 0x008D10CC
// vtable 0x00904838 (vptr 0x00904840), offset_to_top 0, 5 entries
class Order : public ::ListNodePriority
{
public:
    Order(); // ctor candidate(s) 0x0052DC28 (unverified)
    virtual ~Order(); // 0x0052DC58 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0052DC54 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x002BB2B8 slot 0x08 | virtual slot, introduced by demo::Order
    virtual void vf_0x0C(); // 0x0056B88C slot 0x0C | virtual slot, introduced by demo::Order
    virtual void vf_0x10(); // 0x0071DEAC slot 0x10 | virtual slot, introduced by demo::Order
};
} // namespace demo
