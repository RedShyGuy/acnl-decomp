#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 14BsCockroachMgr @ 0x008CBB48
// vtable 0x008EFB04 (vptr 0x008EFB0C), offset_to_top 0, 16 entries
class BsCockroachMgr : public ::Base
{
public:
    BsCockroachMgr(); // ctor candidate(s) 0x00251820 (unverified)
    virtual ~BsCockroachMgr(); // 0x002518C0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00251878 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00251384 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002517C4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0025166C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0025137C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
