#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 15AcNpcDemoDollSp @ 0x008CBE78
// vtable 0x008F0B88 (vptr 0x008F0B90), offset_to_top 0, 91 entries
class AcNpcDemoDollSp : public ::AcNpcDemoDollBase
{
public:
    AcNpcDemoDollSp(); // ctor address unknown
    virtual ~AcNpcDemoDollSp(); // 0x0027E54C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0027E448 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x0027E334 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xA8(); // 0x0071B59C slot 0xA8 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x0027E43C slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x0071B5C8 slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x0027E430 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x0071B5BC slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x0027E3B4 slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x0027E424 slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
