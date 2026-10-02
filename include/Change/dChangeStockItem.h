#pragma once

#include "decomp.h"
#include "Other/dCabinet.h"

// RTTI 15ChangeStockItem @ 0x008CC00C
// vtable 0x008F1850 (vptr 0x008F1858), offset_to_top 0, 17 entries
// vtable 0x008F189C (vptr 0x008F18A4), offset_to_top -2624, 3 entries
class ChangeStockItem : public ::Cabinet
{
public:
    ChangeStockItem(); // ctor candidate(s) 0x00297CE0 (unverified)
    virtual ~ChangeStockItem(); // 0x00297DF0 slot 0x00 | slot vf_0x00 of ChangeRentalBase
    // 0x00297DA0 slot 0x04 | slot vf_0x04 of ChangeRentalBase (deleting dtor)
    virtual void vf_0x20(); // 0x00297CA4 slot 0x20 | virtual slot, introduced by ChangeListBase
    virtual void vf_0x24(); // 0x00297A84 slot 0x24 | virtual slot, introduced by Cabinet
    virtual void vf_0x28(); // 0x0071B8CC slot 0x28 | virtual slot, introduced by Cabinet
    virtual void vf_0x2C(); // 0x0071B8A0 slot 0x2C | virtual slot, introduced by Cabinet
    virtual void vf_0x30(); // 0x002B8F58 slot 0x30 | virtual slot, introduced by Cabinet
    virtual void vf_0x34(); // 0x0071B904 slot 0x34 | virtual slot, introduced by Cabinet
    virtual void vf_0x38(); // 0x00297C2C slot 0x38 | virtual slot, introduced by Cabinet
    virtual void vf_0x3C(); // 0x00297A40 slot 0x3C | virtual slot, introduced by Cabinet
    virtual void vf_0x40(); // 0x002979EC slot 0x40 | virtual slot, introduced by Cabinet
};
