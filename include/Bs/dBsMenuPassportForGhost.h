#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 22BsMenuPassportForGhost @ 0x008CCD7C
// vtable 0x008F68B8 (vptr 0x008F68C0), offset_to_top 0, 25 entries
// vtable 0x008F6924 (vptr 0x008F692C), offset_to_top -40, 3 entries
class BsMenuPassportForGhost : public ::MenuBase, public ::state::Mode<BsMenuPassportForGhost>
{
public:
    BsMenuPassportForGhost(); // ctor address unknown
    virtual ~BsMenuPassportForGhost(); // 0x0032EBEC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0032EB48 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0032E890 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0032EA54 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0032E9B4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0032E864 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
