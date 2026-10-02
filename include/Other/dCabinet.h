#pragma once

#include "decomp.h"
#include "Change/dChangeListBase.h"

// RTTI 7Cabinet @ 0x008CD39C
// vtable 0x008F907C (vptr 0x008F9084), offset_to_top 0, 17 entries
// vtable 0x008F90C8 (vptr 0x008F90D0), offset_to_top -2624, 3 entries
class Cabinet : public ::ChangeListBase
{
public:
    Cabinet(); // ctor address unknown
    virtual ~Cabinet(); // 0x0060CD10 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x0060CCC0 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
};
