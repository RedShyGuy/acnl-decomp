#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace shopfollower {
// RTTI N12shopfollower18VisibleNpcPosGetCBE @ 0x008CD92C
// vtable 0x008FB1D0 (vptr 0x008FB1D8), offset_to_top 0, 1 entries
class VisibleNpcPosGetCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    VisibleNpcPosGetCB(); // ctor address unknown
    virtual void vf_0x00(); // 0x0020E734 slot 0x00 | virtual slot, introduced by shopfollower::VisibleNpcPosGetCB
};
} // namespace shopfollower
