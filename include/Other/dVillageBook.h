#pragma once

#include "decomp.h"
#include "Other/dAddressBook.h"

// RTTI 11VillageBook @ 0x008CB370
// vtable 0x008ECE20 (vptr 0x008ECE28), offset_to_top 0, 8 entries
class VillageBook : public ::AddressBook
{
public:
    VillageBook(); // ctor candidate(s) 0x001E5E38 (unverified)
    virtual void vf_0x00(); // 0x001E5E4C slot 0x00 | virtual slot, introduced by VillageBook
    virtual void vf_0x04(); // 0x001E5E48 slot 0x04 | virtual slot, introduced by VillageBook
    virtual void vf_0x08(); // 0x001E5E30 slot 0x08 | virtual slot, introduced by VillageBook
    virtual void vf_0x0C(); // 0x001E5E34 slot 0x0C | virtual slot, introduced by VillageBook
    virtual void vf_0x10(); // 0x00759EA8 slot 0x10 | virtual slot, introduced by VillageBook
    virtual void vf_0x14(); // 0x0071248C slot 0x14 | virtual slot, introduced by VillageBook
    virtual void vf_0x18(); // 0x001C0B40 slot 0x18 | virtual slot, introduced by AutoCampBook
    virtual void vf_0x1C(); // 0x001C0B3C slot 0x1C | virtual slot, introduced by DowntownBook
};
