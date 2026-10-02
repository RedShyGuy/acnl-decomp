#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 21BsMenuRoomLightSwitch @ 0x008CCCA0
// vtable 0x008F60C4 (vptr 0x008F60CC), offset_to_top 0, 16 entries
// vtable 0x008F610C (vptr 0x008F6114), offset_to_top -20, 3 entries
class BsMenuRoomLightSwitch : public ::Base, public ::state::Mode<BsMenuRoomLightSwitch>
{
public:
    BsMenuRoomLightSwitch(); // ctor candidate(s) 0x00328004 (unverified)
    virtual ~BsMenuRoomLightSwitch(); // 0x00328160 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x003280E4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00327D18 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00327F80 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00327E00 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00327CE8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
