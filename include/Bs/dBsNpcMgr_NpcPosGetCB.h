#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI N8BsNpcMgr11NpcPosGetCBE @ 0x008D3FDC
// vtable 0x0090C06C (vptr 0x0090C074), offset_to_top 0, 1 entries
class BsNpcMgr::NpcPosGetCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcPosGetCB(); // ctor candidate(s) 0x002A83E0, 0x004E4104, 0x0051CCF4, 0x0057384C, 0x006493F8, 0x006960A8, 0x006E0960 (unverified)
    virtual void vf_0x00(); // 0x006960E4 slot 0x00 | virtual slot, introduced by BsNpcMgr::NpcPosGetCB
};
