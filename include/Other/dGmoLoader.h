#pragma once

#include "decomp.h"
#include "Other/ddLoadSplit.h"

// RTTI 9GmoLoader @ 0x008CD79C
// vtable 0x008FA6A8 (vptr 0x008FA6B0), offset_to_top 0, 4 entries
class GmoLoader : public ::dLoadSplit
{
public:
    GmoLoader(); // ctor address unknown
    virtual ~GmoLoader(); // 0x006E5774 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x006E5764 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x006E56BC slot 0x08 | virtual slot, introduced by ssys::ma::LoadSplit
};
