#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 18BsMenuLetterEditer @ 0x008CC780
// vtable 0x008F42CC (vptr 0x008F42D4), offset_to_top 0, 25 entries
// vtable 0x008F4338 (vptr 0x008F4340), offset_to_top -40, 3 entries
class BsMenuLetterEditer : public ::MenuBase, public ::state::Mode<BsMenuLetterEditer>
{
public:
    BsMenuLetterEditer(); // ctor address unknown
    virtual ~BsMenuLetterEditer(); // 0x002DB138 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DB0C0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DAD44 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DB0A0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DAFF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002DAD18 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
