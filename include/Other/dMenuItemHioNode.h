#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 15MenuItemHioNode @ 0x008CC0C8
// vtable 0x008F1B20 (vptr 0x008F1B28), offset_to_top 0, 1 entries
class MenuItemHioNode : public ::sead::hostio::Node
{
public:
    MenuItemHioNode(); // ctor candidate(s) 0x00791E20 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
