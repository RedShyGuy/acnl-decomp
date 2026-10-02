#pragma once

#include "decomp.h"
#include "Other/ddLoadSplit.h"
#include "script/dLoader.h"

// RTTI N6script6Loader9LoadSplitE @ 0x008D3A98
// vtable 0x0090A8C4 (vptr 0x0090A8CC), offset_to_top 0, 4 entries
class script::Loader::LoadSplit : public ::dLoadSplit
{
public:
    LoadSplit(); // ctor candidate(s) 0x005F1C08 (unverified)
    virtual ~LoadSplit(); // 0x005F1C04 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x005F1BF4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x0C(); // 0x005F1BD8 slot 0x0C | virtual slot, introduced by ssys::ma::LoadSplit
};
