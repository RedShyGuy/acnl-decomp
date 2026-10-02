#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI N8BsNpcMgr14SearchFunctionE @ 0x008D3FE8
// vtable 0x0090C078 (vptr 0x0090C080), offset_to_top 0, 1 entries
class BsNpcMgr::SearchFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    SearchFunction(); // ctor candidate(s) 0x0051D530, 0x006962E0, 0x006969B0, 0x0069F958 (unverified)
    virtual void vf_0x00(); // 0x0069670C slot 0x00 | virtual slot, introduced by BsNpcMgr::SearchFunction
};
