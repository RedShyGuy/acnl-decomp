#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 14BsCommonDialog @ 0x008CBB54
// vtable 0x008EFB4C (vptr 0x008EFB54), offset_to_top 0, 25 entries
// vtable 0x008EFBB8 (vptr 0x008EFBC0), offset_to_top -40, 3 entries
class BsCommonDialog : public ::MenuBase, public ::state::Mode<BsCommonDialog>
{
public:
    BsCommonDialog(); // ctor candidate(s) 0x007EE8C4 (unverified)
    virtual ~BsCommonDialog(); // 0x00251E88 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00251E5C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00251B70 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00251D90 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00251CD0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00251B5C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x00251940 slot 0x44 | slot vf_0x44 of MenuBase
};
