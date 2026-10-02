#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 10BlackToBtn @ 0x008CAF54
// vtable 0x008EBCD0 (vptr 0x008EBCD8), offset_to_top 0, 3 entries
class BlackToBtn : public ::state::Mode<BlackToBtn>
{
public:
    BlackToBtn(); // ctor candidate(s) 0x0018F540 (unverified)
    virtual void vf_0x00(); // 0x0018F5E4 slot 0x00 | virtual slot, introduced by BlackToBtn
    virtual void vf_0x04(); // 0x0018F5AC slot 0x04 | virtual slot, introduced by BlackToBtn
    virtual void vf_0x08(); // 0x0082A680 slot 0x08 | virtual slot, introduced by BlackToBtn
};
