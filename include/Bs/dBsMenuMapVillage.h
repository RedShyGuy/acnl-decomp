#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 16BsMenuMapVillage @ 0x008CC254
// vtable 0x008F2740 (vptr 0x008F2748), offset_to_top 0, 16 entries
// vtable 0x008F2788 (vptr 0x008F2790), offset_to_top -20, 3 entries
class BsMenuMapVillage : public ::Base, public ::state::Mode<BsMenuMapVillage>
{
public:
    BsMenuMapVillage(); // ctor candidate(s) 0x007F0250 (unverified)
    virtual ~BsMenuMapVillage(); // 0x002B401C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B400C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002B38BC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002B3D68 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002B3C68 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002B3878 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
