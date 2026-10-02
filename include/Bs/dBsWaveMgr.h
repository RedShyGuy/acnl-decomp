#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 9BsWaveMgr @ 0x008CD76C
// vtable 0x008FA578 (vptr 0x008FA580), offset_to_top 0, 22 entries
class BsWaveMgr : public ::UtlBase<Base>
{
public:
    BsWaveMgr(); // ctor candidate(s) 0x006E1BBC (unverified)
    virtual ~BsWaveMgr(); // 0x006E1BFC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E1BEC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006E1B3C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006E1B34 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x006E1700 slot 0x40 | virtual slot, introduced by BsWaveMgr
    virtual void vf_0x44(); // 0x006E16F8 slot 0x44 | virtual slot, introduced by BsWaveMgr
    virtual void vf_0x48(); // 0x006E16E4 slot 0x48 | virtual slot, introduced by BsWaveMgr
    virtual void vf_0x4C(); // 0x006E1B24 slot 0x4C | virtual slot, introduced by BsWaveMgr
    virtual void vf_0x50(); // 0x006E1B1C slot 0x50 | virtual slot, introduced by BsWaveMgr
    virtual void vf_0x54(); // 0x006E1AF4 slot 0x54 | virtual slot, introduced by BsWaveMgr
};
