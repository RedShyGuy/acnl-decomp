#pragma once

#include "decomp.h"
#include "sead/seadProjection.h"

namespace sead {
// RTTI N4sead15OrthoProjectionE @ 0x008D189C
// vtable 0x00905AFC (vptr 0x00905B04), offset_to_top 0, 8 entries
class OrthoProjection : public ::sead::Projection
{
public:
    OrthoProjection(); // ctor candidate(s) 0x005461D0, 0x00546290 (unverified)
    virtual void vf_0x00(); // 0x0074C614 slot 0x00 | virtual slot, introduced by sead::Projection
    virtual void vf_0x04(); // 0x0074C568 slot 0x04 | virtual slot, introduced by sead::Projection
    virtual void vf_0x08(); // 0x00546324 slot 0x08 | virtual slot, introduced by sead::Projection
    virtual void vf_0x0C(); // 0x00546320 slot 0x0C | virtual slot, introduced by sead::Projection
    virtual void vf_0x10(); // 0x0074C560 slot 0x10 | virtual slot, introduced by sead::Projection
    virtual void vf_0x14(); // 0x0074C494 slot 0x14 | virtual slot, introduced by sead::Projection
    virtual void vf_0x1C(); // 0x0074C5B4 slot 0x1C | virtual slot, introduced by sead::Projection
};
} // namespace sead
