#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 17BsMenuNumberInput @ 0x008CC4A0
// vtable 0x008F3570 (vptr 0x008F3578), offset_to_top 0, 25 entries
// vtable 0x008F35DC (vptr 0x008F35E4), offset_to_top -40, 3 entries
class BsMenuNumberInput : public ::MenuBase, public ::state::Mode<BsMenuNumberInput>
{
public:
    BsMenuNumberInput(); // ctor address unknown
    virtual ~BsMenuNumberInput(); // 0x002C953C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C952C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C8F88 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002C931C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002C9104 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C8F58 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
