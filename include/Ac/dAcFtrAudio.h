#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// RTTI 10AcFtrAudio @ 0x008CAF1C
// vtable 0x008EB61C (vptr 0x008EB624), offset_to_top 0, 67 entries
// vtable 0x008EB730 (vptr 0x008EB738), offset_to_top -104, 4 entries
// vtable 0x008EB748 (vptr 0x008EB750), offset_to_top -176, 72 entries
// vtable 0x008EB870 (vptr 0x008EB878), offset_to_top -300, 14 entries
class AcFtrAudio : public ::AcFtr
{
public:
    AcFtrAudio(); // ctor candidate(s) 0x007EBD1C (unverified)
    virtual ~AcFtrAudio(); // 0x005781E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005782B0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x50(); // 0x0018C884 slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x94(); // 0x0070B280 slot 0x94 | virtual slot, introduced by AcFtr
    virtual void vf_0x98(); // 0x0018A170 slot 0x98 | virtual slot, introduced by AcFtr
    virtual void vf_0x9C(); // 0x0070B290 slot 0x9C | virtual slot, introduced by AcFtr
    virtual void vf_0xB0(); // 0x0018C540 slot 0xB0 | virtual slot, introduced by AcFtr
    virtual void vf_0xB4(); // 0x0018BD84 slot 0xB4 | virtual slot, introduced by AcFtr
    virtual void vf_0xB8(); // 0x0018A05C slot 0xB8 | virtual slot, introduced by AcFtr
    virtual void vf_0xBC(); // 0x0018A9B4 slot 0xBC | virtual slot, introduced by AcFtr
    virtual void vf_0xC0(); // 0x0018A95C slot 0xC0 | virtual slot, introduced by AcFtr
};
