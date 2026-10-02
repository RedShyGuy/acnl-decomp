#pragma once

#include "decomp.h"
#include "ssys/co/dFaderBase.h"

// RTTI 11FaderMoment @ 0x008CB2CC
// vtable 0x008ECB50 (vptr 0x008ECB58), offset_to_top 0, 8 entries
class FaderMoment : public ::ssys::co::FaderBase
{
public:
    FaderMoment(); // ctor candidate(s) 0x001C884C (unverified)
    virtual void vf_0x00(); // 0x001C8874 slot 0x00 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x04(); // 0x001C8864 slot 0x04 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x08(); // 0x00566FAC slot 0x08 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x10(); // 0x00566F48 slot 0x10 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x14(); // 0x00566F7C slot 0x14 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x18(); // 0x001C878C slot 0x18 | virtual slot, introduced by ssys::co::FaderBase
    virtual void vf_0x1C(); // 0x001C8848 slot 0x1C | virtual slot, introduced by ssys::co::FaderBase
};
