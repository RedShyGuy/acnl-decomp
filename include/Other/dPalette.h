#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 7Palette @ 0x008CD3D4
// vtable 0x008F9184 (vptr 0x008F918C), offset_to_top 0, 4 entries
class Palette : public ::state::Mode<Palette>
{
public:
    Palette(); // ctor address unknown
    virtual void vf_0x00(); // 0x00611AC8 slot 0x00 | virtual slot, introduced by Palette
    virtual void vf_0x04(); // 0x00611A00 slot 0x04 | virtual slot, introduced by Palette
    virtual void vf_0x08(); // 0x0082D1A0 slot 0x08 | virtual slot, introduced by Palette
    virtual void vf_0x0C(); // 0x0075E96C slot 0x0C | virtual slot, introduced by Palette
};
