#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "script/dITalkRecept.h"

// RTTI 10AcPhotoMgr @ 0x008CAF34
// vtable 0x008EBB44 (vptr 0x008EBB4C), offset_to_top 0, 32 entries
// vtable 0x008EBBCC (vptr 0x008EBBD4), offset_to_top -104, 63 entries
class AcPhotoMgr : public ::DemoActor, public ::script::ITalkRecept
{
public:
    AcPhotoMgr(); // ctor address unknown
    virtual ~AcPhotoMgr(); // 0x0018E45C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0018E3E4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0018E188 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0018E2A0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0018E1E8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0018E15C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x50(); // 0x0018E2C4 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x0018E128 slot 0x78 | virtual slot, introduced by AcPhotoMgr
    virtual void vf_0x7C(); // 0x0018DC40 slot 0x7C | virtual slot, introduced by AcPhotoMgr
};
