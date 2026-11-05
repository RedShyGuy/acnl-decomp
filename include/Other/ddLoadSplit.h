#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

// a LoadSplit of the game (an implicit constructor: value-initialized members are cleared first)
// RTTI 10dLoadSplit @ 0x008CB214
// vtable 0x008EC5F4 (vptr 0x008EC5FC), offset_to_top 0, 4 entries
class dLoadSplit : public ::ssys::ma::LoadSplit
{
public:
    DECOMP_EMIT_COPY dLoadSplit() = default; // 0x0012F950 (name after the RTTI)
    virtual ~dLoadSplit(); // 0x0013CA04 slot 0x00 (name after the RTTI)
    // 0x001BA034 slot 0x04 (deleting dtor)
};
