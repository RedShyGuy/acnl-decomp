#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 22BsWeatherSakuraHioNode @ 0x008CCDC8
// vtable 0x008F6A14 (vptr 0x008F6A1C), offset_to_top 0, 1 entries
class BsWeatherSakuraHioNode : public ::sead::hostio::Node
{
public:
    BsWeatherSakuraHioNode(); // ctor candidate(s) 0x0079B1DC (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
