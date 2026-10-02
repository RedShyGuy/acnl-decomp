#pragma once

#include "decomp.h"
#include "Other/ddLoadSplit.h"

// RTTI 8RoModule @ 0x008CD554
// vtable 0x008F9D90 (vptr 0x008F9D98), offset_to_top 0, 4 entries
class RoModule : public ::dLoadSplit
{
public:
    RoModule(); // ctor address unknown
    virtual ~RoModule(); // 0x006B2200 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x006B21E4 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x006B20F0 slot 0x08 | virtual slot, introduced by ssys::ma::LoadSplit
    virtual void vf_0x0C(); // 0x006B2198 slot 0x0C | virtual slot, introduced by ssys::ma::LoadSplit
};
