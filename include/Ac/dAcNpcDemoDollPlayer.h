#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 19AcNpcDemoDollPlayer @ 0x008CC928
// vtable 0x008F4BF4 (vptr 0x008F4BFC), offset_to_top 0, 91 entries
class AcNpcDemoDollPlayer : public ::AcNpcDemoDollBase
{
public:
    AcNpcDemoDollPlayer(); // ctor candidate(s) 0x007F1594 (unverified)
    virtual ~AcNpcDemoDollPlayer(); // 0x002ECDD8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002ECD80 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x002ECC48 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x164(); // 0x002ECD10 slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x001D3168 slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
