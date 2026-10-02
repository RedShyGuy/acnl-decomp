#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 13AcNpcDemoDoll @ 0x008CB85C
// vtable 0x008EE93C (vptr 0x008EE944), offset_to_top 0, 91 entries
class AcNpcDemoDoll : public ::AcNpcDemoDollBase
{
public:
    AcNpcDemoDoll(); // ctor candidate(s) 0x007EDD78 (unverified)
    virtual ~AcNpcDemoDoll(); // 0x0020ECB8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0020EBB8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x0020E9D0 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0xF4(); // 0x0020EBAC slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x00713F14 slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x0020EBA0 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x00713F08 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x164(); // 0x0020EA5C slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x00304254 slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
