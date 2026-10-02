#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 24ForeachOnStrugglePitfall @ 0x008CD000
// vtable 0x008F789C (vptr 0x008F78A4), offset_to_top 0, 1 entries
class ForeachOnStrugglePitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachOnStrugglePitfall(); // ctor candidate(s) 0x0069B2B4 (unverified)
    virtual void vf_0x00(); // 0x0033DDCC slot 0x00 | virtual slot, introduced by ForeachOnStrugglePitfall
};
