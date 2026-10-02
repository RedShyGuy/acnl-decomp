#pragma once

#include "decomp.h"
#include "bgcheck/dMoveBg.h"

namespace bgcheck {
// RTTI N7bgcheck8GuardBoxE @ 0x008D3F00
// vtable 0x0090BCAC (vptr 0x0090BCB4), offset_to_top 0, 4 entries
class GuardBox : public ::bgcheck::MoveBg
{
public:
    GuardBox(); // ctor candidate(s) 0x006154C0 (unverified)
    virtual void vf_0x00(); // 0x00317F74 slot 0x00 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x04(); // 0x006154E0 slot 0x04 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x0C(); // 0x00615434 slot 0x0C | virtual slot, introduced by bgcheck::MoveBg
};
} // namespace bgcheck
