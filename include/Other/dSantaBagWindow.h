#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 14SantaBagWindow @ 0x008CBD88
// vtable 0x008F071C (vptr 0x008F0724), offset_to_top 0, 3 entries
class SantaBagWindow : public ::state::Mode<SantaBagWindow>
{
public:
    SantaBagWindow(); // ctor candidate(s) 0x00275DC4 (unverified)
    virtual void vf_0x00(); // 0x00275F50 slot 0x00 | virtual slot, introduced by SantaBagWindow
    virtual void vf_0x04(); // 0x00275EA4 slot 0x04 | virtual slot, introduced by SantaBagWindow
    virtual void vf_0x08(); // 0x0082B580 slot 0x08 | virtual slot, introduced by SantaBagWindow
};
