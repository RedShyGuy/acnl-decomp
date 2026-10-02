#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 8WaitIcon @ 0x008CD5B4
// vtable 0x008F9F78 (vptr 0x008F9F80), offset_to_top 0, 3 entries
class WaitIcon : public ::state::Mode<WaitIcon>
{
public:
    WaitIcon(); // ctor address unknown
    virtual void vf_0x00(); // 0x006C9CA4 slot 0x00 | virtual slot, introduced by WaitIcon
    virtual void vf_0x04(); // 0x006C9C64 slot 0x04 | virtual slot, introduced by WaitIcon
    virtual void vf_0x08(); // 0x0082D470 slot 0x08 | virtual slot, introduced by WaitIcon
};
