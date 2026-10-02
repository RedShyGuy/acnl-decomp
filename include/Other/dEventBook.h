#pragma once

#include "decomp.h"
#include "Other/dAddressBook.h"

// RTTI 9EventBook @ 0x008CD784
// vtable 0x008FA658 (vptr 0x008FA660), offset_to_top 0, 8 entries
class EventBook : public ::AddressBook
{
public:
    EventBook(); // ctor candidate(s) 0x006E5374 (unverified)
    virtual void vf_0x00(); // 0x006E5388 slot 0x00 | virtual slot, introduced by EventBook
    virtual void vf_0x04(); // 0x006E5384 slot 0x04 | virtual slot, introduced by EventBook
    virtual void vf_0x08(); // 0x006E536C slot 0x08 | virtual slot, introduced by EventBook
    virtual void vf_0x0C(); // 0x006E5370 slot 0x0C | virtual slot, introduced by EventBook
    virtual void vf_0x10(); // 0x0075A25C slot 0x10 | virtual slot, introduced by EventBook
    virtual void vf_0x14(); // 0x0077083C slot 0x14 | virtual slot, introduced by EventBook
    virtual void vf_0x18(); // 0x001C0B40 slot 0x18 | virtual slot, introduced by AutoCampBook
    virtual void vf_0x1C(); // 0x001C0B3C slot 0x1C | virtual slot, introduced by DowntownBook
};
