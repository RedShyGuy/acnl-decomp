#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 8ComTitle @ 0x008CD4E4
// vtable 0x008F9BF4 (vptr 0x008F9BFC), offset_to_top 0, 3 entries
class ComTitle : public ::state::Mode<ComTitle>
{
public:
    ComTitle(); // ctor address unknown
    virtual void vf_0x00(); // 0x006A4C6C slot 0x00 | virtual slot, introduced by ComTitle
    virtual void vf_0x04(); // 0x006A4BF4 slot 0x04 | virtual slot, introduced by ComTitle
    virtual void vf_0x08(); // 0x0082D308 slot 0x08 | virtual slot, introduced by ComTitle
};
