#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

// RTTI 10dLoadSplit @ 0x008CB214
// vtable 0x008EC5F4 (vptr 0x008EC5FC), offset_to_top 0, 4 entries
class dLoadSplit : public ::ssys::ma::LoadSplit
{
public:
    dLoadSplit(); // ctor candidate(s) 0x0012F950, 0x0078BECC, 0x007963A4, 0x007F1474 (unverified)
    virtual ~dLoadSplit(); // 0x0013CA04 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x001BA034 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
