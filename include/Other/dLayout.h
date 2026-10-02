#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dLayout.h"

// RTTI 6Layout @ 0x008CD2F0
// vtable 0x008F8C70 (vptr 0x008F8C78), offset_to_top 0, 7 entries
class Layout : public ::ssys::ma::lyt::Layout
{
public:
    virtual ~Layout(); // 0x005C0214 slot 0x00 | libgarden
    // 0x005C01E4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x14(); // 0x00569D98 slot 0x14 | virtual slot, introduced by ssys::ma::lyt::Layout
    virtual void vf_0x18(); // 0x005C004C slot 0x18 | virtual slot, introduced by ssys::ma::lyt::Layout
    Layout(); // 0x00126550 | libgarden [tier A]
};
