#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 16BsIndoorPlateMgr @ 0x008CC1BC
// vtable 0x008F2350 (vptr 0x008F2358), offset_to_top 0, 22 entries
class BsIndoorPlateMgr : public ::UtlBase<Base>
{
public:
    BsIndoorPlateMgr(); // ctor candidate(s) 0x002AAB18 (unverified)
    virtual ~BsIndoorPlateMgr(); // 0x002AAB48 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002AAB38 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002AAB10 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002AAB08 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x002AAAE8 slot 0x40 | virtual slot, introduced by BsIndoorPlateMgr
    virtual void vf_0x44(); // 0x002AAAB8 slot 0x44 | virtual slot, introduced by BsIndoorPlateMgr
    virtual void vf_0x48(); // 0x002AA6E4 slot 0x48 | virtual slot, introduced by BsIndoorPlateMgr
    virtual void vf_0x4C(); // 0x002AAB00 slot 0x4C | virtual slot, introduced by BsIndoorPlateMgr
    virtual void vf_0x50(); // 0x002AAAF8 slot 0x50 | virtual slot, introduced by BsIndoorPlateMgr
    virtual void vf_0x54(); // 0x002AAAF0 slot 0x54 | virtual slot, introduced by BsIndoorPlateMgr
};
