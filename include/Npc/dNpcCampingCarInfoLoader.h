#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

// RTTI 23NpcCampingCarInfoLoader @ 0x008CCF2C
// vtable 0x008F72CC (vptr 0x008F72D4), offset_to_top 0, 4 entries
class NpcCampingCarInfoLoader : public ::ssys::ma::LoadSplit
{
public:
    NpcCampingCarInfoLoader(); // ctor candidate(s) 0x00335A80 (unverified)
    virtual ~NpcCampingCarInfoLoader(); // 0x00335AB0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00335AA0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
