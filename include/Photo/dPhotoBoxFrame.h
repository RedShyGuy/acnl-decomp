#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 13PhotoBoxFrame @ 0x008CBA6C
// vtable 0x008EF6B8 (vptr 0x008EF6C0), offset_to_top 0, 3 entries
class PhotoBoxFrame : public ::state::Mode<PhotoBoxFrame>
{
public:
    PhotoBoxFrame(); // ctor address unknown
    virtual void vf_0x00(); // 0x0023D088 slot 0x00 | virtual slot, introduced by PhotoBoxFrame
    virtual void vf_0x04(); // 0x0023D058 slot 0x04 | virtual slot, introduced by PhotoBoxFrame
    virtual void vf_0x08(); // 0x0082B058 slot 0x08 | virtual slot, introduced by PhotoBoxFrame
};
