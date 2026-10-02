#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 4Fade @ 0x008CD24C
// vtable 0x008F8588 (vptr 0x008F8590), offset_to_top 0, 3 entries
class Fade : public ::state::Mode<Fade>
{
public:
    Fade(); // ctor candidate(s) 0x0052AA64 (unverified)
    virtual void vf_0x00(); // 0x0052B114 slot 0x00 | virtual slot, introduced by Fade
    virtual void vf_0x04(); // 0x0052B0C4 slot 0x04 | virtual slot, introduced by Fade
    virtual void vf_0x08(); // 0x0082CED0 slot 0x08 | virtual slot, introduced by Fade
};
