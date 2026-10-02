#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 24BsSeadParticleMgrHioNode @ 0x008CCFD0
// vtable 0x008F7850 (vptr 0x008F7858), offset_to_top 0, 1 entries
class BsSeadParticleMgrHioNode : public ::sead::hostio::Node
{
public:
    BsSeadParticleMgrHioNode(); // ctor candidate(s) 0x0033DB50, 0x0079C5EC (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
