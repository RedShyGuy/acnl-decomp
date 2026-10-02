#include "nn/snd/CTR/snd_Voice.h"

namespace nn {
namespace snd {
namespace CTR {
// TODO: default ctor added so derived stubs compile - may not exist
nn::snd::CTR::Voice::Voice()
{
}

// 0x00463634 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::Initialize()
{
}

// 0x00463710 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetPriority(int)
{
}

// 0x00463744 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::EnableBiquadFilter(bool)
{
}

// 0x00463764 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetMonoFilterCoefficients(unsigned short)
{
}

// 0x004637F0 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetPitch(float)
{
}

// 0x00463810 | nintendogs:bytes [tier A]
nn::snd::CTR::Voice::Voice(int)
{
}

// 0x0046647C | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetMixParam(const nn::snd::CTR::MixParam&)
{
}

// 0x00466A60 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::EnableMonoFilter(bool)
{
}

// 0x00466A94 | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetSampleRate(int)
{
}

// 0x00466D10 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Voice::AppendWaveBuffer(nn::snd::CTR::WaveBuffer*)
{
}

// 0x00466EA4 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Voice::SetInterpolationType(nn::snd::CTR::InterpolationType)
{
}

// 0x00466F0C | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetBiquadFilterCoefficients(const nn::snd::CTR::BiquadFilterCoefficients&)
{
}

// 0x00466FAC | nintendogs:bytes [tier A]
void nn::snd::CTR::Voice::SetState(nn::snd::CTR::Voice::State)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
