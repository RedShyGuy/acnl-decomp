#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 16ChangeRentalBase @ 0x008CC2E4
// vtable 0x008F2978 (vptr 0x008F2980), offset_to_top 0, 7 entries
class ChangeRentalBase : public ::state::Mode<ChangeRentalBase>
{
public:
    ChangeRentalBase(); // ctor address unknown
    virtual ~ChangeRentalBase(); // 0x0011C12F slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x08(); // 0x0082BC88 slot 0x08 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x0C(); // 0x002B7BA0 slot 0x0C | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x10(); // 0x002B7E48 slot 0x10 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x14(); // 0x0071DC60 slot 0x14 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x18(); // 0x002B7BC8 slot 0x18 | virtual slot, introduced by ChangeRentalBase
};
