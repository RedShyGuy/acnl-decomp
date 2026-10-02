#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// RTTI 8AcFtrBed @ 0x008CD41C
// vtable 0x008F9298 (vptr 0x008F92A0), offset_to_top 0, 67 entries
// vtable 0x008F93AC (vptr 0x008F93B4), offset_to_top -104, 4 entries
// vtable 0x008F93C4 (vptr 0x008F93CC), offset_to_top -176, 72 entries
// vtable 0x008F94EC (vptr 0x008F94F4), offset_to_top -300, 14 entries
class AcFtrBed : public ::AcFtr
{
public:
    AcFtrBed(); // ctor candidate(s) 0x007F3368 (unverified)
    virtual ~AcFtrBed(); // 0x00631F14 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00631F04 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0xB0(); // 0x00631BB0 slot 0xB0 | virtual slot, introduced by AcFtr
    virtual void vf_0xB8(); // 0x0063191C slot 0xB8 | virtual slot, introduced by AcFtr
    virtual void vf_0xF8(); // 0x006319F8 slot 0xF8 | virtual slot, introduced by AcFtr
};
