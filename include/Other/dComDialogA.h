#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 10ComDialogA @ 0x008CB040
// vtable 0x008EC160 (vptr 0x008EC168), offset_to_top 0, 3 entries
class ComDialogA : public ::state::Mode<ComDialogA>
{
public:
    ComDialogA(); // ctor candidate(s) 0x001AA4FC (unverified)
    virtual void vf_0x00(); // 0x001AA6B8 slot 0x00 | virtual slot, introduced by ComDialogA
    virtual void vf_0x04(); // 0x001AA63C slot 0x04 | virtual slot, introduced by ComDialogA
    virtual void vf_0x08(); // 0x0082A950 slot 0x08 | virtual slot, introduced by ComDialogA
};
