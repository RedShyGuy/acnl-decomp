#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00463850 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::Initialize(bool)
{
}

// 0x00463A44 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetSyncMode(nn::snd::CTR::SyncMode)
{
}

// 0x00463A98 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::EnableAuxBus(nn::snd::CTR::AuxBusId, bool)
{
}

// 0x00463B24 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetRearRatio(unsigned short)
{
}

// 0x00463CF0 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetChannelMix(unsigned char, const nn::snd::CTR::MixParam*)
{
}

// 0x00463DCC | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelRIM(unsigned char, unsigned short, unsigned short)
{
}

// 0x00463F70 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelTimer(unsigned char, float)
{
}

// 0x00463FEC | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetClippingMode(nn::snd::CTR::ClippingMode)
{
}

// 0x0046404C | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetMasterVolume(float)
{
}

// 0x004640AC | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetSurroundDepth(unsigned short)
{
}

// 0x00464118 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetAuxFrontBypass(nn::snd::CTR::AuxBusId, bool)
{
}

// 0x004641A4 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetDspDelayEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxDelayParams&)
{
}

// 0x004642A8 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetAuxReturnVolume(nn::snd::CTR::AuxBusId, float)
{
}

// 0x00464320 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelPlayStop(unsigned char)
{
}

// 0x00464394 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetDspReverbEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxReverbParams&)
{
}

// 0x00464500 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetSoundOutputMode(nn::snd::CTR::OutputMode)
{
}

// 0x00464560 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::InitializeVariables(bool)
{
}

// 0x00464810 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelPlayStart(unsigned char)
{
}

// 0x00464880 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::SetChannelSyncCount(unsigned char, short)
{
}

// 0x004648F0 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::GetDroppedFrameCount()
{
}

// 0x00464940 | nintendogs:callseq [tier A]
void nn::snd::CTR::Dspsnd::SetChannelAdpcmParam(unsigned char, const nn::snd::CTR::AdpcmParam*)
{
}

// 0x004649CC | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetOutputBufferCount(int)
{
}

// 0x00464A20 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetIsHeadsetConnected(bool)
{
}

// 0x00464A80 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::ResetChannelNextBuffer(unsigned char)
{
}

// 0x00464AE8 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::AppendChannelNextBuffer(unsigned char, const nn::snd::CTR::WaveBuffer*, int)
{
}

// 0x00464C64 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelIirFilterType(unsigned char, nn::snd::CTR::FilterType)
{
}

// 0x00464CD4 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::UpdateChannelNextBuffer(unsigned char, const nn::snd::CTR::WaveBuffer*)
{
}

// 0x00464D94 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelIIRFilter_Mono(unsigned char, short, short)
{
}

// 0x00464E0C | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetChannelIIRFilter_Biquad(unsigned char, short, short, short, short, short)
{
}

// 0x00464E98 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::SetSurroundSpeakerPosition(nn::snd::CTR::SurroundSpeakerPosition)
{
}

// 0x00464F04 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::InitializeChannelParameters(unsigned char)
{
}

// 0x00464F6C | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::Finalize(bool)
{
}

// 0x004650B0 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::WaitPipe(nn::fnd::TimeSpan)
{
}

// 0x00465120 | fefates:bytes [tier B]
void nn::snd::CTR::Dspsnd::WaitPipe()
{
}

// 0x00465178 | nintendogs:callgraph [tier A]
void nn::snd::CTR::Dspsnd::AssignPCM(unsigned char, const nn::snd::CTR::WaveBuffer*, nn::snd::CTR::DspsndAudioInfo)
{
}

// 0x00465344 | fefates:bytes [tier B]
nn::snd::CTR::Dspsnd::~Dspsnd()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
