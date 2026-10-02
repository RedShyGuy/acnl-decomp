#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 16GetVisitNpcIdxCB @ 0x008CC320
// vtable 0x008F29E8 (vptr 0x008F29F0), offset_to_top 0, 1 entries
class GetVisitNpcIdxCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetVisitNpcIdxCB(); // ctor candidate(s) 0x00643288, 0x0064C57C (unverified)
    virtual void vf_0x00(); // 0x002B8638 slot 0x00 | virtual slot, introduced by GetVisitNpcIdxCB
};
