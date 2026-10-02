#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 16NpcOnPreviewFunc @ 0x008CC368
// vtable 0x008F2B38 (vptr 0x008F2B40), offset_to_top 0, 1 entries
class NpcOnPreviewFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcOnPreviewFunc(); // ctor candidate(s) 0x00696438 (unverified)
    virtual void vf_0x00(); // 0x002BB598 slot 0x00 | virtual slot, introduced by NpcOnPreviewFunc
};
