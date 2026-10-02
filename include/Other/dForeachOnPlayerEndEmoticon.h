#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 26ForeachOnPlayerEndEmoticon @ 0x008CD0BC
// vtable 0x008F7EC8 (vptr 0x008F7ED0), offset_to_top 0, 1 entries
class ForeachOnPlayerEndEmoticon : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachOnPlayerEndEmoticon(); // ctor candidate(s) 0x006994B8 (unverified)
    virtual void vf_0x00(); // 0x00342B50 slot 0x00 | virtual slot, introduced by ForeachOnPlayerEndEmoticon
};
