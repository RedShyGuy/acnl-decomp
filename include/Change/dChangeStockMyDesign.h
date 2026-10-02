#pragma once

#include "decomp.h"
#include "Change/dChangeListBase.h"
#include "state/dMode.h"

// RTTI 19ChangeStockMyDesign @ 0x008CC9D0
// vtable 0x008F4F9C (vptr 0x008F4FA4), offset_to_top 0, 9 entries
// vtable 0x008F4FC8 (vptr 0x008F4FD0), offset_to_top -2624, 3 entries
// vtable 0x008F4FDC (vptr 0x008F4FE4), offset_to_top -6280, 3 entries
class ChangeStockMyDesign : public ::ChangeListBase, public ::state::Mode<ChangeStockMyDesign>
{
public:
    ChangeStockMyDesign(); // ctor candidate(s) 0x002F93C0 (unverified)
    virtual ~ChangeStockMyDesign(); // 0x002F94E0 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x002F94B8 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x0C(); // 0x00267D40 slot 0x0C | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x20(); // 0x002F8B44 slot 0x20 | virtual slot, introduced by ChangeListBase
};
