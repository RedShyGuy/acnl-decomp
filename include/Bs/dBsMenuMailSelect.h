#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 16BsMenuMailSelect @ 0x008CC234
// vtable 0x008F26C0 (vptr 0x008F26C8), offset_to_top 0, 25 entries
// vtable 0x008F272C (vptr 0x008F2734), offset_to_top -40, 3 entries
class BsMenuMailSelect : public ::MenuBase, public ::state::Mode<BsMenuMailSelect>
{
public:
    BsMenuMailSelect(); // ctor address unknown
    virtual ~BsMenuMailSelect(); // 0x002B21D4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B21C4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002B1B4C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002B1F54 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002B1DBC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002B1B04 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
