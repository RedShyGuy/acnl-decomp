#pragma once

#include "decomp.h"
#include "Other/dControlButtonBase.h"

// RTTI 14Control1Button @ 0x008CBCA0
// vtable 0x008F0254 (vptr 0x008F025C), offset_to_top 0, 15 entries
// vtable 0x008F0298 (vptr 0x008F02A0), offset_to_top -228, 3 entries
class Control1Button : public ::ControlButtonBase
{
public:
    Control1Button(); // ctor candidate(s) 0x0025EAC8 (unverified)
    virtual ~Control1Button(); // 0x00268CE4 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00268C9C slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
