#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 29AcNpcTopsChangeDemoDollSpBase @ 0x008CD124
// vtable 0x008F8138 (vptr 0x008F8140), offset_to_top 0, 91 entries
class AcNpcTopsChangeDemoDollSpBase : public ::AcNpcDemoDollBase
{
public:
    AcNpcTopsChangeDemoDollSpBase(); // ctor address unknown
    virtual ~AcNpcTopsChangeDemoDollSpBase(); // 0x00343C98 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00343C4C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x00343B88 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xA8(); // 0x00726818 slot 0xA8 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x00343C44 slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x00726840 slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x00343C3C slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x00726838 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x00343C04 slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x00343C2C slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
