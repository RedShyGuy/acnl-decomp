#pragma once

#include "decomp.h"
#include "sead/ptcl/seadEmitterSimpleCalc.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl18EmitterComplexCalcE @ 0x008D2084
// vtable 0x00906B38 (vptr 0x00906B40), offset_to_top 0, 1 entries
// The emitters with fields, fluctuation, children and stripes.
class EmitterComplexCalc : public ::sead::ptcl::EmitterSimpleCalc
{
public:
    explicit EmitterComplexCalc(PtclSystem* system) : EmitterSimpleCalc(system) {}

    virtual u32* calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl); // 0x0055A0D0 slot 0x00

    // the children of the particles of emitter
    u32* calcChild_(u32* cmd, EmitterInstance* emitter, s32* numPtcl); // 0x00558990 | mk7dlp:bytes-fuzzy [tier B]
    // the children a particle emits in this frame
    void calcComplex_(EmitterInstance* emitter, PtclInstance* ptcl); // 0x00558F84 | mk7dlp
    // the particles of emitter
    u32* calcEmitter_(u32* cmd, EmitterInstance* emitter, s32* numPtcl); // 0x00559850 | mk7dlp
    // the texture, blending and combiner of the children of emitter (as emitterDrawSetting_)
    u32* childDrawSetting_(u32* cmd, EmitterInstance* emitter); // 0x00559CF0 (name is ours)
};
} // namespace ptcl
} // namespace sead
