#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 21BsWeatherPaperHioNode @ 0x008CCCC0
// vtable 0x008F6120 (vptr 0x008F6128), offset_to_top 0, 1 entries
class BsWeatherPaperHioNode : public ::sead::hostio::Node
{
public:
    BsWeatherPaperHioNode(); // ctor candidate(s) 0x00799DD0 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
