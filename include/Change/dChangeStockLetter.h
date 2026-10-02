#pragma once

#include "decomp.h"
#include "Change/dChangeListBase.h"
#include "state/dMode.h"

// RTTI 17ChangeStockLetter @ 0x008CC4FC
// vtable 0x008F3760 (vptr 0x008F3768), offset_to_top 0, 9 entries
// vtable 0x008F378C (vptr 0x008F3794), offset_to_top -2624, 3 entries
// vtable 0x008F37A0 (vptr 0x008F37A8), offset_to_top -6280, 3 entries
class ChangeStockLetter : public ::ChangeListBase, public ::state::Mode<ChangeStockLetter>
{
public:
    ChangeStockLetter(); // ctor candidate(s) 0x002CCA88 (unverified)
    virtual ~ChangeStockLetter(); // 0x002CCB90 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x002CCB80 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x20(); // 0x002CCA08 slot 0x20 | virtual slot, introduced by ChangeListBase
};
