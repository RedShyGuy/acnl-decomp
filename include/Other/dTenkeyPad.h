#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 9TenkeyPad @ 0x008CD7F0
// vtable 0x008FA834 (vptr 0x008FA83C), offset_to_top 0, 3 entries
class TenkeyPad : public ::state::Mode<TenkeyPad>
{
public:
    TenkeyPad(); // ctor address unknown
    virtual void vf_0x00(); // 0x006F6B4C slot 0x00 | virtual slot, introduced by TenkeyPad
    virtual void vf_0x04(); // 0x006F6ACC slot 0x04 | virtual slot, introduced by TenkeyPad
    virtual void vf_0x08(); // 0x0082D830 slot 0x08 | virtual slot, introduced by TenkeyPad
};
