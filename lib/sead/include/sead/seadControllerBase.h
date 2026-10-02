#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead14ControllerBaseE @ 0x008D16D8
// vtable 0x00905738 (vptr 0x00905740), offset_to_top 0, 2 entries
class ControllerBase
{
public:
    ControllerBase(); // ctor candidate(s) 0x0054471C (unverified)
    virtual void vf_0x00(); // 0x0074BD64 slot 0x00 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x04(); // 0x0074BD18 slot 0x04 | virtual slot, introduced by sead::ControllerBase
    void getStickHold_(unsigned, const sead::Vector2<float>&, float, float, int); // 0x00544264 | nintendogs:bytes [tier A]
    void updateDerivativeParams_(unsigned, bool); // 0x00544554 | nintendogs:callseq [tier A]
    ControllerBase(int, int, int, int); // 0x0054471C | nintendogs:callseq [tier A]
};
} // namespace sead
