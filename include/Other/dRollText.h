#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 8RollText @ 0x008CD560
// vtable 0x008F9DA8 (vptr 0x008F9DB0), offset_to_top 0, 3 entries
class RollText : public ::state::Mode<RollText>
{
public:
    RollText(); // ctor address unknown
    virtual void vf_0x00(); // 0x006B25CC slot 0x00 | virtual slot, introduced by RollText
    virtual void vf_0x04(); // 0x006B2594 slot 0x04 | virtual slot, introduced by RollText
    virtual void vf_0x08(); // 0x0082D3F8 slot 0x08 | virtual slot, introduced by RollText
};
