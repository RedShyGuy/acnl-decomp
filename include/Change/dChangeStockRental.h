#pragma once

#include "decomp.h"
#include "Change/dChangeRentalBase.h"

// RTTI 17ChangeStockRental @ 0x008CC51C
// vtable 0x008F37B4 (vptr 0x008F37BC), offset_to_top 0, 7 entries
class ChangeStockRental : public ::ChangeRentalBase
{
public:
    ChangeStockRental(); // ctor candidate(s) 0x002CD054 (unverified)
    virtual ~ChangeStockRental(); // 0x002CD108 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x002CD0BC slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
};
