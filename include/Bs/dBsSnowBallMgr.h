#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 13BsSnowBallMgr @ 0x008CB970
// vtable 0x008EF08C (vptr 0x008EF094), offset_to_top 0, 22 entries
class BsSnowBallMgr : public ::UtlBase<Base>
{
public:
    BsSnowBallMgr(); // ctor candidate(s) 0x0022705C (unverified)
    virtual ~BsSnowBallMgr(); // 0x002270B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0022708C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00226F5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00226F54 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00225CF8 slot 0x40 | virtual slot, introduced by BsSnowBallMgr
    virtual void vf_0x44(); // 0x00225C9C slot 0x44 | virtual slot, introduced by BsSnowBallMgr
    virtual void vf_0x48(); // 0x00225C64 slot 0x48 | virtual slot, introduced by BsSnowBallMgr
    virtual void vf_0x4C(); // 0x0022616C slot 0x4C | virtual slot, introduced by BsSnowBallMgr
    virtual void vf_0x50(); // 0x00226164 slot 0x50 | virtual slot, introduced by BsSnowBallMgr
    virtual void vf_0x54(); // 0x00225EE4 slot 0x54 | virtual slot, introduced by BsSnowBallMgr
};
