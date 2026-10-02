#pragma once

#include "decomp.h"
#include "Bs/dBsTourMgr.h"
#include "sead/hostio/seadNode.h"

// RTTI N9BsTourMgr14TourMgrHioNodeE @ 0x008D40B4
// vtable 0x0090C21C (vptr 0x0090C224), offset_to_top 0, 1 entries
class BsTourMgr::TourMgrHioNode : public ::sead::hostio::Node
{
public:
    TourMgrHioNode(); // ctor candidate(s) 0x007893A0 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
