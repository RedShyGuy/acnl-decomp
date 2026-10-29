#pragma once

#include "decomp.h"
#include "sead/ptcl/seadEmitterCalc.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl20EmitterSimpleGPUCalcE @ 0x008D2090
// vtable 0x00906B44 (vptr 0x00906B4C), offset_to_top 0, 1 entries
// The emitters whose particles the shaders of GpuPtcle calculate from the frame.
class EmitterSimpleGPUCalc : public ::sead::ptcl::EmitterCalc
{
public:
    explicit EmitterSimpleGPUCalc(PtclSystem* system) : EmitterCalc(system) {}

    virtual u32* calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl); // 0x0055B76C slot 0x00 | mk7dlp

    u32* emitterDrawSetting_(u32* cmd, EmitterInstance* emitter); // 0x0055B480 | mk7dlp
};
} // namespace ptcl
} // namespace sead
