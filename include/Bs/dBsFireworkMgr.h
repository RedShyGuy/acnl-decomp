#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 13BsFireworkMgr @ 0x008CB8C0
// vtable 0x008EECFC (vptr 0x008EED04), offset_to_top 0, 16 entries
class BsFireworkMgr : public ::Base
{
public:
    BsFireworkMgr(); // ctor candidate(s) 0x0021A6C4 (unverified)
    virtual ~BsFireworkMgr(); // 0x0021A7E4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0021A770 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00219CB0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0021A69C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00219FAC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00219CAC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
