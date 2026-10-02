#pragma once

#include "decomp.h"
#include "Other/dSoPaCaWindowBase.h"
#include "state/dMode.h"

// RTTI 8Passport @ 0x008CD534
// vtable 0x008F9D2C (vptr 0x008F9D34), offset_to_top 0, 11 entries
// vtable 0x008F9D60 (vptr 0x008F9D68), offset_to_top -36, 10 entries
class Passport : public ::state::Mode<Passport>, public ::SoPaCaWindowBase
{
public:
    Passport(); // ctor candidate(s) 0x006B12FC (unverified)
    virtual ~Passport(); // 0x006B164C slot 0x00 | slot vf_0x00 of state::Mode<Passport>
    // 0x006B1634 slot 0x04 | slot vf_0x04 of state::Mode<Passport> (deleting dtor)
    virtual void vf_0x0C(); // 0x006B068C slot 0x0C | virtual slot, introduced by Passport
    virtual void vf_0x10(); // 0x00767CB0 slot 0x10 | virtual slot, introduced by Passport
    virtual void vf_0x14(); // 0x006B06C4 slot 0x14 | virtual slot, introduced by Passport
    virtual void vf_0x18(); // 0x00767D34 slot 0x18 | virtual slot, introduced by Passport
    virtual void vf_0x1C(); // 0x006B06E4 slot 0x1C | virtual slot, introduced by Passport
    virtual void vf_0x20(); // 0x006B1030 slot 0x20 | virtual slot, introduced by Passport
    virtual void vf_0x24(); // 0x006B05F0 slot 0x24 | virtual slot, introduced by Passport
    virtual void vf_0x28(); // 0x006B1124 slot 0x28 | virtual slot, introduced by Passport
};
