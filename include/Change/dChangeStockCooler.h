#pragma once

#include "decomp.h"
#include "Change/dChangeListBase.h"

// RTTI 17ChangeStockCooler @ 0x008CC4F0
// vtable 0x008F371C (vptr 0x008F3724), offset_to_top 0, 10 entries
// vtable 0x008F374C (vptr 0x008F3754), offset_to_top -2624, 3 entries
class ChangeStockCooler : public ::ChangeListBase
{
public:
    ChangeStockCooler(); // ctor candidate(s) 0x002CC5D8 (unverified)
    virtual ~ChangeStockCooler(); // 0x002CC6E4 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x002CC694 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x20(); // 0x002CC4EC slot 0x20 | virtual slot, introduced by ChangeListBase
    virtual void vf_0x24(); // 0x002CC188 slot 0x24 | virtual slot, introduced by ChangeStockCooler
};
