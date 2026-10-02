#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class Voice
{
public:
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Voice(); // TODO: default ctor added so derived stubs compile - may not exist
    void Initialize(); // 0x00463634 | nintendogs:bytes [tier A]
    void SetPriority(int); // 0x00463710 | nintendogs:bytes [tier A]
    void EnableBiquadFilter(bool); // 0x00463744 | nintendogs:bytes [tier A]
    void SetMonoFilterCoefficients(unsigned short); // 0x00463764 | nintendogs:bytes [tier A]
    void SetPitch(float); // 0x004637F0 | nintendogs:bytes [tier A]
    Voice(int); // 0x00463810 | nintendogs:bytes [tier A]
    void SetMixParam(const nn::snd::CTR::MixParam&); // 0x0046647C | nintendogs:bytes [tier A]
    void EnableMonoFilter(bool); // 0x00466A60 | nintendogs:bytes [tier A]
    void SetSampleRate(int); // 0x00466A94 | nintendogs:bytes [tier A]
    void AppendWaveBuffer(nn::snd::CTR::WaveBuffer*); // 0x00466D10 | nintendogs:callgraph [tier A]
    void SetInterpolationType(nn::snd::CTR::InterpolationType); // 0x00466EA4 | nintendogs:callgraph [tier A]
    void SetBiquadFilterCoefficients(const nn::snd::CTR::BiquadFilterCoefficients&); // 0x00466F0C | nintendogs:bytes [tier A]
    void SetState(nn::snd::CTR::Voice::State); // 0x00466FAC | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
