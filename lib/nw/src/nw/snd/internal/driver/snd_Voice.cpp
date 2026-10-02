#include "nw/snd/internal/driver/snd_Voice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CA1E0 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Voice::Initialize(const nw::snd::internal::WaveInfo&, unsigned)
{
}

// 0x004CA2B4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetLpfFreq(float)
{
}

// 0x004CA2D8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPanMode(nw::snd::PanMode)
{
}

// 0x004CA2F8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::FrameToByte(unsigned, nw::snd::SampleFormat)
{
}

// 0x004CA364 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetMainSend(float)
{
}

// 0x004CA3A8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPanCurve(nw::snd::PanCurve)
{
}

// 0x004CA3C8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPriority(int)
{
}

// 0x004CA420 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::CalcMixParam(int, nn::snd::CTR::MixParam*)
{
}

// 0x004CA730 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::StopFinished()
{
}

// 0x004CA79C | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::Voice::SetAdpcmParam(int, const nn::snd::CTR::AdpcmParam&)
{
}

// 0x004CA7EC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetSurroundPan(float)
{
}

// 0x004CA810 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetBiquadFilter(int, float)
{
}

// 0x004CA878 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::AppendWaveBuffer(int, nn::snd::CTR::WaveBuffer*, bool)
{
}

// 0x004CA8AC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::CalcOffsetAdpcmParam(nn::snd::CTR::AdpcmContext*, const nn::snd::CTR::AdpcmParam&, unsigned, const void*)
{
}

// 0x004CA900 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetInterpolationType(unsigned char)
{
}

// 0x004CA964 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::UpdateVoicesPriority()
{
}

// 0x004CA9EC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SdkVoiceDropCallbackFunc(nn::snd::CTR::Voice*, unsigned)
{
}

// 0x004CAA3C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SdkVoiceDropCallbackFuncMulti(nn::snd::CTR::Voice*, unsigned)
{
}

// 0x004CAB24 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::Calc()
{
}

// 0x004CAD70 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Free()
{
}

// 0x004CADD8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Stop()
{
}

// 0x004CAE34 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Alloc(int, int, void(*)(nw::snd::internal::driver::Voice*, nw::snd::internal::driver::Voice::VoiceCallbackStatus, void*), void*)
{
}

// 0x004CAFCC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Pause(bool)
{
}

// 0x004CAFEC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::Start()
{
}

// 0x004CB00C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPan(float)
{
}

// 0x004CB030 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Voice::Update()
{
}

// 0x004CB220 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::CalcMix()
{
}

// 0x004CB2E8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetPitch(float)
{
}

// 0x004CB30C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetFxSend(nw::snd::AuxBus, float)
{
}

// 0x004CB348 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::SetVolume(float)
{
}

// 0x00741424 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::IsRun() const
{
}

// 0x00741444 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::Voice::GetFormat() const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
