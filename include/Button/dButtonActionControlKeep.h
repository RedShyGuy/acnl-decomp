#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// RTTI 23ButtonActionControlKeep @ 0x008CCEE4
// vtable 0x008F719C (vptr 0x008F71A4), offset_to_top 0, 15 entries
class ButtonActionControlKeep : public ::ButtonActionControl
{
public:
    ButtonActionControlKeep(); // ctor address unknown
    virtual ~ButtonActionControlKeep(); // 0x00335038 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00335028 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void vf_0x2C(); // 0x00334FFC slot 0x2C | virtual slot, introduced by ButtonActionControl
};
