#pragma once

#include "decomp.h"
#include "sead/ptcl/seadEmitterCalc.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl17EmitterSimpleCalcE @ 0x008D2078
// vtable 0x00906B2C (vptr 0x00906B34), offset_to_top 0, 1 entries
// The emitters without fields and children: the particles are calculated on the CPU and drawn as quads.
class EmitterSimpleCalc : public ::sead::ptcl::EmitterCalc
{
public:
    explicit EmitterSimpleCalc(PtclSystem* system) : EmitterCalc(system) {}

    virtual u32* calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl); // 0x00558464 slot 0x00

    // the texture, blending and combiner of emitter (what changed since the last emitter)
    u32* emitterDrawSetting_(u32* cmd, EmitterInstance* emitter); // 0x0055805C | mk7dlp
};
} // namespace ptcl
} // namespace sead
