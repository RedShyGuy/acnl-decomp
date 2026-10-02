#pragma once

#include "decomp.h"
#include "Bs/dBsLightMgr.h"
#include "sead/hostio/seadNode.h"

// RTTI N10BsLightMgr15LightMgrHioNodeE @ 0x008CD814
// vtable 0x008FA958 (vptr 0x008FA960), offset_to_top 0, 1 entries
class BsLightMgr::LightMgrHioNode : public ::sead::hostio::Node
{
public:
    LightMgrHioNode(); // ctor candidate(s) 0x00791AE8 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
