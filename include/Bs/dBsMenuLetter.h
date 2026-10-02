#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 12BsMenuLetter @ 0x008CB56C
// vtable 0x008EE180 (vptr 0x008EE188), offset_to_top 0, 25 entries
// vtable 0x008EE1EC (vptr 0x008EE1F4), offset_to_top -40, 3 entries
class BsMenuLetter : public ::MenuBase, public ::state::Mode<BsMenuLetter>
{
public:
    BsMenuLetter(); // ctor address unknown
    virtual ~BsMenuLetter(); // 0x001FCB64 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FCB54 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FBDAC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FC400 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FC33C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FBD80 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x001FB344 slot 0x44 | slot vf_0x44 of MenuBase
};
