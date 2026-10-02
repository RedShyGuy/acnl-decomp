#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 9FaderWipe @ 0x008CD790
// vtable 0x008FA680 (vptr 0x008FA688), offset_to_top 0, 8 entries
class FaderWipe : public ::ssys::co::FaderBase
{
public:
    FaderWipe(); // ctor candidate(s) 0x006E5598 (unverified)
    virtual void vf_0x00(); // 0x006E55C0 slot 0x00 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x04(); // 0x006E55B0 slot 0x04 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x08(); // 0x006E5568 slot 0x08 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x10(); // 0x006E5498 slot 0x10 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x14(); // 0x006E5500 slot 0x14 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x18(); // 0x006E538C slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x1C(); // 0x006E5494 slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
};
