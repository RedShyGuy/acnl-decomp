#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 14NpcOnBuildFunc @ 0x008CBD20
// vtable 0x008F053C (vptr 0x008F0544), offset_to_top 0, 1 entries
class NpcOnBuildFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcOnBuildFunc(); // ctor candidate(s) 0x00696124 (unverified)
    virtual void vf_0x00(); // 0x00270AC4 slot 0x00 | virtual slot, introduced by NpcOnBuildFunc
};
