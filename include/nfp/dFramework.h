#pragma once

#include "decomp.h"
#include "state/dMode.h"

namespace nfp {
// RTTI N3nfp9FrameworkE @ 0x008D101C
// vtable 0x00904758 (vptr 0x00904760), offset_to_top 0, 3 entries
class Framework : public ::state::Mode<nfp::Framework>
{
public:
    class Delegate;
    class Step;
    Framework(); // ctor candidate(s) 0x0051CB24 (unverified)
    virtual void vf_0x00(); // 0x0051CCB4 slot 0x00 | virtual slot, introduced by nfp::Framework
    virtual void vf_0x04(); // 0x0051CC94 slot 0x04 | virtual slot, introduced by nfp::Framework
    virtual void vf_0x08(); // 0x0082D920 slot 0x08 | virtual slot, introduced by nfp::Framework
};
} // namespace nfp
