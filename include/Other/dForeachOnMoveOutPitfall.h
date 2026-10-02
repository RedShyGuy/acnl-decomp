#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 23ForeachOnMoveOutPitfall @ 0x008CCF08
// vtable 0x008F7230 (vptr 0x008F7238), offset_to_top 0, 1 entries
class ForeachOnMoveOutPitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachOnMoveOutPitfall(); // ctor candidate(s) 0x0069B094 (unverified)
    virtual void vf_0x00(); // 0x003353EC slot 0x00 | virtual slot, introduced by ForeachOnMoveOutPitfall
};
