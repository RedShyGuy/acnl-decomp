#pragma once

#include "decomp.h"
#include "Other/dCabinet.h"

// RTTI 20ChangeStockWarehouse @ 0x008CCB64
// vtable 0x008F57B4 (vptr 0x008F57BC), offset_to_top 0, 17 entries
// vtable 0x008F5800 (vptr 0x008F5808), offset_to_top -2624, 3 entries
class ChangeStockWarehouse : public ::Cabinet
{
public:
    ChangeStockWarehouse(); // ctor candidate(s) 0x0031D308 (unverified)
    virtual ~ChangeStockWarehouse(); // 0x0031D424 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x0031D3D4 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x20(); // 0x0031D2BC slot 0x20 | virtual slot, introduced by ChangeListBase
    virtual void vf_0x24(); // 0x0031D0BC slot 0x24 | virtual slot, introduced by Cabinet
    virtual void vf_0x28(); // 0x007248E8 slot 0x28 | virtual slot, introduced by Cabinet
    virtual void vf_0x2C(); // 0x007248BC slot 0x2C | virtual slot, introduced by Cabinet
    virtual void vf_0x30(); // 0x0031D018 slot 0x30 | virtual slot, introduced by Cabinet
    virtual void vf_0x34(); // 0x00724920 slot 0x34 | virtual slot, introduced by Cabinet
    virtual void vf_0x38(); // 0x0031D240 slot 0x38 | virtual slot, introduced by Cabinet
    virtual void vf_0x3C(); // 0x0031D078 slot 0x3C | virtual slot, introduced by Cabinet
    virtual void vf_0x40(); // 0x0031CFB0 slot 0x40 | virtual slot, introduced by Cabinet
};
