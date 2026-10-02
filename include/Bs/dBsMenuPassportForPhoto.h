#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 22BsMenuPassportForPhoto @ 0x008CCD9C
// vtable 0x008F6938 (vptr 0x008F6940), offset_to_top 0, 25 entries
// vtable 0x008F69A4 (vptr 0x008F69AC), offset_to_top -40, 3 entries
class BsMenuPassportForPhoto : public ::MenuBase, public ::state::Mode<BsMenuPassportForPhoto>
{
public:
    BsMenuPassportForPhoto(); // ctor candidate(s) 0x0032F658 (unverified)
    virtual ~BsMenuPassportForPhoto(); // 0x0032F6DC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0032F6B0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0032F3E0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0032F640 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0032F5A0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0032F3CC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
