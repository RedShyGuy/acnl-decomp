#pragma once

#include "decomp.h"
#include "ssys/st/dListPriority.h"

namespace demo {
// RTTI N4demo9OrderListE @ 0x008D10D8
// vtable 0x00904854 (vptr 0x0090485C), offset_to_top 0, 2 entries
class OrderList : public ::ssys::st::ListPriority
{
public:
    OrderList(); // ctor candidate(s) 0x007A04D8 (unverified)
    virtual ~OrderList(); // 0x0052E1B4 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x0052E1B0 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
} // namespace demo
