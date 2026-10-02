#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 14BsMenuMyDesign @ 0x008CBBB0
// vtable 0x008EFD84 (vptr 0x008EFD8C), offset_to_top 0, 25 entries
// vtable 0x008EFDF0 (vptr 0x008EFDF8), offset_to_top -40, 63 entries
// vtable 0x008EFEF4 (vptr 0x008EFEFC), offset_to_top -164, 3 entries
class BsMenuMyDesign : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuMyDesign>
{
public:
    BsMenuMyDesign(); // ctor candidate(s) 0x0025B628 (unverified)
    virtual ~BsMenuMyDesign(); // 0x0025B77C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0025B76C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0025AA14 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0025B0B0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0025AEC0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0025A9C4 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x00257278 slot 0x44 | slot vf_0x44 of MenuBase
};
