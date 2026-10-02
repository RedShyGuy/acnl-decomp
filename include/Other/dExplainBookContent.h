#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 18ExplainBookContent @ 0x008CC844
// vtable 0x008F471C (vptr 0x008F4724), offset_to_top 0, 3 entries
class ExplainBookContent : public ::state::Mode<ExplainBookContent>
{
public:
    ExplainBookContent(); // ctor candidate(s) 0x002E2F40 (unverified)
    virtual void vf_0x00(); // 0x002E30A4 slot 0x00 | virtual slot, introduced by ExplainBookContent
    virtual void vf_0x04(); // 0x002E301C slot 0x04 | virtual slot, introduced by ExplainBookContent
    virtual void vf_0x08(); // 0x0082C228 slot 0x08 | virtual slot, introduced by ExplainBookContent
};
