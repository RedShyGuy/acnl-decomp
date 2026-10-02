#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 24BsMenuWhatTreeIsThisTree @ 0x008CCFB0
// vtable 0x008F77D0 (vptr 0x008F77D8), offset_to_top 0, 25 entries
// vtable 0x008F783C (vptr 0x008F7844), offset_to_top -40, 3 entries
class BsMenuWhatTreeIsThisTree : public ::MenuBase, public ::state::Mode<BsMenuWhatTreeIsThisTree>
{
public:
    BsMenuWhatTreeIsThisTree(); // ctor address unknown
    virtual ~BsMenuWhatTreeIsThisTree(); // 0x0033DAA0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0033D9EC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0033D220 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0033D6CC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0033D5A8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0033D1B8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
