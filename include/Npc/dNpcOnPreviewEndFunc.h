#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// RTTI 19NpcOnPreviewEndFunc @ 0x008CCA20
// vtable 0x008F50D0 (vptr 0x008F50D8), offset_to_top 0, 1 entries
class NpcOnPreviewEndFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcOnPreviewEndFunc(); // ctor candidate(s) 0x00697BCC (unverified)
    virtual void vf_0x00(); // 0x002FA634 slot 0x00 | virtual slot, introduced by NpcOnPreviewEndFunc
};
