#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollBase.h"

// RTTI 27AcNpcBoardingDemoDollPlayer @ 0x008CD0D4
// vtable 0x008F7EE8 (vptr 0x008F7EF0), offset_to_top 0, 91 entries
class AcNpcBoardingDemoDollPlayer : public ::AcNpcDemoDollBase
{
public:
    AcNpcBoardingDemoDollPlayer(); // ctor address unknown
    virtual ~AcNpcBoardingDemoDollPlayer(); // 0x00342ED0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00342E74 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x80(); // 0x00342CE4 slot 0x80 | virtual slot, introduced by UtlBase<DemoActor>
    virtual void vf_0x164(); // 0x00342DAC slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
    virtual void vf_0x168(); // 0x00342E1C slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
};
