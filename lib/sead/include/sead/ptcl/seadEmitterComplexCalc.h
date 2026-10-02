#pragma once

#include "decomp.h"
#include "sead/ptcl/seadEmitterSimpleCalc.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl18EmitterComplexCalcE @ 0x008D2084
// vtable 0x00906B38 (vptr 0x00906B40), offset_to_top 0, 1 entries
class EmitterComplexCalc : public ::sead::ptcl::EmitterSimpleCalc
{
public:
    EmitterComplexCalc(); // ctor address unknown
    virtual void vf_0x00(); // 0x0055A0D0 slot 0x00 | virtual slot, introduced by sead::ptcl::EmitterCalc
    void calcChild_(unsigned*, sead::ptcl::EmitterInstance*, int*); // 0x00558990 | mk7dlp:bytes-fuzzy [tier B]
};
} // namespace ptcl
} // namespace sead
