#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// RTTI 21ButtonActionControlEx @ 0x008CCCCC
// vtable 0x008F612C (vptr 0x008F6134), offset_to_top 0, 15 entries
class ButtonActionControlEx : public ::ButtonActionControl
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ButtonActionControlEx(); // ctor candidate(s) 0x00328338 (unverified)
    virtual ~ButtonActionControlEx(); // 0x003283AC slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00328370 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    void Initialize(ButtonActionControlEx::Description const&); // 0x003281D8 | libgarden [tier A]
    void AddNode(ButtonActionNodeEx*, bool); // 0x003282D4 | libgarden [tier A]
};
