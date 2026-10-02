#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 4Wipe @ 0x008CD258
// vtable 0x008F859C (vptr 0x008F85A4), offset_to_top 0, 3 entries
class Wipe : public ::state::Mode<Wipe>
{
public:
    Wipe(); // ctor candidate(s) 0x0052BEC4 (unverified)
    virtual void vf_0x00(); // 0x0052C45C slot 0x00 | virtual slot, introduced by Wipe
    virtual void vf_0x04(); // 0x0052C41C slot 0x04 | virtual slot, introduced by Wipe
    virtual void vf_0x08(); // 0x0082CF48 slot 0x08 | virtual slot, introduced by Wipe
};
