#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 10FaderColor @ 0x008CB0DC
// vtable 0x008EC174 (vptr 0x008EC17C), offset_to_top 0, 8 entries
class FaderColor : public ::ssys::co::FaderBase
{
public:
    FaderColor(); // ctor candidate(s) 0x001AA9F8 (unverified)
    virtual void vf_0x00(); // 0x00567008 slot 0x00 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x04(); // 0x001AAA48 slot 0x04 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x08(); // 0x001AA9C8 slot 0x08 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x10(); // 0x001AA840 slot 0x10 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x14(); // 0x001AA908 slot 0x14 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x18(); // 0x001AA734 slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x1C(); // 0x001AA83C slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
};
