#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace npcutil {
// RTTI N7npcutil11NpcSearchCBE @ 0x008D3F94
// vtable 0x0090BD90 (vptr 0x0090BD98), offset_to_top 0, 1 entries
class NpcSearchCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcSearchCB(); // ctor candidate(s) 0x0062D7C4 (unverified)
    virtual void vf_0x00(); // 0x0062D4A0 slot 0x00 | virtual slot, introduced by npcutil::NpcSearchCB
};
} // namespace npcutil
