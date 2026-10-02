#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Other/dObjTalkRecept.h"
#include "Utl/dUtlBase.h"

// RTTI 24AcAutoCampEntrySignboard @ 0x008CCF58
// vtable 0x008F7494 (vptr 0x008F749C), offset_to_top 0, 37 entries
// vtable 0x008F7530 (vptr 0x008F7538), offset_to_top -104, 72 entries
// vtable 0x008F7658 (vptr 0x008F7660), offset_to_top -228, 14 entries
class AcAutoCampEntrySignboard : public ::UtlBase<DemoActor>, public ::ObjTalkRecept
{
public:
    AcAutoCampEntrySignboard(); // ctor candidate(s) 0x003360A0 (unverified)
    virtual ~AcAutoCampEntrySignboard(); // 0x00336104 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x003360E8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x00336038 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00336030 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x00726684 slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x50(); // 0x00336088 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x78(); // 0x00335EF4 slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x00335EEC slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x00335E70 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x84(); // 0x00335F0C slot 0x84 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x00335F04 slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x8C(); // 0x00335EFC slot 0x8C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x90(); // 0x00335F14 slot 0x90 | virtual slot, introduced by AcAutoCampEntrySignboard
};
