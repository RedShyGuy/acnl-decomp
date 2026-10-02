#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// RTTI 17AcNpcDemoDollBase @ 0x008CC444
// vtable 0x008F31C8 (vptr 0x008F31D0), offset_to_top 0, 91 entries
class AcNpcDemoDollBase : public ::AcNpc
{
public:
    AcNpcDemoDollBase(); // ctor candidate(s) 0x002C522C (unverified)
    virtual ~AcNpcDemoDollBase(); // 0x002C52D8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C52C8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Calc(); // 0x002C5204 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x78(); // 0x0010B92D slot 0x78 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x7C(); // 0x0010B8D9 slot 0x7C | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x80(); // 0x0010B869 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x88(); // 0x0010B965 slot 0x88 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x10C(); // 0x002C51A8 slot 0x10C | virtual slot, introduced by AcNpc
    virtual void vf_0x110(); // 0x0071F900 slot 0x110 | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x002C5068 slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x0011C12F slot 0x164 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x168(); // 0x0011C12F slot 0x168 | slot vf_0x00 of ChangeRentalBase
};
