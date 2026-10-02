#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 9BlackWait @ 0x008CD6B0
// vtable 0x008FA298 (vptr 0x008FA2A0), offset_to_top 0, 3 entries
class BlackWait : public ::state::Mode<BlackWait>
{
public:
    class SingletonDisposer_;
    BlackWait(); // ctor candidate(s) 0x001215C0 (unverified)
    virtual void vf_0x00(); // 0x006CFD64 slot 0x00 | virtual slot, introduced by BlackWait
    virtual void vf_0x04(); // 0x006CFD18 slot 0x04 | virtual slot, introduced by BlackWait
    virtual void vf_0x08(); // 0x0082D560 slot 0x08 | virtual slot, introduced by BlackWait
    static BlackWait* s_pInstance; // 0x0094F4C4
};
