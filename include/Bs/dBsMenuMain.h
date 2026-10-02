#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 10BsMenuMain @ 0x008CB00C
// vtable 0x008EC0C4 (vptr 0x008EC0CC), offset_to_top 0, 16 entries
// vtable 0x008EC10C (vptr 0x008EC114), offset_to_top -20, 3 entries
class BsMenuMain : public ::Base, public ::state::Mode<BsMenuMain>
{
public:
    BsMenuMain(); // ctor candidate(s) 0x001A15BC (unverified)
    virtual ~BsMenuMain(); // 0x00522188 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001A1638 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001A1450 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001A15B4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001A1514 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001A1448 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
