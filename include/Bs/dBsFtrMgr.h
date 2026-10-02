#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 8BsFtrMgr @ 0x008CD46C
// vtable 0x008F9904 (vptr 0x008F990C), offset_to_top 0, 22 entries
class BsFtrMgr : public ::UtlBase<Base>
{
public:
    class CommonResLoader;
    struct CreateFtrArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsFtrMgr(); // ctor address unknown
    virtual ~BsFtrMgr(); // 0x006950A0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00695004 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00694800 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006946D4 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0069114C slot 0x40 | virtual slot, introduced by BsFtrMgr
    virtual void vf_0x44(); // 0x00691044 slot 0x44 | virtual slot, introduced by BsFtrMgr
    virtual void vf_0x48(); // 0x006906C8 slot 0x48 | virtual slot, introduced by BsFtrMgr
    virtual void vf_0x4C(); // 0x006921E0 slot 0x4C | virtual slot, introduced by BsFtrMgr
    virtual void vf_0x50(); // 0x006921C0 slot 0x50 | virtual slot, introduced by BsFtrMgr
    virtual void vf_0x54(); // 0x00691E6C slot 0x54 | virtual slot, introduced by BsFtrMgr
    void CreateFtrActor(BsFtrMgr::CreateFtrArg const&); // 0x00690F8C | libgarden [tier A]
};
