#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 11BsPlayerMgr @ 0x008CB284
// vtable 0x008EC94C (vptr 0x008EC954), offset_to_top 0, 22 entries
class BsPlayerMgr : public ::UtlBase<Base>
{
public:
    BsPlayerMgr(); // ctor address unknown
    virtual ~BsPlayerMgr(); // 0x001C50E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C5038 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001C4BF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001C4B40 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x001C4748 slot 0x40 | virtual slot, introduced by BsPlayerMgr
    virtual void vf_0x44(); // 0x001C4740 slot 0x44 | virtual slot, introduced by BsPlayerMgr
    virtual void vf_0x48(); // 0x001C472C slot 0x48 | virtual slot, introduced by BsPlayerMgr
    virtual void vf_0x4C(); // 0x001C481C slot 0x4C | virtual slot, introduced by BsPlayerMgr
    virtual void vf_0x50(); // 0x001C4800 slot 0x50 | virtual slot, introduced by BsPlayerMgr
    virtual void vf_0x54(); // 0x001C47D0 slot 0x54 | virtual slot, introduced by BsPlayerMgr
};
