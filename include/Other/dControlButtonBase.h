#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "state/dMode.h"

// RTTI 17ControlButtonBase @ 0x008CC528
// vtable 0x008F37D8 (vptr 0x008F37E0), offset_to_top 0, 15 entries
// vtable 0x008F381C (vptr 0x008F3824), offset_to_top -228, 3 entries
class ControlButtonBase : public ::ButtonActionControl, public ::state::Mode<ControlButtonBase>
{
public:
    ControlButtonBase(); // ctor candidate(s) 0x002CDA10 (unverified)
    virtual ~ControlButtonBase(); // 0x002F8184 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x002CDA78 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void Update(); // 0x002CD748 slot 0x08 | slot vf_0x08 of ButtonActionControl
};
