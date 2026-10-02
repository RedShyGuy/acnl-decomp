#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 20BsWeatherRainHioNode @ 0x008CCB40
// vtable 0x008F5754 (vptr 0x008F575C), offset_to_top 0, 1 entries
class BsWeatherRainHioNode : public ::sead::hostio::Node
{
public:
    BsWeatherRainHioNode(); // ctor candidate(s) 0x007988CC (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
