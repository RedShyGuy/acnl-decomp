#pragma once

#include "decomp.h"
#include "Other/dControlButtonBase.h"

// RTTI 14Control2Button @ 0x008CBCAC
// vtable 0x008F02AC (vptr 0x008F02B4), offset_to_top 0, 15 entries
// vtable 0x008F02F0 (vptr 0x008F02F8), offset_to_top -228, 3 entries
class Control2Button : public ::ControlButtonBase
{
public:
    Control2Button(); // ctor address unknown
    virtual ~Control2Button(); // 0x00268F80 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00268F38 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
