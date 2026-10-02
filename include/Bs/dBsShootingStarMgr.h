#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 17BsShootingStarMgr @ 0x008CC4CC
// vtable 0x008F3638 (vptr 0x008F3640), offset_to_top 0, 16 entries
class BsShootingStarMgr : public ::Base
{
public:
    BsShootingStarMgr(); // ctor candidate(s) 0x002CA458 (unverified)
    virtual ~BsShootingStarMgr(); // 0x002CA584 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002CA4F4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C9F48 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002CA400 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002CA090 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C9E30 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
