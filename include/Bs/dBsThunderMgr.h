#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 12BsThunderMgr @ 0x008CB5B8
// vtable 0x008EE2C8 (vptr 0x008EE2D0), offset_to_top 0, 16 entries
class BsThunderMgr : public ::Base
{
public:
    BsThunderMgr(); // ctor candidate(s) 0x001FEF84 (unverified)
    virtual ~BsThunderMgr(); // 0x001FEFB8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FEFA8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FEDF4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FEF5C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FEE70 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FEDF0 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
