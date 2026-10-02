#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 13SetDateTimeBg @ 0x008CBAB0
// vtable 0x008EF780 (vptr 0x008EF788), offset_to_top 0, 3 entries
class SetDateTimeBg : public ::state::Mode<SetDateTimeBg>
{
public:
    SetDateTimeBg(); // ctor address unknown
    virtual void vf_0x00(); // 0x00244F88 slot 0x00 | virtual slot, introduced by SetDateTimeBg
    virtual void vf_0x04(); // 0x00244F50 slot 0x04 | virtual slot, introduced by SetDateTimeBg
    virtual void vf_0x08(); // 0x0082B148 slot 0x08 | virtual slot, introduced by SetDateTimeBg
};
