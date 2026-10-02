#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 23AcNpcBoardingDemoDollSp @ 0x008CCE74
// vtable 0x008F6CF4 (vptr 0x008F6CFC), offset_to_top 0, 91 entries
class AcNpcBoardingDemoDollSp : public ::AcNpcDemoDollBase
{
public:
    AcNpcBoardingDemoDollSp(); // ctor address unknown
    virtual ~AcNpcBoardingDemoDollSp(); // 0x00333334 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00333238 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x0033316C slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xA8(); // 0x00726530 slot 0xA8 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x0033322C slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x0072655C slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x00333220 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x00726550 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x003331EC slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x00333214 slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
