#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class Voice
{
public:
    struct VoiceCallbackStatus { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Initialize(const nw::snd::internal::WaveInfo&, unsigned); // 0x004CA1E0 | nintendogs:callseq [tier A]
    void SetLpfFreq(float); // 0x004CA2B4 | nintendogs:bytes [tier A]
    void SetPanMode(nw::snd::PanMode); // 0x004CA2D8 | nintendogs:bytes [tier A]
    void FrameToByte(unsigned, nw::snd::SampleFormat); // 0x004CA2F8 | nintendogs:bytes [tier A]
    void SetMainSend(float); // 0x004CA364 | nintendogs:bytes [tier A]
    void SetPanCurve(nw::snd::PanCurve); // 0x004CA3A8 | nintendogs:bytes [tier A]
    void SetPriority(int); // 0x004CA3C8 | nintendogs:bytes [tier A]
    void CalcMixParam(int, nn::snd::CTR::MixParam*); // 0x004CA420 | nintendogs:callgraph [tier A]
    void StopFinished(); // 0x004CA730 | nintendogs:callgraph [tier A]
    void SetAdpcmParam(int, const nn::snd::CTR::AdpcmParam&); // 0x004CA79C | nintendogs:callseq-callee [tier A]
    void SetSurroundPan(float); // 0x004CA7EC | nintendogs:bytes [tier A]
    void SetBiquadFilter(int, float); // 0x004CA810 | nintendogs:bytes [tier A]
    void AppendWaveBuffer(int, nn::snd::CTR::WaveBuffer*, bool); // 0x004CA878 | nintendogs:bytes [tier A]
    void CalcOffsetAdpcmParam(nn::snd::CTR::AdpcmContext*, const nn::snd::CTR::AdpcmParam&, unsigned, const void*); // 0x004CA8AC | nintendogs:bytes [tier A]
    void SetInterpolationType(unsigned char); // 0x004CA900 | nintendogs:bytes [tier A]
    void UpdateVoicesPriority(); // 0x004CA964 | nintendogs:bytes [tier A]
    void SdkVoiceDropCallbackFunc(nn::snd::CTR::Voice*, unsigned); // 0x004CA9EC | nintendogs:bytes [tier A]
    void SdkVoiceDropCallbackFuncMulti(nn::snd::CTR::Voice*, unsigned); // 0x004CAA3C | nintendogs:bytes [tier A]
    void Calc(); // 0x004CAB24 | nintendogs:callgraph [tier A]
    void Free(); // 0x004CAD70 | nintendogs:bytes [tier A]
    void Stop(); // 0x004CADD8 | nintendogs:bytes [tier A]
    void Alloc(int, int, void(*)(nw::snd::internal::driver::Voice*, nw::snd::internal::driver::Voice::VoiceCallbackStatus, void*), void*); // 0x004CAE34 | nintendogs:bytes [tier A]
    void Pause(bool); // 0x004CAFCC | nintendogs:bytes [tier A]
    void Start(); // 0x004CAFEC | nintendogs:bytes [tier A]
    void SetPan(float); // 0x004CB00C | nintendogs:bytes [tier A]
    void Update(); // 0x004CB030 | nintendogs:callgraph [tier A]
    void CalcMix(); // 0x004CB220 | nintendogs:bytes [tier A]
    void SetPitch(float); // 0x004CB2E8 | nintendogs:bytes [tier A]
    void SetFxSend(nw::snd::AuxBus, float); // 0x004CB30C | nintendogs:bytes [tier A]
    void SetVolume(float); // 0x004CB348 | nintendogs:bytes [tier A]
    void IsRun() const; // 0x00741424 | nintendogs:bytes [tier A]
    void GetFormat() const; // 0x00741444 | nintendogs:bytes [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
