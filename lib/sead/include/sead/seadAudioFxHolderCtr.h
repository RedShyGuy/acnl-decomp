#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>  typeinfo 0x008D1AAC  vtable 0x00905D2C
//   sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>  typeinfo 0x008D1AB8  vtable 0x00905D50
//   sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>  typeinfo 0x008D1AC4  vtable 0x00905D74
//   sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>  typeinfo 0x008D1AD0  vtable 0x00905D98
//   sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>  typeinfo 0x008D1ADC  vtable 0x00905DBC
//   sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>  typeinfo 0x008D1AE8  vtable 0x00905DE0
template <typename T0, typename T1>
class AudioFxHolderCtr
{
public:
    // TODO: members unknown
};
} // namespace sead
