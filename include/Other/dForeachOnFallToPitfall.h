#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 22ForeachOnFallToPitfall @ 0x008CCDE0
// vtable 0x008F6A48 (vptr 0x008F6A50), offset_to_top 0, 1 entries
class ForeachOnFallToPitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachOnFallToPitfall(); // ctor candidate(s) 0x0069A254 (unverified)
    virtual void vf_0x00(); // 0x003300A8 slot 0x00 | virtual slot, introduced by ForeachOnFallToPitfall
};
