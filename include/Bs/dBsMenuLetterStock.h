#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 17BsMenuLetterStock @ 0x008CC480
// vtable 0x008F34F0 (vptr 0x008F34F8), offset_to_top 0, 25 entries
// vtable 0x008F355C (vptr 0x008F3564), offset_to_top -40, 3 entries
class BsMenuLetterStock : public ::MenuBase, public ::state::Mode<BsMenuLetterStock>
{
public:
    BsMenuLetterStock(); // ctor address unknown
    virtual ~BsMenuLetterStock(); // 0x002C8B88 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C8B78 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C8338 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002C8904 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002C8680 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C82DC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
