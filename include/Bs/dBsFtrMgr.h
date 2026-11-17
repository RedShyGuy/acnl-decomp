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
    virtual oml::framework::Result Calc(); // 0x00694800 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006946D4 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x0069114C slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00691044 slot 0x44
    bool EndInitialize() override; // 0x006906C8 slot 0x48
    bool StartFinalize() override; // 0x006921E0 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x006921C0 slot 0x50
    bool EndFinalize() override; // 0x00691E6C slot 0x54
    void CreateFtrActor(BsFtrMgr::CreateFtrArg const&); // 0x00690F8C | libgarden [tier A]
};
