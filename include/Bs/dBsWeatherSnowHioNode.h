#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 20BsWeatherSnowHioNode @ 0x008CCB4C
// vtable 0x008F5760 (vptr 0x008F5768), offset_to_top 0, 1 entries
class BsWeatherSnowHioNode : public ::sead::hostio::Node
{
public:
    BsWeatherSnowHioNode(); // ctor candidate(s) 0x00798908 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
