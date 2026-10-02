#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 28ForeachOnPlayerStartEmoticon @ 0x008CD10C
// vtable 0x008F8120 (vptr 0x008F8128), offset_to_top 0, 1 entries
class ForeachOnPlayerStartEmoticon : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachOnPlayerStartEmoticon(); // ctor candidate(s) 0x0069A2C4 (unverified)
    virtual void vf_0x00(); // 0x00343974 slot 0x00 | virtual slot, introduced by ForeachOnPlayerStartEmoticon
};
