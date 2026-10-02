#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Voice.h"

namespace nn {
namespace snd {
namespace CTR {
class VoiceImpl
{
public:
    VoiceImpl(); // TODO: default ctor added so derived stubs compile - may not exist
    void Initialize(); // 0x00466374 | nintendogs:callgraph [tier A]
    void SetMixParam(const nn::snd::CTR::MixParam&); // 0x004664A8 | nintendogs:bytes [tier A]
    void SetMixVolume(); // 0x004664E8 | nintendogs:bytes [tier A]
    void SetSyncCount(); // 0x004665D0 | nintendogs:bytes [tier A]
    void UpdateStatus(const void*); // 0x004668E8 | nintendogs:bytes [tier A]
    void SetFilterType(nn::snd::CTR::FilterType); // 0x00466A80 | nintendogs:bytes [tier A]
    void SetSampleRate(int); // 0x00466AAC | nintendogs:bytes [tier A]
    void SendWaveBuffer(); // 0x00466ACC | nintendogs:bytes [tier A]
    void SetChannelCount(int); // 0x00466CD4 | nintendogs:bytes [tier A]
    void SetSampleFormat(nn::snd::CTR::SampleFormat); // 0x00466CF4 | nintendogs:bytes [tier A]
    void AppendWaveBuffer(nn::snd::CTR::WaveBuffer*); // 0x00466D18 | fefates:bytes [tier B]
    void UpdateWaveBuffer(nn::snd::CTR::WaveBuffer*); // 0x00466DBC | fefates:bytes [tier B]
    void ReleaseWaveBuffer(); // 0x00466E1C | nintendogs:bytes [tier A]
    void SetFrontBypassFlag(bool); // 0x00466E90 | nintendogs:bytes [tier A]
    void SetInterpolationType(nn::snd::CTR::InterpolationType); // 0x00466EB0 | nintendogs:bytes [tier A]
    void UpdateWaveBufferList(); // 0x00466EC4 | nintendogs:bytes [tier A]
    void Set3dSurroundPreprocessed(bool); // 0x00466EE0 | fefates:bytes [tier B]
    void SetMonoFilterCoefficients(const nn::snd::CTR::MonoFilterCoefficients&); // 0x00466EF4 | nintendogs:bytes [tier A]
    void SetBiquadFilterCoefficients(const nn::snd::CTR::BiquadFilterCoefficients&); // 0x00466F34 | nintendogs:bytes [tier A]
    void Start(); // 0x00466F5C | nintendogs:bytes [tier A]
    void SetPitch(float); // 0x00466F84 | nintendogs:bytes [tier A]
    void SetState(nn::snd::CTR::Voice::State); // 0x00466FD8 | nintendogs:bytes [tier A]
    void SetVolume(float); // 0x00467050 | nintendogs:bytes [tier A]
    VoiceImpl(int); // 0x00467064 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
