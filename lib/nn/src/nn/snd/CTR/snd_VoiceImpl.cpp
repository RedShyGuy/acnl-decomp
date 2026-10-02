#include "nn/snd/CTR/snd_Voice.h"
#include "nn/snd/CTR/snd_VoiceImpl.h"

namespace nn {
namespace snd {
namespace CTR {
// TODO: default ctor added so derived stubs compile - may not exist
nn::snd::CTR::VoiceImpl::VoiceImpl()
{
}

// 0x00466374 | nintendogs:callgraph [tier A]
void nn::snd::CTR::VoiceImpl::Initialize()
{
}

// 0x004664A8 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetMixParam(const nn::snd::CTR::MixParam&)
{
}

// 0x004664E8 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetMixVolume()
{
}

// 0x004665D0 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetSyncCount()
{
}

// 0x004668E8 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::UpdateStatus(const void*)
{
}

// 0x00466A80 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetFilterType(nn::snd::CTR::FilterType)
{
}

// 0x00466AAC | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetSampleRate(int)
{
}

// 0x00466ACC | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SendWaveBuffer()
{
}

// 0x00466CD4 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetChannelCount(int)
{
}

// 0x00466CF4 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetSampleFormat(nn::snd::CTR::SampleFormat)
{
}

// 0x00466D18 | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::AppendWaveBuffer(nn::snd::CTR::WaveBuffer*)
{
}

// 0x00466DBC | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::UpdateWaveBuffer(nn::snd::CTR::WaveBuffer*)
{
}

// 0x00466E1C | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::ReleaseWaveBuffer()
{
}

// 0x00466E90 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetFrontBypassFlag(bool)
{
}

// 0x00466EB0 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetInterpolationType(nn::snd::CTR::InterpolationType)
{
}

// 0x00466EC4 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::UpdateWaveBufferList()
{
}

// 0x00466EE0 | fefates:bytes [tier B]
void nn::snd::CTR::VoiceImpl::Set3dSurroundPreprocessed(bool)
{
}

// 0x00466EF4 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetMonoFilterCoefficients(const nn::snd::CTR::MonoFilterCoefficients&)
{
}

// 0x00466F34 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetBiquadFilterCoefficients(const nn::snd::CTR::BiquadFilterCoefficients&)
{
}

// 0x00466F5C | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::Start()
{
}

// 0x00466F84 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetPitch(float)
{
}

// 0x00466FD8 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetState(nn::snd::CTR::Voice::State)
{
}

// 0x00467050 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceImpl::SetVolume(float)
{
}

// 0x00467064 | nintendogs:bytes [tier A]
nn::snd::CTR::VoiceImpl::VoiceImpl(int)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
