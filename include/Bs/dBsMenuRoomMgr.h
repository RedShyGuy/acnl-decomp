#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 13BsMenuRoomMgr @ 0x008CB944
// vtable 0x008EEFE8 (vptr 0x008EEFF0), offset_to_top 0, 16 entries
// vtable 0x008EF030 (vptr 0x008EF038), offset_to_top -20, 3 entries
class BsMenuRoomMgr : public ::Base, public ::state::Mode<BsMenuRoomMgr>
{
public:
    BsMenuRoomMgr(); // ctor candidate(s) 0x007EE098 (unverified)
    virtual ~BsMenuRoomMgr(); // 0x00225B94 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00225B84 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0022562C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00225838 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00225798 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00225624 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
