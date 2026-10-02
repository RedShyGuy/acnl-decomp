#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace snd {
namespace CTR {
class Dspsnd
{
public:
    void Initialize(bool); // 0x00463850 | nintendogs:callgraph [tier A]
    void SetSyncMode(nn::snd::CTR::SyncMode); // 0x00463A44 | fefates:bytes [tier B]
    void EnableAuxBus(nn::snd::CTR::AuxBusId, bool); // 0x00463A98 | nintendogs:callgraph [tier A]
    void SetRearRatio(unsigned short); // 0x00463B24 | fefates:bytes [tier B]
    void SetChannelMix(unsigned char, const nn::snd::CTR::MixParam*); // 0x00463CF0 | nintendogs:callgraph [tier A]
    void SetChannelRIM(unsigned char, unsigned short, unsigned short); // 0x00463DCC | fefates:bytes [tier B]
    void SetChannelTimer(unsigned char, float); // 0x00463F70 | fefates:bytes [tier B]
    void SetClippingMode(nn::snd::CTR::ClippingMode); // 0x00463FEC | fefates:bytes [tier B]
    void SetMasterVolume(float); // 0x0046404C | nintendogs:callgraph [tier A]
    void SetSurroundDepth(unsigned short); // 0x004640AC | fefates:bytes [tier B]
    void SetAuxFrontBypass(nn::snd::CTR::AuxBusId, bool); // 0x00464118 | nintendogs:callgraph [tier A]
    void SetDspDelayEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxDelayParams&); // 0x004641A4 | nintendogs:callgraph [tier A]
    void SetAuxReturnVolume(nn::snd::CTR::AuxBusId, float); // 0x004642A8 | nintendogs:callgraph [tier A]
    void SetChannelPlayStop(unsigned char); // 0x00464320 | fefates:bytes [tier B]
    void SetDspReverbEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxReverbParams&); // 0x00464394 | nintendogs:callgraph [tier A]
    void SetSoundOutputMode(nn::snd::CTR::OutputMode); // 0x00464500 | fefates:bytes [tier B]
    void InitializeVariables(bool); // 0x00464560 | fefates:bytes [tier B]
    void SetChannelPlayStart(unsigned char); // 0x00464810 | fefates:bytes [tier B]
    void SetChannelSyncCount(unsigned char, short); // 0x00464880 | nintendogs:callgraph [tier A]
    void GetDroppedFrameCount(); // 0x004648F0 | fefates:bytes [tier B]
    void SetChannelAdpcmParam(unsigned char, const nn::snd::CTR::AdpcmParam*); // 0x00464940 | nintendogs:callseq [tier A]
    void SetOutputBufferCount(int); // 0x004649CC | fefates:bytes [tier B]
    void SetIsHeadsetConnected(bool); // 0x00464A20 | fefates:bytes [tier B]
    void ResetChannelNextBuffer(unsigned char); // 0x00464A80 | fefates:bytes [tier B]
    void AppendChannelNextBuffer(unsigned char, const nn::snd::CTR::WaveBuffer*, int); // 0x00464AE8 | nintendogs:callgraph [tier A]
    void SetChannelIirFilterType(unsigned char, nn::snd::CTR::FilterType); // 0x00464C64 | fefates:bytes [tier B]
    void UpdateChannelNextBuffer(unsigned char, const nn::snd::CTR::WaveBuffer*); // 0x00464CD4 | fefates:bytes [tier B]
    void SetChannelIIRFilter_Mono(unsigned char, short, short); // 0x00464D94 | fefates:bytes [tier B]
    void SetChannelIIRFilter_Biquad(unsigned char, short, short, short, short, short); // 0x00464E0C | fefates:bytes [tier B]
    void SetSurroundSpeakerPosition(nn::snd::CTR::SurroundSpeakerPosition); // 0x00464E98 | fefates:bytes [tier B]
    void InitializeChannelParameters(unsigned char); // 0x00464F04 | fefates:bytes [tier B]
    void Finalize(bool); // 0x00464F6C | nintendogs:callgraph [tier A]
    void WaitPipe(nn::fnd::TimeSpan); // 0x004650B0 | fefates:bytes [tier B]
    void WaitPipe(); // 0x00465120 | fefates:bytes [tier B]
    void AssignPCM(unsigned char, const nn::snd::CTR::WaveBuffer*, nn::snd::CTR::DspsndAudioInfo); // 0x00465178 | nintendogs:callgraph [tier A]
    ~Dspsnd(); // 0x00465344 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace snd
} // namespace nn
