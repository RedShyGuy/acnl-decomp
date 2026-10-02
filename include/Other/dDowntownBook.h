#pragma once

#include "decomp.h"
#include "Other/dAddressBook.h"

// RTTI 12DowntownBook @ 0x008CB5E4
// vtable 0x008EE374 (vptr 0x008EE37C), offset_to_top 0, 8 entries
class DowntownBook : public ::AddressBook
{
public:
    DowntownBook(); // ctor candidate(s) 0x001FFA1C (unverified)
    virtual void vf_0x00(); // 0x001FFA30 slot 0x00 | virtual slot, introduced by DowntownBook
    virtual void vf_0x04(); // 0x001FFA2C slot 0x04 | virtual slot, introduced by DowntownBook
    virtual void vf_0x08(); // 0x001FF730 slot 0x08 | virtual slot, introduced by DowntownBook
    virtual void vf_0x0C(); // 0x001FFA04 slot 0x0C | virtual slot, introduced by DowntownBook
    virtual void vf_0x10(); // 0x00712FB8 slot 0x10 | virtual slot, introduced by DowntownBook
    virtual void vf_0x14(); // 0x00712F1C slot 0x14 | virtual slot, introduced by DowntownBook
    virtual void vf_0x18(); // 0x001C0B40 slot 0x18 | virtual slot, introduced by AutoCampBook
    virtual void vf_0x1C(); // 0x001C0B3C slot 0x1C | virtual slot, introduced by DowntownBook
};
